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
#include "J1/Game/Stat/J1StatComponent.h"
#include "J1/Game/Object/J1Boss.h"

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
	FIPv4Address::Parse(IpAddress, Ip); // IpAddress를 Ip에 저장
	
	// InternetAddr : 붙어야 하는 서버의 주소
	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	// Ip주소와 Port번호 저장
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
		GameServerSession->Run(); // SendWorker, RecvWorker 스레드 생성

		// TEMP : Lobby에서 캐릭터 선택창 등
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

	// 중복 처리 체크
	const uint64 ObjectId = ObjectInfo.object_id();
	//if (UJ1ObjectManager::Get().GetActorById(ObjectId) != nullptr)
	if (GetManager(Object)->GetActorById(ObjectId) != nullptr)
		return;

	FVector SpawnLocation(ObjectInfo.pos_info().x(), ObjectInfo.pos_info().y(), ObjectInfo.pos_info().z());

	if (IsMine) // MyPlayer일 경우
	{
		auto* PC = UGameplayStatics::GetPlayerController(this, 0);
		auto* abc = PC->GetPawn();
		AJ1MyPlayer* MyPlayer = Cast<AJ1MyPlayer>(PC->GetPawn());
		if (MyPlayer == nullptr)
			return;

		MyPlayer->SetInfo(ObjectInfo);
		GetManager(Object)->MyPlayer = MyPlayer;
		GetManager(Object)->AddActorToMap(PC->GetPawn(), ObjectId);
	}
	else // 내가 아닌 Player일 경우
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
	TObjectPtr<AJ1Creature> FindActor = GetManager(Object)->GetCreatureById(ObjectId);
	if (FindActor == nullptr)
		return;

	const Protocol::PosInfo& Info = MovePkt.info();
	if (Info.state() == Protocol::MoveState::MOVE_STATE_DEAD)
	{
		FindActor->SetMoveState(Info.state());
		FindActor->OnDead();
	}
	else
	{
		FindActor->ProcessMove(Info);
	}
}

void UJ1NetworkManager::HandleNotifyPos(const Protocol::S_NOTIFY_POS& NotifyPosPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = NotifyPosPkt.info().object_id();
	TObjectPtr<AJ1Creature> FindActor = GetManager(Object)->GetCreatureById(ObjectId);
	if (FindActor == nullptr)
		return;

	const Protocol::PosInfo& Info = NotifyPosPkt.info();
	FindActor->ProcessNotifyPos(Info);
}

void UJ1NetworkManager::HandleSkill(const Protocol::S_SKILL& SkillPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = SkillPkt.object_id();
	TObjectPtr<AJ1Creature> FindActor = GetManager(Object)->GetCreatureById(ObjectId);
	if (FindActor == nullptr)
		return;

	const Protocol::SkillSlot& slot = SkillPkt.slot();
	FindActor->ProcessSkill(SkillPkt);
}

void UJ1NetworkManager::HandleGimmick(const Protocol::S_GIMMICK& GimmickPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = GimmickPkt.object_id();
	TObjectPtr<AJ1Creature> FindActor = GetManager(Object)->GetCreatureById(ObjectId);
	if (FindActor == nullptr)
		return;

	TObjectPtr<AJ1Boss> boss = Cast<AJ1Boss>(FindActor);
	if (boss == nullptr)
		return;
	
	boss->ProcessGimmick(GimmickPkt);
}

void UJ1NetworkManager::HandleProjectile(const Protocol::S_PROJECTILE& ProjectilePkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const Protocol::ProjectileInfo projInfo = ProjectilePkt.projectile_info();
	TObjectPtr<AJ1Creature> FindActor = GetManager(Object)->GetCreatureById(projInfo.owner_object_id());
	if (FindActor == nullptr)
		return;

	FindActor->ProcessProjectile(projInfo);
}

void UJ1NetworkManager::HandleStat(const Protocol::S_STAT& StatPkt)
{
	if (Socket == nullptr || GameServerSession == nullptr)
		return;

	auto* World = GetWorld();
	if (World == nullptr)
		return;

	const uint64 ObjectId = StatPkt.object_id();
	TObjectPtr<AJ1Creature> creature = GetManager(Object)->GetCreatureById(ObjectId);
	if (creature == nullptr)
		return;

	TObjectPtr<UJ1StatComponent> stats = creature->StatComponent;
	stats->ProcessStat(StatPkt.stat_info());
}
