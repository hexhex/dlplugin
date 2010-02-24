/**
 * TODO copyright etc.
 *
 * This file contains the base converter used for DL syntactic sugar: the parser
 * detects DL syntactic sugar and passes it on to the DLAtomConverter for
 * rewriting, everything that is not DL syntactic sugar is simply streamed out
 * output unaltered.
 */

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_eoi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/qi_match.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
//#include <boost/spirit/include/phoenix_core.hpp>

#define DLPLUGIN_DEBUG_LEXER_TOKENIZATION
#undef DLPLUGIN_USE_POSITION_ITERATOR

//#define BOOST_SPIRIT_LEXERTL_DEBUG
//#define BOOST_SPIRIT_DEBUG

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
    std::cerr << "going to lexer state '" << state << "'" << std::endl;
    ctx.set_state_name(state.c_str());
  }

  std::string state;
};

struct lex_atom
{
  template<typename Iterator, typename Context, typename IdType>
  void operator()(Iterator& start, Iterator& end, 
    BOOST_SCOPED_ENUM(boost::spirit::lex::pass_flags)&,
    IdType&, Context& ctx) const
  {
    // go to [ with iterator tmp
    Iterator tmp = start;
    while( *tmp != '[' ) ++tmp;

    // end is tmp
    end = tmp;

    // start parsing DL atom inputs
    ctx.set_state_name("DLATOMINPUT");
    std::cerr << "going to lexer state '" << ctx.get_state_name() << "' *" << std::endl;

    // debugging code:
    //std::string atom(start, end);
    //std::cerr << "in dl atom '" << atom << "'!" << std::endl;
  }
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
    // macros
    this->self.add_pattern
      ("BLANK", "[ \\t]")
      ("NEWLINE", "\\r?\\n")
      ("INPUTLIST", "\\[[^\\]]*\\]")
      ("OUTPUTLIST", "\\([^\\)]*\\)")
      ("NUMBER", "\\-?[0-9]+")
      ("STRING", "[A-Za-z][A-Za-z_0-9\\'\\_]*")
      ("QUOTEDSTRING", "\\\"[^\\\"]*\\\"")
    ;

    // find comments and strings in regular text
    iComment = "{BLANK}*%.*$";
    iDLAtom = "DL{INPUTLIST}{OUTPUTLIST}?";
    iNewline = "{NEWLINE}";
    iAny = ".";
    // = "&dlC"; kwdlR = "&dlR"; kwdlDR = "&dlDR"; kwdlCQ = "&dlCQ"; kwdlUCQ = "&dlUCQ";
    this->self("INITIAL") =
        // comments
        iComment
        // string starter
      | iDLAtom [ lex_atom() ]
      | lex::token_def<lex::omit>("{BLANK}")
      | iNewline
      | iAny
      ;

    aiEquals  = "==";
    aiNEquals = "!=";
    aiPlusop  = "\\+=";
    aiMinusop = "-=";
    aiTerm  = "{STRING}|{QUOTEDSTRING}";
    this->self("DLATOMINPUT") =
        lex::token_def<>(']') [ set_lexer_state("DLATOMAFTERINPUT") ]
      | '[' | ';' | ',' | '-'
      | aiEquals | aiNEquals
      | aiPlusop | aiMinusop
      | aiTerm
      | lex::token_def<lex::omit>("{BLANK}")
      ;

    aaiSentinel = "[,\\.]";
    this->self("DLATOMAFTERINPUT") =
      // continue with atom inputs
        lex::token_def<>('(') [ set_lexer_state("DLATOMOUTPUT") ]
      // continue with toplevel parsing (dl atom finished)
      | aaiSentinel [ set_lexer_state("INITIAL") ]
      | lex::token_def<lex::omit>("{BLANK}")
      ;

    aoTerm = "{QUOTEDSTRING}|{STRING}|{NUMBER}|\\_";
    this->self("DLATOMOUTPUT") =
        lex::token_def<>(')') [ set_lexer_state("INITIAL") ]
      | ','
      | aoTerm
      | lex::token_def<lex::omit>("{BLANK}")
      ;

    this->self("WS") =
      lex::token_def<>("[ \\t\\n]+");
  }

  lex::token_def<> iNewline, iAny;
  lex::token_def<std::string> iComment, iDLAtom;
  lex::token_def<lex::omit> kwDL, kwdlC, kwdlR, kwdlDR, kwdlCQ, kwdlUCQ;
  lex::token_def<std::string> aiTerm, aiEquals, aiNEquals, aiPlusop, aiMinusop;
  lex::token_def<>            aaiSentinel;
  lex::token_def<std::string> aoTerm;
};

#if 0
void do_fee()
{
  std::cerr << "found fee " << std::endl;
}
void do_foo(std::string s)
{
  std::cerr << "found foo '" << s << "'" << std::endl;
}
void do_dlatom(boost::fusion::vector<std::string, std::string> input)
{
  using boost::phoenix::at_c;
  std::cerr << "found dlatom '" << at_c<0>(input) << "'/'" << at_c<1>(input) << "'" << std::endl;
}
void do_passthrough(const std::string& s)
{
  std::cerr << "found passthrough '" << s << "'" << std::endl;
}

const std::string& mkstring(char c)
{
  std::string s(" ");
  s[0] = c;
  return s;
}

/**
 * This grammar uses the above lexer to pick DL atoms out of an incoming data stream, and to rewrite
 * just these atoms and passthrough the rest of the input.
 */
template <typename Iterator, typename Lexer>
struct DLGrammar: qi::grammar<Iterator, qi::in_state_skipper<Lexer> >
{
  typedef typename qi::in_state_skipper<Lexer> Skipper;
  template <typename TokenDef>
  DLGrammar(TokenDef const& tok, std::ostream& out):
    DLGrammar::base_type(root),
    outStream(out)
  {
    using qi::lexeme;
    using qi::char_;
    //root = tok.comment [ &do_passthrough ];// > tok.newline > tok.comment [ &do_passthrough ];
    root =
      *( dlatom2
       | passthrough [ &do_passthrough ]
       );
    passthrough =
        tok.comment
      | tok.newline
      | tok.any1
      | string;
    string %=
      lexeme[tok.stringstart >> *(tok.escaped_stringend | tok.any2) >> tok.stringend];
    dlatom =
      (tok.kwDL >> '[' >> dlquery >> ']') [ &do_foo ];
    dlatom2 = (tok.kwDL >> '[' >> string3 >> ']') [ &do_foo ];
    string3 %= lexeme[*(tok.any3)];
     // |
      //("DL" >> '[' >> ops >> ';' >> dlquery >> ']' >> output) [ &do_dlatom ];
         //tok.dlatombody [ std::cerr << "atombody" << _1 << std::endl ]
      //>> '[' >> *( string | (char_ - ']') ) >> ']'
      //>> -('(' >> *( string | (char_ - ')') ) >> ')' );
    dlquery %=
      lexeme[-char_('-') >> alnum >> *(alnum | '\'' | '_')];
    output %= lexeme[*char_];
  };

  qi::rule<Iterator, Skipper> root, dlatom, dlatom2;
  qi::rule<Iterator, std::string(), Skipper> string, passthrough, dlquery, output, string3;
  //qi::rule<Iterator, dlvhex::AtomSet(), Skipper> ops;
  //qi::rule<Iterator, dlvhex::Tuple(), Skipper> output;
  std::ostream& outStream;
};
#endif

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
  #endif

  // read input stream into string completely (see above TODO)
  std::ostringstream buf;
  buf << i.rdbuf();
  std::string input = buf.str();

  #ifdef DLPLUGIN_DEBUG_LEXER_TOKENIZATION
  std::cerr <<
    "$$$Converting: Input$$$" << std::endl <<
    input << std::endl <<
    "$$$" << std::endl;
  #endif

  typedef std::string::iterator base_iterator_type;

  #ifndef DLPLUGIN_USE_POSITION_ITERATOR
  typedef base_iterator_type iterator_type;
  iterator_type first = input.begin();
  iterator_type last = input.end();
  #else
  // wrap with position iterator, to record the position
  typedef classic::position_iterator<base_iterator_type> iterator_type;

  base_iterator_type base_first = input.begin();
  base_iterator_type base_last = input.end();
  iterator_type first(base_first, base_last, "HexDLConverter::convert stream input");
  iterator_type last;
  #endif

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

  #ifdef DLPLUGIN_DEBUG_LEXER_TOKENIZATION
  // lexer test (tokenize input and output the result)
  std::cerr << "START lexer test" << std::endl;
  {
    iterator_type remember_first = first;
    iterator_type remember_last = last;

    ConcreteLexer lexer;
    lexer_iterator_type start = lexer.begin(first, last);
    lexer_iterator_type end = lexer.end();
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
      #ifdef DLPLUGIN_USE_POSITION_ITERATOR
      if( p != NULL ) std::cerr << "at line " << p->begin().get_position().line << " column " << p->begin().get_position().column << std::endl;
      #endif
    }

    first = remember_first;
    last = remember_last;
  }
  std::cerr << "END lexer test";
  #endif

  #if 0
  //
  // setup parser
  //
  typedef DLGrammar<lexer_iterator_type, ConcreteLexer::lexer_def> ConcreteParser;

  ConcreteLexer lexer(o);
  ConcreteParser parser(lexer, o);

  std::cerr << "$$$parsing" << std::endl;
  lexer_iterator_type start = lexer.begin(first, last);
  lexer_iterator_type end = lexer.end();
  bool r = qi::phrase_parse(start, end, parser, qi::in_state("WS")[lexer.self]);
  std::cerr << "$$$parsing returned " << r << std::endl;
  std::cerr << "at token id" << start->id() << " end?" << (start == end) << std::endl;
  #endif

  #if 0
  {
    std::cerr << "$$$parsing" << std::endl;
    bool r = lex::tokenize_and_parse(first, last, lexer, parser);
    std::cerr << "$$$parsing returned " << r << std::endl;
  }
  #endif
  
  // TODO check ret
  //i >> qi::match(parser);
  // TODO check i.fail();
}

// vim:ts=8:sw=2:tw=100:
