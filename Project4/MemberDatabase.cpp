#include "MemberDatabase.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "PersonProfile.h"
#include <sstream>
MemberDatabase::MemberDatabase()
{
}

MemberDatabase::~MemberDatabase()
{
	for (int i = 0; i < m_members.size(); i++)
	{
		delete m_members[i];
	}
}

bool MemberDatabase::LoadDatabase(std::string filename)
{
	std::ifstream file(filename); // Open file
	std::string templine;
	while (std::getline(file, templine)) // Grab the first line (the name)
	{
		std::string name = templine;
		std::string email;
		if (!std::getline(file, email)) return false; // Grab the email
		if (m_eamp.search(email) != nullptr) return false;
		// Grab the number of pairs and convert to an integer
		std::string pairnum;
		if (!std::getline(file, pairnum)) return false;
		std::istringstream iss1(pairnum);
		int avpairs;
		PersonProfile* temp = new PersonProfile(name, email);
		iss1 >> avpairs;
		// Iterate through all attvalpairs
		for (int i = 0; i < avpairs; i++)
		{
			std::string att;
 			std::string val;
			if (!std::getline(file, templine)) return false; // Grabs the pair
			std::istringstream tempiss(templine);
			// These two lings separate the attribute and the value
			if (!std::getline(tempiss, att, ',')) return false;
			if (!std::getline(tempiss, val)) return false;
			// Add the apir to the person
			temp->AddAttValPair(AttValPair(att, val));
			// Add the email to the attvalpair to email tree
			std::vector<std::string>* vect = m_avea.search(att + "," + val);
			if (vect == nullptr) m_avea.insert(att + "," + val, { email });
			else vect->push_back(email);
			
		}
		// Insert the email and person to the tree 
		m_eamp.insert(email, temp);
		m_members.push_back(temp); // Add the person to the members

		// Skip the blank line at the end
		std::getline(file, templine);
	}
	
	return true;
}

std::vector<std::string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const
{
	std::vector<std::string>* temp = m_avea.search(input.attribute + "," + input.value); // Grabs vector from the Radix Tree
	if (temp == nullptr) return {}; // If nothing came back return an empty vector
	else return *temp;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const
{
	PersonProfile** temp = m_eamp.search(email); // Grap person from tree
	if (temp == nullptr) return nullptr; // If nothing came back we return nullptr
	else return *temp;
}
