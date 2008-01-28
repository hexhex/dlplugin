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
}

DFRewriter::DFRewriter(std::string& input_file_, std::string& ontology_uri_) : input_file(input_file_), ontology_uri(ontology_uri_) {
	program = "";
}

void DFRewriter::readIndividuals() {
	dlvhex::dl::Ontology::shared_pointer o = dlvhex::dl::Ontology::createOntology(ontology_uri);
	dlvhex::dl::ABox abox = o->getABox();
	std::string name_space = o->getNamespace();
	dlvhex::dl::ABox::ObjectsPtr inds = abox.getIndividuals();
	std::set<Term>::iterator pos;
	std::string str_ind;
	std::size_t str_pos1;
	std::size_t str_pos2;
	for (pos = inds->begin(); pos != inds->end(); pos++) {
		std::string ind = pos->getString();
		individuals.push_back(ind);
	}
}

void DFRewriter::readIndividuals(dlvhex::dl::Ontology::shared_pointer o) {
	dlvhex::dl::ABox abox = o->getABox();
	std::string name_space = o->getNamespace();
	dlvhex::dl::ABox::ObjectsPtr inds = abox.getIndividuals();
	std::set<Term>::iterator pos;
	std::string str_ind;
	std::size_t str_pos1;
	std::size_t str_pos2;
	for (pos = inds->begin(); pos != inds->end(); pos++) {
		std::string ind = pos->getString();
		ind.insert(1, "<");
		str_pos2 = ind.length();
		str_pos2--;
		ind.insert(str_pos2, ">");
		individuals.push_back(ind);
	}
}

void DFRewriter::convert() {
	DefaultParser dp;
	Defaults dfs;

	program = "";
	std::ifstream in(input_file.c_str());
	if (!in) {
		std::cerr << "Can't open input file \"" << input_file << "\" !!!" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string str_df;
	getline(in, str_df); 
	while (in) {
		Default df = dp.getParsedDefault(str_df);
		if (df.isNULL()) {
			std::cout << "Not a default!" << std::endl;
			program = program + str_df + "\n";
		} else {
				std::cout << "Default PARSED." << std::endl;
				dfs.addDefault(df);
		}
		getline(in, str_df); 
	}
	//std::cout << "Start to get DL rules" << std::endl;
	DLRules rules = dfs.getDLRules();	
	//std::cout << "Getting DL rules done" << std::endl;	
	std::vector<std::string>::iterator pos_i;
	for (pos_i = individuals.begin(); pos_i != individuals.end(); pos_i++) {
		MTerm t(*pos_i);
		Predicate p("dom", t);
		DLRule r(p);
		rules.addDLRule(r);
	}
	//std::cout << "Generate DOM done" << std::endl;
	program = program + rules.toString();
}

void DFRewriter::transform(std::istream& iss, std::ostream& oss) {
	std::string line;
	std::string program = "";
	DefaultParser dp;
	Defaults dfs;

	input_file = "wine_small.df";
	ontology_uri = "wine_small.owl";
	readIndividuals();
	getline(iss, line);
	while (!iss.eof()) {
		Default df = dp.getParsedDefault(line);
		if (df.isNULL()) {
			std::cout << "Not a default!" << std::endl;
			program = program + line + "\n";
		} else {
				std::cout << "Default PARSED." << std::endl;
				dfs.addDefault(df);
		}
		getline(iss, line);
	}
	//std::cout << "Start to get DL rules" << std::endl;
	DLRules rules = dfs.getDLRules();	
	//std::cout << "Getting DL rules done" << std::endl;	
	std::vector<std::string>::iterator pos_i;
	for (pos_i = individuals.begin(); pos_i != individuals.end(); pos_i++) {
		MTerm t(*pos_i);
		Predicate p("dom", t);
		DLRule r(p);
		rules.addDLRule(r);
	}
	//std::cout << "Generate DOM done" << std::endl;
	program = program + rules.toString();
	oss << program;
}

void DFRewriter::transform(std::istream& iss, std::ostream& oss, dlvhex::dl::Ontology::shared_pointer o) {
	std::string line;
	std::string program = "";
	DefaultParser dp;
	Defaults dfs;

	readIndividuals(o);
	getline(iss, line);
	while (!iss.eof()) {
		Default df = dp.getParsedDefault(line);
		if (df.isNULL()) {
			std::cout << "Not a default!" << std::endl;
			program = program + line + "\n";
		} else {
				std::cout << "Default PARSED." << std::endl;
				dfs.addDefault(df);
		}
		getline(iss, line);
	}
	//std::cout << "Start to get DL rules" << std::endl;
	DLRules rules = dfs.getDLRules();	
	//std::cout << "Getting DL rules done" << std::endl;	
	std::vector<std::string>::iterator pos_i;
	for (pos_i = individuals.begin(); pos_i != individuals.end(); pos_i++) {
		MTerm t(*pos_i);
		Predicate p("dom", t);
		DLRule r(p);
		rules.addDLRule(r);
	}
	//std::cout << "Generate DOM done" << std::endl;
	program = program + rules.toString();
	oss << program;
	std::cerr << program << std::endl;
}

std::string DFRewriter::getTransformedProgram() {
	readIndividuals();
	std::cout << "Individuals are:" << std::endl;
	std::vector<std::string>::iterator pos;
	for (pos = individuals.begin(); pos != individuals.end(); pos++) {
		std::cout << *pos << std::endl;
	}
	convert();
	return program;
}

}} // namespace dlvhex::df
