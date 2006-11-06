/* -*- C++ -*- */

/**
 * @file   HexDLRewriter.cpp
 * @author Thomas Krennwallner
 * @date   Mon Sep  4 14:43:54 2006
 * 
 * @brief  Keeps track of rewriting the body of rules.
 * 
 * 
 */

#include "HexDLRewriter.h"
#include "AtomSeparator.h"
#include "DLError.h"

#include <dlvhex/Error.h>

#include <iosfwd>
#include <iterator>
#include <sstream>

#include <cassert>
#include <cstdlib>

using namespace dlvhex::dl;


HexDLRewriterBase::HexDLRewriterBase()
  : naf(false)
{ }


HexDLRewriterBase::~HexDLRewriterBase()
{ }


BodyRewriter::BodyRewriter()
  : dlbody()
{ }


BodyRewriter::~BodyRewriter()
{ }


void
BodyRewriter::add(DLAtomRewriterBase* atom)
{
  dlbody.push_front(atom);
}


RuleBody_t*
BodyRewriter::getBody() const
{
  //
  // dl-body pushing
  //

  if (dlbody.size() > 1)
    {
      boost::ptr_deque<DLAtomRewriterBase> done; // all pushed atoms end up here
      boost::ptr_deque<DLAtomRewriterBase> park; // park some dl-/cq-atoms until we can push them

      do
	{
	  while (dlbody.size() > 1)
	    {
	      std::auto_ptr<DLAtomRewriterBase> b1(dlbody.pop_front().release());
	      std::auto_ptr<DLAtomRewriterBase> b2(dlbody.pop_front().release());
	      
	      std::auto_ptr<DLAtomRewriterBase> b3 = b1->push(b2);
	      
	      if (b3.get() == 0)
		{
		  // if we couldn't push b1 and b2 to b3, we park b2 and retry with b1
		  park.push_front(b2.release());
		  dlbody.push_front(b1.release());
		}
	      else
		{
		  // we continue with b3 and forget b1,b2
		  dlbody.push_front(b3.release());
		}
	    }
	  
	  // get the last element of dlbody, i.e. a fully pushed atom
	  done.push_front(dlbody.pop_front().release());
	  
	  dlbody = park.release(); // now restart with the rest
	}
      while(!dlbody.empty());
      
      // done contains all pushed dl-atoms
      dlbody = done.release();
    }
      
  RuleBody_t* b = new RuleBody_t;

  // now get the literals of the pushed dl-atoms
  for (boost::ptr_deque<DLAtomRewriterBase>::const_iterator it = dlbody.begin();
       it != dlbody.end(); ++it)
    {
      b->push_back(it->getLiteral());
    }

  return b;
}


DLAtomRewriterBase::DLAtomRewriterBase(const Tuple* i, const Tuple* o)
  : input(i), output(o)
{ }


DLAtomRewriterBase::DLAtomRewriterBase(const DLAtomRewriterBase& b)
  : HexDLRewriterBase(),
    input(new Tuple(*b.input)),
    output(new Tuple(*b.output))
{ }


DLAtomRewriterBase::~DLAtomRewriterBase()
{
  delete input;
  delete output;
}


Literal*
DLAtomRewriterBase::getLiteral() const
{
  AtomPtr ap(new ExternalAtom(getName(), *getOutputTuple(), *getInputTuple(), 0));
  return new Literal(ap, isNAF());
}


const Tuple*
DLAtomRewriterBase::getInputTuple() const
{
  return input;
}


const Tuple*
DLAtomRewriterBase::getOutputTuple() const
{
  return output;
}


void
DLAtomRewriterBase::getCQ(const std::string& query, const Tuple& output, AtomSet& cq) const
{
  //
  // check if query is a dl-query resp. cq-query and parse it into an
  // AtomSet accordingly
  //

  if (query.find("(") != std::string::npos) // query is a cq-atom
    {
      AtomSeparator(query, cq).parse();
    }
  else // query is a dl-atom
    {
      // first check if output has quoted individuals

      Tuple tmpout;

      for (Tuple::const_iterator it = output.begin(); it != output.end(); ++it)
	{
	  if (it->isString()) // remove the quotation
	    {
	      tmpout.push_back(Term(it->getUnquotedString()));
	    }
	  else
	    {
	      tmpout.push_back(*it);
	    }
	}

      // prepare cq as a singleton atomset
      if (query[0] == '-')
	{
	  AtomPtr ap(new Atom(query.substr(1), tmpout, true));
	  cq.insert(ap);
	}
      else
	{
	  AtomPtr ap(new Atom(query, tmpout));
	  cq.insert(ap);
	}
    }
}


std::auto_ptr<DLAtomRewriterBase>
DLAtomRewriterBase::push(const std::auto_ptr<DLAtomRewriterBase>& b) const
{
  const Tuple* input1 = getInputTuple();
  const Tuple* input2 = b->getInputTuple();
  const Tuple* output1 = getOutputTuple();
  const Tuple* output2 = b->getOutputTuple();

  ///@todo for now we don't take unused predicates into account
  if (std::equal(input1->begin(), input1->end() - 1, input2->begin()) &&
      input1->size() == input2->size() &&
      !this->isNAF() && !b->isNAF()
      )
    {
      //
      // create output tuple
      //
      std::set<Term> termset(output1->begin(), output1->end());
      termset.insert(output2->begin(), output2->end());
      Tuple* output3 = new Tuple(termset.begin(), termset.end());

      //
      // create input tuple
      //

      // add the lambda components to input3
      Tuple* input3 = new Tuple(input1->begin(), input1->end() - 1);

      //
      // parse both queries to an AtomSet
      //

      const std::string& query1 = input1->back().getUnquotedString();
      const std::string& query2 = input2->back().getUnquotedString();

      AtomSet cq1;
      AtomSet cq2;

      getCQ(query1, *output1, cq1);
      getCQ(query2, *output2, cq2);

      //
      // get the variables of cq1 and cq2
      //

      std::set<Term> X1; // variables of cq1
      std::set<Term> X2; // variables of cq2

      // get variables of cq1
      for (AtomSet::const_iterator it = cq1.begin(); it != cq1.end(); ++it)
	{
	  const Tuple& args = it->getArguments();
	  for (Tuple::const_iterator a = args.begin(); a != args.end(); ++a)
	    {
	      if (a->isVariable())
		X1.insert(*a);
	    }
	}

      // get variables of cq2
      for (AtomSet::const_iterator it = cq2.begin(); it != cq2.end(); ++it)
	{
	  const Tuple& args = it->getArguments();
	  for (Tuple::const_iterator a = args.begin(); a != args.end(); ++a)
	    {
	      if (a->isVariable())
		X2.insert(*a);
	    }
	}

      //
      // prepare for renaming the variables of cq2
      //

      // get output variables and constants of cq1
      std::set<Term> Y1(output1->begin(), output1->end());
      // get output variables and constants of cq2
      std::set<Term> Y2(output2->begin(), output2->end());

      // Z1 = X1 \ Y1 , i.e. Z1 contains only existential variables of cq1
      std::set<Term> Z1;
      std::set_difference(X1.begin(), X1.end(), Y1.begin(), Y1.end(),
			  std::inserter(Z1, Z1.begin())
			  );
      // Z2 = X2 \ Y2 , i.e. Z2 contains only existential variables of cq2
      std::set<Term> Z2;
      std::set_difference(X2.begin(), X2.end(), Y2.begin(), Y2.end(),
			  std::inserter(Z2, Z2.begin())
			  );

      // this is the variable mapping for cq1
      std::map<Term,Term> vm1;
      // this is the variable mapping for cq2
      std::map<Term,Term> vm2;

      if (!Z1.empty() || !Z2.empty())
	{
	  // the set of all possible variables of cq1 and cq2
	  std::set<Term> vars(X1.begin(), X1.end());
	  vars.insert(X2.begin(), X2.end());
      
	  // we append a counter to the last variable of vars, so whatever
	  // variables may appear in cq1 or cq2, lastvar + n will be
	  // lexicographically greater than lastvar and thus unique in cq1
	  // or cq2
	  std::string lastvar = vars.empty() ? "X" : (--vars.end())->getVariable();

	  // we start at 0
	  unsigned n = 0;
	  
	  std::string::size_type i = lastvar.find_first_of("0123456789");
	  std::string::size_type j = lastvar.find_first_not_of("0123456789", i);

	  // if last variable ends in a number i, we can reuse i and
	  // set n = i + 1
	  if (i != std::string::npos && j == std::string::npos)
	    {
	      lastvar = lastvar.substr(0, i); // lastvar is the string component
	      n = atoi(lastvar.substr(i).c_str()) + 1; // n = i + 1
	    }

	  std::ostringstream nvar;

	  //
	  // for each ex. variable z of Z1 : if z \in X2 then rename z in cq1
	  //
	  for (std::set<Term>::const_iterator z = Z1.begin(); z != Z1.end(); ++z)
	    {
	      if (X2.find(*z) != X2.end())
		{
		  // rename variable z to lastvar+n and increase n
		  nvar.str("");
		  nvar << lastvar << n++;
		  vm1[*z] = Term(nvar.str());
		}
	    }

	  //
	  // for each ex. variable z of Z2 : if z \in X1 then rename z in cq2
	  //
	  for (std::set<Term>::const_iterator z = Z2.begin(); z != Z2.end(); ++z)
	    {
	      if (X1.find(*z) != X1.end())
		{
		  // rename variable z to lastvar+n and increase n
		  nvar.str("");
		  nvar << lastvar << n++;
		  vm2[*z] = Term(nvar.str());
		}
	    }
	}

      // the new pushed conjunctive query
      AtomSet cq3;

      //
      // insert variablemapped cq1 into cq3
      //
      for (AtomSet::const_iterator a1 = cq1.begin(); a1 != cq1.end(); ++a1)
	{
	  Tuple nargs(1, a1->getPredicate()); // a1's predicate + new arguments

	  const Tuple& args = a1->getArguments();

	  // rename variables of a1
	  for (Tuple::const_iterator it = args.begin(); it != args.end(); ++it)
	    {
	      std::map<Term,Term>::const_iterator var = vm1.find(*it);

	      if (var != vm1.end()) // rename it to var
		{
		  nargs.push_back(var->second);
		}
	      else // leave it as is
		{
		  nargs.push_back(*it);
		}
	    }

	  AtomPtr ap(new Atom(nargs, a1->isStronglyNegated()));
	  ap->setAlwaysFO();
	  cq3.insert(ap);
	}

      //
      // insert variablemapped cq2 into cq3
      //
      for (AtomSet::const_iterator a2 = cq2.begin(); a2 != cq2.end(); ++a2)
	{
	  Tuple nargs(1, a2->getPredicate()); // a2's predicate + new arguments

	  const Tuple& args = a2->getArguments();

	  // rename variables of a2
	  for (Tuple::const_iterator it = args.begin(); it != args.end(); ++it)
	    {
	      std::map<Term,Term>::const_iterator var = vm2.find(*it);

	      if (var != vm2.end()) // rename it to var
		{
		  nargs.push_back(var->second);
		}
	      else // leave it as is
		{
		  nargs.push_back(*it);
		}
	    }

	  AtomPtr ap(new Atom(nargs, a2->isStronglyNegated()));
	  ap->setAlwaysFO();
	  cq3.insert(ap);
	}

      //
      // append the new conjunctive query cq3 to input3
      //

      std::ostringstream cq3str;

      // cq3 shouldn't be empty
      std::copy(cq3.begin(), --cq3.end(), std::ostream_iterator<Atom>(cq3str, ","));
      cq3str << *(--cq3.end());

      input3->push_back(Term(cq3str.str(), true));

      // and return a new cq-atom rewriter waiting for more to push
      return std::auto_ptr<DLAtomRewriterBase>(new CQAtomRewriter(input3, output3));
    }

  // b is incompatible to this cq-atom
  return std::auto_ptr<DLAtomRewriterBase>();
}


CQAtomRewriter&
CQAtomRewriter::operator= (const CQAtomRewriter&)
{
  return *this; // ignore
}


CQAtomRewriter::CQAtomRewriter(const Tuple* i, const Tuple* o)
  : DLAtomRewriterBase(i, o)
{
  assert(i != 0);
  assert(o != 0);
}


CQAtomRewriter::CQAtomRewriter(const CQAtomRewriter& c)
  : DLAtomRewriterBase(c)
{ }


std::string
CQAtomRewriter::getName() const
{
  std::ostringstream oss;
  oss << "dlCQ" << getOutputTuple()->size();
  return oss.str();
}


SimpleDLAtomRewriter::SimpleDLAtomRewriter(const std::string* n, const Tuple* i, const Tuple* o)
  : DLAtomRewriterBase(i, o), name(n)
{
  assert(i != 0);
  assert(o != 0);

  if (getOutputTuple()->size() != 1 && getOutputTuple()->size() != 2)
    {
      throw PluginError("dl-atom " + *name + " has wrong arity.");
    }
}


SimpleDLAtomRewriter::~SimpleDLAtomRewriter()
{
  delete name;
}


std::string
SimpleDLAtomRewriter::getName() const
{
  return *name;
}


DLAtomRewriter::DLAtomRewriter(const Ontology::shared_pointer& onto,
			       DLAtomInput& d,
			       const AtomSet& ops,
			       const std::string* q,
			       const Tuple* o)
  : DLAtomRewriterBase(0, o), ontology(onto), query(q), dlinput(d)
{
  assert(query != 0);
  assert(o != 0);

  if (!ontology)
    {
      throw PluginError("Couldn't rewrite dl-atom, ontology is empty.");
    }

  if (getOutputTuple()->size() != 1 && getOutputTuple()->size() != 2)
    {
      throw PluginError("dl-atom has wrong arity.");
    }

  ops.matchPredicate("pc", pc);
  ops.matchPredicate("pr", pr);
  ops.matchPredicate("mc", mc);
  ops.matchPredicate("mr", mr);
}


DLAtomRewriter::DLAtomRewriter(const DLAtomRewriter& d)
  : DLAtomRewriterBase(d),
    ontology(d.ontology),
    query(d.query),
    dlinput(d.dlinput)
{ }


DLAtomRewriter&
DLAtomRewriter::operator= (const DLAtomRewriter&)
{
  return *this; // ignore
}


DLAtomRewriter::~DLAtomRewriter()
{
  delete query;
}


const Tuple*
DLAtomRewriter::getInputTuple() const
{
  if (DLAtomRewriterBase::getInputTuple() == 0)
    {
      Tuple* t = new Tuple;

      t->push_back(Term(ontology->getRealURI().getString(), true));

      std::ostringstream oss;

      oss << "dl_pc_" << dlinput.getInputNo(pc);
      t->push_back(Term(oss.str()));
      oss.str("");

      oss << "dl_mc_" << dlinput.getInputNo(mc);
      t->push_back(Term(oss.str()));
      oss.str("");

      oss << "dl_pr_" << dlinput.getInputNo(pr);
      t->push_back(Term(oss.str()));
      oss.str("");

      oss << "dl_mr_" << dlinput.getInputNo(mr);
      t->push_back(Term(oss.str()));
      oss.str("");

      std::string tmpquery = addNamespace(*query);

      // check if query is strongly negated
      if (query->find("\"-") == 0 || query->find("-") == 0)
	{
	  tmpquery = "-" + tmpquery;
	}

      t->push_back(Term(tmpquery, true));

      const_cast<DLAtomRewriter*>(this)->setInputTuple(t);
    }

  return DLAtomRewriterBase::getInputTuple();
}


std::string
DLAtomRewriter::addNamespace(const std::string& s) const
{
  std::string tmp;

  if (s.find("\"-") == 0)
    {
      tmp = ontology->getNamespace() + s.substr(2, s.length() - 3);
    }
  else if (s.find("\"") == 0)
    {
      tmp = ontology->getNamespace() + s.substr(1, s.length() - 2);
    }
  else if (s.find("-") == 0)
    {
      tmp = ontology->getNamespace() + s.substr(1, s.length() - 1);
    }
  else
    {
      tmp = ontology->getNamespace() + s;
    }

  return tmp;
}


std::string
DLAtomRewriter::getName() const
{
  TBox::ObjectsPtr concepts = ontology->getTBox().getConcepts();
  TBox::ObjectsPtr roles = ontology->getTBox().getRoles();
  TBox::ObjectsPtr datatypeRoles = ontology->getTBox().getDatatypeRoles();

  std::string tmpquery = addNamespace(*query);
  Term q(tmpquery);

  if (concepts->find(q) != concepts->end())
    {
      return "dlC";
    }
  else if (getOutputTuple()->size() == 2 && roles->find(q) != roles->end())
    {
      return "dlR";
    }
  else if (getOutputTuple()->size() == 2 && datatypeRoles->find(q) != datatypeRoles->end())
    {
      return "dlDR";
    }
  else
    {
      throw PluginError("Incompatible dl-atom query supplied.");
    }

}


DLAtomInput::DLAtomInput()
  : asmap(), ncnt(1)
{ }


unsigned
DLAtomInput::getInputNo(const AtomSet& as)
{
  if (as.empty())
    {
      return 0; // no dl-atom-ops
    }

  std::pair<AtomSetMap::iterator,bool> p = asmap.insert(std::make_pair(as, ncnt));
      
  if (p.second) // dl-atom-ops number found
    {
      ncnt++;
    }

  return p.first->second;
}


std::vector<Rule*>
DLAtomInput::getDLInputRules() const
{
  std::vector<Rule*> rules;

  std::ostringstream oss;
  std::string aux;

  // temp. variable and predicate names
  const Term x("X");
  const Term y("Y");
  const Term pr("pr");
  const Term mr("mr");

  for (AtomSetMap::const_iterator it = asmap.begin(); it != asmap.end(); ++it)
    {
      for (AtomSet::atomset_t::const_iterator a = it->first.atoms.begin();
	   a != it->first.atoms.end(); ++a)
	{
	  // pred \in { pc,mc,pr,mr }
	  Term pred = (*a)->getPredicate();

	  // register aux. predicate names, we don't want them to
	  // occur in the answer sets

	  oss.str("");
	  oss << "dl_" << pred << '_' << it->second;
	  aux = oss.str();
	  Term::registerAuxiliaryName(aux);

	  // create output variables 

	  Tuple t;
	  t.push_back(x);
	  if (pred == pr || pred == mr)
	    {
	      t.push_back(y);
	    }

	  AtomPtr b(new Atom((*a)->getArgument(2).getString(), t));
	  RuleBody_t body(1, new Literal(b));

	  // add concept or role to the front of t
	  t.insert(t.begin(), (*a)->getArgument(1));

	  AtomPtr h(new Atom(aux, t));
	  RuleHead_t head(1, h);
	  
	  rules.push_back(new Rule(head, body));
 	}
    }

  return rules;
}
