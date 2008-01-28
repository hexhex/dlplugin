/**
 * @file   Default.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A Default of the form [A:B]/[C].
 * 
 * 
 */

#ifndef _DLVHEX_DF_DEFAULT_H
#define _DLVHEX_DF_DEFAULT_H

#include <vector>
#include "DLRules.h"
#include "Defaults.h"
#include "Predicate.h"
#include "Update.h"

#define PREFIX_IN 	"in_"
#define	PREFIX_OUT 	"out_"
#define	PREFIX_PRED "p_"

namespace dlvhex {
namespace df {

class Defaults;

class Default {
private:
	bool is_null;
	Defaults* parent;
	std::vector<Predicate> premise;
	std::vector<Predicate> justification;
	Predicate conclusion;
	void gen_ground_rules(DLRules&, 
												const std::string&, const std::string&, 
												Terms&, std::vector<MTerm>&,
												std::vector<std::string>&, 
												std::vector<MTerm>::iterator&);
public:
	Default(bool);
	Default(std::vector<Predicate>, std::vector<Predicate>, Predicate);

	static Default null_default;
	bool isNULL();
	void setParent(Defaults*);
	Predicate getConclusion();
	Update getUpdate4Lambda();
	Update getUpdate4LambdaPrime();
	DLRules getDLRules();
	std::string toString();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF__H */
