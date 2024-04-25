#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "J1GameInstance.generated.h"

class AJ1Player;
class AJ1MyPlayer;
class AJ1Boss;

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
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Player> OtherPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1MyPlayer> MyPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Boss> StartBossClass;

};
