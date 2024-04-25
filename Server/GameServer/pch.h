#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "CorePch.h"

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"
#include "ServerPacketHandler.h"
#include "Utils.h"
#include "GameSession.h"
#include "Global.h"
#include "Define.h"

/** network **/
USING_SHARED_PTR(GameSession);

/** object **/
USING_SHARED_PTR(Player);
USING_SHARED_PTR(Monster);
USING_SHARED_PTR(Boss);
USING_SHARED_PTR(Creature);
USING_SHARED_PTR(Object);

/** room **/
USING_SHARED_PTR(RoomBase);
USING_SHARED_PTR(StartRoom);

/** data **/
USING_SHARED_PTR(CreatureData);
USING_SHARED_PTR(PlayerData);
USING_SHARED_PTR(MonsterData);
USING_SHARED_PTR(BossData);
USING_SHARED_PTR(SkillData);
USING_SHARED_PTR(EffectData);
USING_SHARED_PTR(RectangleEffectData);
USING_SHARED_PTR(PizzaEffectData);
USING_SHARED_PTR(BuffData);

/** skill **/
USING_SHARED_PTR(SkillBase);
USING_SHARED_PTR(SkillComponent);

/** buff **/
USING_SHARED_PTR(BuffBase);

/** ai controller **/
USING_SHARED_PTR(BaseAIController);
USING_SHARED_PTR(MonsterAIController);

/** stat **/
USING_SHARED_PTR(StatComponent);

#define SEND_PACKET(pkt)													\
	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);	\
	session->Send(sendBuffer);