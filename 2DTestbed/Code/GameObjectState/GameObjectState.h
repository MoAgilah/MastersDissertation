#pragma once

#include <string>

class GameObjectState
{
public:
	explicit GameObjectState(std::string_view name)
		: m_StateName(name) {}

	std::string_view GetStateName() { return m_StateName; }
	virtual ~GameObjectState() = default;

	virtual void Initialise() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void ProcessInputs() = 0;
	virtual void Update(float deltaTime) = 0;
private:

	std::string m_StateName;
};
