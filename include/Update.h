/**
 * @file   Update.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  An update of the form: Concept/Role +=/-= Predicate.
 * 
 * 
 */

#ifndef _DLVHEX_DF_UPDATE_H
#define _DLVHEX_DF_UPDATE_H

#include <string>

namespace dlvhex {
namespace df {

enum Operator { UPLUS, UMINUS };

class Update {
private:
	std::string cr_name;					// Concept or Role name
	Operator op;									// operator += or -=
	std::string predicate_name;	// predicate used for updating the concept or role
public:
	Update(const std::string&, Operator, const std::string&);

	std::string toString();
};

}	// namespace df
}	// namespace dlvhex

#endif	/* _UPDATE_H */
