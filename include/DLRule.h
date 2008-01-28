/**
 * @file   DLRule.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A DL Rule.
 * 
 * 
 */

#ifndef _DLVHEX_DF_DLRULE_H
#define _DLVHEX_DF_DLRULE_H

#include "DLAtom.h"
#include "Predicate.h"

namespace dlvhex {
namespace df {

class DLRule {
private:
	Predicate head;
	std::vector<Predicate> positive_bodies;		// positive/negative in terms of NAF
	std::vector<Predicate> negative_bodies;
	std::vector<DLAtom> positive_dl_atoms;
	std::vector<DLAtom> negative_dl_atoms;
public:
	DLRule(const Predicate&);

	void addPositiveBody(const Predicate&);
	void addNegativeBody(const Predicate&);
	void addPositiveDLAtom(const DLAtom&);
	void addNegativeDLAtom(const DLAtom&);
	std::string toString();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF_DLRULE_H */
