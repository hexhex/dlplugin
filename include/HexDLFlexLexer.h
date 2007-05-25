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
 * @file   HexDLFlexLexer.h
 * @author Thomas Krennwallner
 * @date   Sat Nov  4 15:21:58 2006
 * 
 * @brief  
 * 
 * 
 */

#ifndef _HEXDLFLEXLEXER
#define _HEXDLFLEXLEXER

// put FlexLexer.h into its own include guards or yyFlexLexer gets
// redefined
#ifndef __FLEX_LEXER_H
#undef yyFlexLexer
#define yyFlexLexer yyDLFlexLexer
#include <FlexLexer.h>
#endif

namespace dlvhex {
  namespace dl {
    // forward declaration
    class HexDLDriver;
  } // namespace dl
} // namespace dlvhex

/**
 * @brief Use a refined yyFlexLexer.
 *
 * Sad but true, we can't put HexDLFlexLexer into the
 * dlvhex::dl namespace, flex is not aware of namespaces.
 */
struct HexDLFlexLexer : public yyDLFlexLexer
{
  explicit
  HexDLFlexLexer(dlvhex::dl::HexDLDriver* d)
    : lexdrv(d), lexloc(0), lexval(0)
  { }

  HexDLFlexLexer(const HexDLFlexLexer& r)
    : yyDLFlexLexer(), lexdrv(r.lexdrv), lexloc(r.lexloc), lexval(r.lexval)
  { }

  HexDLFlexLexer&
  operator= (const HexDLFlexLexer& r)
  {
    if (this != &r)
      {
	lexdrv = r.lexdrv;
	lexloc = r.lexloc;
	lexval = r.lexval;
      }
    return *this;
  }

  ~HexDLFlexLexer() { }

  dlvhex::dl::HexDLDriver* lexdrv;
  yy::HexDLParser::location_type* lexloc;
  yy::HexDLParser::semantic_type* lexval;

  int yylex(); // implemented in HexDLScanner.lpp
};


#endif // _HEXDLFLEXLEXER_H


// Local Variables:
// mode: C++
// End:
