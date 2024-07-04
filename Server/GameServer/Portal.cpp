#include "pch.h"
#include "RoomBase.h"
#include "Portal.h"

Portal::Portal()
{
	objectInfo->set_env_type(Protocol::EnvType::ENV_TYPE_PORTAL);

	_radius = 100.f;
}

Portal::~Portal()
{
	
}

void Portal::SetDestRoom(RoomBaseRef destRoom)
{
	_destRoom = destRoom;
}

RoomBaseRef Portal::GetDestRoom()
{
	return _destRoom.lock();
}

void Portal::SetRadius(float rad)
{
	_radius = rad;
}

float Portal::GetRadius()
{
	return _radius;
}
