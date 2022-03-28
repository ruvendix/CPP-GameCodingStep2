#pragma once

#include "PCH.h"

class ErrorHandler
{
	GENERATE_SINGLETON(ErrorHandler)

public:
	void ShowCustomError(const std::string& strErr);
	void ConvertComErrorString(HRESULT errCode, std::string& refStr);
};