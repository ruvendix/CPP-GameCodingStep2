#pragma once

#include "PCH.h"

class ErrorHandler
{
	GENERATE_SINGLETON(ErrorHandler)

public:
	void ConvertComErrorString(HRESULT errCode, std::string& refStr);
};