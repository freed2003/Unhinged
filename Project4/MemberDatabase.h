#ifndef MEM_DATA
#define MEM_DATA
#include <string>
#include "provided.h"
#include <vector>
#include "RadixTree.h"
#include "PersonProfile.h"
class PersonProfile;
class MemberDatabase {
public:
	MemberDatabase();
	~MemberDatabase();
	bool LoadDatabase(std::string filename);
	std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
	const PersonProfile* GetMemberByEmail(std::string email) const;
private:
	RadixTree<PersonProfile*> m_eamp;
	RadixTree<std::vector<std::string>> m_avea;
	std::vector<PersonProfile*> m_members;
};
#endif // !MEM_DATA

