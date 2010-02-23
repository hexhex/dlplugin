/**
 * TODO copyright etc.
 *
 * This file contains the base converter used for DL syntactic sugar: the parser
 * detects DL syntactic sugar and passes it on to the DLAtomConverter for
 * rewriting, everything that is not DL syntactic sugar is simply streamed out
 * output unaltered.
 */

//#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_eoi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi_match.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
//#include <boost/spirit/include/phoenix_core.hpp>

#define BOOST_SPIRIT_LEXERTL_DEBUG
#define BOOST_SPIRIT_DEBUG

using namespace boost::spirit;
using namespace boost::spirit::ascii;

#if 0
// required for lexer tokens
namespace boost { namespace spirit { namespace traits
{
  template <typename Iterator>
  struct assign_to_attribute_from_iterators<std::string, Iterator>
  {
    static void 
    call(Iterator const& first, Iterator const& last, std::string& attr)
    {
      attr = std::string(first, last);
      std::cerr << "converted string '" << attr << "'" << std::endl;
      /*
      int x, y;
      Iterator b = first;
      qi::parse(b, last, 
          '{' >> qi::int_ >> ',' >> qi::int_ >> '}', x, y);
      attr = rational(x, y);
      */
    }
  };
}}}
#endif

namespace {

// custom semantic action function object to switch the state of the lexer
// (taken from strip_comments_lexer.cpp boost example)
struct set_lexer_state
{
  set_lexer_state(char const* state_):
    state(state_) {}

  // This is called by the semantic action handling code during the lexing
  template <typename Iterator, typename Context>
  void operator()(Iterator const&, Iterator const&,
    BOOST_SCOPED_ENUM(boost::spirit::lex::pass_flags)&,
    std::size_t&, Context& ctx) const
  {
    ctx.set_state_name(state.c_str());
  }

  std::string state;
};

/**
 * This lexer recognizes comments and quoted strings (with possibility to contain \").
 *
 * This lexer is used by a grammar which recognize DL atoms in all possible forms,
 * further analyzes them and rewrites them.
 */
template<typename Lexer>
struct DLLexer: lex::lexer<Lexer>
{
  DLLexer():
    DLLexer::base_type(lex::match_flags::match_default)
  {
    // find comments and strings in regular text
    comment = "[ \t]*%.*$";
    stringstart = "\\\"";
    newline = "\r?\n";
    any1 = ".";
    this->self =
        // comments
        comment
        // string starter
      | stringstart [ set_lexer_state("INSTRING") ]
      | newline
      | any1;

    // wait for final " of string allow escaped " in string
    stringend = "\\\"";
    // this is the regex for matching the following string within the ': '\"'
    escaped_stringend = "\\\\\\\"";
    any2 = ".";
    this->self("INSTRING") =
        stringend [ set_lexer_state("INITIAL") ]
      | escaped_stringend [ lex::_start++ ]
      | any2;
  }

  lex::token_def<> newline;
  lex::token_def<char> stringstart, any1;
  lex::token_def<std::string> comment;

  lex::token_def<char> stringend, escaped_stringend, any2;
};

namespace
{
  void do_dlatom(const std::string& s)
  {
    std::cerr << "found dlatom '" << s << "'" << std::endl;
  }
  void do_passthrough(const std::string& s)
  {
    std::cerr << "found passthrough '" << s << "'" << std::endl;
  }
}

/**
 * This grammar uses the above lexer to pick DL atoms out of an incoming data stream, and to rewrite
 * just these atoms and passthrough the rest of the input.
 */
template <typename Iterator>
struct DLGrammar: qi::grammar<Iterator>
{
  template <typename TokenDef>
  DLGrammar(TokenDef const& tok, std::ostream& out):
    DLGrammar::base_type(root),
    outStream(out)
  {
    using qi::lexeme;
    using qi::char_;
    root =
      *( dlatom //[ &do_dlatom ] // TODO semantic action
       | passthrough //[ &do_passthrough] // TODO semantic action
       ) >> eoi;
    dlatom =
         tok.dlatombody [ std::cerr << "atombody" << _1 << std::endl ]
      >> '[' >> *( string | (char_ - ']') ) >> ']'
      >> -('(' >> *( string | (char_ - ')') ) >> ')' );
    passthrough =
      char_ | tok.comment | tok.comment | string[ &do_dlatom ];
    string =
      lexeme[tok.stringstart >> *char_ >> tok.stringend];
  };

  qi::rule<Iterator> root, passthrough, dlatom;
  qi::rule<Iterator, std::string()> string;
  std::ostream& outStream;
};

} // anonymous namespace

#include "HexDLConverter.h"

dlvhex::dl::HexDLConverter::HexDLConverter()
{
}

dlvhex::dl::HexDLConverter::~HexDLConverter()
{
}

void dlvhex::dl::HexDLConverter::convert(std::istream& i, std::ostream& o)
{
  //
  // setup iterators
  //


  // TODO: use istreambuf_iterator/multi_pass/position_iterator as soon as lexer no longer requires random access iterator (on todolist of boost spirit team)
  #if 0
  // iterate over stream input
  typedef std::istreambuf_iterator<char> stream_iterator_type;
  stream_iterator_type stream_begin(i);

  // convert input iterator to forward iterator, usable by spirit::qi but not by spirit::lex
  typedef multi_pass<base_iterator_type> base_iterator_type;
  base_iterator_type base_begin = make_default_multi_pass(in_begin);
  base_iterator_type base_end = make_default_multi_pass(stream_iterator_type());
  #else
  // read input stream into string completely (see above TODO)
  std::ostringstream buf;
  buf << i.rdbuf();
  std::string input = buf.str();

  std::cerr <<
    "$$$Converting: Input$$$" << std::endl <<
    input << std::endl <<
    "$$$" << std::endl;

  typedef std::string::iterator base_iterator_type;
  base_iterator_type base_begin = input.begin();
  base_iterator_type base_end = input.end();
  #endif

  // wrap with position iterator, to record the position
  typedef classic::position_iterator<base_iterator_type> iterator_type;
  //typedef base_iterator_type iterator_type;
  iterator_type first(base_begin, base_end, "TODO.input");
  iterator_type last;

  //
  // setup lexer
  //

  // This is the token type to return from the lexer iterator
  typedef lex::lexertl::token<iterator_type,
    boost::mpl::vector<char, std::string> > token_type;
  //typedef lex::lexertl::token<iterator_type> token_type;

  // This is the lexer type to use to tokenize the input.
  // Here we use the lexertl based lexer engine.
  typedef lex::lexertl::actor_lexer<token_type> lexer_type;

  // setup DLLexer lexer
  typedef DLLexer<lexer_type> ConcreteLexer;

  // iterator exposed by DLLexer lexer
  typedef ConcreteLexer::iterator_type lexer_iterator_type;

  //
  // setup parser
  //
  //typedef DLGrammar<lexer_iterator_type> ConcreteParser;

  ConcreteLexer lexer;
  lexer_iterator_type start = lexer.begin(first, last);
  lexer_iterator_type end;

  for(lexer_iterator_type it = start; it!= end && token_is_valid(*it); ++it)
  {
    std::cerr << "token id" << it->id() << std::endl;
    const char* c = boost::get<char>(&it->value());
    if( c != NULL ) std::cerr << "iterator c: " << *c << std::endl;
    const std::string* s = boost::get<std::string>(&it->value());
    if( s != NULL ) std::cerr << "iterator s: " << *s << std::endl;
    typedef boost::iterator_range<iterator_type> lexer_iterator_pair;
    const lexer_iterator_pair* p = boost::get<lexer_iterator_pair>(&it->value());
    if( p != NULL ) std::cerr << "iterator p: " << std::string(p->begin(), p->end()) << std::endl;
    if( p != NULL ) std::cerr << "at line " << p->begin().get_position().line << " column " << p->begin().get_position().column << std::endl;
  }
  /*
  ConcreteParser parser(lexer, o);

  std::cerr << "$$$parsing" << std::endl;
  bool r = lex::tokenize_and_parse(first, last, lexer, parser);
  std::cerr << "$$$parsing returned " << r << std::endl;
  (void)r;
  */
  
  // TODO check ret
  //i >> qi::match(parser);
  // TODO check i.fail();
}

// vim:ts=8:sw=2:tw=100:
