/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics. 
 * 
 * Copyright (C) 2007, 2008 DAO Tran Minh 
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
 * @file   DFProcessor.h
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  The processor which contains the parser for default rules. 
 *  
 *  
 */

#ifndef _DLVHEX_DF_DFPROCESSOR_H_
#define _DLVHEX_DF_DFPROCESSOR_H_

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_common.hpp>
#include <boost/spirit/include/classic_confix.hpp>

#include <list>
#include <sstream>
#include "Defaults.h"
#include "Prefixes.h"

namespace dlvhex {
namespace df {

/**
 * @brief The driver of the parsing process
 */
class DFProcessor
{
 private:
  static std::list<MTerm> lst_term;
  static std::list<Predicate> lst_predicate;
  static std::list<Pred1Dim> lst_justification;
  static Pred1Dim prerequisite;
  static Pred1Dim conclusion;
  static Pred2Dim justifications;
  static Predicate argument;
  static Prefixes namespaces;

 public:
  static std::ostringstream others;
  static Defaults defaults;
  static bool hasDefaults();
  static void push_namespace(const char*, const char*);
  static void push_others(const char*, const char*);
  static void push_lst_term(const char*, const char*);
  static void push_lst_literal(const char*, const char*);
  static void push_lst_justification(const char*, const char*);
  static void set_prerequisite(const char*, const char*);
  static void set_conclusion(const char*, const char*);
  static void set_justifications(const char*, const char*);
  static void set_argument(const char*, const char*);
  static void push_default(const char*, const char*);
};

/**
 * @brief The parser
 */
struct DefaultParser : BOOST_SPIRIT_CLASSIC_NS :: grammar<DefaultParser>
{ 
  /** 
   * @brief The grammar for a df file which can contain defaults and
   *        other arbitrary stuff ended by a dot '.'
   */ 
  template <typename ScannerT> 
  struct definition  
  { 
    definition(DefaultParser const& /* self */) 
    {
      name_ = ( BOOST_SPIRIT_CLASSIC_NS :: range_p('a', 'z') | BOOST_SPIRIT_CLASSIC_NS :: range_p('A', 'Z') )
	>> *( BOOST_SPIRIT_CLASSIC_NS :: range_p('a', 'z') 
	      | BOOST_SPIRIT_CLASSIC_NS :: range_p('A', 'Z') 
	      | BOOST_SPIRIT_CLASSIC_NS :: range_p('0', '9') 
	      | BOOST_SPIRIT_CLASSIC_NS :: ch_p('_'));
      
      namespace_ = BOOST_SPIRIT_CLASSIC_NS :: str_p("#namespace")
	>> BOOST_SPIRIT_CLASSIC_NS :: chseq_p("(\"")
	>> (name_ | BOOST_SPIRIT_CLASSIC_NS :: str_p(""))
	>> BOOST_SPIRIT_CLASSIC_NS :: chseq_p("\",\"")
	>> +(BOOST_SPIRIT_CLASSIC_NS :: anychar_p - '\"')
	>> BOOST_SPIRIT_CLASSIC_NS :: chseq_p("\")");
      
      predicate_name_ = !(name_ >> BOOST_SPIRIT_CLASSIC_NS :: ch_p(':')) >> name_;

      constant_ = BOOST_SPIRIT_CLASSIC_NS :: chseq_p("\"")
	>> *(BOOST_SPIRIT_CLASSIC_NS :: anychar_p - '\"')
	>>  BOOST_SPIRIT_CLASSIC_NS :: chseq_p("\"");

      variable_ = BOOST_SPIRIT_CLASSIC_NS :: range_p('A', 'Z') 
	>> *( BOOST_SPIRIT_CLASSIC_NS :: range_p('a', 'z') 
	      | BOOST_SPIRIT_CLASSIC_NS :: range_p('A', 'Z') 
	      | BOOST_SPIRIT_CLASSIC_NS :: range_p('0', '9') 
	      | BOOST_SPIRIT_CLASSIC_NS :: ch_p('_'));
      
      term_ = BOOST_SPIRIT_CLASSIC_NS :: ch_p('_')
	| constant_
	| variable_;
      
      terms_ = term_[&DFProcessor::push_lst_term]
	>> *(BOOST_SPIRIT_CLASSIC_NS :: ch_p(',') >> term_[&DFProcessor::push_lst_term]);
      
      predicate_ = predicate_name_ 
	>> BOOST_SPIRIT_CLASSIC_NS :: ch_p('(')
	>> terms_
	>> BOOST_SPIRIT_CLASSIC_NS :: ch_p(')');
      
      lit_ = BOOST_SPIRIT_CLASSIC_NS :: ch_p('-')
	>> predicate_
	| predicate_;
      
      conjunction_ = lit_[&DFProcessor::push_lst_literal]
	>> *(BOOST_SPIRIT_CLASSIC_NS :: ch_p('&') >> lit_[&DFProcessor::push_lst_literal]);
      
      prerequisite_ = conjunction_;
      
      justification_ = conjunction_;
      
      justifications_ = justification_[&DFProcessor::push_lst_justification]
	>> *(BOOST_SPIRIT_CLASSIC_NS :: ch_p(',') >> justification_[&DFProcessor::push_lst_justification]);
      
      conclusion_ = conjunction_;
      
      default_ = BOOST_SPIRIT_CLASSIC_NS :: ch_p('[')
	>> prerequisite_[&DFProcessor::set_prerequisite]
	>> BOOST_SPIRIT_CLASSIC_NS :: ch_p(';')
	>> justifications_[&DFProcessor::set_justifications]
	>> BOOST_SPIRIT_CLASSIC_NS :: chseq_p("]/[") 
	>> conclusion_[&DFProcessor::set_conclusion]
	>> BOOST_SPIRIT_CLASSIC_NS :: ch_p(']')
	>> !argument_[&DFProcessor::set_argument]
	
	| BOOST_SPIRIT_CLASSIC_NS :: chseq_p("[;")
	>> justifications_[&DFProcessor::set_justifications]
	>> BOOST_SPIRIT_CLASSIC_NS :: chseq_p("]/[") 
	>> conclusion_[&DFProcessor::set_conclusion]
	>> BOOST_SPIRIT_CLASSIC_NS :: ch_p(']')
	>> !argument_[&DFProcessor::set_argument]
	
	|  BOOST_SPIRIT_CLASSIC_NS :: ch_p('[')
	>> prerequisite_[&DFProcessor::set_prerequisite]
	>> BOOST_SPIRIT_CLASSIC_NS :: chseq_p(";]/[") 
	>> conclusion_[&DFProcessor::set_conclusion]
	>> BOOST_SPIRIT_CLASSIC_NS :: ch_p(']')
	>> !argument_[&DFProcessor::set_argument]
	
	|  BOOST_SPIRIT_CLASSIC_NS :: chseq_p("[;]/[")
	>> conclusion_[&DFProcessor::set_conclusion]
	>> BOOST_SPIRIT_CLASSIC_NS :: ch_p(']')
	>> !argument_[&DFProcessor::set_argument];
      
      argument_ = BOOST_SPIRIT_CLASSIC_NS :: ch_p('<')
	>> name_
	>> BOOST_SPIRIT_CLASSIC_NS :: ch_p('(')
	>> terms_
	>> BOOST_SPIRIT_CLASSIC_NS :: chseq_p(")>");
      
      others_ = *(BOOST_SPIRIT_CLASSIC_NS :: anychar_p - BOOST_SPIRIT_CLASSIC_NS :: ch_p('.')) >> BOOST_SPIRIT_CLASSIC_NS :: ch_p('.');
      
      //program_ = *(default_[&DFProcessor::print_out] | namespace_[&DFProcessor::print_out] | others_[&DFProcessor::print_out]);

      program_ = *(namespace_[&DFProcessor::push_namespace] | BOOST_SPIRIT_CLASSIC_NS :: discard_node_d[BOOST_SPIRIT_CLASSIC_NS :: comment_p("%")]) 
       
	>> *(default_[&DFProcessor::push_default] | others_[&DFProcessor::push_others] | BOOST_SPIRIT_CLASSIC_NS :: discard_node_d[BOOST_SPIRIT_CLASSIC_NS :: comment_p("%")]);
    }
    
    BOOST_SPIRIT_CLASSIC_NS :: rule<ScannerT> predicate_name_, name_, namespace_, constant_, variable_, term_, terms_, predicate_, lit_, conjunction_, prerequisite_, justification_, justifications_, conclusion_, argument_, default_, others_, program_;
    
    BOOST_SPIRIT_CLASSIC_NS :: rule<ScannerT> const& start() const 
    {
      return program_;
    }
  };
}; 

} // namespace df
} // namespace dlvhex

#endif // _DLVHEX_DF_DFPROCESSOR_H_
