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
  return os;
}


BodyRewriter::BodyRewriter()
  : body()
{ }


void
BodyRewriter::add(HexDLRewriterBase* atom)
{
  body.push_back(atom);
}


LiteralRewriter::LiteralRewriter(const std::string* l)
  : literal(l)
{ }


LiteralRewriter::LiteralRewriter(const LiteralRewriter& r)
  : HexDLRewriterBase(),
    literal(new std::string(*r.literal))
{ }


LiteralRewriter&
LiteralRewriter::operator= (const LiteralRewriter&)
{
  return *this; // ignore
}


LiteralRewriter::~LiteralRewriter()
{
  delete literal;
}


std::ostream&
LiteralRewriter::rewrite(std::ostream& os) const
{
  return os << *literal;
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

  os << input->back() << ']';

  // append output list

  if (!output->empty())
    {
      os << '(';
	      
      std::copy(output->begin(), output->end() - 1,
		std::ostream_iterator<Term>(os, ",")
		);
      
      os << output->back() << ')';
    }

  return os;
}



DLAtomRewriter::DLAtomRewriter(const Ontology::shared_pointer& onto,
			       int e,
			       const std::string* q,
			       const std::string* t1,
			       const std::string* t2)
  : ontology(onto), extAtomNo(e), query(q), out1(t1), out2(t2)
{
  assert(query != 0);
  assert(out1 != 0);

  if (!ontology)
    {
      throw PluginError("Couldn't rewrite dl-atom, ontology is empty.");
    }
}


DLAtomRewriter::DLAtomRewriter(const DLAtomRewriter& d)
  : HexDLRewriterBase(),
    ontology(d.ontology),
    extAtomNo(d.extAtomNo),
    query(d.query),
    out1(d.out1),
    out2(d.out2)
{ }


DLAtomRewriter&
DLAtomRewriter::operator= (const DLAtomRewriter&)
{
  return *this; // ignore
}


DLAtomRewriter::~DLAtomRewriter()
{
  delete query;
  delete out1;
  delete out2;
}


std::ostream&
DLAtomRewriter::rewrite(std::ostream& os) const
{
  // first commandment: stream thy strings

  TBox::ObjectsPtr concepts = ontology->getTBox().getConcepts();
  TBox::ObjectsPtr roles = ontology->getTBox().getRoles();
  TBox::ObjectsPtr datatypeRoles = ontology->getTBox().getDatatypeRoles();

  std::string tmpquery;
  bool isNegated = false;

  ///@todo always add namespace?
  if (query->find("\"-") == 0)
    {
      isNegated = true;
      tmpquery = ontology->getNamespace() + query->substr(2, query->length() - 3);
    }
  else if (query->find("\"") == 0)
    {
      tmpquery = ontology->getNamespace() + query->substr(1, query->length() - 2);
    }
  else if (query->find("-") == 0)
    {
      isNegated = true;
      tmpquery = ontology->getNamespace() + query->substr(1, query->length() - 1);
    }
  else
    {
      tmpquery = ontology->getNamespace() + *query;
    }

  Term t(tmpquery);

  if (concepts->find(t) != concepts->end())
    {
      os << "&dlC";
    }
  else if (roles->find(t) != roles->end() && out2)
    {
      os << "&dlR";
    }
  else if (datatypeRoles->find(t) != datatypeRoles->end() && out2)
    {
      os << "&dlDR";
    }
  else
    {
      throw PluginError("Incompatible dl-atom query supplied.");
    }

  // output external atoms input list
  os << "[\"" 
     << ontology->getRealURI() 
     << "\",dl_pc_"
     << extAtomNo
     << ",dl_mc_"
     << extAtomNo
     << ",dl_pr_"
     << extAtomNo
     << ",dl_mr_"
     << extAtomNo
     << ",\""
     << (isNegated ? "-" + tmpquery : tmpquery)
     << "\"]";

  // append output list of the ext. atom

  if (out2)
    {
      os << '(' << *out1 << ',' << *out2 << ')';
    }
  else
    {
      os << '(' << *out1 << ')';
    }

  return os;
}



