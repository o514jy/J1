#include "J1CreatureController.h"
#include "J1/Network/J1NetworkManager.h"
#include "J1/Game/Object/J1Creature.h"
#include "Navigation/PathFollowingComponent.h"

AJ1CreatureController::AJ1CreatureController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

AJ1CreatureController::~AJ1CreatureController()
{
}

void AJ1CreatureController::ProcessMove(const Protocol::PosInfo& posInfo)
{
	Super::ProcessMove(posInfo);

	if (GetPawn() != nullptr && GetWorld()->GetNavigationSystem())
	{
		TObjectPtr<AJ1Creature> player = Cast<AJ1Creature>(GetPawn());
		player->SetPosInfo(posInfo);
	
		FVector location;
		location.X = posInfo.dest_x();
		location.Y = posInfo.dest_y();
		location.Z = posInfo.dest_z();
	
		// �̵� ��� ����
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(location);
		MoveRequest.SetAcceptanceRadius(100.f);
	
		// AI���� �̵� ����� ����
		MoveTo(MoveRequest);
	}
}



//void AJ1CreatureController::RegisterNotifyPos()
//{
//	Super::RegisterNotifyPos();
//
//	Protocol::C_NOTIFY_POS notifyPkt;
//	{
//		Protocol::PosInfo* posInfo = new Protocol::PosInfo();
//		TObjectPtr<AJ1Creature> creature = Cast<AJ1Creature>(creature);
//		posInfo->CopyFrom(*creature->GetPosInfo());
//		notifyPkt.set_allocated_info(posInfo);
//	}
//	GetManager(Network)->SendPacket(notifyPkt);
//}

void AJ1CreatureController::ProcessNotifyPos(const Protocol::PosInfo& posInfo)
{
	Super::ProcessNotifyPos(posInfo);

	// Ŭ��� ���̰� ���ϸ� �����̵����� ����
	FVector ServerLocation;
	ServerLocation.X = posInfo.x();
	ServerLocation.Y = posInfo.y();
	ServerLocation.Z = posInfo.z();

	if (FVector::Dist(GetPawn()->GetActorLocation(), ServerLocation) >= 100.f)
	{
		if (GetPawn() != nullptr && GetWorld()->GetNavigationSystem())
		{
			Cast<AJ1Creature>(GetPawn())->SetPosInfo(posInfo, true);

			StopMovement();
			// �ٽ� �̵� ��� ����
			FVector location;
			location.X = posInfo.dest_x();
			location.Y = posInfo.dest_y();
			location.Z = posInfo.dest_z();

			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalLocation(location);
			MoveRequest.SetAcceptanceRadius(50.f);

			// AI���� �̵� ����� ����
			MoveTo(MoveRequest);
		}
	}
}

void AJ1CreatureController::Tick(float DeltaTime)
{
	auto a = GetPawn();
	TObjectPtr<AJ1Creature> owner = Cast<AJ1Creature>(GetPawn());

	if (owner == nullptr)
		return;

	if (owner->GetMoveState() == Protocol::MoveState::MOVE_STATE_IDLE)
	{
		UpdateIdle();
	}
	else if (owner->GetMoveState() == Protocol::MoveState::MOVE_STATE_RUN)
	{
		UpdateRun();
	}
	else if (owner->GetMoveState() == Protocol::MoveState::MOVE_STATE_SKILL)
	{
		UpdateSkill();
	}
	else if (owner->GetMoveState() == Protocol::MoveState::MOVE_STATE_DEAD)
	{
		UpdateDead();
	}
}

void AJ1CreatureController::UpdateIdle()
{
}

void AJ1CreatureController::UpdateRun()
{
}

void AJ1CreatureController::UpdateSkill()
{
}

void AJ1CreatureController::UpdateDead()
{
}
