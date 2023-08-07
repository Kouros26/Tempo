#include "outputManager.hpp"

#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <iostream>
#include <vector>

namespace tempo
{
	namespace outputManager
	{
        namespace
        {
            std::vector<ODevice> g_outPutDevices;
        }

		int initDevices()
		{
			const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
			const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

            HRESULT hr = S_OK;
            IMMDeviceEnumerator* enumerator = nullptr;
            IMMDeviceCollection* collection = nullptr;
            IMMDevice* endpoint = nullptr;
            IPropertyStore* props = nullptr;
            LPWSTR pwszID = nullptr;

            hr = CoInitialize(nullptr);
            hr = CoCreateInstance(
                CLSID_MMDeviceEnumerator, nullptr,
                CLSCTX_ALL, IID_IMMDeviceEnumerator,
                reinterpret_cast<void**>(&enumerator));
            EXIT_ON_ERROR(hr)

			hr = enumerator->EnumAudioEndpoints(
                eRender, DEVICE_STATE_ACTIVE,
                &collection);
            EXIT_ON_ERROR(hr)

			UINT  count;
            hr = collection->GetCount(&count);
            EXIT_ON_ERROR(hr)

            if (count == 0)
                std::cout << "No audio devices found" << std::endl;

            for (ULONG i = 0; i < count; i++)
            {
                hr = collection->Item(i, &endpoint);
                EXIT_ON_ERROR(hr)

            	hr = endpoint->GetId(&pwszID);
                EXIT_ON_ERROR(hr)
            	std::wstring wsId(pwszID);
                std::string sId(wsId.begin(), wsId.end());

            	hr = endpoint->OpenPropertyStore(
                        STGM_READ, &props);
                EXIT_ON_ERROR(hr)

            	PROPVARIANT varName;
                PropVariantInit(&varName);

                hr = props->GetValue(
                    PKEY_Device_FriendlyName, &varName);
                EXIT_ON_ERROR(hr)
            	std::wstring wsName(varName.pwszVal);
                std::string sName(wsName.begin(), wsName.end());

                g_outPutDevices.emplace_back(sId, sName);

                CoTaskMemFree(pwszID);
                pwszID = nullptr;
                PropVariantClear(&varName);
                SAFE_RELEASE(props)
            	SAFE_RELEASE(endpoint)
            }

            SAFE_RELEASE(enumerator)
			SAFE_RELEASE(collection)
			return hr;


			Exit:
				std::cout << "Error !" << std::endl;
				CoTaskMemFree(pwszID);
                SAFE_RELEASE(enumerator)
                SAFE_RELEASE(collection)
                SAFE_RELEASE(endpoint)
                SAFE_RELEASE(props)
                return hr;
		}

		void printDevices()
		{
            for (auto& device : g_outPutDevices)
            {
                std::cout << device.getName() << std::endl;
            }
		}
	}
}
