/**
 * TODO copyright etc.
 *
 * This file contains the base converter used for DL syntactic sugar: the parser
 * detects DL syntactic sugar and passes it on to the DLAtomConverter for
 * rewriting, everything that is not DL syntactic sugar is simply streamed out
 * output unaltered.
 */

#include "HexDLConverter.h"
#include "HexDLRewriter.h"
#include "Ontology.h"
#include <dlvhex/Error.h>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_eoi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/qi_match.hpp>
#include <boost/spirit/include/support_multi_pass.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>

#undef DLPLUGIN_DEBUG_LEXER_TOKENIZATION
#undef DLPLUGIN_USE_POSITION_ITERATOR

//#define BOOST_SPIRIT_LEXERTL_DEBUG
//#define BOOST_SPIRIT_DEBUG

using namespace boost::spirit;
using namespace boost::spirit::ascii;
namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;

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
    ///std::cerr << "going to lexer state '" << state << "'" << std::endl;
    ctx.set_state_name(state.c_str());
  }

  std::string state;
};

struct lexer_start_atom
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
    ///std::cerr << "going to lexer state '" << ctx.get_state_name() << "' *" << std::endl;

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
    iBlank = "{BLANK}";
    iAny = ".";
    iDLExtAtom = "&dl(C|R|(DR)|(CQ)|(UCQ)){INPUTLIST}{OUTPUTLIST}?";
    this->self("INITIAL") =
        // comments
        iComment
        // string starter
      | iDLAtom [ lexer_start_atom() ]
      | iDLExtAtom [ lexer_start_atom() ]
      | iBlank
      | iNewline
      | iAny
      ;

    //aiEquals  = "==";
    //aiNEquals = "!=";
    aiPlusop  = "\\+=";
    aiMinusop = "-=";
    aiMinus = '-';
    aiOr = " v ";
    aiString  = "{STRING}|{QUOTEDSTRING}";
    aiNumber = "{NUMBER}";
    this->self("DLATOMINPUT") =
        lex::token_def<>(']') [ set_lexer_state("DLATOMAFTERINPUT") ]
      | '[' | ';' | ',' | '(' | ')'
     // | aiEquals | aiNEquals
      | aiPlusop | aiMinusop
      | aiMinus
      | aiOr
      | aiString
      | aiNumber
      | lex::token_def<lex::omit>("{BLANK}") [ _pass = lex::pass_flags::pass_ignore ] // do not pass this token to parser
      ;

    aaiSentinel = "[,\\.]";
    this->self("DLATOMAFTERINPUT") =
      // continue with atom inputs
        lex::token_def<>('(') [ set_lexer_state("DLATOMOUTPUT") ]
      // continue with toplevel parsing (dl atom finished)
      | aaiSentinel [ set_lexer_state("INITIAL") ]
      | lex::token_def<lex::omit>("{BLANK}") [ _pass = lex::pass_flags::pass_ignore ] // do not pass this token to parser
      ;

    aoString = "{STRING}|{QUOTEDSTRING}";
    aoNumber = "{NUMBER}";
    aoAnonymousVar = "\\_";
    this->self("DLATOMOUTPUT") =
        lex::token_def<>(')') [ set_lexer_state("INITIAL") ]
      | lex::token_def<lex::omit>(',') [ _pass = lex::pass_flags::pass_ignore ] // do not pass this token to parser
      | aoString
      | aoNumber
      | aoAnonymousVar
      | lex::token_def<lex::omit>("{BLANK}") [ _pass = lex::pass_flags::pass_ignore ] // do not pass this token to parser
      ;

    this->self("WS") =
      lex::token_def<>("[ \\t\\n]+");
  }

  lex::token_def<>             iNewline, iBlank, iAny, iComment;
  lex::token_def<std::string>  iDLAtom, iDLExtAtom;
  lex::token_def<>             aiOr;
  lex::token_def<>             aiMinus;
  lex::token_def<std::string>  aiString, aiEquals, aiNEquals, aiPlusop, aiMinusop;
  lex::token_def<int>          aiNumber;
  lex::token_def<>             aaiSentinel;
  lex::token_def<std::string>  aoString;
  lex::token_def<int>          aoNumber;
  lex::token_def<>             aoAnonymousVar;
};

// state passed to grammar for parsing
struct ConverterState
{
  ConverterState(std::ostream& out, dlvhex::dl::Ontology::shared_pointer ontology,
      dlvhex::dl::DLAtomInput& dlinput):
    out(out), ontology(ontology), dlinput(dlinput) {}

  std::ostream& out;
  dlvhex::dl::Ontology::shared_pointer ontology;
  dlvhex::dl::DLAtomInput& dlinput;
};

struct handle_passthrough
{
  handle_passthrough(ConverterState& state):
    state(state) { }

  void operator()(const std::string& s, qi::unused_type, qi::unused_type) const
  {
    state.out << s;
  }

  ConverterState state;
};

struct handle_dlatom
{
  handle_dlatom(ConverterState& state): state(state) {}

  // dlquery: Query = std::string
  // cq: Query = dlvhex::AtomSet
  // ucq: Query = std::vector<dlvhex::AtomSet>
  template<typename Context, typename Query>
  void operator()(boost::fusion::vector3<boost::optional<dlvhex::AtomSet>, Query, dlvhex::Tuple> const& args,
      Context& ctx, qi::unused_type) const
  {
    dlvhex::AtomSet ops;
    if( boost::none != fusion::at_c<0>(args) )
      ops = fusion::at_c<0>(args).get();

    dlvhex::dl::HexDLRewriterBasePtr rewriter(
      new dlvhex::dl::DLAtomRewriter(
        state.ontology, state.dlinput,
        ops, & fusion::at_c<1>(args), & fusion::at_c<2>(args)));

    dlvhex::Literal* lit = rewriter->getLiteral();
    state.out << *lit;
    delete lit;
  }

  ConverterState& state;
};

struct handle_dlextatom
{
  handle_dlextatom(ConverterState& state): state(state) {}

  template<typename Context>
  void operator()(boost::fusion::vector3<
      std::string, std::vector<dlvhex::Term>, dlvhex::Tuple> const& args,
      Context& ctx, qi::unused_type) const
  {
    std::string atom = fusion::at_c<0>(args);
    atom.erase(atom.begin());

    const dlvhex::Tuple& inputs = fusion::at_c<1>(args);
    const dlvhex::Tuple& outputs = fusion::at_c<2>(args);

    if( atom == "&dlCQ" || atom == "&dlUCQ" )
    {
      // need to append arity
      std::ostringstream oss;
      oss << atom << outputs.size();
      atom = oss.str();
    }

    dlvhex::ExternalAtomPtr extAtom(
        new dlvhex::ExternalAtom(atom, outputs, inputs, 0));

    dlvhex::dl::HexDLRewriterBasePtr rewriter(
      new dlvhex::dl::ExtAtomRewriter(extAtom));

    dlvhex::Literal* lit = rewriter->getLiteral();
    state.out << *lit;
    delete lit;
  }

  ConverterState& state;
};

struct handle_atomset_from_atomptrs
{
  template<typename Context>
  void operator()(std::vector<dlvhex::AtomPtr> const& ops, Context& ctx, qi::unused_type) const
  {
    dlvhex::AtomSet& ruleAttr = fusion::at_c<0>(ctx.attributes);
    for(std::vector<dlvhex::AtomPtr>::const_iterator it = ops.begin(); it != ops.end(); ++it)
      ruleAttr.insert(*it);
  }
};

struct handle_op
{
  handle_op(ConverterState& state):
    state(state) {}

  template<typename Context>
  void operator()(boost::fusion::vector<std::string, std::string, std::string> const& triple, Context& ctx, qi::unused_type) const
  {
    dlvhex::AtomPtr& ruleAttr = fusion::at_c<0>(ctx.attributes);

    const std::string& arg1 = fusion::at_c<0>(triple);
    const std::string& op = fusion::at_c<1>(triple);
    const std::string& arg2 = fusion::at_c<2>(triple);

    dlvhex::dl::Ontology::shared_pointer ontology = state.ontology;

      if (!ontology)
	{
	  throw DLVHEX_NAMESPACE PluginError("Cannot rewrite dl-atoms, option --ontology is empty.");
	}

      std::string tmp;

      if (arg1.find("\"-") == 0)
	{
	  tmp = ontology->getNamespace() + arg1.substr(2, arg1.length() - 3);
	}
      else if (arg1.find("\"") == 0)
	{
	  tmp = ontology->getNamespace() + arg1.substr(1, arg1.length() - 2);
	}
      else if (arg1.find("-") == 0)
	{
	  tmp = ontology->getNamespace() + arg1.substr(1, arg1.length() - 1);
	}
      else
	{
	  tmp = ontology->getNamespace() + arg1;
	}

      DLVHEX_NAMESPACE Tuple t;
      t.push_back(DLVHEX_NAMESPACE Term(tmp, true));
      t.push_back(DLVHEX_NAMESPACE Term(arg2));

      DLVHEX_NAMESPACE Term q(tmp);

      dlvhex::dl::TBox::ObjectsPtr concepts = ontology->getTBox().getConcepts();
      dlvhex::dl::TBox::ObjectsPtr roles = ontology->getTBox().getRoles();
      dlvhex::dl::TBox::ObjectsPtr datatypeRoles = ontology->getTBox().getDatatypeRoles();

      if (op == "+=") // PMOP is +=
	{
	  if (concepts->find(q) != concepts->end())
	    {
	      ruleAttr = DLVHEX_NAMESPACE AtomPtr(new DLVHEX_NAMESPACE Atom("pc", t));
	    }
	  else if (roles->find(q) != roles->end() ||
		   datatypeRoles->find(q) != datatypeRoles->end()
		   )
	    {
	      ruleAttr = DLVHEX_NAMESPACE AtomPtr(new DLVHEX_NAMESPACE Atom("pr", t));
	    }
	  else
	    {
	      std::ostringstream oss;
	      oss << "Incompatible dl-atom-op `" << arg1 << "+=" << arg2 << "' supplied: ";
	      oss << "Cannot find " << q << " in " << ontology->getRealURI();
	      throw DLVHEX_NAMESPACE PluginError(oss.str());
	    }
	}
      else if (op == "-=") // PMOP is -=
	{
	  if (concepts->find(q) != concepts->end())
	    {
	      ruleAttr = DLVHEX_NAMESPACE AtomPtr(new DLVHEX_NAMESPACE Atom("mc", t));
	    }
	  else if (roles->find(q) != roles->end() ||
		   datatypeRoles->find(q) != datatypeRoles->end()
		   )
	    {
	      ruleAttr = DLVHEX_NAMESPACE AtomPtr(new DLVHEX_NAMESPACE Atom("mr", t));
	    }
	  else
	    {
	      std::ostringstream oss;
	      oss << "Incompatible dl-atom-op `" << arg1 << "-=" << arg2 << "' supplied: ";
	      oss << "Cannot find " << q << " in " << ontology->getRealURI();
	      throw DLVHEX_NAMESPACE PluginError(oss.str());
	    }
	}
      else // hm, PMOP is neither '+' nor '-', this is a programming error
	{
	  assert(false);
	}
  }

  ConverterState state;
};

struct handle_output
{
  template<typename Context>
  void operator()(boost::optional<std::vector<dlvhex::Term> > const& output, Context& ctx, qi::unused_type) const
  {
    dlvhex::Tuple& ruleAttr = boost::fusion::at_c<0>(ctx.attributes);
    dlvhex::Tuple t;
    if( boost::none != output && !output.get().empty() )
    {
      for(std::vector<dlvhex::Term>::const_iterator it = output.get().begin();
          it != output.get().end(); ++it)
        t.push_back(*it);
    }
    ruleAttr = t;
  }
};

// TODO: write minimal testcase for rule r = (-tok.foo >> tok.bar) [handle()] where foo attribute
// type is char and bar attribute type is string -> boost::optional is created, but string contains
// initial \0 if foo token is absent

struct handle_atom
{
  template<typename Context, typename LexerTokenAttrib>
  void operator()(boost::fusion::vector4<boost::optional<LexerTokenAttrib>, std::string, dlvhex::Term, boost::optional<dlvhex::Term> > const& tokens, Context& ctx, qi::unused_type) const
  {
    dlvhex::AtomPtr& ruleAttr = fusion::at_c<0>(ctx.attributes);

    // presence of tok.aiMinus sets this optional int, meaning that we have a "-"
    bool neg = (boost::none != fusion::at_c<0>(tokens));

    const std::string& predicate = fusion::at_c<1>(tokens);

    dlvhex::Tuple arguments;
    arguments.push_back( fusion::at_c<2>(tokens) );
    if( boost::none != fusion::at_c<3>(tokens) )
      arguments.push_back( fusion::at_c<3>(tokens).get() );

    //std::cerr << "atom: predicate='" << predicate << "' " << int(predicate[0]) << "'" << int(predicate[1]) << "'  with " << arguments.size() << " arguments" << std::endl;
    ruleAttr = dlvhex::AtomPtr(new dlvhex::Atom(predicate, arguments, neg));
  }
};

/**
 * This grammar uses the above lexer to pick DL atoms out of an incoming data stream, and to rewrite
 * just these atoms and passthrough the rest of the input.
 */
template <typename Iterator, typename Lexer>
struct DLGrammar: qi::grammar<Iterator, qi::in_state_skipper<Lexer> >
{
  typedef typename qi::in_state_skipper<Lexer> Skipper;
  template <typename TokenDef>
  DLGrammar(TokenDef const& tok, ConverterState& state):
    DLGrammar::base_type(root),
    state(state)
  {
    using qi::lexeme;
    root =
     *( dlatom
      | passthrough [ handle_passthrough(state) ]
      );
    dlatom =
        (qi::omit[tok.iDLAtom] >> '[' >> -(ops >> ';') >> dlquery >> ']' >> output) [ handle_dlatom(state) ]
      | (qi::omit[tok.iDLAtom] >> '[' >> -(ops >> ';') >> cq      >> ']' >> output) [ handle_dlatom(state) ]
      | (qi::omit[tok.iDLAtom] >> '[' >> -(ops >> ';') >> ucq     >> ']' >> output) [ handle_dlatom(state) ]
      | (tok.iDLExtAtom >>        '[' >> (iterm % ',') >>            ']' >> output) [ handle_dlextatom(state) ]
      ;
    ops =
      (op % ',') [ handle_atomset_from_atomptrs() ];
    op =
      (tok.aiString >> pmop >> tok.aiString) [ handle_op(state) ];
    pmop %=
      tok.aiPlusop | tok.aiMinusop;
    dlquery %=
      lexeme[-tok.aiMinus >> tok.aiString];
    cq =
      (atom % ',') [ handle_atomset_from_atomptrs() ];
    ucq %=
      (cq % tok.aiOr);
    atom =
      (-tok.aiMinus >> tok.aiString >> '(' >> iterm >> -(',' >> iterm) >> ')') [ handle_atom() ];
    iterm =
        tok.aiString [ _val = phoenix::construct<dlvhex::Term>(_1) ]
      | tok.aiNumber [ _val = phoenix::construct<dlvhex::Term>(_1) ];
    output =
      -('(' >> *oterm >> ')') [ handle_output() ];
    oterm =
        tok.aoString [ _val = phoenix::construct<dlvhex::Term>(_1) ]
      | tok.aoNumber [ _val = phoenix::construct<dlvhex::Term>(_1) ]
      | tok.aoAnonymousVar [ _val = phoenix::construct<dlvhex::Term>() ];
    passthrough %=
        tok.iComment
      | tok.iBlank
      | tok.iNewline
      | tok.iAny
      ;
  };

  qi::rule<Iterator, Skipper> root, dlatom;
  qi::rule<Iterator, std::string(), Skipper> passthrough, dlquery, pmop;
  qi::rule<Iterator, dlvhex::AtomPtr(), Skipper> op, atom;
  qi::rule<Iterator, dlvhex::AtomSet(), Skipper> ops, cq;
  #warning originally, the ucq was a boost::ptr_vector<AtomSet>, perhaps AtomSet needs to be improved to allow this new usage
  qi::rule<Iterator, std::vector<dlvhex::AtomSet>(), Skipper> ucq;
  qi::rule<Iterator, dlvhex::Term(), Skipper> iterm, oterm;
  qi::rule<Iterator, dlvhex::Tuple(), Skipper> output;

  ConverterState& state;
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
      //std::cerr << "token id" << it->id() << std::endl;
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

  //
  // setup parser
  //

  typedef DLGrammar<lexer_iterator_type, ConcreteLexer::lexer_def> ConcreteParser;

  // setup lexer
  ConcreteLexer lexer;

  // setup converter state passed to parser
  dlvhex::dl::DLAtomInput dlinput;
  ConverterState state(o, ontology, dlinput);

  // setup parser
  ConcreteParser parser(lexer, state);

  // do the parsing
  lexer_iterator_type start = lexer.begin(first, last);
  lexer_iterator_type end = lexer.end();
  bool r = qi::phrase_parse(start, end, parser, qi::in_state("WS")[lexer.self]);
  if( start != end )
    throw dlvhex::PluginError("could not parse complete input!");

  // output extra rules
  typedef std::vector<dlvhex::Rule*> RuleVector;
  const RuleVector& rules = dlinput.getDLInputRules();
  for(RuleVector::const_iterator it = rules.begin(); it != rules.end(); ++it)
  {
    o << *(*it) << std::endl;
    delete *it;
  }
}

// vim:ts=8:sw=2:tw=100:
