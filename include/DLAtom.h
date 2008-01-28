/**
 * @file   DLAtom.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A DLAtom of the form DL[\lambda;\query](Terms).
 * 
 * 
 */

#ifndef _DLVHEX_DF_DLATOM_H
#define _DLVHEX_DF_DLATOM_H

#include <string>
#include <vector>
#include "Updates.h"
#include "Terms.h"

namespace dlvhex {
namespace df {

class DLAtom {
private:
	Updates updates;	
	std::string query;
	Terms terms;
public:
	DLAtom(Updates, const std::string&, Terms);
	
	std::string toString();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF_DLATOM_H */
