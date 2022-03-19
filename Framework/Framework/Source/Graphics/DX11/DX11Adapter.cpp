#include "PCH.h"
#include "DX11Adapter.h"

#include "ErrorHandler/ErrorHandler.h"

HRESULT DX11Adapter::StartUp()
{
	HRESULT hRet = S_OK;

	// Factory 패턴이 생각나는 코드
	Microsoft::WRL::ComPtr<IDXGIFactory> spFactory;
	TEST_COM(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(spFactory.GetAddressOf())), hRet);

	// 그래픽 카드에 대한 어댑터를 가져옵니다.
	// 어댑터 자체를 그래픽 카드라고 봐도 무난합니다.
	UINT adapterIdx = 0;
	Microsoft::WRL::ComPtr<IDXGIAdapter> spAdapter;
	while (spFactory->EnumAdapters(adapterIdx, spAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
	{
		AdapterInfo adapterInfo;
		::ZeroMemory(&adapterInfo, sizeof(AdapterInfo));
		spAdapter->GetDesc(&adapterInfo.adapterDesc); // 어댑터 정보를 가져옵니다.
		m_vecAdapterInfo.push_back(adapterInfo);

		// 어댑터와 호환되는 모니터마다의 모드를 가져옵니다.
		UINT monitorIdx = 0;
		Microsoft::WRL::ComPtr<IDXGIOutput> spAdapterOutput;
		while (spAdapter->EnumOutputs(monitorIdx, spAdapterOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		{
			MonitorModeInfo monitorModeInfo;		
			::ZeroMemory(&monitorModeInfo, sizeof(MonitorModeInfo));

			// DXGI_FORMAT_R8G8B8A8_UNORM에 해당되는 모드 개수를 가져옵니다.
			UINT numMode = 0;
			TEST_COM(spAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
				DXGI_ENUM_MODES_INTERLACED, &numMode, nullptr), hRet);

			// 모드 개수를 가져왔으면 그 개수만큼 정보를 알아냅니다.
			monitorModeInfo.vecModeDesc.resize(numMode);

			std::vector<DXGI_MODE_DESC>& vecModeDesc = monitorModeInfo.vecModeDesc;
			TEST_COM(spAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
				DXGI_ENUM_MODES_INTERLACED, &numMode, vecModeDesc.data()), hRet);

			m_vecAdapterInfo[adapterIdx].vecMonitorModeInfo.push_back(monitorModeInfo);
			++monitorIdx;
		}

		++adapterIdx;
	}

	return S_OK;
}

void DX11Adapter::FindValidRefeshRate(UINT clientWidth, UINT clientHeight, DXGI_RATIONAL& refRefreshRate)
{
	for (const auto& adapterInfo : m_vecAdapterInfo)
	{
		bool bMonitorModeFound = false;
		for (const auto& monitorModeInfo : adapterInfo.vecMonitorModeInfo)
		{
			bool bRefreshRateFound = false;
			for (const auto& modeDesc : monitorModeInfo.vecModeDesc)
			{
				if ((modeDesc.Width == clientWidth) &&
					(modeDesc.Height == clientHeight))
				{
					refRefreshRate = modeDesc.RefreshRate;
					bRefreshRateFound = true;
					break;
				}
			}

			if (bRefreshRateFound == true)
			{
				bMonitorModeFound = true;
				break;
			}
		}

		if (bMonitorModeFound == true)
		{
			break;
		}
	}
}