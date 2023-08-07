#include "outputManager.hpp"

int main()
{
	if (tempo::outputManager::initDevices() != 0)
		return -1;

	tempo::outputManager::printDevices();
}