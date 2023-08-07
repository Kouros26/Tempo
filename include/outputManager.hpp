#pragma once
#include "outputDevice.hpp"

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

namespace tempo
{
	namespace outputManager
	{
		int initDevices();
		void printDevices();
	}
}