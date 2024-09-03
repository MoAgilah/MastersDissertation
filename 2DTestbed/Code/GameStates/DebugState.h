#pragma once

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "../Game/GameManager.h"
#include "../GameObjects/Box.h"
#include "../GameObjects/Collectables.h"
#include "../GameObjects/Chuck.h"

class DebugState : public GameState
{
public:
	explicit DebugState(GameManager* gameMgr);
	~DebugState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;

private:

	std::shared_ptr<GameManager> m_gameMgr;
};