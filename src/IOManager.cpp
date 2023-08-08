#include "IOManager.hpp"
#include "outputDevice.hpp"
#include "inputDevice.hpp"

#include <mmdeviceapi.h>
#include <functiondiscoverykeys_devpkey.h>
#include <iostream>
#include <vector>



namespace tempo
{
	namespace IOManager
	{
        namespace
        {
            std::vector<ODevice> g_outputDevices;
            std::vector<IDevice> g_inputDevices;
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

                g_outputDevices.emplace_back(sId, sName);

                CoTaskMemFree(pwszID);
                pwszID = nullptr;
                PropVariantClear(&varName);
                SAFE_RELEASE(props)
            	SAFE_RELEASE(endpoint)
            }

            hr = enumerator->EnumAudioEndpoints(
                eCapture, DEVICE_STATE_ACTIVE,
                &collection);
            EXIT_ON_ERROR(hr)

        	UINT  countOutput;
            hr = collection->GetCount(&countOutput);
            EXIT_ON_ERROR(hr)

                if (countOutput == 0)
                    std::cout << "No audio output devices found" << std::endl;

            for (ULONG i = 0; i < countOutput; i++)
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

                g_inputDevices.emplace_back(sId, sName);

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
            for (auto& device : g_outputDevices)
            {
                std::cout << device.getName() << std::endl;
            }

            std::cout << std::endl;

            for (auto& device : g_inputDevices)
            {
                std::cout << device.getName() << std::endl;
            }
		}
	}
}
