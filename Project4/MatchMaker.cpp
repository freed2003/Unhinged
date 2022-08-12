#include "MatchMaker.h"
#include <unordered_set>
#include "provided.h"
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <map>
MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at)
	:m_mdb(const_cast<MemberDatabase*>(& mdb)), m_att(const_cast<AttributeTranslator*>(&at))
{}
MatchMaker::~MatchMaker() {}

std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const
{
	std::vector<EmailCount> ret;
	std::map<std::string, int> table;
	std::map<int, std::set<std::string>> imtired; // This map will sort by int first then sort the string in the set
	std::vector<AttValPair> pref;
	std::unordered_set<std::string> prefset;
	const PersonProfile* ourperson = m_mdb->GetMemberByEmail(email);
	// Iterates over all the AttValPairs to insert compatible pairs
	for (int i = 0; i < ourperson->GetNumAttValPairs(); i++)
	{
		AttValPair temp;
		ourperson->GetAttVal(i, temp);
		std::vector<AttValPair> temp2 = m_att->FindCompatibleAttValPairs(temp); // Vector of all compatible pairs
		for (int j = 0; j < temp2.size(); j++)
		{
			// For each Compatible pair we check if it was added already, if not we add it
			if (prefset.find(temp2[j].attribute + "," + temp2[j].value) == prefset.end())
			{
				pref.push_back(temp2[j]);
				prefset.insert(temp2[j].attribute + "," + temp2[j].value);
			}
		}
	}
	std::vector<AttValPair>::iterator it = pref.begin();
	// Iterates through all Compatible attvalpairs
	while (it != pref.end())
	{
		AttValPair cur = *it;
		std::vector<std::string> match = m_mdb->FindMatchingMembers(cur); // Returns vector of members who are compatible so we can add them
		for (int i = 0; i < match.size(); i++)
		{
			std::map<std::string, int>::iterator its = table.find(match[i]);
			if (its == table.end()) // Checks to see if the member was added already. If not we add them
			{
				table.insert({ match[i], 1 });
			}
			else
			{
				its->second += 1; // If the member was added already we increment their count by one
			}
		}
		it++; // Move to the next attvalpair
	}
	std::map<std::string, int>::iterator its2 = table.begin();
	// Insert each email into the map based on their number of compatible pairs. the map will sort it for us
	while (its2 != table.end())
	{
		imtired[its2->second].insert(its2->first);
		its2++;
	}
	// Iterator that goes backwards through the map so the compatible numbers are set in decreasing order
	std::map<int, std::set<std::string>>::reverse_iterator its1 = imtired.rbegin();
	while (its1 != imtired.rend()) // Goes through the map in reverse order
	{
		// Makes sure the count is at leas the threshhold
		if (its1->first >= threshold)
		{
			std::set<std::string>::iterator tempit = its1->second.begin(); // Iterator to go through the set of people
			while (tempit != its1->second.end())
			{
				// The emails will be push backed alphabetically
				if(*tempit != ourperson->GetEmail()) ret.push_back(EmailCount(*tempit, its1->first));
				tempit++;
			}
		}
		its1++;
	}

	return ret;
}