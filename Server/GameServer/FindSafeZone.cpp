#include "pch.h"
#include "FindSafeZone.h"
#include "Projectile.h"
#include "ObjectManager.h"
#include "SafeZone.h"
#include "Boss.h"

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
	if (dir == Protocol::Direction::DIR_UP)
	{
		posX = _owner->GetPosInfo()->x() + SAFE_ZONE_RADIUS;
		posY = _owner->GetPosInfo()->y() + 0.f;
	}
	else if (dir == Protocol::Direction::DIR_RIGHT)
	{
		posX = _owner->GetPosInfo()->x() + 0.f;
		posY = _owner->GetPosInfo()->y() + SAFE_ZONE_RADIUS;
	}
	else if (dir == Protocol::Direction::DIR_DOWN)
	{
		posX = _owner->GetPosInfo()->x() - SAFE_ZONE_RADIUS;
		posY = _owner->GetPosInfo()->y() + 0.f;
	}
	else if (dir == Protocol::Direction::DIR_LEFT)
	{
		posX = _owner->GetPosInfo()->x() + 0.f;
		posY = _owner->GetPosInfo()->y() - SAFE_ZONE_RADIUS;
	}
	posZ = _owner->GetPosInfo()->z();

	safeZone = static_pointer_cast<SafeZone>(GObjectManager->CreateProjectile(
		_gimmickData->DataId,
		static_pointer_cast<Creature>(_owner),
		nullptr,
		static_pointer_cast<GimmickBase>(shared_from_this()),
		posX,
		posY,
		posZ
	));

	safeZone->SetDir(dir);
	safeZone->SetInfo(_owner, static_pointer_cast<GimmickBase>(shared_from_this()), _gimmickData->DataId);
	
	_gimmickList.insert(make_pair(dir, safeZone));

	safeZone->SpawnProjectile();

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
