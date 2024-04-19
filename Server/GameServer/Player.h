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
	/** initialize **/
	virtual void SetInfo(int32 templateId) override;

public:
	shared_ptr<PlayerData> _playerDataRef;
};

