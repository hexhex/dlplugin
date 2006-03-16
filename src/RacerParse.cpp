/* -*- C++ -*- */

/**
 * @file   RacerParse.cpp
 * @author Thomas Krennwallner
 * @date   Sat Jul 16 11:39:20 2005
 * 
 * @brief  Various parsers for RACER answers.
 * 
 * 
 */

#include "RacerParse.h"

#include "RacerQuery.h"

#include <iostream>
#include <sstream>

using namespace dlvhex::racer;



RacerParse::RacerParse(std::istream& s)
  : stream(s)
{ }

RacerParse::~RacerParse()
{ }

void
RacerParse::parse(Answer& answer) throw (RacerParsingError)
{
  // RACER   ::= ':' (ANSWER | ERROR)
  // ANSWER  ::= 'answer ' COUNT ' ' LIST ' "' NS? (' ' NS)* '"'
  // ERROR   ::= 'error ' COUNT ' ' MESSAGE ' ""'
  // COUNT   ::= DIGIT+
  // MESSAGE ::= (CHAR | DIGIT | SYMB)+
  // DIGIT   ::= '0' | ... | '9'
  // CHAR    ::= 'a' | ... | 'z' | 'A' | ... | 'Z'
  // SYMB    ::= '(' | ')' | ...
  // LIST    ::= '"' ('NIL' | '(' INDVAR (' ' (LIST | INDVAR))* ')') '"'
  // INDVAR  ::= '|' NS? INDV '|'
  // NS      ::= ('file' | 'http') '://' CHAR+ '#'
  // INDV    ::= (CHAR | DIGIT) INDV*

  // no need for fancy parsing when stream is empty
  // also prevents unneeded multiple calls to parse()
  if (stream.eof())
    {
      return;
    }

  try
    {
      stream.ignore(); // ignore ':'

      std::string anserr;
      int count;
      stream >> anserr >> count;

      if (anserr == "answer")
	{
	  std::stringbuf sb;
      
	  stream.ignore(2);     // ignore ' "'
	  stream.get(sb, '\"'); // get whole LIST until next '"'
	  stream.ignore();      // ignore closing '"'
	  std::string ans = sb.str();

	  
	  sb.str("");      // clear stringbuf
	  stream.ignore(); // ignore ' '
	  stream.get(sb);  // get whole NS list
	  std::string nspace = sb.str();

	  // remove unconsumed characters to allow consecutive calls to parse()
	  stream.sync();

	  // call Template Method for further parsing
	  parseAnswer(answer, ans, nspace);
	}
      else // error
	{
	  std::stringbuf sb;
	  stream.ignore(); // ignore ' '
	  stream.get(sb);
	  std::string ans = sb.str();

	  ///@todo do we need a sync on stream?

	  answer.setErrorMessage(ans);

	  // if kb gets inconsistent just return and set incoherent flag
	  if (ans.find("incoherent") != std::string::npos)
	    {
	      answer.setIncoherent(true);
	      return;
	    }

	  throw RacerParsingError("Received error message \"" + ans + "\"");
	}
    }
  catch (std::exception& e)
    {
      throw RacerParsingError(e.what());
    }
}



RacerIgnoreAnswer::RacerIgnoreAnswer(std::istream& s)
  : RacerParse(s)
{ }

RacerIgnoreAnswer::~RacerIgnoreAnswer()
{ }

void
RacerIgnoreAnswer::parse(Answer&) throw (RacerParsingError)
{
  if (!stream.eof())
    {
      try
	{
	  // get all characters from stream until '\n'
	  std::stringbuf sb;
	  stream.get(sb);

	  // remove unconsumed characters to allow consecutive calls to parse()
	  stream.sync();
	}
      catch (std::exception& e)
	{
	  throw RacerParsingError(e.what());
	}
    }
}

void
RacerIgnoreAnswer::parseAnswer(Answer&,
			       std::string&,
			       std::string&
			       ) const
{
  // just ignore answer
}



RacerSimpleAnswer::RacerSimpleAnswer(std::istream& s)
  : RacerParse(s)
{ }

RacerSimpleAnswer::~RacerSimpleAnswer()
{ }

void
RacerSimpleAnswer::parseAnswer(Answer&,
			       std::string&,
			       std::string&
			       ) const throw (RacerParsingError)
{
//   if (ans != "NIL")
//     {
//       throw RacerParsingError("Got wrong answer: " + ans + ", expected NIL.");
//     }
}



RacerBooleanAnswer::RacerBooleanAnswer(std::istream& s)
  : RacerParse(s)
{ }

RacerBooleanAnswer::~RacerBooleanAnswer()
{ }

void
RacerBooleanAnswer::parseAnswer(Answer& answer,
				std::string& ans,
				std::string&
				) const throw (RacerParsingError)
{
  if (ans == "T")
    {
      answer.setAnswer(true);
      answer.addTuple(Tuple());
    }
  else if (ans == "NIL")
    {
      answer.setAnswer(false);
    }
  else
    {
      throw RacerParsingError("Got wrong answer: " + ans + ", expected T/NIL.");
    }
}



RacerAnswerList::RacerAnswerList(std::istream& s)
  : RacerParse(s)
{ }

RacerAnswerList::~RacerAnswerList()
{ }

Term
RacerAnswerList::parseTerm(const std::string& term) const
{
  if (term[0] == '|' && term[term.length() - 1] == '|') // object type
    {
      // remove '|' ... '|'
      std::string tmp = term.substr(1, term.length() - 2);
      
      std::string::size_type i = tmp.find_last_of('#');

      // strip xml namespace
      tmp.erase(0, i+1);

      // return string Term
      return Term(tmp, true);
    }
  else
    {
      ///@todo right now, only integer data types are supported. For
      ///string data types (escaped with \") we need a better parsing
      ///engine.
      int i;
      std::istringstream iss(term);

      iss >> i;

      // return integer Term
      return Term(i);
    }
}

void
RacerAnswerList::parseAnswer(Answer& answer,
			     std::string& ans,
			     std::string&
			     ) const throw (RacerParsingError)
{
  try
    {
      if (ans == "NIL")
	{
	  return;
	}

      // remove '(' ... ')'
      ans.erase(0, 1);
      ans.erase(ans.size() - 1, 1);

      std::istringstream iss(ans);
      std::string term;
      Tuple tl;

      while (!iss.eof())
	{
	  term.clear();
	  tl.clear();

	  iss >> term;

	  // add the Term
	  tl.push_back(parseTerm(term));

	  answer.addTuple(tl);
	}
    }
  catch(std::exception& e)
    {
      throw RacerParsingError(e.what());
    }
}


RacerAnswerPairList::RacerAnswerPairList(std::istream& s)
  : RacerAnswerList(s)
{ }

RacerAnswerPairList::~RacerAnswerPairList()
{ }

void
RacerAnswerPairList::parseAnswer(Answer& answer,
				 std::string& ans,
				 std::string&
				 ) const throw (RacerParsingError)
{
  try
    {
      if (ans == "NIL")
	{
	  return; // empty list -> nothing to do
	}

      // remove '(' ... ')'
      ans.erase(0, 1);
      ans.erase(ans.size() - 1, 1);

      std::istringstream iss(ans);
      std::string term1;
      std::string term2;
      Tuple tl;

      while (!iss.eof())
	{
	  tl.clear();
	  term1.clear();
	  term2.clear();

	  iss >> term1 >> term2;

	  term1.erase(0, 1);                // remove '('
	  term2.erase(term2.size() - 1, 1); // remove ')'

	  // add pair of Terms
	  tl.push_back(parseTerm(term1));
	  tl.push_back(parseTerm(term2));

	  answer.addTuple(tl);
	}
    }
  catch(std::exception& e)
    {
      throw RacerParsingError(e.what());
    }
}


RacerRetrieveList::RacerRetrieveList(std::istream& s)
  : RacerAnswerList(s)
{ }

RacerRetrieveList::~RacerRetrieveList()
{ }

void
RacerRetrieveList::parseAnswer(Answer& answer,
			       std::string& ans,
			       std::string&
			       ) const throw (RacerParsingError)
{
  try
    {
      if (ans == "NIL")
	{
	  return; // empty list -> nothing to do
	}

      // remove '(' ... ')'
      ans.erase(0, 1);
      ans.erase(ans.size() - 1, 1);

      std::istringstream iss(ans);
      Tuple tl;

      while (!iss.eof())
	{
	  if (iss.get() == '(') // a new tuple ((var term) (var term) ... )
	    {
	      tl.clear();

	      while (iss.peek() != ')') // parse all upcoming (var term) pairs
		{
		  std::stringbuf sb;
		  sb.str("");
		  
		  iss.ignore(); // ignore '('
		  iss.get(sb, ')');
		  iss.ignore(); // ignore ')'
		  if (iss.peek() == ' ') iss.ignore();

		  std::string s = sb.str(); // a pair of "variable term"

		  std::string::size_type pos = s.find(' ');

		  if (std::string::npos == pos)
		    {
		      throw RacerParsingError("RacerParse just made a booboo, bailing out.");
		    }

		  // add term to the tuple
		  tl.push_back(parseTerm(s.substr(pos + 1)));
		}

	      answer.addTuple(tl);
	      
	      iss.ignore();
	    }
	}
    }
  catch(std::exception& e)
    {
      throw RacerParsingError(e.what());
    }
}
