/**
 * @file   DLRules.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A set of DL Rules.
 * 
 * 
 */

#ifndef _DLVHEX_DF_DLRULES_H
#define _DLVHEX_DF_DLRULES_H

#include <vector>
#include "DLRule.h"

namespace dlvhex {
namespace df {

class DLRules {
private:
	std::vector<DLRule> dlrules;
public:
	DLRules();
	void addDLRule(const DLRule&);
	void insertDLRules(const DLRules&);
	std::string toString();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF_DLRULES_H */
