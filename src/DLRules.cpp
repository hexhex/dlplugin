/**
 * @file   DLRules.cpp
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A set of DL Rules.
 * 
 * 
 */

#include "DLRules.h"

namespace dlvhex {
namespace df {

DLRules::DLRules() 
{ }

void 
DLRules::addDLRule(const DLRule& dlrule) 
{
	dlrules.push_back(dlrule);
}

void 
DLRules::insertDLRules(const DLRules& dlrules_) 
{
	if (dlrules_.dlrules.size() > 0)
	{
		if (dlrules.size() > 0) 
		{
			dlrules.insert(dlrules.begin(), dlrules_.dlrules.begin(), dlrules_.dlrules.end());
		} 
		else 
		{
			dlrules = dlrules_.dlrules;
		}
	}
}

std::string 
DLRules::toString() 
{
	std::string tmp = "";
	std::vector<DLRule>::iterator pos;
	if (dlrules.size() > 0) 
	{
		for (pos = dlrules.begin(); pos != dlrules.end(); pos++) 
		{
			tmp += pos->toString();
		}
	}		
	return (tmp + "\n");
}

}	// namespace df
}	// namespace dlvhex
