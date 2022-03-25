#pragma once

#include "PCH.h"

enum class EInputState : UINT
{
	DOWN = 0,
	UP,
};

union InputEvent
{
	struct
	{
		DWORD dwInputCode;
		EInputState inputState;
	};

	UINT64 ID;
};

class Scene;
class InputComponent;

class InputManager
{
	GENERATE_SINGLETON(InputManager);

public:
	InputComponent* CreateInputComponent();

	void PushInputEvent(const InputEvent& inputEvent);
	void Update(); // 객체들 업데이트하기 전에 입력 관련 정보부터 전부 갱신

private:
	std::vector<std::shared_ptr<InputComponent>> m_vecInputComponent;
	std::queue<InputEvent> m_queue;
};