#include "pch.h"
#include "Player.h"
#include "DataManager.h"

Player::Player()
{
	//objectInfo->set_creature_type(Protocol::CreatureType::CREATURE_TYPE_PLAYER);
	_isPlayer = true;

	_playerDataRef = make_shared<PlayerData>();
	//objectInfo->set_allocated_player_data(playerData.get());
}

Player::~Player()
{
	_playerDataRef = nullptr;
}

void Player::UpdateTick()
{
	__super::UpdateTick();
}

void Player::SetInfo(int32 templateId)
{
	__super::SetInfo(templateId);
}
