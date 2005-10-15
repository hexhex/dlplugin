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

      // incoherent?
      if (ans.find("incoherent") != std::string::npos)
	{
	  ///@todo do we need to throw an exception if kb is inconsistent?
	  answer.setIncoherent(true);
	}

      throw RacerParsingError("Received error message \"" + ans + "\"");
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

std::string&
RacerAnswerList::parseTerm(std::string& term) const
{
  // remove '|' ... '|'
  term.erase(0, 1);
  term.erase(term.length() - 1, 1);

  // strip xml namespace
  std::string::size_type i = term.find_last_of('#');
  term.erase(0, i+1);

  return term;
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

	  // add single quoted Term
	  tl.push_back(Term(parseTerm(term), true));

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
	  return;
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
	  iss.ignore(); // ignore '('

	  tl.clear();
	  term1.clear();
	  term2.clear();

	  iss >> term1 >> term2;

	  term2.erase(term2.size() - 1, 1); // remove ')'

	  // add pair of quoted terms
	  tl.push_back(Term(parseTerm(term1), true));
	  tl.push_back(Term(parseTerm(term2), true));

	  answer.addTuple(tl);
	}
    }
  catch(std::exception& e)
    {
      throw RacerParsingError(e.what());
    }
}
