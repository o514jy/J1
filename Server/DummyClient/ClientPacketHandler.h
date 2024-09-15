#pragma once
#include "Protocol.pb.h"

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
#include "J1Core.h"
#include "Utils/J1NetworkUtils.h"
#endif

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

// Utils Func
class UJ1NetworkManager* GetWorldNetwork(const PacketSessionRef& Session);

enum : uint16
{
	PKT_C_LOGIN = 1000,
	PKT_S_LOGIN = 1001,
	PKT_C_ENTER_GAME = 1002,
	PKT_S_ENTER_GAME = 1003,
	PKT_C_LEAVE_GAME = 1004,
	PKT_S_LEAVE_GAME = 1005,
	PKT_S_SPAWN = 1006,
	PKT_S_DESPAWN = 1007,
	PKT_C_MOVE = 1008,
	PKT_S_MOVE = 1009,
	PKT_C_TELEPORT = 1010,
	PKT_S_TELEPORT = 1011,
	PKT_C_TELEPORT_FIN = 1012,
	PKT_S_CHANGE_TARGET = 1013,
	PKT_C_NOTIFY_POS = 1014,
	PKT_S_NOTIFY_POS = 1015,
	PKT_C_SKILL = 1016,
	PKT_S_SKILL = 1017,
	PKT_S_GIMMICK = 1018,
	PKT_S_PROJECTILE = 1019,
	PKT_S_BUFF = 1020,
	PKT_S_APPLY_BUFF = 1021,
	PKT_S_REMOVE_BUFF = 1022,
	PKT_S_STAT = 1023,
	PKT_S_SPAWNING_POOL = 1024,
	PKT_C_EQUIP_ITEM = 1025,
	PKT_C_UNEQUIP_ITEM = 1026,
	PKT_S_ADD_ITEM = 1027,
	PKT_C_CHAT = 1028,
	PKT_S_CHAT = 1029,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt);
bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt);
bool Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt);
bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt);
bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt);
bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt);
bool Handle_S_TELEPORT(PacketSessionRef& session, Protocol::S_TELEPORT& pkt);
bool Handle_S_CHANGE_TARGET(PacketSessionRef& session, Protocol::S_CHANGE_TARGET& pkt);
bool Handle_S_NOTIFY_POS(PacketSessionRef& session, Protocol::S_NOTIFY_POS& pkt);
bool Handle_S_SKILL(PacketSessionRef& session, Protocol::S_SKILL& pkt);
bool Handle_S_GIMMICK(PacketSessionRef& session, Protocol::S_GIMMICK& pkt);
bool Handle_S_PROJECTILE(PacketSessionRef& session, Protocol::S_PROJECTILE& pkt);
bool Handle_S_BUFF(PacketSessionRef& session, Protocol::S_BUFF& pkt);
bool Handle_S_APPLY_BUFF(PacketSessionRef& session, Protocol::S_APPLY_BUFF& pkt);
bool Handle_S_REMOVE_BUFF(PacketSessionRef& session, Protocol::S_REMOVE_BUFF& pkt);
bool Handle_S_STAT(PacketSessionRef& session, Protocol::S_STAT& pkt);
bool Handle_S_SPAWNING_POOL(PacketSessionRef& session, Protocol::S_SPAWNING_POOL& pkt);
bool Handle_S_ADD_ITEM(PacketSessionRef& session, Protocol::S_ADD_ITEM& pkt);
bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_S_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LOGIN>(Handle_S_LOGIN, session, buffer, len); };
		GPacketHandler[PKT_S_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ENTER_GAME>(Handle_S_ENTER_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_LEAVE_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_LEAVE_GAME>(Handle_S_LEAVE_GAME, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWN>(Handle_S_SPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_DESPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_DESPAWN>(Handle_S_DESPAWN, session, buffer, len); };
		GPacketHandler[PKT_S_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_MOVE>(Handle_S_MOVE, session, buffer, len); };
		GPacketHandler[PKT_S_TELEPORT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_TELEPORT>(Handle_S_TELEPORT, session, buffer, len); };
		GPacketHandler[PKT_S_CHANGE_TARGET] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHANGE_TARGET>(Handle_S_CHANGE_TARGET, session, buffer, len); };
		GPacketHandler[PKT_S_NOTIFY_POS] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_NOTIFY_POS>(Handle_S_NOTIFY_POS, session, buffer, len); };
		GPacketHandler[PKT_S_SKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SKILL>(Handle_S_SKILL, session, buffer, len); };
		GPacketHandler[PKT_S_GIMMICK] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_GIMMICK>(Handle_S_GIMMICK, session, buffer, len); };
		GPacketHandler[PKT_S_PROJECTILE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_PROJECTILE>(Handle_S_PROJECTILE, session, buffer, len); };
		GPacketHandler[PKT_S_BUFF] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_BUFF>(Handle_S_BUFF, session, buffer, len); };
		GPacketHandler[PKT_S_APPLY_BUFF] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_APPLY_BUFF>(Handle_S_APPLY_BUFF, session, buffer, len); };
		GPacketHandler[PKT_S_REMOVE_BUFF] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_REMOVE_BUFF>(Handle_S_REMOVE_BUFF, session, buffer, len); };
		GPacketHandler[PKT_S_STAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_STAT>(Handle_S_STAT, session, buffer, len); };
		GPacketHandler[PKT_S_SPAWNING_POOL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_SPAWNING_POOL>(Handle_S_SPAWNING_POOL, session, buffer, len); };
		GPacketHandler[PKT_S_ADD_ITEM] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_ADD_ITEM>(Handle_S_ADD_ITEM, session, buffer, len); };
		GPacketHandler[PKT_S_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_CHAT>(Handle_S_CHAT, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::C_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_C_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_LEAVE_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_LEAVE_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_C_MOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::C_TELEPORT& pkt) { return MakeSendBuffer(pkt, PKT_C_TELEPORT); }
	static SendBufferRef MakeSendBuffer(Protocol::C_TELEPORT_FIN& pkt) { return MakeSendBuffer(pkt, PKT_C_TELEPORT_FIN); }
	static SendBufferRef MakeSendBuffer(Protocol::C_NOTIFY_POS& pkt) { return MakeSendBuffer(pkt, PKT_C_NOTIFY_POS); }
	static SendBufferRef MakeSendBuffer(Protocol::C_SKILL& pkt) { return MakeSendBuffer(pkt, PKT_C_SKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::C_EQUIP_ITEM& pkt) { return MakeSendBuffer(pkt, PKT_C_EQUIP_ITEM); }
	static SendBufferRef MakeSendBuffer(Protocol::C_UNEQUIP_ITEM& pkt) { return MakeSendBuffer(pkt, PKT_C_UNEQUIP_ITEM); }
	static SendBufferRef MakeSendBuffer(Protocol::C_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_CHAT); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		SendBufferRef sendBuffer = MakeShared<SendBuffer>(packetSize);
#else
		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
#endif

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		pkt.SerializeToArray(&header[1], dataSize);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};