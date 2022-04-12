#pragma once

#include "Component.h"

enum class EInputState : UINT8
{
	DOWN = 0,
	UP,
};

// 공용체로 구조체 자체를 ID로 사용하는 방법
union InputEvent
{
	struct
	{
		UINT8 code;
		EInputState state;
	};

	UINT16 ID;
};

class InputComponent : public Component
{
public:
	GENERATE_SMART_POINTER(InputComponent);
	using Listener = std::function<void()>;

	static void NullListener() { } // 아무 기능 없음

	InputComponent() = default;
	virtual ~InputComponent() = default;

	void RegisterListener(const InputEvent& inputEvent, const Listener& listener);

	Listener FindListener(const InputEvent& inputEvent);

private:
	std::unordered_map<UINT16, Listener> m_mapListener;
};