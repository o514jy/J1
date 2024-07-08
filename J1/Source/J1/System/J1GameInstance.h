#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "J1GameInstance.generated.h"

class AJ1Player;
class AJ1MyPlayer;
class AJ1Monster;
class AJ1Boss;
class AJ1Portal;

class UNiagaraSystem;

UCLASS()
class J1_API UJ1GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UJ1GameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void Init() override;
	virtual void Shutdown() override;

public:
	virtual void BeginPlay();

public:
	/** creature **/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Player> OtherPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1MyPlayer> MyPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Monster> GoblinSpearClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Boss> StartBossClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ShieldMaster;

	/** env **/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Portal> PortalClass;
};
