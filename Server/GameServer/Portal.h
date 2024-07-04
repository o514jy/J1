#pragma once

#include "Env.h"

class Portal : public Env
{
public:
	Portal();
	~Portal();

public:
	/** setter & getter **/
	void SetDestRoom(RoomBaseRef destRoom);
	RoomBaseRef GetDestRoom();

	void SetRadius(float rad);
	float GetRadius();

public:
	/** information **/
	weak_ptr<RoomBase> _destRoom;

	float _radius;
};

