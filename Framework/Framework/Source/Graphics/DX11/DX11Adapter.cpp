#include "PCH.h"
#include "DX11Adapter.h"

#include "ErrorHandler/ErrorHandler.h"

HRESULT DX11Adapter::StartUp()
{
	HRESULT hRet = S_OK;

	// Factory ������ �������� �ڵ�
	Microsoft::WRL::ComPtr<IDXGIFactory> spFactory;
	TEST_COM(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(spFactory.GetAddressOf())), hRet);

	// �׷��� ī�忡 ���� ����͸� �����ɴϴ�.
	// ����� ��ü�� �׷��� ī���� ���� �����մϴ�.
	UINT adapterIdx = 0;
	Microsoft::WRL::ComPtr<IDXGIAdapter> spAdapter;
	while (spFactory->EnumAdapters(adapterIdx, spAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
	{
		AdapterInfo adapterInfo;
		::ZeroMemory(&adapterInfo, sizeof(AdapterInfo));
		spAdapter->GetDesc(&adapterInfo.adapterDesc); // ����� ������ �����ɴϴ�.
		m_vecAdapterInfo.push_back(adapterInfo);

		// ����Ϳ� ȣȯ�Ǵ� ����͸����� ��带 �����ɴϴ�.
		UINT monitorIdx = 0;
		Microsoft::WRL::ComPtr<IDXGIOutput> spAdapterOutput;
		while (spAdapter->EnumOutputs(monitorIdx, spAdapterOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND)
		{
			MonitorModeInfo monitorModeInfo;		
			::ZeroMemory(&monitorModeInfo, sizeof(MonitorModeInfo));

			// DXGI_FORMAT_R8G8B8A8_UNORM�� �ش�Ǵ� ��� ������ �����ɴϴ�.
			UINT numMode = 0;
			TEST_COM(spAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
				DXGI_ENUM_MODES_INTERLACED, &numMode, nullptr), hRet);

			// ��� ������ ���������� �� ������ŭ ������ �˾Ƴ��ϴ�.
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