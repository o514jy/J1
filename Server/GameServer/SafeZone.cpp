#include "pch.h"
#include "SafeZone.h"
#include "StartRoom.h"
#include "Player.h"
#include "FindSafeZone.h"
#include "SkillComponent.h"
#include "SpreadCloud.h"
#include "Player.h"

SafeZone::SafeZone()
{
	_dir = Protocol::DIR_NONE;
	_callSpawn = false;
}

SafeZone::~SafeZone()
{
	cout << dirStr << "'s Safe Zone Destroyed" << "\n";
	_matchedPlayer = nullptr;
}

void SafeZone::UpdateTick()
{
	__super::UpdateTick();

	// 이미 matchedPlayer가 있었으면 혹시 나가진 않았는지 확인
	if (_matchedPlayer != nullptr)
	{
		// 아직 잘 있으면 종료
		if (true == IsInSafeZone(_matchedPlayer))
		{
			return;
		}
		else
		{
			// 없으면 해제하고 새로 찾아야됨
			cout << dirStr << "'s Matched Player is disappeared " << "\n";
			_matchedPlayer = nullptr;
		}
	}

	// 없으면 찾기 시작
	RoomBaseRef roomRef = this->room.load().lock();
	for (auto& item : roomRef->_objects)
	{
		ObjectRef object = item.second;
		if (object->IsPlayer() == false)
			continue;
		
		// 안에 있는사람 찾았으면 matchedPlayer로 설정
		if (IsInSafeZone(object) == true)
		{
			cout << dirStr << "'s Matched Player object id : " << object->_objectId << "\n";
			_matchedPlayer = static_pointer_cast<Player>(object);

			// 위아래 안전지대일 경우 처음으로 찾은거면 왼쪽오른쪽 안전지대 생성 요청
			if (_dir == Protocol::Direction::DIR_UP || _dir == Protocol::Direction::DIR_DOWN)
			{
				if (_callSpawn == false)
				{
					_callSpawn = true;
					static_pointer_cast<FindSafeZone>(_ownerGimmick)->NotifyAndGenerateOtherSafeZone(_dir);
					return;
				}
			}
		}
	}
}

void SafeZone::SetInfo(BossRef owner, GimmickBaseRef ownerGimmick, int32 templateId)
{
	__super::SetInfo(owner, ownerGimmick, templateId);
}

void SafeZone::SetDir(Protocol::Direction dir)
{
	_dir = dir;

	if (_dir == Protocol::DIR_UP)
		dirStr = "UP";
	else if (_dir == Protocol::DIR_RIGHT)
		dirStr = "RIGHT";
	else if (_dir == Protocol::DIR_DOWN)
		dirStr = "DOWN";
	else if (_dir == Protocol::DIR_LEFT)
		dirStr = "LEFT";
}

Protocol::Direction SafeZone::GetDir()
{
	return _dir;
}

PlayerRef SafeZone::GetMatchedPlayer()
{
	return _matchedPlayer;
}

void SafeZone::Clear()
{
	if (_matchedPlayer != nullptr)
	{
		SpreadCloudRef finSkill = static_pointer_cast<SpreadCloud>(_owner->GetSkillComponent()->_SpreadCloudSkill);
		if (finSkill->safePlayers.find(_matchedPlayer->_objectId) == finSkill->safePlayers.end())
		{
			cout << _matchedPlayer->_objectId << " becomes safe by " << dirStr << "'s Safe Zone" << "\n";
			finSkill->safePlayers.insert(_matchedPlayer->_objectId);
		}
	}

	__super::Clear();

	_matchedPlayer = nullptr;
}

bool SafeZone::IsInSafeZone(ObjectRef object)
{
	bool ret = false;
	// object의 collision이 safe zone 안에 완전히 있어야 한다.

	// safe zone의 반지름에서 player의 반지름을 뺀 값이 중심간의 거리보다 크거나 같아야한다.
	PlayerRef player = static_pointer_cast<Player>(object);
	float deltaRadius = SAFE_ZONE_PROJECTILE_RADIUS - player->GetPlayerData()->ColliderRadius;
	float dist = Utils::DirectionVectorLen(this->GetPosInfo(), player->GetPosInfo());

	if (deltaRadius >= dist)
		ret = true;
	else
		ret = false;

	return ret;
}

void SafeZone::OnImpactEvent(int32 impactCount)
{
	__super::OnImpactEvent(impactCount);
}

void SafeZone::OnDurationCompleteHandler()
{
	if (_matchedPlayer != nullptr)
	{
		cout << _matchedPlayer->_objectId << " becomes safe by " << dirStr << "'s Safe Zone" << "\n";
		SpreadCloudRef finSkill = static_pointer_cast<SpreadCloud>(_owner->GetSkillComponent()->_SpreadCloudSkill);
		finSkill->safePlayers.insert(_matchedPlayer->_objectId);
	}

	__super::OnDurationCompleteHandler();
}


