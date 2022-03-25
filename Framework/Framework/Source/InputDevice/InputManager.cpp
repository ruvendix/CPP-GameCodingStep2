#include "PCH.h"
#include "InputManager.h"

#include "Scene/Scene.h"
#include "InputComponent.h"

InputComponent* InputManager::CreateInputComponent()
{
	std::shared_ptr<InputComponent> spInputComponent = std::make_shared<InputComponent>();
	m_vecInputComponent.push_back(spInputComponent);
	return (m_vecInputComponent.back().get());
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