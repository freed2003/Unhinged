#ifndef PERSON_PROFILE
#define PERSON_PROFILE
#include <string>
#include "provided.h"
#include <unordered_set>
#include <vector>
#include "RadixTree.h"
#include <unordered_set>
class PersonProfile
{
public:
	PersonProfile(std::string name, std::string email);
	~PersonProfile();
	std::string GetName() const;
	std::string GetEmail() const;
	void AddAttValPair(const AttValPair& attval);
	int GetNumAttValPairs() const;
	bool GetAttVal(int attribute_num, AttValPair& attval) const;
private:
	int m_paircount;
	std::unordered_set<std::string> m_set;
	std::vector<AttValPair> m_vector;
	std::string m_name;
	std::string m_email;
	RadixTree<std::vector<std::string>> m_map;
};

#endif // !PERSON_PROFILE

