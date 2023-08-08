#pragma once

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

namespace tempo
{
	namespace IOManager
	{
		int initDevices();
		void printDevices();
	}
}