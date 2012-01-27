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
 * @file   RacerAnswerDriver.cpp
 * @author Thomas Krennwallner
 * @date   Sat Mar 18 15:04:27 2006
 * 
 * @brief Driver classes which encapsulates the bison parser and flex
 * lexer.
 * 
 * 
 */

//#define CWDEBUG
//#define BOOST_SPIRIT_DEBUG

#ifdef CWDEBUG
# define _GNU_SOURCE
# include <libcwd/sys.h>
# include <libcwd/debug.h>
#endif

#ifdef BOOST_SPIRIT_DEBUG
# define BOOST_SPIRIT_DEBUG_OUT std::cerr
#endif

#include "RacerAnswerDriver.h"
#include "Answer.h"
#include "DLError.h"
#include "Registry.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <sstream>

using namespace dlvhex::dl::racer;

namespace spirit = boost::spirit;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;

namespace
{

//
// useful phoenix operator debugging stuff:
// -> just add [ handle_dbg("message") ] to any rule/parser to get information
//
#if 1
template<typename Attrib>
void houtput(Attrib const& a)
{
#ifdef CWDEBUG
  std::cerr << "XXX handling attribute " << libcwd::type_info_of(a).demangled_name() << std::endl;
#endif
};

template<>
void houtput(std::vector<char> const& a)
{
  std::cerr << "XXX got string  attribute '" << std::string(a.begin(), a.end()) << "'" << std::endl;
}

template<typename Content>
void houtput(boost::optional<Content> const& a)
{
  if( !a )
  {
    std::cerr << "XXX optional (unset):" << std::endl;
  }
  else
  {
    std::cerr << "XXX optional:" << std::endl;
  }
  houtput(a.get());
}

struct handle_dbg
{
  handle_dbg(std::string s): s(s) {}
  std::string s;
  template<typename Attrib>
  void operator()(Attrib& a, qi::unused_type, qi::unused_type) const
  {
    std::cerr << "DBG=" << s << std::endl;
    houtput(a);
  }
};
#endif

struct AnswerState
{
  AnswerState(dlvhex::dl::Answer& answer):
    answer(answer) {}

  dlvhex::dl::Answer& answer;
};

struct handle_error
{
  handle_error(AnswerState& state): state(state) {}

  void operator()(fusion::vector2<std::vector<char>, std::vector<char> >& messages, qi::unused_type, qi::unused_type) const
  {
    dlvhex::dl::Answer& answer = state.answer;

    std::string error(fusion::at_c<0>(messages).begin(), fusion::at_c<0>(messages).end());
    std::string warning(fusion::at_c<1>(messages).begin(), fusion::at_c<1>(messages).end());

    answer.setErrorMessage(error);
    answer.setWarningMessage(warning);

    //std::cerr << "parsed error message '" << error << "' and warning message '" << warning << "'!" << std::endl;

     // Racer returns "ABox XXX is incoherent." for 'old' Racer
     // queries, and "ABox XXX is inconsistent! Querying denied."
     // for nRQL queries.
     if( answer.getErrorMessage().find("incoherent") != std::string::npos ||
         answer.getErrorMessage().find("inconsistent") != std::string::npos)
     {
         // if kb gets inconsistent just return without error and
         // set incoherent flag
       answer.setIncoherent(true);
     }
     else
     {
       throw dlvhex::dl::DLParsingError(answer.getErrorMessage());
     }
  }

  AnswerState& state;
};

struct handle_answer
{
  handle_answer(AnswerState& state): state(state) {}

  void operator()(std::vector<char>& warning, qi::unused_type, qi::unused_type) const
  {
    dlvhex::dl::Answer& answer = state.answer;

    std::string wrn(warning.begin(), warning.end());
    answer.setWarningMessage(wrn);
    //std::cerr << "for previous command: parsed warning message '" << wrn << "'!" << std::endl;
  }

  AnswerState& state;
};

struct handle_individuals
{
  handle_individuals(AnswerState& state): state(state) {}

  void operator()(std::vector<dlvhex::ComfortTerm>& terms, qi::unused_type, qi::unused_type) const
  {
    dlvhex::dl::Answer& answer = state.answer;

    //std::cerr << "creating individuals" << std::endl;
    BOOST_FOREACH(dlvhex::ComfortTerm& t, terms)
    {
      //std::cerr << "adding individual '" << t << "'!" << std::endl;
      answer.insert(dlvhex::ComfortTuple(1, t));
    }
  }

  AnswerState& state;
};

struct handle_tuples
{
  handle_tuples(AnswerState& state): state(state) {}

  void operator()(std::vector<dlvhex::ComfortTuple>& tuples, qi::unused_type, qi::unused_type) const
  {
    dlvhex::dl::Answer& answer = state.answer;

    BOOST_FOREACH(dlvhex::ComfortTuple& t, tuples)
    {
      //std::cerr << "adding individual '" << t << "'!" << std::endl;
      answer.insert(t);
    }
  }

  AnswerState& state;
};

struct handle_boolean
{
  handle_boolean(AnswerState& state, bool val): state(state), val(val) {}

  void operator()(qi::unused_type, qi::unused_type, qi::unused_type) const
  {
    dlvhex::dl::Answer& answer = state.answer;

    //std::cerr << "created boolean " << val << std::endl;
    answer.setAnswer(val);
    if( val )
      answer.insert(dlvhex::ComfortTuple());
  }

  AnswerState& state;
  bool val;
};

struct handle_uri
{
  template <typename Context>
  void operator()(std::string& s, Context& ctx, qi::unused_type) const
  {
    dlvhex::ComfortTerm& ruleAttr = fusion::at_c<0>(ctx.attributes);

    ruleAttr = dlvhex::ComfortTerm::createConstant("\"<" + s + ">\"");
    //std::cerr << "created uri '<" << s << ">'" << std::endl;
  }
};

template<typename Iterator>
struct RacerAnswerGrammar: qi::grammar<Iterator, ascii::space_type>
{
  RacerAnswerGrammar(AnswerState& state):
    RacerAnswerGrammar::base_type(answer), state(state)
  {
    using ascii::no_case;
    using spirit::int_;
    using spirit::_val;
    using spirit::_1;
    using qi::lexeme;
    using qi::char_;
    using qi::omit;
    using qi::lit;
    answer =
       (omit[no_case[":error"] >> *char_("0-9")] >>
        lexeme[*(char_ - char_('\"'))] >> omit['\"'] >>
        lexeme[*(char_ - char_('\"'))] >> omit['\"'] >> (qi::eol | qi::eoi))
          [ handle_error(state) ]
      |(omit[no_case[":answer"] >> *(char_ - char_('\"')) >> '\"'] >>
        (boolean | list | status) >> omit[lit('\"') >> '\"'] >>
        lexeme[*(char_ - char_('\"'))] >> omit['\"'] >> (qi::eol | qi::eoi))
          [ handle_answer(state) ]
      ;

    boolean =
        no_case["t"]   [ handle_boolean(state, true) ] 
      | no_case["nil"] [ handle_boolean(state, false) ] 
      ;

    list =
      (
        ('(' >> +tuple >> ')')       [ handle_tuples(state) ]
      | ('(' >> +pair >> ')')        [ handle_tuples(state) ]
      | ('(' >> +individual >> ')')  [ handle_individuals(state) ]
      );

    pair %=
      '(' >> individual >> individual >> ')';

    status =
        (':' >> lexeme[*(char_ - char_('\"'))])
      | uri
      | lexeme[*(char_ - char_('\"'))]
      ;

    // a tuple is a nonempty repetition of "( var individual )"
    tuple %=
      ('(' >> +(
                 omit['(' >> variable] >> individual >> omit[')']
               ) >> ')');

    variable =
        ( (lit("$?*")|lit("$?")|lit("?*")|lit("?")) >> lexeme[+(char_ - ascii::space)])
      | ('|' >> (lit("?")|lit("$?")) >> uri >> '|')
      ;

    individual =
        uri    [ handle_uri() ]
      | int_   [ _val = phoenix::construct<dlvhex::ComfortTerm>(_1) ]	
      | string [ _val = phoenix::construct<dlvhex::ComfortTerm>(_1, true) ]
      ;

    uri %=
      '|' >> lexeme[*(char_ - char_('|'))] >> '|';

    string %=
        (lit("\\\"") >> lexeme[*(char_ - char_('\\'))] >> lit("\\\""))
      | lexeme[+(char_ - (char_("\"()")|ascii::space))]
      ;

#   ifdef BOOST_SPIRIT_DEBUG
    BOOST_SPIRIT_DEBUG_NODE(answer);
    BOOST_SPIRIT_DEBUG_NODE(boolean);
    BOOST_SPIRIT_DEBUG_NODE(list);
    BOOST_SPIRIT_DEBUG_NODE(pair);
    BOOST_SPIRIT_DEBUG_NODE(status);
    BOOST_SPIRIT_DEBUG_NODE(tuple);
    BOOST_SPIRIT_DEBUG_NODE(variable);
    BOOST_SPIRIT_DEBUG_NODE(individual);
    BOOST_SPIRIT_DEBUG_NODE(uri);
    BOOST_SPIRIT_DEBUG_NODE(string);
#   endif
  }

  qi::rule<Iterator, ascii::space_type> answer, boolean, list, status, variable;
  qi::rule<Iterator, dlvhex::ComfortTuple(), ascii::space_type> tuple, pair;
  qi::rule<Iterator, dlvhex::ComfortTerm(), ascii::space_type> individual;
  qi::rule<Iterator, std::string(), ascii::space_type> uri, string;

  AnswerState state;
};

}

RacerBaseAnswerDriver::RacerBaseAnswerDriver(std::istream& i)
  : QueryBaseParser(i)
{ }


RacerBaseAnswerDriver::RacerBaseAnswerDriver(const RacerBaseAnswerDriver& d)
  : QueryBaseParser(d)
{ }


RacerBaseAnswerDriver&
RacerBaseAnswerDriver::operator= (const RacerBaseAnswerDriver&)
{
  return *this; // ignore
}


RacerBaseAnswerDriver::~RacerBaseAnswerDriver()
{
}

void
RacerBaseAnswerDriver::syncStream()
{
  // sync and clear stream s.t. consecutive reading on the stream
  // works. Otherwise we would need to create a dedicated iostream for
  // each Racer command.

  stream.sync();
  stream.clear();
}


RacerAnswerDriver::RacerAnswerDriver(std::istream& i)
  : RacerBaseAnswerDriver(i)
{ }



void
RacerAnswerDriver::parse(Answer &a) throw (DLParsingError)
{
  try
  {
    #warning TODO use stream iterator and create better error messages!
    std::ostringstream buf;
    buf << stream.rdbuf();
    std::string input = buf.str();

    #ifdef BOOST_SPIRIT_DEBUG
    std::cerr <<
      "$$$Parsing Racer Input$$$" << std::endl <<
      input << std::endl <<
      "$$$" << std::endl;
    #endif

    AnswerState state(a);
    RacerAnswerGrammar<std::string::const_iterator> grammar(state);

    std::string::const_iterator iter = input.begin();
    std::string::const_iterator end = input.end();
    bool r = qi::phrase_parse(iter, end, grammar, ascii::space);

    //std::cerr << "parsing ended with " << !r << " and " << (iter != end) << std::endl;
    if (!r || iter != end)
    {
      throw DLParsingError("failed parsing!");
    }
  }
  catch (std::ios_base::failure& f)
  {
    throw DLParsingError(f.what());
  }
}

RacerIgnoreAnswer::RacerIgnoreAnswer(std::istream& s)
  : RacerBaseAnswerDriver(s)
{ }


void
RacerIgnoreAnswer::parse(Answer&) throw (DLParsingError)
{
  // Just skip the answer. This is useful for such "glorious" Racer
  // responses as "T" for a (set-unique-name-assumption t). Otherwise
  // we would end up with an additional empty tuple in the Answer
  // object.

  try
  {
      #ifdef BOOST_SPIRIT_DEBUG
      std::cout << "$$$Ignoring Racer Input$$$" << std::endl;
      #endif

      char tmp[256];
      
      while (!stream.eof() && !stream.fail())
	{
	  stream.read(tmp, 256);
	}

      // reset stream so we can reutilize it
      syncStream();
    }
  catch (std::ios_base::failure& f)
    {
      throw DLParsingError(f.what());
    }

}


// Local Variables:
// mode: C++
// End:
