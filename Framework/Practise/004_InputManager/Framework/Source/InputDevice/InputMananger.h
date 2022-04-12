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
	void Update(); // ��ü�� ������Ʈ�ϱ� ���� �Է� ���� �������� ���� ����

private:
	std::vector<InputComponent::SPtr> m_vecInputComponent;
	std::queue<InputEvent> m_queue;
};