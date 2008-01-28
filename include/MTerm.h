/**
 * @file   MTern.h
 * @author DAO Tran Minh
 * @date   Wed Dec 19 11:18:24 2007
 * 
 * @brief  A Term. Have to used MTerm to avoid name clash with dlvhex::Term (right now dlvhex doesn't have any namespace)
 * 
 * 
 */

#ifndef _DLVHEX_DF_TERM_H
#define _DLVHEX_DF_TERM_H

#include <string>

namespace dlvhex {
namespace df {

class MTerm {
private:
	std::string term_representation;
	bool isvar;
public:
	MTerm(const std::string&);
	MTerm(const std::string&, bool);
	bool isVar();
	std::string toString();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF_TERM_H */

