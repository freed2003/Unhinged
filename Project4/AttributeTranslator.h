#ifndef ATT_TRANS
#define ATT_TRANS
#include <vector>
#include "RadixTree.h"
#include "provided.h"
#include <string>
class AttributeTranslator {
public:
	AttributeTranslator();
	~AttributeTranslator();
	bool Load(std::string filename);
	std::vector<AttValPair> FindCompatibleAttValPairs(
		const AttValPair& source) const;

private:
	RadixTree<std::vector<AttValPair>> m_map;
};

#endif // !ATT_TRANS

