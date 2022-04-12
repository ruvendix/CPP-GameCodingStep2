#pragma once

#include "CommonInclude/Standard.h"
#include "CommonInclude/Macro.h"

class Component
{
public:
	GENERATE_SMART_POINTER(Component);

	Component() = default;
	virtual ~Component() = default;
};