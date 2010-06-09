/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++

   Copyright (C) 2002, 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


#include "HexDLParser.hpp"

/* User implementation prologue.  */
#line 104 "HexDLParser.ypp"


#include "HexDLFlexLexer.h"

// Announce to bison the prototype we want for the lexing function
#define YY_DECL                                                 \
yy::HexDLParser::token_type                                     \
yylex (yy::HexDLParser::semantic_type* yylval,                  \
       yy::HexDLParser::location_type* yylloc,                  \
       dlvhex::dl::HexDLDriver& driver)

// declare YY_DECL function for the parser
YY_DECL;



/* Line 317 of lalr1.cc.  */
#line 58 "HexDLParser.cpp"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG							\
  for (bool yydebugcond_ = yydebug_; yydebugcond_; yydebugcond_ = false)	\
    (*yycdebug_)

/* Enable debugging if requested.  */
#if YYDEBUG

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab

namespace yy
{
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  HexDLParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  HexDLParser::HexDLParser (dlvhex::dl::HexDLDriver& driver_yyarg, dlvhex::dl::DLAtomInput& dlinput_yyarg)
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
      driver (driver_yyarg),
      dlinput (dlinput_yyarg)
  {
  }

  HexDLParser::~HexDLParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  HexDLParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
        case 5: /* "STRING" */
#line 126 "HexDLParser.ypp"
	{ debug_stream() << *(yyvaluep->sval); };
#line 189 "HexDLParser.cpp"
	break;
       default:
	  break;
      }
  }


  void
  HexDLParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif /* ! YYDEBUG */

  void
  HexDLParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
        case 5: /* "STRING" */
#line 122 "HexDLParser.ypp"
	{ delete (yyvaluep->sval); };
#line 224 "HexDLParser.cpp"
	break;
      case 22: /* "dlatom" */
#line 122 "HexDLParser.ypp"
	{ delete (yyvaluep->dlrewriterval); };
#line 229 "HexDLParser.cpp"
	break;

	default:
	  break;
      }
  }

  void
  HexDLParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

  std::ostream&
  HexDLParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  HexDLParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  HexDLParser::debug_level_type
  HexDLParser::debug_level () const
  {
    return yydebug_;
  }

  void
  HexDLParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }


  int
  HexDLParser::parse ()
  {
    /// Look-ahead and look-ahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the look-ahead.
    semantic_type yylval;
    /// Location of the look-ahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;
    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without look-ahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a look-ahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc, driver);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Accept?  */
    if (yyn == yyfinal_)
      goto yyacceptlab;

    /* Shift the look-ahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted unless it is eof.  */
    if (yychar != yyeof_)
      yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 3:
#line 133 "HexDLParser.ypp"
    {
	   DLVHEX_NAMESPACE Literal* l = (yysemantic_stack_[(3) - (2)].dlrewriterval)->getLiteral();
	   // replace the parsed dl-atom
	   driver.getOutput() << *l << (yysemantic_stack_[(3) - (3)].cval);
	   delete l;
	   delete (yysemantic_stack_[(3) - (2)].dlrewriterval);
	 ;}
    break;

  case 4:
#line 148 "HexDLParser.ypp"
    {
	  //
	  // create a rewriter without dl-atom ops
	  //
	  (yyval.dlrewriterval) = new dlvhex::dl::DLAtomRewriter(driver.getOntology(), dlinput, DLVHEX_NAMESPACE AtomSet(), (yysemantic_stack_[(5) - (3)].sval), (yysemantic_stack_[(5) - (5)].tupleval));
	;}
    break;

  case 5:
#line 155 "HexDLParser.ypp"
    {
	  //
	  // create a rewriter with dl-atom ops
	  //

	  (yyval.dlrewriterval) = new dlvhex::dl::DLAtomRewriter(driver.getOntology(), dlinput, *(yysemantic_stack_[(7) - (3)].atomsetval), (yysemantic_stack_[(7) - (5)].sval), (yysemantic_stack_[(7) - (7)].tupleval));
	  delete (yysemantic_stack_[(7) - (3)].atomsetval);
	;}
    break;

  case 6:
#line 164 "HexDLParser.ypp"
    {
	  //
	  // create a rewriter with cq-atom ops
	  //

	  (yyval.dlrewriterval) = new dlvhex::dl::DLAtomRewriter(driver.getOntology(), dlinput, DLVHEX_NAMESPACE AtomSet(), (yysemantic_stack_[(5) - (3)].atomsetval), (yysemantic_stack_[(5) - (5)].tupleval));
	;}
    break;

  case 7:
#line 172 "HexDLParser.ypp"
    {
	  //
	  // create a rewriter with cq-atom ops
	  //

	  (yyval.dlrewriterval) = new dlvhex::dl::DLAtomRewriter(driver.getOntology(), dlinput, *(yysemantic_stack_[(7) - (3)].atomsetval), (yysemantic_stack_[(7) - (5)].atomsetval), (yysemantic_stack_[(7) - (7)].tupleval));
	  delete (yysemantic_stack_[(7) - (3)].atomsetval);
	;}
    break;

  case 8:
#line 181 "HexDLParser.ypp"
    {
	  //
	  // create a rewriter with ucq-atom ops
	  //

	  (yyval.dlrewriterval) = new dlvhex::dl::DLAtomRewriter(driver.getOntology(), dlinput, DLVHEX_NAMESPACE AtomSet(), (yysemantic_stack_[(5) - (3)].atomsetsval), (yysemantic_stack_[(5) - (5)].tupleval));
	;}
    break;

  case 9:
#line 189 "HexDLParser.ypp"
    {
	  //
	  // create a rewriter with ucq-atom ops
	  //

	  (yyval.dlrewriterval) = new dlvhex::dl::DLAtomRewriter(driver.getOntology(), dlinput, *(yysemantic_stack_[(7) - (3)].atomsetval), (yysemantic_stack_[(7) - (5)].atomsetsval), (yysemantic_stack_[(7) - (7)].tupleval));
	  delete (yysemantic_stack_[(7) - (3)].atomsetval);
	;}
    break;

  case 10:
#line 198 "HexDLParser.ypp"
    { 
	  // a cq-atom is the external atom
	  // &dlCQ[uri,a,b,c,d,q](X_1,...X_n), we rewrite it to
	  // &dlCQn[uri,a,b,c,d,q](X_1,...,X_n)

	  std::ostringstream oss;
	  oss << "dlCQ" << (yysemantic_stack_[(5) - (5)].tupleval)->size();

	  DLVHEX_NAMESPACE AtomPtr eap(new DLVHEX_NAMESPACE ExternalAtom(oss.str(), *(yysemantic_stack_[(5) - (5)].tupleval), *(yysemantic_stack_[(5) - (3)].tupleval), 0));
	  (yyval.dlrewriterval) = new dlvhex::dl::ExtAtomRewriter(eap);

	  delete (yysemantic_stack_[(5) - (3)].tupleval);
	  delete (yysemantic_stack_[(5) - (5)].tupleval);
	;}
    break;

  case 11:
#line 213 "HexDLParser.ypp"
    { 
	  // a ucq-atom is the external atom
	  // &dlUCQ[uri,a,b,c,d,q](X_1,...X_n), we rewrite it to
	  // &dlUCQn[uri,a,b,c,d,q](X_1,...,X_n)

	  std::ostringstream oss;
	  oss << "dlCQ" << (yysemantic_stack_[(5) - (5)].tupleval)->size();

	  DLVHEX_NAMESPACE AtomPtr eap(new DLVHEX_NAMESPACE ExternalAtom(oss.str(), *(yysemantic_stack_[(5) - (5)].tupleval), *(yysemantic_stack_[(5) - (3)].tupleval), 0));
	  (yyval.dlrewriterval) = new dlvhex::dl::ExtAtomRewriter(eap);

	  delete (yysemantic_stack_[(5) - (3)].tupleval);
	  delete (yysemantic_stack_[(5) - (5)].tupleval);
	;}
    break;

  case 12:
#line 228 "HexDLParser.ypp"
    {
	  // create simple dlatom rewriter
	  DLVHEX_NAMESPACE AtomPtr eap(new DLVHEX_NAMESPACE ExternalAtom(*(yysemantic_stack_[(5) - (1)].sval), *(yysemantic_stack_[(5) - (5)].tupleval), *(yysemantic_stack_[(5) - (3)].tupleval), 0));
	  (yyval.dlrewriterval) = new dlvhex::dl::ExtAtomRewriter(eap);

	  delete (yysemantic_stack_[(5) - (1)].sval);
	  delete (yysemantic_stack_[(5) - (3)].tupleval);
	  delete (yysemantic_stack_[(5) - (5)].tupleval);
	;}
    break;

  case 13:
#line 242 "HexDLParser.ypp"
    {
	   (yyval.sval) = (yysemantic_stack_[(1) - (1)].sval);
	 ;}
    break;

  case 14:
#line 246 "HexDLParser.ypp"
    {
	   (yyval.sval) = (yysemantic_stack_[(2) - (2)].sval);
	   (yyval.sval)->insert(0, 1, '-');
	 ;}
    break;

  case 15:
#line 255 "HexDLParser.ypp"
    {
	DLVHEX_NAMESPACE Tuple t;
	t.push_back(*(yysemantic_stack_[(4) - (3)].termval));

	bool neg = (*(yysemantic_stack_[(4) - (1)].sval))[0] == '-';
	(yyval.atomval) = new DLVHEX_NAMESPACE AtomPtr(new DLVHEX_NAMESPACE Atom(neg ? (yysemantic_stack_[(4) - (1)].sval)->substr(1) : *(yysemantic_stack_[(4) - (1)].sval), t, neg));

	delete (yysemantic_stack_[(4) - (1)].sval);
	delete (yysemantic_stack_[(4) - (3)].termval);
      ;}
    break;

  case 16:
#line 266 "HexDLParser.ypp"
    {
	DLVHEX_NAMESPACE Tuple t;
	t.push_back(*(yysemantic_stack_[(6) - (3)].termval));
	t.push_back(*(yysemantic_stack_[(6) - (5)].termval));

	bool neg = (*(yysemantic_stack_[(6) - (1)].sval))[0] == '-';
	(yyval.atomval) = new DLVHEX_NAMESPACE AtomPtr(new DLVHEX_NAMESPACE Atom(neg ? (yysemantic_stack_[(6) - (1)].sval)->substr(1) : *(yysemantic_stack_[(6) - (1)].sval), t, neg));

	delete (yysemantic_stack_[(6) - (1)].sval);
	delete (yysemantic_stack_[(6) - (3)].termval);
	delete (yysemantic_stack_[(6) - (5)].termval);
      ;}
    break;

  case 17:
#line 282 "HexDLParser.ypp"
    {
      (yyval.atomsetval) = new DLVHEX_NAMESPACE AtomSet;
      (yyval.atomsetval)->insert(*(yysemantic_stack_[(1) - (1)].atomval));
      delete (yysemantic_stack_[(1) - (1)].atomval);
    ;}
    break;

  case 18:
#line 288 "HexDLParser.ypp"
    {
      (yyval.atomsetval) = (yysemantic_stack_[(3) - (1)].atomsetval);
      (yyval.atomsetval)->insert(*(yysemantic_stack_[(3) - (3)].atomval));
      delete (yysemantic_stack_[(3) - (3)].atomval);
    ;}
    break;

  case 19:
#line 297 "HexDLParser.ypp"
    {
       (yyval.atomsetsval) = new boost::ptr_vector<DLVHEX_NAMESPACE AtomSet>;
       (yyval.atomsetsval)->push_back((yysemantic_stack_[(3) - (1)].atomsetval));
       (yyval.atomsetsval)->push_back((yysemantic_stack_[(3) - (3)].atomsetval));
     ;}
    break;

  case 20:
#line 303 "HexDLParser.ypp"
    {
       (yyval.atomsetsval) = (yysemantic_stack_[(3) - (1)].atomsetsval);
       (yyval.atomsetsval)->push_back((yysemantic_stack_[(3) - (3)].atomsetval));
     ;}
    break;

  case 21:
#line 311 "HexDLParser.ypp"
    {
	  (yyval.tupleval) = new DLVHEX_NAMESPACE Tuple;
	;}
    break;

  case 22:
#line 315 "HexDLParser.ypp"
    {
	  (yyval.tupleval) = new DLVHEX_NAMESPACE Tuple;
	;}
    break;

  case 23:
#line 319 "HexDLParser.ypp"
    {
	  (yyval.tupleval) = (yysemantic_stack_[(3) - (2)].tupleval);
	;}
    break;

  case 24:
#line 327 "HexDLParser.ypp"
    {
	 (yyval.tupleval) = new DLVHEX_NAMESPACE Tuple(1, *(yysemantic_stack_[(1) - (1)].termval));
	 delete (yysemantic_stack_[(1) - (1)].termval);
       ;}
    break;

  case 25:
#line 332 "HexDLParser.ypp"
    {
	 (yyval.tupleval) = (yysemantic_stack_[(3) - (1)].tupleval);
	 (yyval.tupleval)->push_back(*(yysemantic_stack_[(3) - (3)].termval));
	 delete (yysemantic_stack_[(3) - (3)].termval);
       ;}
    break;

  case 26:
#line 342 "HexDLParser.ypp"
    {
	(yyval.termval) = new DLVHEX_NAMESPACE Term(*(yysemantic_stack_[(1) - (1)].sval));
	delete (yysemantic_stack_[(1) - (1)].sval);
      ;}
    break;

  case 27:
#line 347 "HexDLParser.ypp"
    {
	(yyval.termval) = new DLVHEX_NAMESPACE Term((yysemantic_stack_[(1) - (1)].ival));
      ;}
    break;

  case 28:
#line 351 "HexDLParser.ypp"
    {
	(yyval.termval) = new DLVHEX_NAMESPACE Term;
      ;}
    break;

  case 29:
#line 360 "HexDLParser.ypp"
    {
       (yyval.atomsetval) = new DLVHEX_NAMESPACE AtomSet;
       (yyval.atomsetval)->insert(*(yysemantic_stack_[(1) - (1)].atomval));
       delete (yysemantic_stack_[(1) - (1)].atomval);
     ;}
    break;

  case 30:
#line 366 "HexDLParser.ypp"
    {
       (yyval.atomsetval) = (yysemantic_stack_[(3) - (1)].atomsetval);
       (yyval.atomsetval)->insert(*(yysemantic_stack_[(3) - (3)].atomval));
       delete (yysemantic_stack_[(3) - (3)].atomval);
     ;}
    break;

  case 31:
#line 379 "HexDLParser.ypp"
    {
      dlvhex::dl::Ontology::shared_pointer ontology = driver.getOntology();

      if (!ontology)
	{
	  throw DLVHEX_NAMESPACE PluginError("Cannot rewrite dl-atoms, option --ontology is empty.");
	}

      std::string tmp;

      if ((yysemantic_stack_[(3) - (1)].sval)->find("\"-") == 0)
	{
	  tmp = ontology->getNamespace() + (yysemantic_stack_[(3) - (1)].sval)->substr(2, (yysemantic_stack_[(3) - (1)].sval)->length() - 3);
	}
      else if ((yysemantic_stack_[(3) - (1)].sval)->find("\"") == 0)
	{
	  tmp = ontology->getNamespace() + (yysemantic_stack_[(3) - (1)].sval)->substr(1, (yysemantic_stack_[(3) - (1)].sval)->length() - 2);
	}
      else if ((yysemantic_stack_[(3) - (1)].sval)->find("-") == 0)
	{
	  tmp = ontology->getNamespace() + (yysemantic_stack_[(3) - (1)].sval)->substr(1, (yysemantic_stack_[(3) - (1)].sval)->length() - 1);
	}
      else
	{
	  tmp = ontology->getNamespace() + *(yysemantic_stack_[(3) - (1)].sval);
	}

      DLVHEX_NAMESPACE Tuple t;
      t.push_back(DLVHEX_NAMESPACE Term(tmp, true));
      t.push_back(DLVHEX_NAMESPACE Term(*(yysemantic_stack_[(3) - (3)].sval)));

      DLVHEX_NAMESPACE Term q(tmp);

      dlvhex::dl::TBox::ObjectsPtr concepts = ontology->getTBox().getConcepts();
      dlvhex::dl::TBox::ObjectsPtr roles = ontology->getTBox().getRoles();
      dlvhex::dl::TBox::ObjectsPtr datatypeRoles = ontology->getTBox().getDatatypeRoles();

      if ((yysemantic_stack_[(3) - (2)].cval) == '+') // PMOP is +=
	{
	  if (concepts->find(q) != concepts->end())
	    {
	      (yyval.atomval) = new DLVHEX_NAMESPACE AtomPtr(new DLVHEX_NAMESPACE Atom("pc", t));
	    }
	  else if (roles->find(q) != roles->end() ||
		   datatypeRoles->find(q) != datatypeRoles->end()
		   )
	    {
	      (yyval.atomval) = new DLVHEX_NAMESPACE AtomPtr(new DLVHEX_NAMESPACE Atom("pr", t));
	    }
	  else
	    {
	      std::ostringstream oss;
	      oss << "Incompatible dl-atom-op `" << *(yysemantic_stack_[(3) - (1)].sval) << "+=" << *(yysemantic_stack_[(3) - (3)].sval) << "' supplied: ";
	      oss << "Cannot find " << q << " in " << ontology->getRealURI();
	      throw DLVHEX_NAMESPACE PluginError(oss.str());
	    }
	}
      else if ((yysemantic_stack_[(3) - (2)].cval) == '-') // PMOP is -=
	{
	  if (concepts->find(q) != concepts->end())
	    {
	      (yyval.atomval) = new DLVHEX_NAMESPACE AtomPtr(new DLVHEX_NAMESPACE Atom("mc", t));
	    }
	  else if (roles->find(q) != roles->end() ||
		   datatypeRoles->find(q) != datatypeRoles->end()
		   )
	    {
	      (yyval.atomval) = new DLVHEX_NAMESPACE AtomPtr(new DLVHEX_NAMESPACE Atom("mr", t));
	    }
	  else
	    {
	      std::ostringstream oss;
	      oss << "Incompatible dl-atom-op `" << *(yysemantic_stack_[(3) - (1)].sval) << "-=" << *(yysemantic_stack_[(3) - (3)].sval) << "' supplied: ";
	      oss << "Cannot find " << q << " in " << ontology->getRealURI();
	      throw DLVHEX_NAMESPACE PluginError(oss.str());
	    }
	}
      else // hm, PMOP is neither '+' nor '-', this is a programming error
	{
	  assert(false);
	}

      delete (yysemantic_stack_[(3) - (1)].sval);
      delete (yysemantic_stack_[(3) - (3)].sval);
    ;}
    break;


    /* Line 675 of lalr1.cc.  */
#line 795 "HexDLParser.cpp"
	default: break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse look-ahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    if (yyn == yyfinal_)
      goto yyacceptlab;

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the look-ahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		   &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyeof_ && yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  HexDLParser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char HexDLParser::yypact_ninf_ = -33;
  const signed char
  HexDLParser::yypact_[] =
  {
       -33,    13,   -33,    16,    23,    54,    55,    22,     9,     3,
       9,     9,   -33,   -33,   -33,   -33,    31,   -33,    63,    64,
      45,   -33,    21,    12,    38,   -33,    33,    37,    27,     9,
      65,   -33,    27,     9,    27,    15,    15,    27,    15,    15,
      66,    27,    27,    -1,   -33,   -33,   -33,   -33,    36,   -33,
     -33,    53,   -33,    56,   -33,    56,    49,    29,    14,    63,
     -33,   -33,   -33,   -33,    46,   -33,     9,    27,    27,    27,
     -33,    58,   -33,   -33,   -33,   -33
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  HexDLParser::yydefact_[] =
  {
         2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     3,    26,    27,    28,     0,    24,    13,     0,
       0,    17,     0,     0,     0,    29,     0,     0,    21,     0,
       0,    14,    21,     0,    21,     0,     0,    21,     0,     0,
       0,    21,    21,     0,    12,    25,    31,     4,     0,     6,
      13,     0,    18,    19,     8,    20,     0,     0,     0,     0,
      30,    10,    11,    22,     0,    15,     0,    21,    21,    21,
      23,     0,     5,     7,     9,    16
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  HexDLParser::yypgoto_[] =
  {
       -33,   -33,   -33,    -8,    40,    20,    39,   -32,     1,   -26,
     -33,    32
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  HexDLParser::yydefgoto_[] =
  {
        -1,     1,     7,    51,    21,    22,    23,    44,    16,    17,
      24,    25
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char HexDLParser::yytable_ninf_ = -1;
  const unsigned char
  HexDLParser::yytable_[] =
  {
        47,    20,    49,    45,    13,    54,    14,    48,    18,    61,
      62,    26,    27,     2,    13,    63,    14,    19,    15,     3,
      50,     4,     5,     6,    37,    12,    69,     8,    15,    19,
      38,    56,    38,    34,     9,    72,    73,    74,    35,    36,
      71,    68,    43,    28,    64,    41,    35,    36,    29,    42,
      29,    39,    65,    66,    29,    40,    53,    32,    55,    57,
      33,    67,    70,    29,    33,    10,    11,    30,    33,    31,
      46,    59,    60,    35,    75,    52,     0,     0,    58
  };

  /* YYCHECK.  */
  const signed char
  HexDLParser::yycheck_[] =
  {
        32,     9,    34,    29,     5,    37,     7,    33,     5,    41,
      42,    10,    11,     0,     5,    16,     7,    14,    19,     6,
       5,     8,     9,    10,    12,     3,    12,    11,    19,    14,
      18,    39,    18,    12,    11,    67,    68,    69,    17,    18,
      66,    12,    15,    12,    43,    12,    17,    18,    17,    12,
      17,    13,    16,    17,    17,    17,    36,    12,    38,    39,
      15,    12,    16,    17,    15,    11,    11,     4,    15,     5,
       5,     5,    40,    17,    16,    35,    -1,    -1,    39
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  HexDLParser::yystos_[] =
  {
         0,    21,     0,     6,     8,     9,    10,    22,    11,    11,
      11,    11,     3,     5,     7,    19,    28,    29,     5,    14,
      23,    24,    25,    26,    30,    31,    28,    28,    12,    17,
       4,     5,    12,    15,    12,    17,    18,    12,    18,    13,
      17,    12,    12,    15,    27,    29,     5,    27,    29,    27,
       5,    23,    24,    25,    27,    25,    23,    25,    26,     5,
      31,    27,    27,    16,    28,    16,    17,    12,    12,    12,
      16,    29,    27,    27,    27,    16
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  HexDLParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,    91,    93,    59,    45,    40,    41,    44,   118,    95
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  HexDLParser::yyr1_[] =
  {
         0,    20,    21,    21,    22,    22,    22,    22,    22,    22,
      22,    22,    22,    23,    23,    24,    24,    25,    25,    26,
      26,    27,    27,    27,    28,    28,    29,    29,    29,    30,
      30,    31
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  HexDLParser::yyr2_[] =
  {
         0,     2,     0,     3,     5,     7,     5,     7,     5,     7,
       5,     5,     5,     1,     2,     4,     6,     1,     3,     3,
       3,     0,     2,     3,     1,     3,     1,     1,     1,     1,
       3,     3
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const HexDLParser::yytname_[] =
  {
    "$end", "error", "$undefined", "SENTINEL", "PMOP", "STRING",
  "DLEXTATOM", "NUMBER", "DLATOM", "CQATOM", "UCQATOM", "'['", "']'",
  "';'", "'-'", "'('", "')'", "','", "'v'", "'_'", "$accept", "dlatoms",
  "dlatom", "dlquery", "atom", "cq", "ucq", "output", "terms", "term",
  "ops", "op", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const HexDLParser::rhs_number_type
  HexDLParser::yyrhs_[] =
  {
        21,     0,    -1,    -1,    21,    22,     3,    -1,     8,    11,
      23,    12,    27,    -1,     8,    11,    30,    13,    23,    12,
      27,    -1,     8,    11,    25,    12,    27,    -1,     8,    11,
      30,    13,    25,    12,    27,    -1,     8,    11,    26,    12,
      27,    -1,     8,    11,    30,    13,    26,    12,    27,    -1,
       9,    11,    28,    12,    27,    -1,    10,    11,    28,    12,
      27,    -1,     6,    11,    28,    12,    27,    -1,     5,    -1,
      14,     5,    -1,    23,    15,    29,    16,    -1,    23,    15,
      29,    17,    29,    16,    -1,    24,    -1,    25,    17,    24,
      -1,    25,    18,    25,    -1,    26,    18,    25,    -1,    -1,
      15,    16,    -1,    15,    28,    16,    -1,    29,    -1,    28,
      17,    29,    -1,     5,    -1,     7,    -1,    19,    -1,    31,
      -1,    30,    17,    31,    -1,     5,     4,     5,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  HexDLParser::yyprhs_[] =
  {
         0,     0,     3,     4,     8,    14,    22,    28,    36,    42,
      50,    56,    62,    68,    70,    73,    78,    85,    87,    91,
      95,    99,   100,   103,   107,   109,   113,   115,   117,   119,
     121,   125
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  HexDLParser::yyrline_[] =
  {
         0,   131,   131,   132,   147,   154,   163,   171,   180,   188,
     197,   212,   227,   241,   245,   254,   265,   281,   287,   296,
     302,   311,   314,   318,   326,   331,   341,   346,   350,   359,
     365,   378
  };

  // Print the state stack on the debug stream.
  void
  HexDLParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  HexDLParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "), ";
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  HexDLParser::token_number_type
  HexDLParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      15,    16,     2,     2,    17,    14,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    13,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    11,     2,    12,     2,    19,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    18,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int HexDLParser::yyeof_ = 0;
  const int HexDLParser::yylast_ = 78;
  const int HexDLParser::yynnts_ = 12;
  const int HexDLParser::yyempty_ = -2;
  const int HexDLParser::yyfinal_ = 2;
  const int HexDLParser::yyterror_ = 1;
  const int HexDLParser::yyerrcode_ = 256;
  const int HexDLParser::yyntokens_ = 20;

  const unsigned int HexDLParser::yyuser_token_number_max_ = 265;
  const HexDLParser::token_number_type HexDLParser::yyundef_token_ = 2;

} // namespace yy

#line 467 "HexDLParser.ypp"



//
// error method of bison parser -> call error method of the
// corresponding driver
//
void
yy::HexDLParser::error(const yy::HexDLParser::location_type& l, const std::string& m)
{
  driver.error(l, m);
}


//
// abuse YY_DECL as bison callback for a pure C++ parsing engine
//
YY_DECL
{
  HexDLFlexLexer* lex = driver.getLexer();
  lex->lexval = yylval; // set current semantic value
  lex->lexloc = yylloc; // and the current location,
  return (yy::HexDLParser::token_type) lex->yylex (); // finally call the drivers lexer
} 


// Local Variables:
// mode: C++
// End:

