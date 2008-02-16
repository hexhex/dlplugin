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
#include "Registry.h"
#include "DFRewriter.h"

namespace dlvhex {
namespace df {

DFRewriter::DFRewriter() 
{
	program = "";
}

void DFRewriter::readIndividuals(dlvhex::dl::Ontology::shared_pointer o) 
{
	dlvhex::dl::ABox abox = o->getABox();
	dlvhex::dl::ABox::ObjectsPtr inds = abox.getIndividuals();
	std::set<Term>::iterator pos;
	std::size_t str_pos;

	for (pos = inds->begin(); pos != inds->end(); pos++) 
	{
		std::string ind = pos->getString();
		ind.insert(1, "<");
		str_pos = ind.length();
		str_pos--;
		ind.insert(str_pos, ">");
		individuals.push_back(ind);
	}
}

std::string& DFRewriter::delete_comment(std::string& s) 
{
	int pos = s.find("%", 0);
	if (pos > -1) 
	{
		s.erase(pos, s.length());
	}
	return s;
}

void DFRewriter::transform(std::string filename, std::ostream& oss, dlvhex::dl::Ontology::shared_pointer o) 
{
	try 
	{
		DefaultParser dp;
		std::string program;
		dp.parseInputStream(filename, program);
		if (!o) 
		{
			throw "No ontology specified!";
		}
		DLRules rules;
		readIndividuals(o);
		std::vector<std::string>::iterator pos_i;
		for (pos_i = individuals.begin(); pos_i != individuals.end(); pos_i++) 
		{
			MTerm t(*pos_i);
			Predicate p("dom", t);
			DLRule r(p);
			rules.addDLRule(r);
		}
		program = program + rules.toString();		

		oss << program;
		if (dlvhex::dl::Registry::getVerbose() > 1) 
		{
			std::cerr << "Transformed defaults to dlrules:" << std::endl;
			std::cerr << program << std::endl;
		}
	}
	catch (char const* mess) 
	{
		std::cout << "Error: " << mess << std::endl;
	}
}

}} // namespace dlvhex::df
