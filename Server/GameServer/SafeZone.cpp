#include "pch.h"
#include "SafeZone.h"
#include "StartRoom.h"
#include "Player.h"
#include "FindSafeZone.h"

SafeZone::SafeZone()
{
	_dir = Protocol::DIR_NONE;
	_callSpawn = false;
}

SafeZone::~SafeZone()
{
}

void SafeZone::UpdateTick()
{
	__super::UpdateTick();

	// �̹� matchedPlayer�� �־����� Ȥ�� ������ �ʾҴ��� Ȯ��
	if (_matchedPlayer != nullptr)
	{
		// ���� �� ������ ����
		if (true == IsInSafeZone(_matchedPlayer))
		{
			return;
		}
		else
		{
			// ������ �����ϰ� ���� ã�ƾߵ�
			_matchedPlayer = nullptr;
		}
	}

	// ������ ã�� ����
	RoomBaseRef roomRef = this->room.load().lock();
	for (auto& item : roomRef->_objects)
	{
		ObjectRef object = item.second;
		if (object->IsPlayer() == false)
			continue;
		
		// �ȿ� �ִ»�� ã������ matchedPlayer�� ����
		if (IsInSafeZone(object) == true)
		{
			_matchedPlayer = static_pointer_cast<Player>(object);

			// ���Ʒ� ���������� ��� ó������ ã���Ÿ� ���ʿ����� �������� ���� ��û
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
	__super::Clear();

	_matchedPlayer = nullptr;
}

bool SafeZone::IsInSafeZone(ObjectRef object)
{
	bool ret = false;
	// object�� collision�� safe zone �ȿ� ������ �־�� �Ѵ�.

	// safe zone�� ���������� player�� �������� �� ���� �߽ɰ��� �Ÿ����� ũ�ų� ���ƾ��Ѵ�.
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


