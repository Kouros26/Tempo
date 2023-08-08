#include "IOManager.hpp"

int main()
{
	if (tempo::IOManager::initDevices() != 0)
		return -1;

	tempo::IOManager::printDevices();
}