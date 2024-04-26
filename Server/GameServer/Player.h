#pragma once
#include "Creature.h"

class GameSession;
class Room;

class Player : public Creature
{
public:
	Player();
	virtual ~Player();

public:
	/** update tick **/
	virtual void UpdateTick() override;

public:
	/** initialize **/
	virtual void SetInfo(int32 templateId) override;

public:
	/** setter & getter **/
	PlayerDataRef GetPlayerData() { return static_pointer_cast<PlayerData>(_creatureData); }

public:
};

