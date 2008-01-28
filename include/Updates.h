/**
 * @file   Updates.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 17:20:24 2007
 * 
 * @brief  A set of Updates.
 * 
 * 
 */

#ifndef _DLVHEX_DF_UPDATES_H
#define _DLVHEX_DF_UPDATES_H

#include <vector>
#include "Update.h"

namespace dlvhex {
namespace df {

class Updates {
private:
	std::vector<Update> updates;
public:
	Updates();

	void addUpdate(const Update&);
	std::string toString();
};

}	// namespace df
}	// namespace dlvhex

#endif /* _DLVHEX_DF_UPDATES_H */
