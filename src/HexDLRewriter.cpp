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


void
BodyRewriter::add(HexDLRewriterBase* atom)
{
  body.push_back(atom);
}


LiteralRewriter::LiteralRewriter(const std::string* l)
  : literal(l)
{ }


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



DLAtomRewriter::DLAtomRewriter(const std::string& u, int e,
			       const std::string* q,
			       const std::string* t1,
			       const std::string* t2)
  : uri(u), extAtomNo(e), query(q), out1(t1), out2(t2)
{
  assert(query != 0);
  assert(out1 != 0);

  if (uri.empty())
    {
      throw PluginError("Couldn't rewrite dl-atom, ontology URI is empty.");
    }
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

  if (out2) // concept or role dl-atom?
    {
      os << "&dlR";
    }
  else
    {
      os << "&dlC";
    }

  // output external atoms input list
  os << "[\"" 
     << uri 
     << "\",dl_pc_"
     << extAtomNo
     << ",dl_mc_"
     << extAtomNo
     << ",dl_pr_"
     << extAtomNo
     << ",dl_mr_"
     << extAtomNo
     << ','
     << ((*query)[0] != '"' ? "\"" + *query + "\"" : *query)
     << ']';

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



