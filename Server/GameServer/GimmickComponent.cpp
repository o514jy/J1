#include "pch.h"
#include "GimmickComponent.h"
#include "GimmickBase.h"
#include "Data.h"
#include "SkillManager.h"
#include "Boss.h"
#include "StartRoom.h"

GimmickComponent::GimmickComponent()
{
	_owner = nullptr;
	_activeGimmick = nullptr;
}

GimmickComponent::~GimmickComponent()
{
	_owner = nullptr;
	_activeGimmick = nullptr;
}

void GimmickComponent::SetInfo(BossRef owner)
{
	_owner = owner;

	//_canActiveGimmickList.push_back(GSkillManager->GenerateGimmickById(FIND_SAFE_ZONE_DATA_ID));
	AddGimmick(FIND_SAFE_ZONE_DATA_ID);


}

void GimmickComponent::DoGimmick(int32 gimmickId)
{
	//for (GimmickBaseRef gimmick : _canActiveGimmickList)
	//{
	//	if (gimmick->_gimmickData->DataId == gimmickId)
	//	{
	//		gimmick->DoGimmick();
	//
	//		// 기믹 시작과 관련된 정보가 필요한 경우 패킷 보내기
	//		Protocol::S_GIMMICK gmkPkt;
	//		gmkPkt.set_object_id(_owner->_objectId);
	//		gmkPkt.set_gimmick_data_id(gimmickId);
	//		RoomBaseRef roomRef = _owner->room.load().lock();
	//		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(gmkPkt);
	//		roomRef->Broadcast(sendBuffer);
	//
	//		return;
	//	}
	//}

	for (auto& item : _gimmicks)
	{
		if (item.first == gimmickId)
		{
			if (item.second->GetCanUseGimmick() == false)
			{
				return;
			}

			SetActiveGimmick(item.second);
			item.second->DoGimmick();

			// 기믹 시작과 관련된 정보가 필요한 경우 패킷 보내기
			Protocol::S_GIMMICK gmkPkt;
			gmkPkt.set_object_id(_owner->_objectId);
			gmkPkt.set_gimmick_data_id(gimmickId);
			RoomBaseRef roomRef = _owner->room.load().lock();
			SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(gmkPkt);
			roomRef->Broadcast(sendBuffer);

			return;
		}
	}
}

void GimmickComponent::AddGimmick(int32 gimmickId)
{
	if (gimmickId < 1)
		return;
	GimmickBaseRef gimmick = GSkillManager->GenerateGimmickById(FIND_SAFE_ZONE_DATA_ID);
	gimmick->SetInfo(_owner, gimmickId);

	_gimmicks.insert(make_pair(FIND_SAFE_ZONE_DATA_ID, gimmick));
}

int32 GimmickComponent::PopCanActiveGimmickId()
{
	if (_gimmickIdQueue.empty() == true)
	{
		return 0;
	}
	else
	{
		int32 ret = _gimmickIdQueue.front();
		_gimmickIdQueue.pop();
		return ret;
	}
}

void GimmickComponent::SetActiveGimmick(GimmickBaseRef gimmick)
{
	_activeGimmick = gimmick;
}

GimmickBaseRef GimmickComponent::GetActiveGimmick()
{
	return _activeGimmick;
}
