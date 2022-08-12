#include "PersonProfile.h"
#include <string>
PersonProfile::PersonProfile(std::string name, std::string email)
	:m_paircount(0), m_name(name), m_email(email)
{

}
PersonProfile::~PersonProfile()
{}
std::string PersonProfile::GetName() const
{
	return m_name;
}

std::string PersonProfile::GetEmail() const
{
	return m_email;
}

void PersonProfile::AddAttValPair(const AttValPair& attval)
{
	// If the attvalpair was already inserted we do nothing
	if (m_set.find(attval.attribute + "," + attval.value) != m_set.end()) return;

	m_set.insert(attval.attribute + "," + attval.value); // Insert attvalpair for easy checking 
	m_vector.push_back(attval); // Add attvalpair to collection for easy grabbing
	std::vector<std::string>* value = m_map.search(attval.attribute); // Search for the attribute
	if (value == nullptr) // If there was no attribute we add it and the value
	{
		m_map.insert(attval.attribute, std::vector<std::string>{attval.value});
		m_paircount += 1;
	}
	else // If the attribute was already inserted we also add a new value
	{
		value->push_back(attval.value);
		m_paircount += 1;
	}

}

int PersonProfile::GetNumAttValPairs() const
{
	return m_paircount;
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const
{
	if (attribute_num < 0 || attribute_num >= m_paircount) return false; // Return false if out of bounds

	attval = m_vector[attribute_num];
	return true;

}