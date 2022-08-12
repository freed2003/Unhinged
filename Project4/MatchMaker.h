#ifndef MATCH_MAKER
#define MATCH_MAKER
#include <vector>
#include "MemberDatabase.h"
#include "AttributeTranslator.h"
class MemberDatabase;
class AttributeTranslator;
class MatchMaker {
public:
	MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
	~MatchMaker();
	std::vector<EmailCount> IdentifyRankedMatches(std::string email,
		int threshold) const;
private:
	MemberDatabase* m_mdb;
	AttributeTranslator* m_att;
};

#endif // !MATCH_MAKER
