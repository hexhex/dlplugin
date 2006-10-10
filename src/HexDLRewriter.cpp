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


void
HexDLRewriterBase::setNAF(bool n)
{
  this->naf = n;
}


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
BodyRewriter::add(CQAtomRewriter* atom)
{
  dlbody.push_back(atom);
}

void
BodyRewriter::add(DLAtomRewriter* atom)
{
  dlbody.push_back(atom);
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
  for (boost::ptr_vector<HexDLRewriterBase>::const_iterator it = dlbody.begin();
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


CQAtomRewriter::CQAtomRewriter(Tuple* in, Tuple* out)
  : input(in), output(out)
{
  assert(input != 0);
  assert(output != 0);
}


CQAtomRewriter::CQAtomRewriter(const CQAtomRewriter& c)
  : HexDLRewriterBase(),
    input(c.input),
    output(c.output)
{ }


CQAtomRewriter&
CQAtomRewriter::operator= (const CQAtomRewriter&)
{
  return *this; // ignore
}


CQAtomRewriter::~CQAtomRewriter()
{
  delete input;
  delete output;
}


std::ostream&
CQAtomRewriter::rewrite(std::ostream& os) const
{
  // rewrite external cq-atom plus arity

  os << "&dlCQ" << output->size();

  // append input list (we assume a non-empty input list)

  os << '[';

  std::copy(input->begin(), input->end() - 1,
	    std::ostream_iterator<Term>(os, ",")
	    );

  os << input->back() << "](";

  // append output list

  if (!output->empty())
    {
      std::copy(output->begin(), output->end() - 1,
		std::ostream_iterator<Term>(os, ",")
		);
      
      os << output->back();
    }

  return os << ')';
}


DLAtomRewriter::DLAtomRewriter(const Ontology::shared_pointer& onto,
			       const AtomSet& ops,
			       const std::string* q,
			       const Tuple* o)
  : ontology(onto), query(q), out(o)
{
  assert(query != 0);
  assert(out->size() != 0);

  if (!ontology)
    {
      throw PluginError("Couldn't rewrite dl-atom, ontology is empty.");
    }

  TBox::ObjectsPtr concepts = ontology->getTBox().getConcepts();
  TBox::ObjectsPtr roles = ontology->getTBox().getRoles();
  TBox::ObjectsPtr datatypeRoles = ontology->getTBox().getDatatypeRoles();

  // dispatch dl-atom-ops
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
	  if (pred == Term("p"))
	    pc.insert(ap);
	  else if (pred == Term("m"))
	    mc.insert(ap);
	}
      else if (roles->find(t) != roles->end() ||
	       datatypeRoles->find(t) != datatypeRoles->end())
	{
	  if (pred == Term("p"))
	    pr.insert(ap);
	  else if (pred == Term("m"))
	    mr.insert(ap);
	}
      else
	{
	  throw PluginError("Incompatible dl-atom-op " + tmpstr + " supplied.");
	}
    }
}


DLAtomRewriter::DLAtomRewriter(const DLAtomRewriter& d)
  : HexDLRewriterBase(),
    ontology(d.ontology),
    query(d.query),
    out(d.out)
{ }


DLAtomRewriter&
DLAtomRewriter::operator= (const DLAtomRewriter&)
{
  return *this; // ignore
}


DLAtomRewriter::~DLAtomRewriter()
{
  delete query;
  delete out;
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


unsigned
DLAtomRewriter::getInputNo(const AtomSet& as) const
{
  typedef std::map<AtomSet,unsigned> AtomSetMap;
  static unsigned ncnt = 1;
  static AtomSetMap asmap;

  if (as.empty())
    {
      return 0; // no dl-atom-ops
    }

  std::pair<AtomSetMap::iterator,bool> p = asmap.insert(std::make_pair(as, ncnt));

  if (p.second)
    {
      ncnt++;
    }

  return p.first->second;
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

  // check if query is strongly negated
  if (query->find("\"-") == 0 || query->find("-") == 0)
    {
      tmpquery = "-" + tmpquery;
    }

  if (concepts->find(q) != concepts->end())
    {
      os << "&dlC";
    }
  else if (out->size() == 2 && roles->find(q) != roles->end())
    {
      os << "&dlR";
    }
  else if (out->size() == 2 && datatypeRoles->find(q) != datatypeRoles->end())
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

  os << "[\"" << ontology->getRealURI() << "\",";

  os << "dl_pc_" << getInputNo(pc) << ',';
  os << "dl_mc_" << getInputNo(mc) << ',';
  os << "dl_pr_" << getInputNo(pr) << ',';
  os << "dl_mr_" << getInputNo(mr) << ',';

  os << '\"' << tmpquery << "\"]";

  //
  // append output list of the ext. atom
  //

  if (out->size() == 2)
    {
      os << '(' << out->front() << ',' << out->back() << ')';
    }
  else if (out->size() == 1)
    {
      os << '(' << out->front() << ')';
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

  unsigned pcno = getInputNo(pc);
  unsigned mcno = getInputNo(mc);
  unsigned prno = getInputNo(pr);
  unsigned mrno = getInputNo(mr);

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
