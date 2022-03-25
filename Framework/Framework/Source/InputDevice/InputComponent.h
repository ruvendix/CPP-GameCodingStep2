#pragma once

#include "PCH.h"
#include "InputManager.h"

class InputComponent
{
public:
	using Listener = std::function<void()>;

	static void NullListener() { }

	void RegisterListener(const InputEvent& inputEvent, const Listener& listener);

	Listener FindListener(const InputEvent& inputEvent);

private:
	std::unordered_map<UINT16, Listener> m_mapListener;
};