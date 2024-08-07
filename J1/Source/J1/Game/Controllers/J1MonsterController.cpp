#include "Game/Controllers/J1MonsterController.h"
#include "Game/Object/J1Monster.h"
#include "Game/Object/J1Player.h"
#include "Navigation/PathFollowingComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

AJ1MonsterController::AJ1MonsterController(const FObjectInitializer& ObjectInitializer)
	//: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
	: Super(ObjectInitializer)
{
	
}

AJ1MonsterController::~AJ1MonsterController()
{
}

void AJ1MonsterController::BeginPlay()
{
	Super::BeginPlay();

	UCrowdFollowingComponent* comp = FindComponentByClass<UCrowdFollowingComponent>();

	if (comp)
	{
		comp->SetCrowdSeparation(true);
		comp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
	}
}

void AJ1MonsterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJ1MonsterController::UpdateIdle()
{
	Super::UpdateIdle();

	if (GetOwner() == nullptr)
		return;

	if (GetOwner()->GetTargetPlayer() == nullptr)
		return;

	TObjectPtr<AJ1Player> targetPlayer = GetOwner()->GetTargetPlayer();
	if (targetPlayer != nullptr)
		GetOwner()->SetMoveState(Protocol::MoveState::MOVE_STATE_RUN);
}

void AJ1MonsterController::UpdateRun()
{
	Super::UpdateRun();

	return;

	if (GetOwner() == nullptr)
		return;

	if (GetOwner()->GetTargetPlayer() == nullptr)
		return;

	// Move to Target
	if (GetWorld()->GetNavigationSystem() == nullptr)
		return;

	TObjectPtr<AJ1Player> player = GetOwner()->GetTargetPlayer();

	//FVector location;
	//location.X = player->GetObjectInfo()->pos_info().x();
	//location.Y = player->GetObjectInfo()->pos_info().y();
	//location.Z = player->GetObjectInfo()->pos_info().z();

	// �������� �ؾ��ϴ� ��� �̰Ÿ���
	//if (GetOwner()->bSetSyncPos == true)
	//{
	//	// �����ؾ��� ��ġ�� �������� ��� ���� ��
	//	if (FVector::Dist(GetOwner()->GetActorLocation(), GetOwner()->GetTempSyncPos()) < 15.f)
	//	{
	//		GetOwner()->SetbSetSyncPos(false);
	//	}
	//	else
	//	{
	//		FAIMoveRequest MoveReq;
	//		MoveReq.SetGoalLocation(GetOwner()->GetTempSyncPos());
	//		MoveReq.SetAcceptanceRadius(0.f);
	//		MoveTo(MoveReq);
	//		return;
	//	}
	//}

	// �̵� ��� ����
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(player);
	float OwnerAttackDist = GetOwner()->GetMonsterData()->DefaultAtkRange;
	// OwnerAttackDist + player->GetCreatureData()->ColliderRadius
	MoveRequest.SetAcceptanceRadius(OwnerAttackDist - player->GetCreatureData()->ColliderRadius);

	// AI���� �̵� ����� ����
	MoveTo(MoveRequest);
	
}

void AJ1MonsterController::UpdateSkill()
{
	Super::UpdateSkill();
}

void AJ1MonsterController::UpdateDead()
{
	Super::UpdateDead();
}
