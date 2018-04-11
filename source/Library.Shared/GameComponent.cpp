#include "pch.h"
#include "GameComponent.h"

namespace Library
{
	RTTI_DEFINITIONS(GameComponent)

	GameComponent::GameComponent(Game& game) :
		mGame(&game)
	{
	}

	Game* GameComponent::GetGame()
	{
		return mGame;
	}

	void GameComponent::SetGame(Game& game)
	{
		mGame = &game;
	}

	bool GameComponent::Enabled() const
	{
		return mEnabled;
	}

	void GameComponent::SetEnabled(bool enabled)
	{
		mEnabled = enabled;
	}

	void GameComponent::Initialize()
	{
	}

	void GameComponent::Update(const GameTime&)
	{		
	}
}