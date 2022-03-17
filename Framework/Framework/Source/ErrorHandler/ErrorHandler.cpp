#include "PCH.h"
#include "ErrorHandler.h"

void ErrorHandler::ConvertComErrorString(HRESULT errCode, std::string& refStr)
{
	_com_error err(errCode);
	refStr = err.ErrorMessage();
}