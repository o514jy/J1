#include "pch.h"
#include "Player.h"
#include "DataManager.h"
#include "Inventory.h"

Player::Player()
{
	//objectInfo->set_creature_type(Protocol::CreatureType::CREATURE_TYPE_PLAYER);
	_isPlayer = true;

	//objectInfo->set_allocated_player_data(playerData.get());
	_inventoryComponent = nullptr;
}

Player::~Player()
{
	 
}

void Player::UpdateTick()
{
	__super::UpdateTick();
}

void Player::SetInfo(int32 templateId)
{
	__super::SetInfo(templateId);

	_inventoryComponent = make_shared<Inventory>();
	_inventoryComponent->SetInfo(static_pointer_cast<Player>(shared_from_this()));
}

void Player::Clear()
{
	__super::Clear();

	_inventoryComponent = nullptr;
}
