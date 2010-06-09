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


#include "RacerAnswerParser.hpp"

/* User implementation prologue.  */
#line 88 "RacerAnswerParser.ypp"


#include "RacerFlexLexer.h"

#define YY_DECL \
yy::RacerAnswerParser::token_type \
yylex (yy::RacerAnswerParser::semantic_type* yylval, \
       yy::RacerAnswerParser::location_type* yylloc, \
       dlvhex::dl::racer::RacerAnswerDriver& driver)

// declare YY_DECL function
YY_DECL;



/* Line 317 of lalr1.cc.  */
#line 57 "RacerAnswerParser.cpp"

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
  RacerAnswerParser::yytnamerr_ (const char *yystr)
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
  RacerAnswerParser::RacerAnswerParser (dlvhex::dl::racer::RacerAnswerDriver& driver_yyarg, dlvhex::dl::Answer& answer_yyarg)
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
      driver (driver_yyarg),
      answer (answer_yyarg)
  {
  }

  RacerAnswerParser::~RacerAnswerParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  RacerAnswerParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
        case 3: /* "STRING" */
#line 121 "RacerAnswerParser.ypp"
	{ debug_stream() << *(yyvaluep->sval); };
#line 188 "RacerAnswerParser.cpp"
	break;
      case 4: /* "URI" */
#line 121 "RacerAnswerParser.ypp"
	{ debug_stream() << *(yyvaluep->sval); };
#line 193 "RacerAnswerParser.cpp"
	break;
      case 5: /* "NUMBER" */
#line 122 "RacerAnswerParser.ypp"
	{ debug_stream() << (yyvaluep->ival); };
#line 198 "RacerAnswerParser.cpp"
	break;
       default:
	  break;
      }
  }


  void
  RacerAnswerParser::yy_symbol_print_ (int yytype,
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
  RacerAnswerParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
        case 3: /* "STRING" */
#line 117 "RacerAnswerParser.ypp"
	{ delete (yyvaluep->sval); };
#line 233 "RacerAnswerParser.cpp"
	break;
      case 4: /* "URI" */
#line 117 "RacerAnswerParser.ypp"
	{ delete (yyvaluep->sval); };
#line 238 "RacerAnswerParser.cpp"
	break;
      case 26: /* "pair" */
#line 117 "RacerAnswerParser.ypp"
	{ delete (yyvaluep->tupleval); };
#line 243 "RacerAnswerParser.cpp"
	break;
      case 28: /* "tuple" */
#line 117 "RacerAnswerParser.ypp"
	{ delete (yyvaluep->tupleval); };
#line 248 "RacerAnswerParser.cpp"
	break;
      case 29: /* "individual" */
#line 117 "RacerAnswerParser.ypp"
	{ delete (yyvaluep->termval); };
#line 253 "RacerAnswerParser.cpp"
	break;

	default:
	  break;
      }
  }

  void
  RacerAnswerParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

  std::ostream&
  RacerAnswerParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  RacerAnswerParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  RacerAnswerParser::debug_level_type
  RacerAnswerParser::debug_level () const
  {
    return yydebug_;
  }

  void
  RacerAnswerParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }


  int
  RacerAnswerParser::parse ()
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
	  case 2:
#line 127 "RacerAnswerParser.ypp"
    {
	 answer.setWarningMessage(*(yysemantic_stack_[(4) - (3)].sval));
	 delete (yysemantic_stack_[(4) - (3)].sval);
       ;}
    break;

  case 3:
#line 132 "RacerAnswerParser.ypp"
    {
	 answer.setErrorMessage(*(yysemantic_stack_[(4) - (2)].sval));
	 delete (yysemantic_stack_[(4) - (2)].sval);

	 answer.setWarningMessage(*(yysemantic_stack_[(4) - (3)].sval));
	 delete (yysemantic_stack_[(4) - (3)].sval);

	 // Racer returns "ABox XXX is incoherent." for 'old' Racer
	 // queries, and "ABox XXX is inconsistent! Querying denied."
	 // for nRQL queries.
	 if (answer.getErrorMessage().find("incoherent") != std::string::npos ||
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
       ;}
    break;

  case 7:
#line 162 "RacerAnswerParser.ypp"
    {
	   answer.setAnswer(true);
 	   answer.addTuple(DLVHEX_NAMESPACE Tuple());
	 ;}
    break;

  case 8:
#line 167 "RacerAnswerParser.ypp"
    {
	   answer.setAnswer(false);
	 ;}
    break;

  case 12:
#line 182 "RacerAnswerParser.ypp"
    {
	  delete (yysemantic_stack_[(2) - (2)].sval);
	;}
    break;

  case 13:
#line 186 "RacerAnswerParser.ypp"
    {
	  delete (yysemantic_stack_[(3) - (2)].sval);
	;}
    break;

  case 14:
#line 190 "RacerAnswerParser.ypp"
    {
	  delete (yysemantic_stack_[(1) - (1)].sval);
	;}
    break;

  case 15:
#line 197 "RacerAnswerParser.ypp"
    {
	       answer.addTuple(DLVHEX_NAMESPACE Tuple(1, *(yysemantic_stack_[(1) - (1)].termval)));
	       delete (yysemantic_stack_[(1) - (1)].termval);
	     ;}
    break;

  case 16:
#line 202 "RacerAnswerParser.ypp"
    { 
	       answer.addTuple(DLVHEX_NAMESPACE Tuple(1, *(yysemantic_stack_[(2) - (2)].termval)));
	       delete (yysemantic_stack_[(2) - (2)].termval);
	     ;}
    break;

  case 17:
#line 210 "RacerAnswerParser.ypp"
    {
	 answer.addTuple(*(yysemantic_stack_[(3) - (2)].tupleval));
	 delete (yysemantic_stack_[(3) - (2)].tupleval);
       ;}
    break;

  case 18:
#line 215 "RacerAnswerParser.ypp"
    {
	 answer.addTuple(*(yysemantic_stack_[(4) - (3)].tupleval));
	 delete (yysemantic_stack_[(4) - (3)].tupleval);
       ;}
    break;

  case 19:
#line 224 "RacerAnswerParser.ypp"
    {
	(yyval.tupleval) = new DLVHEX_NAMESPACE Tuple;
        (yyval.tupleval)->push_back(*(yysemantic_stack_[(2) - (1)].termval));
        (yyval.tupleval)->push_back(*(yysemantic_stack_[(2) - (2)].termval));
	delete (yysemantic_stack_[(2) - (1)].termval);
	delete (yysemantic_stack_[(2) - (2)].termval);
      ;}
    break;

  case 20:
#line 235 "RacerAnswerParser.ypp"
    {
	  answer.addTuple(*(yysemantic_stack_[(3) - (2)].tupleval));
	  delete (yysemantic_stack_[(3) - (2)].tupleval);
	;}
    break;

  case 21:
#line 240 "RacerAnswerParser.ypp"
    {
	  answer.addTuple(*(yysemantic_stack_[(4) - (3)].tupleval));
	  delete (yysemantic_stack_[(4) - (3)].tupleval);
	;}
    break;

  case 22:
#line 249 "RacerAnswerParser.ypp"
    {
	 (yyval.tupleval) = new DLVHEX_NAMESPACE Tuple(1, *(yysemantic_stack_[(4) - (3)].termval));
	 delete (yysemantic_stack_[(4) - (3)].termval);
       ;}
    break;

  case 23:
#line 254 "RacerAnswerParser.ypp"
    {
	 (yyval.tupleval) = (yysemantic_stack_[(5) - (1)].tupleval);
	 (yyval.tupleval)->push_back(*(yysemantic_stack_[(5) - (4)].termval));
	 delete (yysemantic_stack_[(5) - (4)].termval);
       ;}
    break;

  case 24:
#line 264 "RacerAnswerParser.ypp"
    {
	      // escape individual in turtle syntax
	      (yysemantic_stack_[(3) - (2)].sval)->insert(0, 1, '<');
	      (yysemantic_stack_[(3) - (2)].sval)->push_back('>');
	      (yyval.termval) = new DLVHEX_NAMESPACE Term(*(yysemantic_stack_[(3) - (2)].sval), true);
	      delete (yysemantic_stack_[(3) - (2)].sval);
	    ;}
    break;

  case 25:
#line 272 "RacerAnswerParser.ypp"
    {
	      (yyval.termval) = new DLVHEX_NAMESPACE Term(*(yysemantic_stack_[(1) - (1)].sval), true);
	      delete (yysemantic_stack_[(1) - (1)].sval);
	    ;}
    break;

  case 26:
#line 277 "RacerAnswerParser.ypp"
    {
	      (yyval.termval) = new DLVHEX_NAMESPACE Term((yysemantic_stack_[(1) - (1)].ival));
	    ;}
    break;

  case 27:
#line 296 "RacerAnswerParser.ypp"
    {
	    delete (yysemantic_stack_[(2) - (2)].sval);
	  ;}
    break;

  case 28:
#line 300 "RacerAnswerParser.ypp"
    {
	    delete (yysemantic_stack_[(3) - (3)].sval);
	  ;}
    break;

  case 29:
#line 304 "RacerAnswerParser.ypp"
    {
	    delete (yysemantic_stack_[(3) - (3)].sval);
	  ;}
    break;

  case 30:
#line 308 "RacerAnswerParser.ypp"
    {
	    delete (yysemantic_stack_[(4) - (4)].sval);
	  ;}
    break;

  case 31:
#line 312 "RacerAnswerParser.ypp"
    {
	    delete (yysemantic_stack_[(4) - (3)].sval);
	  ;}
    break;

  case 32:
#line 316 "RacerAnswerParser.ypp"
    {
	    delete (yysemantic_stack_[(5) - (4)].sval);
	  ;}
    break;


    /* Line 675 of lalr1.cc.  */
#line 658 "RacerAnswerParser.cpp"
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
  RacerAnswerParser::yysyntax_error_ (int yystate, int tok)
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
  const signed char RacerAnswerParser::yypact_ninf_ = -8;
  const signed char
  RacerAnswerParser::yypact_[] =
  {
        34,    14,     1,     6,    -8,    -8,    -8,     0,     5,     8,
       7,    -8,    -8,    -8,    16,    -8,    -8,    -8,    26,    22,
      27,    11,    33,    -8,    -8,    36,    41,    45,    19,    24,
      35,     4,    42,    -8,    -8,     4,    -8,    46,    -8,    -8,
      -8,    -8,    38,    -2,    43,     4,    -8,    19,    -8,    -8,
      -8,    47,    37,    56,    48,    -8,    58,    -1,    50,     4,
      -8,    -8,    51,    60,    -8,    -8,    63,    -8,    55,    -8,
      54,    -8,    -8,    -8
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  RacerAnswerParser::yydefact_[] =
  {
         0,     0,     0,     0,    14,     8,     7,     0,     0,     0,
       0,     4,     5,     6,     0,     1,    25,    26,     0,     0,
       0,     0,     0,    15,    12,     0,     0,     0,     0,     0,
       0,     0,     0,     9,    16,     0,    10,     0,    11,    13,
       2,     3,     0,     0,     0,     0,    17,     0,    20,    19,
      24,     0,     0,     0,     0,    27,     0,     0,     0,     0,
      18,    21,     0,     0,    29,    28,     0,    22,     0,    31,
       0,    30,    23,    32
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  RacerAnswerParser::yypgoto_[] =
  {
        -8,    -8,    -8,    -8,    -8,    -8,    -8,    -8,    39,    -8,
      32,    -7,    23
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  RacerAnswerParser::yydefgoto_[] =
  {
        -1,     3,    10,    11,    12,    13,    20,    21,    29,    22,
      30,    31,    45
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char RacerAnswerParser::yytable_ninf_ = -1;
  const unsigned char
  RacerAnswerParser::yytable_[] =
  {
        23,    55,    65,    16,    14,    17,    15,    16,    24,    17,
      26,    18,    25,    34,    19,    56,    66,     4,    19,    27,
       5,     6,    35,    36,    49,     7,    32,     8,     9,    16,
      16,    17,    17,    42,    43,    44,    46,    28,    58,    33,
      19,    19,     1,     2,    37,    38,    47,    48,    47,    61,
      39,    40,    68,    53,    54,    41,    50,    28,    57,    60,
      62,    64,    67,    63,    70,    69,    71,    72,    73,    52,
      59,     0,     0,     0,    51
  };

  /* YYCHECK.  */
  const signed char
  RacerAnswerParser::yycheck_[] =
  {
         7,     3,     3,     3,     3,     5,     0,     3,     3,     5,
       3,    11,     4,    20,    14,    17,    17,     3,    14,     3,
       6,     7,    11,    12,    31,    11,     4,    13,    14,     3,
       3,     5,     5,    14,    15,    16,    12,    11,    45,    12,
      14,    14,     8,     9,    11,    12,    11,    12,    11,    12,
      14,    10,    59,    15,    16,    10,    14,    11,    15,    12,
       4,     3,    12,    15,     4,    14,     3,    12,    14,    37,
      47,    -1,    -1,    -1,    35
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  RacerAnswerParser::yystos_[] =
  {
         0,     8,     9,    19,     3,     6,     7,    11,    13,    14,
      20,    21,    22,    23,     3,     0,     3,     5,    11,    14,
      24,    25,    27,    29,     3,     4,     3,     3,    11,    26,
      28,    29,     4,    12,    29,    11,    12,    11,    12,    14,
      10,    10,    14,    15,    16,    30,    12,    11,    12,    29,
      14,    26,    28,    15,    16,     3,    17,    15,    29,    30,
      12,    12,     4,    15,     3,     3,    17,    12,    29,    14,
       4,     3,    12,    14
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  RacerAnswerParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
      10,    40,    41,    58,   124,    63,    36,    42
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  RacerAnswerParser::yyr1_[] =
  {
         0,    18,    19,    19,    20,    20,    20,    21,    21,    22,
      22,    22,    23,    23,    23,    24,    24,    25,    25,    26,
      27,    27,    28,    28,    29,    29,    29,    30,    30,    30,
      30,    30,    30
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  RacerAnswerParser::yyr2_[] =
  {
         0,     2,     4,     4,     1,     1,     1,     1,     1,     3,
       3,     3,     2,     3,     1,     1,     2,     3,     4,     2,
       3,     4,     4,     5,     3,     1,     1,     2,     3,     3,
       4,     4,     5
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const RacerAnswerParser::yytname_[] =
  {
    "$end", "error", "$undefined", "STRING", "URI", "NUMBER", "NIL", "TRUE",
  "ANSWER", "ERROR", "'\\n'", "'('", "')'", "':'", "'|'", "'?'", "'$'",
  "'*'", "$accept", "racer", "message", "boolean", "list", "status",
  "individuals", "pairs", "pair", "tuples", "tuple", "individual",
  "variable", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const RacerAnswerParser::rhs_number_type
  RacerAnswerParser::yyrhs_[] =
  {
        19,     0,    -1,     8,    20,     3,    10,    -1,     9,     3,
       3,    10,    -1,    21,    -1,    22,    -1,    23,    -1,     7,
      -1,     6,    -1,    11,    24,    12,    -1,    11,    25,    12,
      -1,    11,    27,    12,    -1,    13,     3,    -1,    14,     4,
      14,    -1,     3,    -1,    29,    -1,    24,    29,    -1,    11,
      26,    12,    -1,    25,    11,    26,    12,    -1,    29,    29,
      -1,    11,    28,    12,    -1,    27,    11,    28,    12,    -1,
      11,    30,    29,    12,    -1,    28,    11,    30,    29,    12,
      -1,    14,     4,    14,    -1,     3,    -1,     5,    -1,    15,
       3,    -1,    16,    15,     3,    -1,    15,    17,     3,    -1,
      16,    15,    17,     3,    -1,    14,    15,     4,    14,    -1,
      14,    16,    15,     4,    14,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  RacerAnswerParser::yyprhs_[] =
  {
         0,     0,     3,     8,    13,    15,    17,    19,    21,    23,
      27,    31,    35,    38,    42,    44,    46,    49,    53,    58,
      61,    65,    70,    75,    81,    85,    87,    89,    92,    96,
     100,   105,   110
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  RacerAnswerParser::yyrline_[] =
  {
         0,   126,   126,   131,   157,   157,   157,   161,   166,   175,
     176,   177,   181,   185,   189,   196,   201,   209,   214,   223,
     234,   239,   248,   253,   263,   271,   276,   295,   299,   303,
     307,   311,   315
  };

  // Print the state stack on the debug stream.
  void
  RacerAnswerParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  RacerAnswerParser::yy_reduce_print_ (int yyrule)
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
  RacerAnswerParser::token_number_type
  RacerAnswerParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      10,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    16,     2,     2,     2,
      11,    12,    17,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    13,     2,
       2,     2,     2,    15,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    14,     2,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int RacerAnswerParser::yyeof_ = 0;
  const int RacerAnswerParser::yylast_ = 74;
  const int RacerAnswerParser::yynnts_ = 13;
  const int RacerAnswerParser::yyempty_ = -2;
  const int RacerAnswerParser::yyfinal_ = 15;
  const int RacerAnswerParser::yyterror_ = 1;
  const int RacerAnswerParser::yyerrcode_ = 256;
  const int RacerAnswerParser::yyntokens_ = 18;

  const unsigned int RacerAnswerParser::yyuser_token_number_max_ = 264;
  const RacerAnswerParser::token_number_type RacerAnswerParser::yyundef_token_ = 2;

} // namespace yy

#line 321 "RacerAnswerParser.ypp"


// error method of bison parser -> call error method of the
// corresponding driver
void
yy::RacerAnswerParser::error(const yy::RacerAnswerParser::location_type& l,
			     const std::string& m)
{
  driver.error(l, m);
}


// abuse YY_DECL as bison callback for a pure C++ parsing engine
YY_DECL
{
  RacerFlexLexer* lex = driver.getLexer();
  lex->lexval = yylval; // set current semantic value
  lex->lexloc = yylloc; // and the current location,
  return (yy::RacerAnswerParser::token_type) lex->yylex (); // finally call the drivers lexer
}


// Local Variables:
// mode: C++
// End:

