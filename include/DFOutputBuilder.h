/* dlvhex -- Answer-Set Programming with external interfaces.
 * Copyright (C) 2008 DAO Tran Minh
 * 
 * This file is part of dlvhex-dlplugin.
 *
 * dlvhex is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * dlvhex is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with dlvhex; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */


/**
 * @file   DFOutputBuilder.h
 * @author DAO Tran Minh
 * @date   Fri Dec 12 16:20:01 CET 2008
 * 
 * @brief  Builders for default rules output.
 * 
 * 
 */

#ifndef _DLVHEX_DF_OUTPUT_BUILDER_H_
#define _DLVHEX_DF_OUTPUT_BUILDER_H_

#include <iostream>

//#include "dlvhex/OutputBuilder.h"
//#include "dlvhex/PrintVisitor.h"
//#include "dlvhex/ResultContainer.h"

#include "DFProcessor.h"
#include "dlvhex/ComfortPluginInterface.hpp"

namespace dlvhex {

class ResultContainer;

namespace df {


  class DLVHEX_EXPORT DFOutputBuilder // @TODO:      : public dlvhex::OutputBuilder
    {
    public:
      virtual
	~DFOutputBuilder();

      DFOutputBuilder();

      virtual void
	buildResult(std::ostream&, const std::set<ComfortInterpretation>&);
    };

} // namespace df
} // namespace dlvhex

#endif /* _DLVHEX_DF_OUTPUT_BUILDER_H_ */
