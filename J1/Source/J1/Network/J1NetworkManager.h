#pragma once

#include "CoreMinimal.h"
#include "ClientPacketHandler.h"
#include "PacketSession.h"
#include "J1Core.h"
#include "J1NetworkManager.generated.h"

class AJ1Player;

UCLASS()
class J1_API UJ1NetworkManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:
	// Tick마다 HandleRecvPackets을 호출한다.
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

public:
	// Server에 Connect 시도를 통해 입장한다.
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	// Socket과 GameServerSession을 날려주고 서버로부터 나간다.
	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	// ServerSession이 Server로부터 온 Packet을 처리하게 한다.
	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	// ServerSession이 SendBuffer을 담은 Packet을 보내게 한다.
	void SendPacket(SendBufferRef SendBuffer);

	// ServerSession이 Packet을 보내게 한다. (범용성)
	template<typename T>
	void SendPacket(T packet) const;

public:
	// ObjectInfo를 받아서 Players에 추가하고 초기화를 해준다.
	void HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine);
	// EnterGamePkt 내의 Player를 스폰한다.
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	// SpawnPkt 내의 Player들을 스폰한다.
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	// ObjectId에 해당하는 객체를 destroy한다.
	void HandleDespawn(uint64 ObjectId);
	// 패킷 내의 ObjectId에 해당하는 객체를 destroy한다.
	void HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt);

	void HandleMove(const Protocol::S_MOVE& MovePkt);

	void HandleNotifyPos(const Protocol::S_NOTIFY_POS& NotifyPosPkt);

	void HandleSkill(const Protocol::S_SKILL& SkillPkt);

public:
	// GameServer
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1");
	int16 Port = 7777;

	TSharedPtr<class PacketSession> GameServerSession;
};

/**
 * <패킷버전> 서버에 패킷을 전송하는 것을 요청하는 함수 입니다.
 * 요청이 즉시 반영되지는 않습니다 SendPacketQueue에 저장을하고 SendThread에서 여유가 되면 보내주는 방식입니다.
 */
template <typename T>
void UJ1NetworkManager::SendPacket(T packet) const
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	const SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	GameServerSession->SendPacket(SendBuffer);
}