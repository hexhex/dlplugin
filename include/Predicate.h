/**
 * @file   Predicate.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A Predicate.
 * 
 * 
 */

#ifndef _DLVHEX_DF_PREDICATE_H
#define _DLVHEX_DF_PREDICATE_H

#include "Terms.h"

namespace dlvhex {
namespace df {

class Predicate {
private:
	bool isStrongNegated;
	std::string predicate_name;
	Terms terms;
public:
	Predicate();
	Predicate(const std::string&);
	Predicate(const std::string&, const Terms&);
	Predicate(const std::string&, const MTerm&);
	Predicate(bool, const std::string&);
	Predicate(bool, const std::string&, const Terms&);

	bool isStronglyNegated();
	std::string getPredicateName();
	std::string getLiteralName();
	std::string getNegatedLiteralName();
	Terms getTerms();
	void applyNegation();
	std::string toString();
	std::string toNegatedString();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF_PREDICATE_H */
