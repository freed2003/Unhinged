#ifndef  RADIX_TREE
#define RADIX_TREE
#include <vector>
#include <string>

template <typename ValueType>
class RadixTree {
public:
	RadixTree();
	~RadixTree();
	void insert(std::string key, const ValueType& value);
	ValueType* search(std::string key) const;
private:
	struct Node
	{
		Node(bool end1, std::string cur1, Node* mom)
			:end(end1), let (cur1), parent(mom), next(128, nullptr)
		{
		}
		Node* parent;
		ValueType val;
		bool end;
		std::string let;
		std::vector<Node*> next;
	};
	Node* head;
	void deletehelper(Node* todelete);
	int firstdiff(std::string str1, std::string str2) const;
};
template <typename ValueType>
RadixTree<ValueType>::RadixTree()
{
	head = new Node(false, "", nullptr); // Initalizes head
}

template <typename ValueType>
RadixTree<ValueType>::~RadixTree()
{
	deletehelper(head); // Calls recursive delete on tree
}

template <typename ValueType>
void RadixTree<ValueType>::insert(std::string key, const ValueType& value)
{
		std::string curs = key;
		Node* cur = head; // Starts at head
		bool done = false;
		while (!done) // While not done inserting
		{
			int mark = firstdiff(curs, cur->let); // Finds first character that differs between strings
			std::string shared = curs.substr(0, mark); // Creates the longest common string starting from the beginning
			std::string restcurs = curs.substr(mark, curs.size() - mark); // The rest of the string that isn't shared
			std::string restcurrent = cur->let.substr(mark, cur->let.size() - mark); // The rest of the string in the node that isn't shared

			if (curs.size() > cur->let.size()) // Checks if our input string is bigger than our current node's string
			{
				if (restcurrent.size() == 0) // Checks if the current node's string is a substring
				{
					curs = restcurs;
					if (cur->next[(int)restcurs[0]] == nullptr) // If there is no next node we insert one
					{
						Node* toinsert2 = new Node(true, restcurs, cur); // Creates new node
						toinsert2->val = value;
						cur->next[(int)restcurs[0]] = toinsert2; // Insert Node
						done = true; // Mark Done
					}
					else
					{
						cur = cur->next[(int)restcurs[0]]; // Progress to next node
						curs = restcurs;
					}
				}
				else
				{
					Node* toinsert = new Node(false, shared, cur->parent); // Create new parent node
					Node* toinsert2 = new Node(true, restcurs, toinsert); // Create a new child
					toinsert2->val = value; 
					cur->parent->next[(int)shared[0]] = toinsert; // Link the new nodes
					cur->let = restcurrent; // Redefine cur
					cur->parent = toinsert;
					// Link the new parent to the two children nodes
					toinsert->next[(int)restcurrent[0]] = cur;
					toinsert->next[(int)restcurs[0]] = toinsert2;
					done = true;

				}
			}
			else if (curs.size() < cur->let.size()) // Checks for the case where our current input string is shorter than the node's
			{
				if (restcurs.size() == 0) // Case where our string is a substring of the node's
				{
					Node* toinsert = new Node(true, curs, cur->parent); // Initializes new node
					toinsert->val = value;
					cur->parent->next[(int)curs[0]] = toinsert; // Link's parent to new node
					cur->parent = toinsert; // Link to new parent
					cur->let = restcurrent; // Redfine current node
					toinsert->next[(int)restcurrent[0]] = cur; // Links new parent to current node
					done = true;
				}
				else
				{
					Node* toinsert = new Node(false, shared, cur->parent); // Create new parent node
					Node* toinsert2 = new Node(true, restcurs, toinsert); // Create a new child
					toinsert2->val = value;
					cur->parent->next[(int)shared[0]] = toinsert; // Link the new nodes
					cur->let = restcurrent; // Redefine cur
					cur->parent = toinsert;
					// Link the new parent to the two children nodes
					toinsert->next[(int)restcurrent[0]] = cur;
					toinsert->next[(int)restcurs[0]] = toinsert2;
					done = true;
				}
			}
			else if (curs.size() == cur->let.size())
			{
				if (mark == curs.size()) // Checks if the two strings are equal
				{
					// Set's the current node to the value
					cur->val = value;
					cur->end = true;
					done = true;
				}
				else
				{
					Node* toinsert = new Node(false, shared, cur->parent); // Create new parent node
					Node* toinsert2 = new Node(true, restcurs, toinsert); // Create a new child
					toinsert2->val = value;
					cur->parent->next[(int)shared[0]] = toinsert; // Link the new nodes
					cur->let = restcurrent; // Redefine cur
					cur->parent = toinsert;
					// Link the new parent to the two children nodes
					toinsert->next[(int)restcurrent[0]] = cur;
					toinsert->next[(int)restcurs[0]] = toinsert2;
					done = true;
				}
			}

		}
	
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(std::string key) const
{
	std::string curs = key;
	bool done = false;
	Node* cur = head;
	while (!done)
	{
		if (cur == nullptr) return nullptr;
		int mark = firstdiff(curs, cur->let); // Finds first character that differs between strings
		std::string shared = curs.substr(0, mark); // Creates the longest common string starting from the beginning
		std::string restcurs = curs.substr(mark, curs.size() - mark); // The rest of the string that isn't shared
		std::string restcurrent = cur->let.substr(mark, cur->let.size() - mark); // The rest of the string in the node that isn't shared
		if (curs.size() == cur->let.size()) // Case where the two strings are the same length
		{
			if (shared.size() == curs.size()) // Checks that the two strings are equal
			{
				if (cur->end) return &(cur->val); // If this is a valid end node return the address
				else return nullptr; 
			}
			else return nullptr;
		}
		else if (curs.size() > cur->let.size())
		{
			if (restcurrent.size() != 0) return nullptr; // If there's left overs the search is not valid
			cur = cur->next[(int)restcurs[0]]; // Progress to the next node
			curs = restcurs;
		}
		else if (curs.size() < cur->let.size()) return nullptr; // If our string is smaller it can not be valid
	}
	return nullptr;
}

template <typename ValueType>
int RadixTree<ValueType>::firstdiff(std::string str1, std::string str2) const
{
	if (str1.length() >= str2.length())
	{
		int ret = str2.length(); // If not updated we reached the end of the string
		// Iterates until finds first different and returns it
		for (int i = 0; i < str2.length(); i++)
		{
			if (str1[i] != str2[i])
			{
				ret = i;
				break;
			}
		}
		return ret;
	} 
	else
	{
		int ret = str1.length(); // If not updated we reached the end of the string
		// Iterates until finds first different and returns it
		for (int i = 0; i < str1.length(); i++)
		{
			if (str1[i] != str2[i])
			{
				ret = i;
				break;
			}
		}
		return ret;
	}
}
template <typename ValueType>
void RadixTree<ValueType>::deletehelper(Node* todelete)
{
	// First goes through all nodes in the subtree before deleting itself
	for (int i = 0; i < 128; i++)
	{
		if (todelete->next[i] != nullptr) deletehelper(todelete->next[i]);
	}
	delete todelete;
}

#endif // ! RADIX_TREE
