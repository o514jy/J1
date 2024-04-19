#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "Protocol.pb.h"
#include "RoomBase.h"
#include "Player.h"
#include "GameSession.h"
#include "ObjectManager.h"
#include "RoomManager.h"
#include "StartRoom.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	// TODO : DB���� Account ���� �ܾ�´�
	// TODO : DB���� ���� ���� �ܾ�´�
	Protocol::S_LOGIN loginPkt;

	for (int32 i = 0; i < 3; i++)
	{
		Protocol::ObjectInfo* player = loginPkt.add_players();
		Protocol::PosInfo* posInfo = player->mutable_pos_info();
		posInfo->set_x(Utils::GetRandom(0.f, 100.f));
		posInfo->set_y(Utils::GetRandom(0.f, 100.f));
		posInfo->set_z(Utils::GetRandom(0.f, 100.f));
		posInfo->set_yaw(Utils::GetRandom(0.f, 45.f));
	}

	loginPkt.set_success(true);

	SEND_PACKET(loginPkt);

	return true;
}

bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	// �÷��̾� ����
	PlayerRef player = GObjectManager->CreatePlayer(static_pointer_cast<GameSession>(session), 1);
	
	// �濡 ����
	GRoomManager->GetStartRoom()->DoAsync(&StartRoom::HandleEnterPlayer, player);

	return true;
}

bool Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomBaseRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->HandleLeavePlayer(player);

	return true;
}

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomBaseRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&RoomBase::HandleMove, pkt);

	return true;
}

bool Handle_C_NOTIFY_POS(PacketSessionRef& session, Protocol::C_NOTIFY_POS& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomBaseRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&RoomBase::HandleNotifyPos, pkt);

	return true;
}

bool Handle_C_SKILL(PacketSessionRef& session, Protocol::C_SKILL& pkt)
{
	auto gameSession = static_pointer_cast<GameSession>(session);

	PlayerRef player = gameSession->player.load();
	if (player == nullptr)
		return false;

	RoomBaseRef room = player->room.load().lock();
	if (room == nullptr)
		return false;

	room->DoAsync(&RoomBase::HandleSkill, pkt);

	return true;
}

bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	return true;
}
