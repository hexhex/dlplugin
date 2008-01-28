/**
 * @file   Terms.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A set of Terms.
 * 
 * 
 */

#ifndef _DLVHEX_DF_TERMS_H
#define _DLVHEX_DF_TERMS_H

#include <vector>
#include "MTerm.h"

namespace dlvhex {
namespace df {

class Terms {
private:
	std::vector<MTerm> terms;
public:
	Terms();

	void addTerm(MTerm term_);
	void removeLastTerm();
	bool isEmpty();
	std::string toString();
	std::vector<MTerm> getMTerms();
};

}	// df
}	// dlvhex

#endif /* _DLVHEX_DF_TERMS_H */
