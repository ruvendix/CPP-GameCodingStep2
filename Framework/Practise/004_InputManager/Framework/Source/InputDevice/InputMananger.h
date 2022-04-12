#pragma once

#include "CommonInclude/Standard.h"
#include "CommonInclude/Windows.h"
#include "CommonInclude/Macro.h"
#include "Component/InputComponent.h"

class InputManager
{
	GENERATE_SINGLETON(InputManager);

public:
	InputComponent::SPtr CreateInputComponent();

	void PushInputEvent(const InputEvent& inputEvent);
	void Update(); // 객체들 업데이트하기 전에 입력 관련 정보부터 전부 갱신

private:
	std::vector<InputComponent::SPtr> m_vecInputComponent;
	std::queue<InputEvent> m_queue;
};