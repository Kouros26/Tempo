#include "device.hpp"

Device::Device(const std::string& pId)
	: m_id(pId)
{
}

Device::Device(const std::string& pId, const std::string& pName)
	: m_id(pId), m_name(pName)
{
}
