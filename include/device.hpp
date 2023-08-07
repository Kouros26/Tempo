#pragma once
#include <string>

class Device
{
public:

	explicit Device(const std::string& pId);
	explicit Device(const std::string& pId, const std::string& pName);

	constexpr std::string& getId() { return m_id; }
	constexpr std::string& getName() { return m_name; }

protected:

	std::string m_id{};
	std::string m_name{};
};
