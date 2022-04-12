#include "PCH.h"
#include "InputMananger.h"

InputComponent::SPtr InputManager::CreateInputComponent()
{
	InputComponent::SPtr spInputComponent = std::make_shared<InputComponent>();
	m_vecInputComponent.push_back(spInputComponent);
	return (m_vecInputComponent.back()); // ���� �������� ���� �� ��ȯ�մϴ�.
}

void InputManager::PushInputEvent(const InputEvent& inputEvent)
{
	m_queue.push(inputEvent);
}

void InputManager::Update()
{
	while (m_queue.empty() == false)
	{
		InputEvent inputEvent = m_queue.front();

		// ��ϵ� ��� �Է� ��ü�� ������
		for (auto& iter : m_vecInputComponent)
		{
			const InputComponent::Listener& listener = iter->FindListener(inputEvent);
			listener();
		}

		m_queue.pop();
	}
}
