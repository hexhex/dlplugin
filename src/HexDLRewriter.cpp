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

#include <cassert>

using namespace dlvhex::dl;


HexDLRewriterBase::HexDLRewriterBase()
  : naf(false)
{ }


HexDLRewriterBase::~HexDLRewriterBase()
{ }


Literal*
HexDLRewriterBase::getLiteral() const
{
  std::ostringstream oss;

  rewrite(oss);

  std::string at = oss.str();
  bool neg = false;

  if (at[0] == '-')
    {
      at.erase(0);
      neg = true;
    }

  AtomPtr ap(new Atom(at, neg));

  return new Literal(ap, naf);
}


std::ostream&
BodyRewriter::rewrite(std::ostream& os) const
{
  if (!body.empty())
    {
      std::copy(body.begin(), body.end() - 1,
		std::ostream_iterator<HexDLRewriterBase>(os, ", ")
		);
      os << body.back();
    }
  if (!dlbody.empty())
    {
      if (!body.empty()) os << ", "; // add a body delimiter in case body was non-empty

      std::copy(dlbody.begin(), dlbody.end() - 1,
		std::ostream_iterator<HexDLRewriterBase>(os, ", ")
		);
      os << dlbody.back();
    }
  return os;
}


BodyRewriter::BodyRewriter()
  : body()
{ }


BodyRewriter::~BodyRewriter()
{ }


void
BodyRewriter::add(LiteralRewriter* atom)
{
  body.push_back(atom);
}


void
BodyRewriter::add(BodyRewriter* body0)
{
  body.transfer(body.end(), body0->body);
  dlbody.transfer(dlbody.end(), body0->dlbody);
}


void
BodyRewriter::add(DLAtomRewriterBase* atom)
{
  dlbody.push_front(atom);
}


RuleBody_t*
BodyRewriter::getBody() const
{
  RuleBody_t* b = new RuleBody_t;

  for (boost::ptr_vector<HexDLRewriterBase>::const_iterator it = body.begin();
       it != body.end(); ++it)
    {
      b->push_back(it->getLiteral());
    }

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
      
  // now get the literals of the pushed dl-atoms
  for (boost::ptr_deque<DLAtomRewriterBase>::const_iterator it = dlbody.begin();
       it != dlbody.end(); ++it)
    {
      b->push_back(it->getLiteral());
    }

  return b;
}


LiteralRewriter::LiteralRewriter(const std::string* l)
  : literal((*l)[0] == '-' ?
	    new Atom(l->substr(1), true) :
	    new Atom(*l)
	    )
{ }


LiteralRewriter::LiteralRewriter(Atom* a)
  : literal(a)
{ }


LiteralRewriter::LiteralRewriter(const LiteralRewriter& r)
  : HexDLRewriterBase(),
    literal(new Atom(*r.literal))
{ }


LiteralRewriter&
LiteralRewriter::operator= (const LiteralRewriter&)
{
  return *this; // ignore
}


LiteralRewriter::~LiteralRewriter()
{ }


std::ostream&
LiteralRewriter::rewrite(std::ostream& os) const
{
  return os << *literal;
}


Literal*
LiteralRewriter::getLiteral() const
{
  return new Literal(literal, this->naf);
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
      // prepare cq as a singleton atomset
      if (query[0] == '-')
	{
	  AtomPtr ap(new Atom(query.substr(1), output, true));
	  cq.insert(ap);
	}
      else
	{
	  AtomPtr ap(new Atom(query, output));
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

      // get output variables and constants of cq2
      std::set<Term> Y2(output2->begin(), output2->end());

      // Z2 = X2 \ Y2 , i.e. Z2 contains only existential variables of cq2
      std::set<Term> Z2;
      std::set_difference(X2.begin(), X2.end(), Y2.begin(), Y2.end(),
			  std::inserter(Z2, Z2.begin())
			  );

      // this is the variable mapping for cq2
      std::map<Term,Term> variablemapping;

      if (!Z2.empty())
	{
	  // the set of all possible variables of cq1 and cq2
	  std::set<Term> vars(X1.begin(), X1.end());
	  vars.insert(X2.begin(), X2.end());
      
	  // we append a counter to the last variable of vars, so whatever
	  // variables may appear in cq1 or cq2, lastvar + n will be
	  // lexicographically greater than lastvar and thus unique in cq1
	  // or cq2
	  const char* lastvar = vars.empty() ? "X" : (--vars.end())->getVariable().c_str();
	  unsigned n = 0;
	  std::ostringstream nvar;

	  //
	  // for each variable z of Z2 : if z \in X1 then rename z
	  //
	  for (std::set<Term>::const_iterator z = Z2.begin(); z != Z2.end(); ++z)
	    {
	      if (X1.find(*z) != X1.end())
		{
		  // rename variable z to lastvar+n and increase n
		  nvar.str("");
		  nvar << lastvar << n++;
		  variablemapping[*z] = Term(nvar.str());
		}
	    }
	}

      //
      // insert variablemapped cq2 into cq1
      //
      for (AtomSet::const_iterator a2 = cq2.begin(); a2 != cq2.end(); ++a2)
	{
	  Tuple nargs(1, a2->getPredicate()); // a2's predicate + new arguments

	  const Tuple& args = a2->getArguments();

	  // rename variables of a2
	  for (Tuple::const_iterator it = args.begin(); it != args.end(); ++it)
	    {
	      std::map<Term,Term>::const_iterator var = variablemapping.find(*it);

	      if (var != variablemapping.end()) // rename it to var
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
	  cq1.insert(ap);
	}

      //
      // append the new conjunctive query cq3 to input3
      //

      std::ostringstream cq3;

      if (!cq1.empty())
	{
	  std::copy(cq1.begin(), --cq1.end(), std::ostream_iterator<Atom>(cq3, ","));
	  cq3 << *(--cq1.end());
	}

      input3->push_back(Term(cq3.str(), true));

      // and return a new cq-atom rewriter waiting for more to push
      return std::auto_ptr<DLAtomRewriterBase>(new CQAtomRewriter(input3, output3));
    }

  // b is incompatible to this cq-atom
  return std::auto_ptr<DLAtomRewriterBase>();
}


CQAtomRewriter::CQAtomRewriter(const Tuple* in, const Tuple* out)
  : DLAtomRewriterBase(in,out)
{
  assert(in != 0);
  assert(out != 0);
}


SimpleDLAtomRewriter::SimpleDLAtomRewriter(const Tuple* i, const Tuple* o)
  : DLAtomRewriterBase(i, o)
{
  assert(i != 0);
  assert(o != 0);
}


std::ostream&
SimpleDLAtomRewriter::rewrite(std::ostream& os) const
{
  const Tuple* i = getInputTuple();
  const Tuple* o = getOutputTuple();

  if (o->size() == 2)
    {
      os << "&dlR[";
    }
  else if (o->size() == 1)
    {
      os << "&dlC[";
    }
  else
    {
      throw PluginError("Wrong output tuple size for dl-atom");
    }

  // input tuple
  std::copy(i->begin(), i->end() - 1, std::ostream_iterator<Term>(os, ","));
  os << i->back() << "](";

  // output tuple
  std::copy(o->begin(), o->end() - 1, std::ostream_iterator<Term>(os, ","));
  return os << o->back() << ')';
}


CQAtomRewriter::CQAtomRewriter(const CQAtomRewriter& c)
  : DLAtomRewriterBase(c)
{ }


CQAtomRewriter&
CQAtomRewriter::operator= (const CQAtomRewriter&)
{
  return *this; // ignore
}


std::ostream&
CQAtomRewriter::rewrite(std::ostream& os) const
{
  // rewrite external cq-atom plus arity

  os << "&dlCQ" << getOutputTuple()->size();

  // append input list (we assume a non-empty input list)

  os << '[';

  std::copy(getInputTuple()->begin(), getInputTuple()->end() - 1,
	    std::ostream_iterator<Term>(os, ",")
	    );

  os << getInputTuple()->back() << "](";

  // append output list

  if (!getOutputTuple()->empty())
    {
      std::copy(getOutputTuple()->begin(), getOutputTuple()->end() - 1,
		std::ostream_iterator<Term>(os, ",")
		);
      
      os << getOutputTuple()->back();
    }

  return os << ')';
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

  TBox::ObjectsPtr concepts = ontology->getTBox().getConcepts();
  TBox::ObjectsPtr roles = ontology->getTBox().getRoles();
  TBox::ObjectsPtr datatypeRoles = ontology->getTBox().getDatatypeRoles();

  // dispatch dl-atom-ops
  Term p("p");
  Term m("m");

  for (AtomSet::atomset_t::const_iterator it = ops.atoms.begin();
       it != ops.atoms.end(); ++it)
    {
      const Term& pred = (*it)->getPredicate();

      const Tuple tup = (*it)->getArguments();
      const Term& tmp = tup[0];
      const std::string& tmpstr = tmp.getString();

      Term t(addNamespace(tmpstr));
      
      AtomPtr ap(*it);

      if (concepts->find(t) != concepts->end())
	{
	  if (pred == p)
	    pc.insert(ap);
	  else if (pred == m)
	    mc.insert(ap);
	}
      else if (roles->find(t) != roles->end() ||
	       datatypeRoles->find(t) != datatypeRoles->end())
	{
	  if (pred == p)
	    pr.insert(ap);
	  else if (pred == m)
	    mr.insert(ap);
	}
      else
	{
	  throw PluginError("Incompatible dl-atom-op " + tmpstr + " supplied.");
	}
    }
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


std::ostream&
DLAtomRewriter::rewrite(std::ostream& os) const
{
  // first commandment: stream thy strings

  TBox::ObjectsPtr concepts = ontology->getTBox().getConcepts();
  TBox::ObjectsPtr roles = ontology->getTBox().getRoles();
  TBox::ObjectsPtr datatypeRoles = ontology->getTBox().getDatatypeRoles();

  std::string tmpquery = addNamespace(*query);
  Term q(tmpquery);

  if (concepts->find(q) != concepts->end())
    {
      os << "&dlC";
    }
  else if (getOutputTuple()->size() == 2 && roles->find(q) != roles->end())
    {
      os << "&dlR";
    }
  else if (getOutputTuple()->size() == 2 && datatypeRoles->find(q) != datatypeRoles->end())
    {
      os << "&dlDR";
    }
  else
    {
      throw PluginError("Incompatible dl-atom query supplied.");
    }

  //
  // output external atoms input list
  //

  os << '[';

  std::copy(getInputTuple()->begin(), getInputTuple()->end() - 1,
	    std::ostream_iterator<Term>(os, ","));
  os << getInputTuple()->back();

  os << ']';

  //
  // append output list of the ext. atom
  //

  if (getOutputTuple()->size() == 2)
    {
      os << '(' << getOutputTuple()->front() << ',' << getOutputTuple()->back() << ')';
    }
  else if (getOutputTuple()->size() == 1)
    {
      os << '(' << getOutputTuple()->front() << ')';
    }
  else
    {
      throw PluginError("Incompatible output list for dl-atom.");
    }

  return os;
}


std::vector<Rule*>
DLAtomRewriter::getDLInputRules() const
{
  std::vector<Rule*> rules;

  unsigned pcno = dlinput.getInputNo(pc);
  unsigned mcno = dlinput.getInputNo(mc);
  unsigned prno = dlinput.getInputNo(pr);
  unsigned mrno = dlinput.getInputNo(mr);

  std::ostringstream oss;

  oss << "dl_pc_" << pcno;
  std::string spc = oss.str();
  oss.str("");

  oss << "dl_mc_" << mcno;
  std::string smc = oss.str();
  oss.str("");

  oss << "dl_pr_" << prno;
  std::string spr = oss.str();
  oss.str("");

  oss << "dl_mr_" << mrno;
  std::string smr = oss.str();
  oss.str("");

  //
  // register aux. predicate names, we don't want them to occur in the
  // answer sets
  //

  if (!pc.empty())
    {
      Term::registerAuxiliaryName(spc);
    }
  if (!mc.empty())
    {
      Term::registerAuxiliaryName(smc);
    }
  if (!pr.empty())
    {
      Term::registerAuxiliaryName(spr);
    }
  if (!mr.empty())
    {
      Term::registerAuxiliaryName(smr);
    }


  // temp. variable names
  Term x("X");
  Term y("Y");

  //
  // for each dl-atom operation we create a rule
  //

  for (AtomSet::const_iterator it = pc.begin(); it != pc.end(); ++it)
    {
      Tuple t;

      t.push_back(it->getArgument(1));
      t.push_back(x);

      AtomPtr h(new Atom(spc, t));
      RuleHead_t head(1, h);

      t.clear();
      t.push_back(it->getArgument(2));
      t.push_back(x);

      AtomPtr b(new Atom(t));
      RuleBody_t body(1, new Literal(b));

      rules.push_back(new Rule(head, body));
    }

  for (AtomSet::const_iterator it = mc.begin(); it != mc.end(); ++it)
    {
      Tuple t;

      t.push_back(it->getArgument(1));
      t.push_back(x);

      AtomPtr h(new Atom(smc, t));
      RuleHead_t head(1, h);

      t.clear();
      t.push_back(it->getArgument(2));
      t.push_back(x);

      AtomPtr b(new Atom(t));
      RuleBody_t body(1, new Literal(b));

      rules.push_back(new Rule(head, body));
    }

  for (AtomSet::const_iterator it = pr.begin(); it != pr.end(); ++it)
    {
      Tuple t;

      t.push_back(it->getArgument(1));
      t.push_back(x);
      t.push_back(y);

      AtomPtr h(new Atom(spr, t));
      RuleHead_t head(1, h);

      t.clear();
      t.push_back(it->getArgument(2));
      t.push_back(x);
      t.push_back(y);

      AtomPtr b(new Atom(t));
      RuleBody_t body(1, new Literal(b));

      rules.push_back(new Rule(head, body));
    }

  for (AtomSet::const_iterator it = mr.begin(); it != mr.end(); ++it)
    {
      Tuple t;

      t.push_back(it->getArgument(1));
      t.push_back(x);
      t.push_back(y);

      AtomPtr h(new Atom(smr, t));
      RuleHead_t head(1, h);

      t.clear();
      t.push_back(it->getArgument(2));
      t.push_back(x);
      t.push_back(y);

      AtomPtr b(new Atom(t));
      RuleBody_t body(1, new Literal(b));

      rules.push_back(new Rule(head, body));
    }

  return rules;
}
