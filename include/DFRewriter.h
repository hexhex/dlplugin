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
	std::string input_file;
	std::string ontology_uri;
	std::string program;
	std::vector<std::string> individuals;
	void readIndividuals();
	void readIndividuals(dlvhex::dl::Ontology::shared_pointer o);
	void convert();
	std::string& triml(std::string&);
	bool incomplete_input(std::string&);
public:
	DFRewriter(std::string&, std::string&);
	DFRewriter();
	void transform(std::istream&, std::ostream&);
	void transform(std::istream&, std::ostream&, dlvhex::dl::Ontology::shared_pointer o);
	std::string getTransformedProgram();
};

}} // dlvhex::df

#endif /* _DLVHEX_DF_REWRITER_H */
