#pragma once

#include "PCH.h"

enum class EInputState : UINT8
{
	DOWN = 0,
	UP,
};

union InputEvent
{
	struct
	{
		UINT8 inputCode;
		EInputState inputState;
	};

	UINT16 ID;
};

class Scene;
class InputComponent;

class InputManager
{
	GENERATE_SINGLETON(InputManager);

public:
	InputComponent* CreateInputComponent();

	void PushInputEvent(const InputEvent& inputEvent);
	void Update(); // ��ü�� ������Ʈ�ϱ� ���� �Է� ���� �������� ���� ����

private:
	std::vector<std::shared_ptr<InputComponent>> m_vecInputComponent;
	std::queue<InputEvent> m_queue;
};