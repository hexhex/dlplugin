/** 
 * @file   Defaults.h 
 * @author DAO Tran Minh 
 * @date   Tue Dec 18 17:20:24 2007 
 *  
 * @brief  A set of Defaults. 
 *  
 *  
 */ 
 
#ifndef _DLVHEX_DF_DEFAULTS_H 
#define _DLVHEX_DF_DEFAULTS_H 
 
#include <vector> 
#include "Default.h" 
#include "Updates.h" 
 
namespace dlvhex { 
namespace df { 
 
  class Default; 
  
  /** 
   * @brief A set of defaults 
   */ 
  class Defaults  
    { 
    private: 
      /// Number of defaults in this set 
      unsigned int count; 
      
      /// The vector to restore all defaults 
      std::vector<Default> dfs; 
      
      /** 
       * Rename variables in a set of predicates and a Terms 
       * 
       * @param ps The set of Predicate to be renamed 
       * @param terms The set of Terms to be renamed 
       * @param str_id The id from a Default, to be added to each variable's name 
       */ 
      void 
	rename_terms(Pred1Dim& ps, Terms& terms, std::string& str_id); 
      
      /** 
       * Check if one predicate is unifiable with some predicates in a vector of Predicate 
       * 
       * @param p The predicate to be checked 
       * @param ps The vetor of Predicate(s) to be checked 
       * 
       * @return A vector of Unifier(s) 
       */ 
      Unifier1Dim 
	check_unifiable(Predicate& p, Pred1Dim& ps); 
      
      /** 
       * Check if one vector of Predicate(s) can force another vector of Predicate(s) to be IN 
       * ps1 forces ps2 to be IN iff for all p2 in ps2, there exists some p1 in ps1  
       * such that p1 and p2 are unifiable 
       * 
       * @param ps1 The forcing Predicate(s) 
       * @param ps2 The forced Predicate(s) 
       * 
       * @return A vector (V) of vector (v) of Unifier(s) 
       *         Each (v_i) must not be empty and includes a set of unifier 
       *         of p2_i corresponding to its unifiable predicate(s) in ps1 
       */ 
      Unifier2Dim 
	check_forcing_in(Pred1Dim&, Pred1Dim&); 
      
      /** 
       * Generate forcing in rules RECURSIVELY 
       * 
       * @param dlrs The set of DLRules to be generated 
       * @param uniset The set of Unifier(s) 
       * @param uniset_pos The iterator marking where we are in the uniset 
       * @param unifier The overall Unifier so far 
       * @param pn1 Forcing Default's ALL_IN predicate name 
       * @param ts1 Forcing Default's ALL_IN predicate Terms  
       * @param pn2 Forced Default's ALL_IN predicate name 
       * @param ts2 Forced Default's ALL_IN predicate Terms  
       */ 
      void 
	gen_force_in_rules(DLRules& dlrs, Unifier2Dim& uniset, 
			   Unifier2Dim::iterator uniset_pos, 
			   Unifier unifier, 
			   std::string pn1, Terms& ts1, 
			   std::string pn2, Terms& ts2); 
 
    public: 
      Defaults(); 
      
      void  
	addDefault(Default&); 
      
      Updates  
	getLambda(); 
      
      Updates  
	getLambdaPrime(); 
      
      /** 
       * @return All forcing IN and OUT rules 
       */ 
      DLRules 
	getForcingDLRules(); 
      
      /** 
       * @return All direct query to the KB rules 
       */ 
      DLRules 
	getDirectRules(); 
      
      /** 
       * @return All DLRules generated from this set of Defaults 
       * 
       * @param cqmode Mode for constructing queries in DLAtoms
       */ 
      DLRules  
	getDLRules(bool cqmode, int trans); 
    }; 
  
}	// namespace df 
}	// namespace dlvhex 
 
#endif /* _DLVHEX_DF_DEFAULTS_H */
