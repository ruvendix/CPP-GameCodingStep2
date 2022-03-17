#include "PCH.h"
#include "Config.h"

Config::Config()
{
	::ZeroMemory(&m_deviceMode, sizeof(DEVMODE));
	m_deviceMode.dmSize = sizeof(DEVMODE);
	m_deviceMode.dmBitsPerPel = 32; // 픽셀당 비트수
	m_deviceMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
}

void Config::ChangeDeviceResolution(UINT width, UINT height)
{
	m_deviceMode.dmPelsWidth = width;
	m_deviceMode.dmPelsHeight = height;
	::ChangeDisplaySettings(&m_deviceMode, CDS_UPDATEREGISTRY);
}
