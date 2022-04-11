#include "PCH.h"
#include "ErrorHandler.h"

void ErrorHandler::ShowCustomError(const std::string& strErr)
{
	::MessageBox(nullptr, strErr.c_str(), "¿À·ù", MB_ICONERROR);
}

void ErrorHandler::ConvertComErrorString(HRESULT errCode, std::string& refStr)
{
	_com_error err(errCode);
	refStr = err.ErrorMessage();
}