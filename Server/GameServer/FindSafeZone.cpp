#include "pch.h"
#include "FindSafeZone.h"
#include "Projectile.h"
#include "ObjectManager.h"
#include "SafeZone.h"
#include "Boss.h"
#include "StartRoom.h"

FindSafeZone::FindSafeZone()
{
}

FindSafeZone::~FindSafeZone()
{
}

void FindSafeZone::SetInfo(BossRef owner, int32 templateId)
{
	__super::SetInfo(owner, templateId);


}

void FindSafeZone::DoGimmick()
{
	__super::DoGimmick();

	// 일단 위아래 안전장판 두개 생성
	SafeZoneRef upZone = GenerateSafeZone(Protocol::Direction::DIR_UP);
	SafeZoneRef downZone = GenerateSafeZone(Protocol::Direction::DIR_DOWN);

	// 나머지 왼쪽오른쪽 장판은 위아래 안전장판이 생성을 요청한다.
}

void FindSafeZone::NotifyAndGenerateOtherSafeZone(Protocol::Direction dir)
{
	if (dir == Protocol::Direction::DIR_UP)
	{
		// Generate Right Zone
		if (_gimmickList.find(Protocol::Direction::DIR_RIGHT) == _gimmickList.end())
		{
			SafeZoneRef RightZone = GenerateSafeZone(Protocol::Direction::DIR_RIGHT);
		}
	}
	else if (dir == Protocol::Direction::DIR_DOWN)
	{
		// Generate Left Zone
		if (_gimmickList.find(Protocol::Direction::DIR_LEFT) == _gimmickList.end())
		{
			SafeZoneRef LeftZone = GenerateSafeZone(Protocol::Direction::DIR_LEFT);
		}
	}
}

SafeZoneRef FindSafeZone::GenerateSafeZone(Protocol::Direction dir)
{
	SafeZoneRef safeZone = nullptr;

	float posX = 0.f;
	float posY = 0.f;
	float posZ = 0.f;
	float centerPosX = _owner->GetPosInfo()->x() - 200.f;
	float centerPosY = _owner->GetPosInfo()->y() - 0.f;
	if (dir == Protocol::Direction::DIR_UP)
	{
		posX = centerPosX + SAFE_ZONE_RADIUS;
		posY = centerPosY + 0.f;
	}
	else if (dir == Protocol::Direction::DIR_RIGHT)
	{
		posX = centerPosX + 0.f;
		posY = centerPosY + SAFE_ZONE_RADIUS;
	}
	else if (dir == Protocol::Direction::DIR_DOWN)
	{
		posX = centerPosX - SAFE_ZONE_RADIUS;
		posY = centerPosY + 0.f;
	}
	else if (dir == Protocol::Direction::DIR_LEFT)
	{
		posX = centerPosX + 0.f;
		posY = centerPosY - SAFE_ZONE_RADIUS;
	}
	posZ = _owner->GetPosInfo()->z();

	safeZone = static_pointer_cast<SafeZone>(GObjectManager->CreateProjectile(
		_gimmickData->ProjectileIdList[0],
		static_pointer_cast<Creature>(_owner),
		nullptr,
		static_pointer_cast<GimmickBase>(shared_from_this()),
		posX,
		posY,
		posZ
	));

	safeZone->SetDir(dir);
	
	_gimmickList.insert(make_pair(dir, safeZone));

	safeZone->SpawnProjectile();

	// send packet Protocol::PosInfo* info = movePkt.mutable_info();
	Protocol::S_PROJECTILE projPkt;
	{
		Protocol::ProjectileInfo* info = projPkt.mutable_projectile_info();
		info->set_object_id(safeZone->_objectId);
		info->set_owner_object_id(safeZone->_owner->_objectId);
		info->set_owner_gimmick_id(safeZone->_ownerGimmick->_gimmickData->DataId);
		info->set_data_id(safeZone->_projectileData->DataId);
		{
			Protocol::SimplePosInfo* spInfo = info->mutable_spawn_simple_pos_info();
			spInfo->set_x(posX);
			spInfo->set_y(posY);
			spInfo->set_z(posZ);
		}
		info->set_safe_zone_dir(dir);
	}
	RoomBaseRef roomRef = _owner->room.load().lock();
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(projPkt);
	roomRef->Broadcast(sendBuffer);

	return safeZone;
}

vector<uint64> FindSafeZone::GetSafePlayerList()
{
	vector<uint64> players;

	for (auto& item : _gimmickList)
	{
		players.push_back(item.second->_objectId);
	}

	return players;
}

void FindSafeZone::OnEvent(int32 eventCount)
{
	__super::OnEvent(eventCount);


}
