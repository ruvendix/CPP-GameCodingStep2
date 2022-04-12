#include "PCH.h"
#include "InputMananger.h"

InputComponent::SPtr InputManager::CreateInputComponent()
{
	InputComponent::SPtr spInputComponent = std::make_shared<InputComponent>();
	m_vecInputComponent.push_back(spInputComponent);
	return (m_vecInputComponent.back()); // 가장 마지막에 넣은 걸 반환합니다.
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

		// 등록된 모든 입력 객체로 돌리기
		for (auto& iter : m_vecInputComponent)
		{
			const InputComponent::Listener& listener = iter->FindListener(inputEvent);
			listener();
		}

		m_queue.pop();
	}
}
