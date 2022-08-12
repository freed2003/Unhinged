#include <iostream>
#include <fstream>
#include <algorithm>
#include "AttributeTranslator.h"
#include <string>
#include "provided.h"
#include <sstream>
AttributeTranslator::AttributeTranslator()
	:m_map()
{}

AttributeTranslator::~AttributeTranslator()
{
}

bool AttributeTranslator::Load(std::string filename)
{
	std::ifstream trans(filename); // Opens file
	std::string templine;
	while (std::getline(trans, templine)) // Grabs the next line of source compatible pair
	{
		if (templine.length() == 0) continue;
		std::istringstream iss(templine);
		std::string part1;
		std::string part2;
		std::string source;
		std::string targetatt;
		std::string targetval;
		if(!std::getline(iss, part1, ',')) return false; // Get's the source att
		if(!std::getline(iss, part2, ',')) return false; // Get's the source val
		if (!std::getline(iss, targetatt, ',')) return false; // Get's the compatible att
		if (!std::getline(iss, targetval)) return false; // Get's the compatible val
		source = part1 + "," + part2; // Combines the source attribute and the source val
		std::vector<AttValPair>* temp = m_map.search(source); // Returns the vector of all compatible attvalpairs to the source
		AttValPair news(targetatt, targetval);
		if (temp == nullptr) m_map.insert(source, { AttValPair(targetatt, targetval) }); // If no vector came back we add one
		else
		{
			bool found = false;
			for (int i = 0; i < temp->size(); i++)
			{
				if ((*temp)[i] == news) found = true;
			}
			if (!found) temp->push_back(AttValPair(targetatt, targetval)); // If the compatible pair wasn't inserted already we add it
		}
	}
	return true;
}

std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const
{
	std::vector<AttValPair>* ret = m_map.search(source.attribute + "," + source.value); // Grabs the vector of all compatible pairs from the tree
	if (ret == nullptr) return {}; // If nothing came back we return an empty vector
	else return *ret;

}