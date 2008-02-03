/**
 * @file   DFRewriter.cpp
 * @author DAO Tran Minh
 * @date   Tue Dec 18 23:49:21 2007
 * 
 * @brief  A Rewriter from Defaults to DLRules.
 * 
 * 
 */

#include <fstream>
#include <iostream>
#include "Ontology.h"
#include "DFRewriter.h"

namespace dlvhex {
namespace df {

DFRewriter::DFRewriter() {
	program = "";
}

void DFRewriter::readIndividuals(dlvhex::dl::Ontology::shared_pointer o) {
	dlvhex::dl::ABox abox = o->getABox();
	dlvhex::dl::ABox::ObjectsPtr inds = abox.getIndividuals();
	std::set<Term>::iterator pos;
	std::size_t str_pos;

	for (pos = inds->begin(); pos != inds->end(); pos++) {
		std::string ind = pos->getString();
		ind.insert(1, "<");
		str_pos = ind.length();
		str_pos--;
		ind.insert(str_pos, ">");
		individuals.push_back(ind);
	}
}

bool DFRewriter::incomplete_input(std::string& line) {
	// remove all blank at the end of the line
	int pos(line.size());
	pos = line.size();	
	for (; pos && (line[pos-1] == ' ' || line[pos-1] == '\t'); --pos);
	line.erase(pos, line.size()-pos);

	// check whether the last character is a dot or not
	/*if (line[line.length()-1] == '.') {
		return false;
	}
	return true;*/
	int dot = line.find(".", 0);
	if (dot > -1) {
		return false;
	}
	return true;
}

std::string& DFRewriter::triml(std::string& s) {
	int pos(0);
	for (; s[pos]==' ' || s[pos]=='\t'; ++pos);
	if (pos > 0) {
		s.erase(0, pos);
	}
	return s;
}

std::string& DFRewriter::delete_comment(std::string& s) {
	int pos = s.find("%", 0);
	if (pos > -1) {
		s.erase(pos, s.length());
	}
	return s;
}

void DFRewriter::transform(std::istream& iss, std::ostream& oss, dlvhex::dl::Ontology::shared_pointer o) {
	try {
		std::string line;
		std::string program = "";
		DefaultParser dp;
		Defaults dfs;
		bool has_defaults = false;

		getline(iss, line);
		line = triml(line);
		if (dlvhex::dl::Registry::getVerbose() > 1) {
			std::cerr << "Get new line" << line << std::endl;
		}
		while (!iss.eof()) {
			if (line.length() > 0) {
				if (line[0] == '#') {					program = program + line + "\n";
					getline(iss, line); 
					line = triml(line);
					if (dlvhex::dl::Registry::getVerbose() > 1) {
						std::cerr << "Get new line" << line << std::endl;
					}
				} else {
					line = delete_comment(line);
					if (incomplete_input(line)) {
						std::string next;
						getline(iss, next);
						if (dlvhex::dl::Registry::getVerbose() > 1) {
							std::cerr << "Not completed yet!:" << line << std::endl;
						}
						line += triml(next);
						if (dlvhex::dl::Registry::getVerbose() > 1) {
							std::cerr << "Appended result   :" << line << std::endl;
						}
					}
					else {
						Default df = dp.getParsedDefault(line);
						if (df.isNULL()) {
							if (dlvhex::dl::Registry::getVerbose() > 1) {
								std::cerr << "Not a default!" << std::endl;
							}
							program = program + line + "\n";
						} else {
							if (dlvhex::dl::Registry::getVerbose() > 1) {
								std::cerr << "Default PARSED." << std::endl;
							}
							dfs.addDefault(df);
							has_defaults = true;
						}
						getline(iss, line);
						line = triml(line);
						if (dlvhex::dl::Registry::getVerbose() > 1) {
							std::cerr << "Get new line" << line << std::endl;
						}
					}
				}
			} else {
				getline(iss, line);
				line = triml(line);
				if (dlvhex::dl::Registry::getVerbose() > 1) {
					std::cerr << "Get new line" << line << std::endl;
				}
			}					
		}

		if (has_defaults) {
			if (!o) {
				throw "No ontology specified!";
			}
			DLRules rules = dfs.getDLRules();
			readIndividuals(o);
			std::vector<std::string>::iterator pos_i;
			for (pos_i = individuals.begin(); pos_i != individuals.end(); pos_i++) {
				MTerm t(*pos_i);
				Predicate p("dom", t);
				DLRule r(p);
				rules.addDLRule(r);
			}
			program = program + rules.toString();
		}

		oss << program;
		if (dlvhex::dl::Registry::getVerbose() > 1) {
			std::cerr << "Transformed defaults to dlrules:" << std::endl;
			std::cerr << program << std::endl;
		}
	}
	catch (char const* mess) {
		std::cout << "Error: " << mess << std::endl;
	}
}

}} // namespace dlvhex::df
