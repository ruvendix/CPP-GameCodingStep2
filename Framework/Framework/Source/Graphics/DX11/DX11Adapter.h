#pragma once

#include "PCH.h"

struct MonitorModeInfo
{
	std::vector<DXGI_MODE_DESC> vecModeDesc;
};

struct AdapterInfo
{
	DXGI_ADAPTER_DESC adapterDesc;
	std::vector<MonitorModeInfo> vecMonitorModeInfo;
};

class DX11Adapter
{
public:
	DX11Adapter() = default;
	~DX11Adapter() = default;

	HRESULT StartUp();

	void FindValidRefeshRate(UINT clientWidth, UINT clientHeight, DXGI_RATIONAL& refRefreshRate);

private:
	std::vector<AdapterInfo> m_vecAdapterInfo;
};