/* dlvhex-dlplugin -- Integration of Answer-Set Programming and Description Logics.
 *
 * Copyright (C) 2007, 2008 DAO Tran Minh
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex-dlplugin is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * dlvhex-dlplugin is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dlvhex-dlplugin; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
 * @file   DFRewriter.h
 * @author DAO Tran Minh
 * @date   Tue Dec 18 22:10:21 2007
 * 
 * @brief  A Rewriter from Defaults to DLRules.
 * 
 * 
 */

#ifndef _DLVHEX_DF_REWRITER_H
#define _DLVHEX_DF_REWRITER_H

#include "Ontology.h"
#include "DefaultParser.h"

namespace dlvhex { namespace df {

class DFRewriter {
private:
	std::string program;
	std::vector<std::string> individuals;

	void readIndividuals(dlvhex::dl::Ontology::shared_pointer);
	std::string& triml(std::string&);
	std::string& delete_comment(std::string&);
	bool incomplete_input(std::string&);
public:
	DFRewriter();
	void transform(std::istream&, std::ostream&, dlvhex::dl::Ontology::shared_pointer);
	std::string getTransformedProgram();
};

}} // dlvhex::df

#endif /* _DLVHEX_DF_REWRITER_H */
