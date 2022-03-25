#include "PCH.h"
#include "InputComponent.h"

void InputComponent::RegisterListener(const InputEvent& inputEvent, const Listener& listener)
{
	m_mapListener.emplace(inputEvent.ID, listener);
}

InputComponent::Listener InputComponent::FindListener(const InputEvent& inputEvent)
{
	const auto& iter = m_mapListener.find(inputEvent.ID);
	if (iter == m_mapListener.cend())
	{
		return std::bind(&NullListener);
	}

	return iter->second;
}