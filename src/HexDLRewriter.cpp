/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2005, 2006, 2007  Thomas Krennwallner
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


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

#include <dlvhex2/Error.h>

#include <iosfwd>
#include <iterator>
#include <sstream>

#include <cassert>
#include <cstdlib>

namespace dlvhex {
  namespace dl {

#if 0
// @TODO

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
BodyRewriter::add(ExtAtomRewriter* atom)
{
  dlbody.push_front(atom);
}


void
BodyRewriter::bodyOptimizer(Tuple& body) const
{
  typedef boost::ptr_deque<ExtAtomRewriter> ExtAtomDeque;

  //
  // dl-body pushing
  //

  if (dlbody.size() > 1)
    {
      ExtAtomDeque done; // all pushed atoms end up here
      ExtAtomDeque park; // park some dl-/cq-atoms until we can push them

      do
	{
	  while (dlbody.size() > 1)
	    {
	      // get 2 atoms from the deque

	      std::auto_ptr<ExtAtomRewriter> b1(dlbody.pop_front().release());
	      std::auto_ptr<ExtAtomRewriter> b2(dlbody.pop_front().release());
	      
	      //
	      // and now perform the pushing
	      //

	      std::auto_ptr<ExtAtomRewriter> b3 = b1->push(b2);
	      
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
	  
	  // get the last element of dlbody, i.e., a fully pushed atom
	  done.push_front(dlbody.pop_front().release());
	  
	  dlbody = park.release(); // now restart with the rest
	}
      while(!dlbody.empty());
      
      // done contains all pushed dl-atoms
      dlbody = done.release();
    }
      
  // now get the literals of the pushed dl-atoms
  for (ExtAtomDeque::const_iterator it = dlbody.begin(); it != dlbody.end(); ++it)
    {
      body.insert(it->getLiteral());
    }
}


ExtAtomRewriter::ExtAtomRewriter(const AtomPtr& ea)
  : extAtom(ea)
{
  assert(typeid(ea.get()) != typeid(ExternalAtom));
}


ExtAtomRewriter::ExtAtomRewriter(const ExtAtomRewriter& b)
  : HexDLRewriterBase(),
    extAtom(b.extAtom)
{ }


ExtAtomRewriter::~ExtAtomRewriter()
{ }


ExternalAtom*
ExtAtomRewriter::getExtAtom() const
{
  return dynamic_cast<ExternalAtom*>(extAtom.get());
}


ID
ExtAtomRewriter::getLiteral() const
{
  const std::string& query = getInputTuple().back().getUnquotedString();

  if (query.find("(") != std::string::npos) // (u)cq-atom
    {
      std::ostringstream oss;

      if (query.find(" v ") != std::string::npos) // ucq-atom
	{
	  oss << "dlUCQ" << getOutputTuple().size();
	}
      else // cq-atom
	{
	  oss << "dlCQ" << getOutputTuple().size();
	}

      getExtAtom()->setFunctionName(oss.str());
    }

  return new Literal(extAtom, isNAF());
}


const Tuple&
ExtAtomRewriter::getInputTuple() const
{
  return getExtAtom()->getInputTerms();
}


void
ExtAtomRewriter::getCQ(const std::string& query, const Tuple& output, AtomSet& cq) const
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


std::auto_ptr<ExtAtomRewriter>
ExtAtomRewriter::push(const std::auto_ptr<ExtAtomRewriter>& b) const
{
  const Tuple& input1 = getInputTuple();
  const Tuple& input2 = b->getInputTuple();
  const Tuple& output1 = getOutputTuple();
  const Tuple& output2 = b->getOutputTuple();

  ///@todo for now we don't take unused input predicates into account
  if (std::equal(input1.begin(), input1.end() - 1, input2.begin()) &&
      input1.size() == input2.size() &&
      !isNAF() && !b->isNAF()
      )
    {
      //
      // create output tuple
      //
      std::set<ComfortTerm> termset(output1.begin(), output1.end());
      termset.insert(output2.begin(), output2.end());
      ComfortTuple output3 = ComfortTuple(termset.begin(), termset.end());

      //
      // create input tuple
      //

      // add the lambda components to input3
      ComfortTuple input3 = ComfortTuple(input1.begin(), input1.end() - 1);

      //
      // parse both queries to an AtomSet
      //

      const std::string& query1 = input1.back().getUnquotedString();
      const std::string& query2 = input2.back().getUnquotedString();

      ComfortInterpretation cq1;
      ComfortInterpretation cq2;

      getCQ(query1, output1, cq1);
      getCQ(query2, output2, cq2);

      //
      // get the variables of cq1 and cq2
      //

      std::set<ComfortTerm> X1; // variables of cq1
      std::set<ComfortTerm> X2; // variables of cq2

      // get variables of cq1
      for (ComfortInterpretation::const_iterator it = cq1.begin(); it != cq1.end(); ++it)
	{
	  const ComfortTuple& args = it->getArguments();
	  for (ComfortTuple::const_iterator a = args.begin(); a != args.end(); ++a)
	    {
	      if (a->isVariable())
		X1.insert(*a);
	    }
	}

      // get variables of cq2
      for (ComfortInterpretation::const_iterator it = cq2.begin(); it != cq2.end(); ++it)
	{
	  const ComfortTuple& args = it->getArguments();
	  for (ComfortTuple::const_iterator a = args.begin(); a != args.end(); ++a)
	    {
	      if (a->isVariable())
		X2.insert(*a);
	    }
	}

      //
      // prepare for renaming the variables of cq2
      //

      // get output variables and constants of cq1
      std::set<ComfortTerm> Y1(output1.begin(), output1.end());
      // get output variables and constants of cq2
      std::set<ComfortTerm> Y2(output2.begin(), output2.end());

      // Z1 = X1 \ Y1 , i.e. Z1 contains only existential variables of cq1
      std::set<ComfortTerm> Z1;
      std::set_difference(X1.begin(), X1.end(), Y1.begin(), Y1.end(),
			  std::inserter(Z1, Z1.begin())
			  );
      // Z2 = X2 \ Y2 , i.e. Z2 contains only existential variables of cq2
      std::set<ComfortTerm> Z2;
      std::set_difference(X2.begin(), X2.end(), Y2.begin(), Y2.end(),
			  std::inserter(Z2, Z2.begin())
			  );

      // this is the variable mapping for cq1
      std::map<ComfortTerm,ComfortTerm> vm1;
      // this is the variable mapping for cq2
      std::map<ComfortTerm,ComfortTerm> vm2;

      if (!Z1.empty() || !Z2.empty())
	{
	  // the set of all possible variables of cq1 and cq2
	  std::set<ComfortTerm> vars(X1.begin(), X1.end());
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
	  for (std::set<ComfortTerm>::const_iterator z = Z1.begin(); z != Z1.end(); ++z)
	    {
	      if (X2.find(*z) != X2.end())
		{
		  // rename variable z to lastvar+n and increase n
		  nvar.str("");
		  nvar << lastvar << n++;
		  vm1[*z] = ComfortTerm(nvar.str());
		}
	    }

	  //
	  // for each ex. variable z of Z2 : if z \in X1 then rename z in cq2
	  //
	  for (std::set<ComfortTerm>::const_iterator z = Z2.begin(); z != Z2.end(); ++z)
	    {
	      if (X1.find(*z) != X1.end())
		{
		  // rename variable z to lastvar+n and increase n
		  nvar.str("");
		  nvar << lastvar << n++;
		  vm2[*z] = ComfortTerm(nvar.str());
		}
	    }
	}

      // the new pushed conjunctive query
      ComfortInterpretation cq3;

      //
      // insert variablemapped cq1 into cq3
      //
      for (ComfortInterpretation::const_iterator a1 = cq1.begin(); a1 != cq1.end(); ++a1)
	{
	  ComfortTuple nargs(1, a1->getPredicate()); // a1's predicate + new arguments

	  const ComfortTuple& args = a1->getArguments();

	  // rename variables of a1
	  for (ComfortTuple::const_iterator it = args.begin(); it != args.end(); ++it)
	    {
	      std::map<ComfortTerm,ComfortTerm>::const_iterator var = vm1.find(*it);

	      if (var != vm1.end()) // rename it to var
		{
		  nargs.push_back(var->second);
		}
	      else // leave it as is
		{
		  nargs.push_back(*it);
		}
	    }

	  ComfortAtom ap(nargs, a1->isStronglyNegated());
    /// @todo: this could be a problem
	  //ap->setAlwaysFO();
	  cq3.insert(ap);
	}

      //
      // insert variablemapped cq2 into cq3
      //
      for (ComfortInterpretation::const_iterator a2 = cq2.begin(); a2 != cq2.end(); ++a2)
	{
	  ComfortTuple nargs(1, a2->getPredicate()); // a2's predicate + new arguments

	  const ComfortTuple& args = a2->getArguments();

	  // rename variables of a2
	  for (ComfortTuple::const_iterator it = args.begin(); it != args.end(); ++it)
	    {
	      std::map<ComfortTerm,ComfortTerm>::const_iterator var = vm2.find(*it);

	      if (var != vm2.end()) // rename it to var
		{
		  nargs.push_back(var->second);
		}
	      else // leave it as is
		{
		  nargs.push_back(*it);
		}
	    }

	  ComfortAtom ap(nargs, a2->isStronglyNegated());
    /// @todo: this could be a problem
	  //ap->setAlwaysFO();
	  cq3.insert(ap);
	}

      //
      // append the new conjunctive query cq3 to input3
      //

      std::ostringstream cq3str;

      // cq3 shouldn't be empty
      std::copy(cq3.begin(), --cq3.end(), std::ostream_iterator<Atom>(cq3str, ","));
      cq3str << *(--cq3.end());

      input3.push_back(Term(cq3str.str(), true));

      //
      // setup our external atom for the next push
      //

      getExtAtom()->setArguments(output3);
      getExtAtom()->setInputTerms(input3);

      // and return a new cq-atom rewriter waiting for more to push
      return std::auto_ptr<ExtAtomRewriter>(new ExtAtomRewriter(*this));
    }

  // b is incompatible to this cq-atom
  return std::auto_ptr<ExtAtomRewriter>();
}




DLAtomRewriter::DLAtomRewriter(const Ontology::shared_pointer& onto,
			       DLAtomInput& d,
			       const ComfortInterpretation& ops,
			       const std::string* q,
			       const ComfortTuple* o)
  : HexDLRewriterBase(),
    ontology(onto),
    query(q),
    cq(0),
    ucq(0),
    dlinput(d),
    output(o),
    input(0)
{
  assert(q != 0);
  assert(o != 0);

  if (!ontology)
    {
      throw PluginError("Couldn't rewrite dl-atom, ontology is empty.");
    }

  if (o->size() != 1 && o->size() != 2)
    {
      throw PluginError("dl-atom has wrong arity.");
    }

  ops.matchPredicate("pc", pc);
  ops.matchPredicate("pr", pr);
  ops.matchPredicate("mc", mc);
  ops.matchPredicate("mr", mr);
}


DLAtomRewriter::DLAtomRewriter(const Ontology::shared_pointer& onto,
			       DLAtomInput& d,
			       const ComfortInterpretation& ops,
			       const ComfortInterpretation* c,
			       const ComfortTuple* o)
  : HexDLRewriterBase(),
    ontology(onto),
    query(0),
    cq(c),
    ucq(0),
    dlinput(d),
    output(o),
    input(0)
{
  assert(c != 0);
  assert(o != 0);

  if (!ontology)
    {
      throw PluginError("Couldn't rewrite dl-atom, ontology is empty.");
    }

  ops.matchPredicate("pc", pc);
  ops.matchPredicate("pr", pr);
  ops.matchPredicate("mc", mc);
  ops.matchPredicate("mr", mr);
}


DLAtomRewriter::DLAtomRewriter(const Ontology::shared_pointer& onto,
			       DLAtomInput& d,
			       const ComfortInterpretation& ops,
			       const std::vector<ComfortInterpretation>* u,
			       const ComfortTuple* o)
  : HexDLRewriterBase(),
    ontology(onto),
    query(0),
    cq(0),
    ucq(u),
    dlinput(d),
    output(o),
    input(0)
{
  assert(u != 0);
  assert(o != 0);

  if (!ontology)
    {
      throw PluginError("Couldn't rewrite dl-atom, ontology is empty.");
    }

  ops.matchPredicate("pc", pc);
  ops.matchPredicate("pr", pr);
  ops.matchPredicate("mc", mc);
  ops.matchPredicate("mr", mr);
}


DLAtomRewriter::DLAtomRewriter(const DLAtomRewriter& d)
  : HexDLRewriterBase(d),
    ontology(d.ontology),
    query(d.query),
    cq(d.cq),
    ucq(d.ucq),
    dlinput(d.dlinput),
    output(d.output),
    input(0)
{ }


DLAtomRewriter&
DLAtomRewriter::operator= (const DLAtomRewriter&)
{
  return *this; // ignore
}


DLAtomRewriter::~DLAtomRewriter()
{
  // these are given to us from references to heap-allocated objects, they will be deleted elsewhere!
  //delete query;
  //delete cq;
  //delete ucq;
  //delete output;
  //delete input;
}


ID
DLAtomRewriter::getLiteral() const
{
  AtomPtr ap(new ExternalAtom(getName(), *output, getInputTuple(), 0));
  return new Literal(ap, isNAF());
}


namespace dlvhex {
namespace dl {

  inline std::string
  toURIReference(const Ontology::shared_pointer& ontology, const std::string& s)
  {
    std::string tmp;
    
    if (s.find("\"-") == 0 || s.find("-\"") == 0) ///@todo "-XXX" should never happen!
      {
	tmp = s.substr(2, s.length() - 3);
      }
    else if (s.find("\"") == 0)
      {
	tmp = s.substr(1, s.length() - 2);
      }
    else if (s.find("-") == 0)
      {
	tmp = s.substr(1, s.length() - 1);
      }
    else
      {
	tmp = s;
      }

    return URI::isValid(tmp) ? tmp : ontology->getNamespace() + tmp;
  }

} // namespace dl
} // namespace dlvhex


const Tuple&
DLAtomRewriter::getInputTuple() const
{
  if (this->input != 0)
    {
      return *this->input;
    }

  this->input = new ComfortTuple;

  input->push_back(Term(ontology->getRealURI().getString(), true));

  std::ostringstream oss;

  oss << "dl_pc_" << dlinput.getInputNo(pc);
  input->push_back(Term(oss.str()));
  oss.str("");

  oss << "dl_mc_" << dlinput.getInputNo(mc);
  input->push_back(Term(oss.str()));
  oss.str("");

  oss << "dl_pr_" << dlinput.getInputNo(pr);
  input->push_back(Term(oss.str()));
  oss.str("");

  oss << "dl_mr_" << dlinput.getInputNo(mr);
  input->push_back(Term(oss.str()));
  oss.str("");

  if (query != 0 && cq == 0 && ucq == 0) // dl-atom
    {
      std::string tmpquery = dlvhex::dl::toURIReference(ontology, *query);
      
      // check if query is strongly negated
      if (query->find("\"-") == 0 || query->find("-") == 0)
	{
	  tmpquery = "-" + tmpquery;
	}
      
      input->push_back(ComfortTerm(tmpquery, true));
    }
  else if (query == 0 && cq != 0 && ucq == 0) // cq-atom
    {
      std::ostringstream oss;
      
      if (cq->size() > 1)
	{
	  std::copy(cq->begin(), --cq->end(), std::ostream_iterator<Atom>(oss, ","));
	}
      
      oss << *(--cq->end());
      
      input->push_back(Term(oss.str(), true));
    }
  else if (query == 0 && cq == 0 && ucq != 0) // ucq-atom
    {
      std::ostringstream oss;
      
      for(std::vector<ComfortInterpretation>::const_iterator it = ucq->begin();
	   it != --ucq->end(); ++it)
	{
	  if (it->size() > 1)
	    {
	      std::copy(it->begin(), --it->end(), std::ostream_iterator<Atom>(oss, ","));
	    }
	  
	  oss << *(--it->end()) << " v ";
	}
      
      const ComfortInterpretation& last = *(--ucq->end());
      
      if (last.size() > 1)
	{
	  std::copy(last.begin(), --last.end(), std::ostream_iterator<Atom>(oss, ","));
	}
      
      oss << *(--last.end());
      
      input->push_back(ComfortTerm(oss.str(), true));
    }
  else // programming error
    {
      assert(false);
    }
  
  return *input;
}


std::string
DLAtomRewriter::getName() const
{
  if (query != 0) // dl-atom
    {
      TBox::ObjectsPtr concepts = ontology->getTBox().getConcepts();
      TBox::ObjectsPtr roles = ontology->getTBox().getRoles();
      TBox::ObjectsPtr datatypeRoles = ontology->getTBox().getDatatypeRoles();

      std::string tmpquery = dlvhex::dl::toURIReference(ontology, *query);
      ComfortTerm q(tmpquery);

      if (concepts->find(q) != concepts->end())
	{
	  return "dlC";
	}
      else if (output->size() == 2 && roles->find(q) != roles->end())
	{
	  return "dlR";
	}
      else if (output->size() == 2 && datatypeRoles->find(q) != datatypeRoles->end())
	{
	  return "dlDR";
	}
      else
	{
	  std::ostringstream oss;
	  oss << "Incompatible dl-atom query '" << *query << "' supplied: ";
	  oss << "Cannot find " << q << " in " << ontology->getRealURI();
	  throw PluginError(oss.str());
	}
    }
  else if (cq != 0) // cq-atom
    {
      std::ostringstream oss;
      oss << "dlCQ" << output->size();
      return oss.str();
    }
  else if (ucq != 0) // ucq-atom
    {
      std::ostringstream oss;
      oss << "dlUCQ" << output->size();
      return oss.str();
    }

  assert(false);
}


DLAtomInput::DLAtomInput()
  : asmap(), ncnt(1)
{ }


unsigned
DLAtomInput::getInputNo(const ComfortInterpretation& as)
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


std::vector<ID>
DLAtomInput::getDLInputRules() const
{
  std::vector<ID> rules;

  std::ostringstream oss;
  std::string aux;

  // temp. variable and predicate names
  const ComfortTerm x("X");
  const ComfortTerm y("Y");
  const ComfortTerm pr("pr");
  const ComfortTerm mr("mr");

  for (AtomSetMap::const_iterator it = asmap.begin(); it != asmap.end(); ++it)
    {
      for (ComfortInterpretation::atomset_t::const_iterator a = it->first.atoms.begin();
	   a != it->first.atoms.end(); ++a)
	{
	  // pred \in { pc,mc,pr,mr }
	  ComfortTerm pred = (*a)->getPredicate();

	  // register aux. predicate names, we don't want them to
	  // occur in the answer sets

	  oss.str("");
	  oss << "dl_" << pred << '_' << it->second;
	  aux = oss.str();
	  ComfortTerm::registerAuxiliaryName(aux);

	  // create output variables 

	  ComfortTuple t;
	  t.push_back(x);
	  if (pred == pr || pred == mr)
	    {
	      t.push_back(y);
	    }

	  Tuple body;
	  ComfortAtom((*a)->getArgument(2).getString(), t);
	  body.insert(new Literal(b));

	  // add concept or role to the front of t
	  t.insert(t.begin(), (*a)->getArgument(1));

	  Tuple head;
	  ComfortAtom haux, t);
	  head.insert(h);
	  
          Rule r(head, body);
	  rules.push_back(reg->storeRule(r));
 	}
    }

  return rules;
}
#endif

  } // namespace dl
} // namespace dlvhex

// Local Variables:
// mode: C++
// End:

