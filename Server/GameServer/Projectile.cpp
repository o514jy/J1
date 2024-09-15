#include "pch.h"
#include "Projectile.h"
#include "DataManager.h"
#include "StartRoom.h"
#include "Creature.h"
#include "SkillBase.h"
#include "BuffInstant.h"
#include "GimmickBase.h"
#include "Boss.h"
#include "ObjectManager.h"
#include "BuffComponent.h"

Projectile::Projectile()
{
	_projectileData = nullptr;
	_owner = nullptr;
	_ownerSkill = nullptr;
	_ownerGimmick = nullptr;
	_destPos = FVector3();
	_impactCount = 0;

	_projectileInfo = new Protocol::ProjectileInfo();
	_projectileType = Protocol::ProjectileType::PROJECTILE_TYPE_NONE;
	objectInfo->set_allocated_projectile_info(_projectileInfo);
	objectInfo->set_projectile_type(Protocol::ProjectileType::PROJECTILE_TYPE_GENERAL);
}

Projectile::~Projectile()
{
	_projectileData = nullptr;
	_owner = nullptr;
	_ownerSkill = nullptr;
	_ownerGimmick = nullptr;
}

void Projectile::UpdateTick()
{
	__super::UpdateTick();

	// �������� ���� �̵�
	if (_projectileData != nullptr)
		MoveToDestPos();
}

void Projectile::MoveToDestPos()
{
	if (_destPos != FVector3())
	{
		FVector3 dir = _destPos - GetPosInfoVec();
		float distLen = dir.Magnitude();
		dir = dir.Normalize();
		FVector3 delta = dir * _projectileData->MoveSpeed * TICK_COUNT_SEC;
		if (distLen > delta.Magnitude())
		{
			posInfo->set_x(posInfo->x() + delta.X);
			posInfo->set_y(posInfo->y() + delta.Y);
			posInfo->set_z(posInfo->z() + delta.Z);
		}
		else
		{
			posInfo->set_x(_destPos.X);
			posInfo->set_y(_destPos.Y);
			posInfo->set_z(_destPos.Z);
		}
	}
}

void Projectile::SetInfo(CreatureRef owner, SkillBaseRef ownerSkill, int32 templateId)
{
	_owner = owner;
	_ownerSkill = ownerSkill;

	_objectId = objectInfo->object_id();
	_templateId = objectInfo->template_id();

	_projectileData = GDataManager->GetProjectileDataById(templateId);

	_projectileInfo->set_object_id(_objectId);
	_projectileInfo->set_owner_object_id(_owner->_objectId);
	_projectileInfo->set_owner_skill_id(_ownerSkill->_skillData->DataId);
	_projectileInfo->set_data_id(templateId);
	{
		Protocol::SimplePosInfo* spInfo = _projectileInfo->mutable_spawn_simple_pos_info();
		spInfo->set_x(_owner->posInfo->x());
		spInfo->set_y(_owner->posInfo->y());
		spInfo->set_z(_owner->posInfo->z());
	}

	_impactCount = 0;
}

void Projectile::SetInfo(BossRef owner, GimmickBaseRef ownerGimmick, int32 templateId)
{
	_owner = owner;
	_ownerGimmick = ownerGimmick;

	_objectId = objectInfo->object_id();
	_templateId = objectInfo->template_id();

	_projectileData = GDataManager->GetProjectileDataById(templateId);

	_projectileInfo->set_object_id(_objectId);
	_projectileInfo->set_owner_object_id(_owner->_objectId);
	_projectileInfo->set_owner_gimmick_id(_ownerGimmick->_gimmickData->DataId);
	_projectileInfo->set_data_id(templateId);
	{
		Protocol::SimplePosInfo* spInfo = _projectileInfo->mutable_spawn_simple_pos_info();
		spInfo->set_x(_owner->posInfo->x());
		spInfo->set_y(_owner->posInfo->y());
		spInfo->set_z(_owner->posInfo->z());
	}

	_impactCount = 0;
}

void Projectile::Clear()
{
	__super::Clear();

	_projectileData = nullptr;
	_owner = nullptr;
	_ownerSkill = nullptr;
	_ownerGimmick = nullptr;

	for (int i = 0; i < _timerJobs.size(); i++)
	{
		if (_timerJobs[i].lock())
		{
			_timerJobs[i].lock()->SetCancled(true);
		}
		_timerJobs[i] = weak_ptr<Job>();
	}

	_timerJobs.clear();

}

void Projectile::SetDestPos(FVector3 destPos)
{
	_destPos = destPos;
}

void Projectile::ForceDelete()
{
	Clear();

	GObjectManager->RemoveObject(this->_objectId);
}

void Projectile::OnImpactTimeHandler()
{
	OnImpactEvent(_impactCount++);
}

void Projectile::OnImpactEvent(int32 impactCount)
{
	// ���������� �̻��� ��Ȳ�̸� ����
	if (impactCount >= _projectileData->ImpactTimeList.size())
		return;

	// ���������� ����� �ϰ��� �����ϸ� �о��ֱ�
	//_animImpactJobs[timeCount] = nullptr;

	// ���� ���� �� �ش��ϴ� object���� ���� �ο�
	int32 effectId = _projectileData->EffectIdList[_impactCount];
	vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);
	ProcessBuff(objects);

}

void Projectile::OnDurationCompleteHandler()
{
	_impactCount = 0;

	// ������ ó���� ������ ������ϴ� ���
	if (_projectileData->ImpactTimeList.size() == 0 && _projectileData->EffectIdList.size() != 0)
	{
		int32 effectId = _projectileData->EffectIdList[_impactCount];
		vector<ObjectRef> objects = GatherObjectInEffectArea(effectId);
		ProcessBuff(objects);
	}

	// �� �������� �Ҹ�
	GObjectManager->RemoveObject(this->_objectId);
	//RoomBaseRef roomRef = this->room.load().lock();
	//roomRef->RemoveObject(this->_objectId);
}

void Projectile::SpawnProjectile()
{
	_impactCount = 0;
	// do projectile

	RoomBaseRef roomRef = GetRoomRef();
	if (roomRef == nullptr || roomRef == GEmptyRoom)
		return;

	for (int32 i = 0; i < _projectileData->ImpactTimeList.size(); i++)
	{
		_timerJobs.push_back(roomRef->DoTimer(_projectileData->ImpactTimeList[i], static_pointer_cast<Projectile>(shared_from_this()), &Projectile::OnImpactTimeHandler));
	}
	
	if (_destPos != FVector3())
	{
		// ���� �ð� ����
		float dist = (GetPosInfoVec() - _destPos).Magnitude();
		// �и��� ���� �ð� ���
		uint64 time = (uint64)(dist / (_projectileData->MoveSpeed * 0.001f));
		_timerJobs.push_back(roomRef->DoTimer(time, static_pointer_cast<Projectile>(shared_from_this()), &Projectile::OnDurationCompleteHandler));
	}
	else
	{
		_timerJobs.push_back(roomRef->DoTimer(_projectileData->Duration, static_pointer_cast<Projectile>(shared_from_this()), &Projectile::OnDurationCompleteHandler));
	}

	//// broadcast packet
	//Protocol::S_SPAWN spawnPkt;
	//{
	//	Protocol::ObjectInfo* objectInfo = spawnPkt.add_players();
	//	objectInfo->CopyFrom(*objectInfo);
	//}
	//SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(spawnPkt);
	//roomRef->Broadcast(sendBuffer);
}

vector<ObjectRef> Projectile::GatherObjectInEffectArea(int32 effectId)
{
	vector<ObjectRef> objects;

	RoomBaseRef room = _owner->room.load().lock();
	if (room == nullptr || room == GEmptyRoom)
		return objects;

	for (auto& item : room->_objects)
	{
		ObjectRef object = item.second;

		if (_owner == nullptr)
			continue;

		if (object == nullptr)
			continue;

		// ��ų ������ ����
		if (object->_objectId == _owner->_objectId)
			continue;

		// ũ���� �ƴϸ� ����
		if (object->_objectType != Protocol::ObjectType::OBJECT_TYPE_CREATURE)
			continue;

		// ���� ���̸� �켱 ����
		CreatureRef creature = static_pointer_cast<Creature>(object);
		if (creature->GetCreatureData()->CreatureType == _owner->GetCreatureData()->CreatureType)
			continue;

		EffectDataRef effectData = GDataManager->GetEffectDataById(effectId);
		wstring effectType = effectData->EffectType;
		if (effectType == L"Rectangle")
		{

		}
		else if (effectType == L"Pizza")
		{
			
		}
		else if (effectType == L"Circle")
		{
			CircleEffectDataRef circleData = static_pointer_cast<CircleEffectData>(effectData);

			bool flag = IsInCircleArea(object, circleData->Radius);
			if (true == flag)
			{
				objects.push_back(object);
			}
		}
	}

	return objects;
}

void Projectile::ProcessBuff(vector<ObjectRef>& objects)
{
	// buff ó��
	for (auto& object : objects)
	{
		if (object == nullptr)
			continue;

		RoomBaseRef room = object->GetRoomRef();
		if (room == nullptr || room == GEmptyRoom)
			return;

		object->_buffComponent->ApplyBuff(_projectileData->BuffIdList[_impactCount], _owner);

		//// 1) attack ������ ����� buff ������ ���� ���� Ÿ�� �� ���ӽð� Ȯ��
		//wstring buffDurationType = GDataManager->GetBuffDataById(_projectileData->BuffIdList[_impactCount])->BuffDurationType;
		//
		//// 2) ���� ���ӽð� Ÿ�Կ� �´� ���� ����
		//BuffBaseRef buff = nullptr;
		//if (buffDurationType == L"Instant")
		//{
		//	buff = make_shared<BuffInstant>();
		//}
		//
		//// 3) ���� �ʱ�ȭ �� ����
		//buff->SetInfo(_projectileData->BuffIdList[_impactCount], object, _ownerSkill);
		//
		//// 4) ���� object���� buff �ο�
		//buff->ApplyBuff();
	}
}

bool Projectile::IsInCircleArea(ObjectRef object, float effectRadius/*ȿ�� �Ÿ�*/)
{
	bool ret = false;

	float dist = Utils::DirectionVectorLen(posInfo, object->posInfo);

	CreatureRef creature = static_pointer_cast<Creature>(object);
	float radiusSum = creature->GetCreatureData()->ColliderRadius + effectRadius;

	// �߽ɰ��� �Ÿ��� �������� �պ��� ���� ���
	if (dist < radiusSum)
	{
		ret = true;
	}
	else
	{
		ret = false;
	}

	return ret;
}
