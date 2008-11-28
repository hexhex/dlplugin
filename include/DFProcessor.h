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

#include <boost/spirit/core.hpp>
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
struct DefaultParser : boost::spirit::grammar<DefaultParser>
{ 
  /** 
   * @brief The grammar for a df file which can contain defaults and
   *        other arbitrary stuff ended by a dot '.'
   */ 
  template <typename ScannerT> 
  struct definition  
  { 
    definition(DefaultParser const& self) 
    {
      name_ = ( boost::spirit::range_p('a', 'z') | boost::spirit::range_p('A', 'Z') )
	>> *( boost::spirit::range_p('a', 'z') 
	      | boost::spirit::range_p('A', 'Z') 
	      | boost::spirit::range_p('0', '9') 
	      | boost::spirit::ch_p('_'));
      
      namespace_ = boost::spirit::str_p("#namespace")
	>> boost::spirit::chseq_p("(\"")
	>> (name_ | boost::spirit::str_p(""))
	>> boost::spirit::chseq_p("\",\"")
	>> +(boost::spirit::anychar_p - '\"')
	>> boost::spirit::chseq_p("\")");
      
      predicate_name_ = !(name_ >> boost::spirit::ch_p(':')) >> name_;

      variable_ = boost::spirit::range_p('A', 'Z') 
	>> *( boost::spirit::range_p('a', 'z') 
	      | boost::spirit::range_p('A', 'Z') 
	      | boost::spirit::range_p('0', '9') 
	      | boost::spirit::ch_p('_'));
      
      term_ = boost::spirit::ch_p('_')
	| constant_
	| variable_;
      
      terms_ = term_[&DFProcessor::push_lst_term]
	>> *(boost::spirit::ch_p(',') >> term_[&DFProcessor::push_lst_term]);
      
      predicate_ = predicate_name_ 
	>> boost::spirit::ch_p('(')
	>> terms_
	>> boost::spirit::ch_p(')');
      
      lit_ = boost::spirit::ch_p('-')
	>> predicate_
	| predicate_;
      
      conjunction_ = lit_[&DFProcessor::push_lst_literal]
	>> *(boost::spirit::ch_p('&') >> lit_[&DFProcessor::push_lst_literal]);
      
      prerequisite_ = conjunction_;
      
      justification_ = conjunction_;
      
      justifications_ = justification_[&DFProcessor::push_lst_justification]
	>> *(boost::spirit::ch_p(',') >> justification_[&DFProcessor::push_lst_justification]);
      
      conclusion_ = conjunction_;
      
      default_ = boost::spirit::ch_p('[')
	>> prerequisite_[&DFProcessor::set_prerequisite]
	>> boost::spirit::ch_p(';')
	>> justifications_[&DFProcessor::set_justifications]
	>> boost::spirit::chseq_p("]/[") 
	>> conclusion_[&DFProcessor::set_conclusion]
	>> boost::spirit::ch_p(']')
	>> !argument_[&DFProcessor::set_argument]
	
	| boost::spirit::chseq_p("[;")
	>> justifications_[&DFProcessor::set_justifications]
	>> boost::spirit::chseq_p("]/[") 
	>> conclusion_[&DFProcessor::set_conclusion]
	>> boost::spirit::ch_p(']')
	>> !argument_[&DFProcessor::set_argument]
	
	|  boost::spirit::ch_p('[')
	>> prerequisite_[&DFProcessor::set_prerequisite]
	>> boost::spirit::chseq_p(";]/[") 
	>> conclusion_[&DFProcessor::set_conclusion]
	>> boost::spirit::ch_p(']')
	>> !argument_[&DFProcessor::set_argument]
	
	|  boost::spirit::chseq_p("[;]/[")
	>> conclusion_[&DFProcessor::set_conclusion]
	>> boost::spirit::ch_p(']')
	>> !argument_[&DFProcessor::set_argument];
      
      argument_ = boost::spirit::ch_p('<')
	>> name_
	>> boost::spirit::ch_p('(')
	>> terms_
	>> boost::spirit::chseq_p(")>");
      
      others_ = *(boost::spirit::anychar_p - boost::spirit::ch_p('.')) >> boost::spirit::ch_p('.');
      
      //program_ = *(default_[&DFProcessor::print_out] | namespace_[&DFProcessor::print_out] | others_[&DFProcessor::print_out]);

      program_ = *(namespace_[&DFProcessor::push_namespace]) 
	>> *(default_[&DFProcessor::push_default] | others_[&DFProcessor::push_others]);
    }
    
    boost::spirit::rule<ScannerT> predicate_name_, name_, namespace_, constant_, variable_, term_, terms_, predicate_, lit_, conjunction_, prerequisite_, justification_, justifications_, conclusion_, argument_, default_, others_, program_;
    
    boost::spirit::rule<ScannerT> const& start() const 
    {
      return program_;
    }
  };
}; 

} // namespace df
} // namespace dlvhex

#endif // _DLVHEX_DF_DFPROCESSOR_H_
