#include "J1NetworkManager.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "PacketSession.h"
#include "Tickable.h"
#include "System/J1GameInstance.h"
#include "Common/TcpSocketBuilder.h"
#include "Kismet/GameplayStatics.h"
#include "Object/J1MyPlayer.h"
#include "Object/J1ObjectManager.h"

void UJ1NetworkManager::Tick(float DeltaTime)
{
	HandleRecvPackets();
}

bool UJ1NetworkManager::IsTickable() const
{
	return true;
}

TStatId UJ1NetworkManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UJ1NetworkManager, STATGROUP_Tickables);
}

void UJ1NetworkManager::ConnectToGameServer()
{
	// Create Socket
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip); // IpAddress�� Ip�� ����
	
	// InternetAddr : �پ�� �ϴ� ������ �ּ�
	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	// Ip�ּҿ� Port��ȣ ����
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting To Server...")));

	// Connect Socket (Blocking)
	bool Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Success")));

		// Create Server Session
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run(); // SendWorker, RecvWorker ������ ����

		// TEMP : Lobby���� ĳ���� ����â ��
		{
			Protocol::C_LOGIN Pkt;
			SendBufferRef SendBuffer = ClientPacketHandler::MakeSendBuffer(Pkt);
			SendPacket(SendBuffer);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection Failed")));
	}
}

void UJ1NetworkManager::DisconnectFromGameServer()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	
	Protocol::C_LEAVE_GAME LeavePkt;
	SendPacket(LeavePkt);
}

void UJ1NetworkManager::HandleRecvPackets()
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->HandleRecvPackets();
}

void UJ1NetworkManager::SendPacket(SendBufferRef SendBuffer)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	GameServerSession->SendPacket(SendBuffer);
}

void UJ1NetworkManager::HandleSpawn(const Protocol::ObjectInfo& ObjectInfo, bool IsMine)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;
	
	auto* World = GetWorld();
	if (World == nullptr)
		return;

	// �ߺ� ó�� üũ
	const uint64 ObjectId = ObjectInfo.object_id();
	//if (UJ1ObjectManager::Get().GetActorById(ObjectId) != nullptr)
	if (GetManager(Object)->GetActorById(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	if (IsMine) // MyPlayer�� ���
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		AJ1MyPlayer* MyPlayer = Cast<AJ1MyPlayer>(PC->GetPawn());
		if (MyPlayer == nullptr)
			return;

		MyPlayer->SetInfo(ObjectInfo);
		GetManager(Object)->MyPlayer = MyPlayer;
		GetManager(Object)->AddActorToMap(PC->GetPawn(), ObjectId);
	}
	else // ���� �ƴ� Player�� ���
	{
		GetManager(Object)->SpawnObject(ObjectInfo);
	}
}

void UJ1NetworkManager::HandleSpawn(const Protocol::S_ENTER_GAME& EnterGamePkt)
{
	HandleSpawn(EnterGamePkt.player(), true);
}

void UJ1NetworkManager::HandleSpawn(const Protocol::S_SPAWN& SpawnPkt)
{
	for (auto& Player : SpawnPkt.players())
	{
		HandleSpawn(Player, false);
	}
}

void UJ1NetworkManager::HandleDespawn(uint64 ObjectId)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	GetManager(Object)->DespawnObject(ObjectId);
}

void UJ1NetworkManager::HandleDespawn(const Protocol::S_DESPAWN& DespawnPkt)
{
	for (auto& ObjectId : DespawnPkt.object_ids())
	{
		HandleDespawn(ObjectId);
	}
}

void UJ1NetworkManager::HandleMove(const Protocol::S_MOVE& MovePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = MovePkt.info().object_id();
	TObjectPtr<AJ1Creature> FindActor = GetManager(Object)->Creatures[ObjectId];
	if (FindActor == nullptr)
		return;

	const Protocol::PosInfo& Info = MovePkt.info();
	FindActor->ProcessMove(Info);
}

void UJ1NetworkManager::HandleNotifyPos(const Protocol::S_NOTIFY_POS& NotifyPosPkt)
{
}

void UJ1NetworkManager::HandleSkill(const Protocol::S_SKILL& SkillPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = SkillPkt.object_id();
	TObjectPtr<AJ1Creature> FindActor = GetManager(Object)->Creatures[ObjectId];
	if (FindActor == nullptr)
		return;

	const Protocol::SkillSlot& slot = SkillPkt.slot();
	FindActor->ProcessSkill(slot);
}
