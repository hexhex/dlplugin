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

class Defaults {
private:
	unsigned int count;
	std::vector<Default> dfs;
public:
	Defaults();
	void addDefault(Default&);
	Updates getLambda();
	Updates getLambdaPrime();
	DLRules getDLRules();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF_DEFAULTS_H */
