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
	// Tick���� HandleRecvPackets�� ȣ���Ѵ�.
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

public:
	// Server�� Connect �õ��� ���� �����Ѵ�.
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	// Socket�� GameServerSession�� �����ְ� �����κ��� ������.
	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	// ServerSession�� Server�κ��� �� Packet�� ó���ϰ� �Ѵ�.
	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	// ServerSession�� SendBuffer�� ���� Packet�� ������ �Ѵ�.
	void SendPacket(SendBufferRef SendBuffer);

	// ServerSession�� Packet�� ������ �Ѵ�. (���뼺)
	template<typename T>
	void SendPacket(T packet) const;

public:
	// ObjectInfo�� �޾Ƽ� Players�� �߰��ϰ� �ʱ�ȭ�� ���ش�.
	void HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine);
	// EnterGamePkt ���� Player�� �����Ѵ�.
	void HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt);
	// SpawnPkt ���� Player���� �����Ѵ�.
	void HandleSpawn(const Protocol::S_SPAWN& SpawnPkt);

	// ObjectId�� �ش��ϴ� ��ü�� destroy�Ѵ�.
	void HandleDespawn(uint64 ObjectId);
	// ��Ŷ ���� ObjectId�� �ش��ϴ� ��ü�� destroy�Ѵ�.
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
 * <��Ŷ����> ������ ��Ŷ�� �����ϴ� ���� ��û�ϴ� �Լ� �Դϴ�.
 * ��û�� ��� �ݿ������� �ʽ��ϴ� SendPacketQueue�� �������ϰ� SendThread���� ������ �Ǹ� �����ִ� ����Դϴ�.
 */
template <typename T>
void UJ1NetworkManager::SendPacket(T packet) const
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	const SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(packet);
	GameServerSession->SendPacket(SendBuffer);
}