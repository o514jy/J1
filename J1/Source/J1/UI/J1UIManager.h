#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "J1UIManager.generated.h"

class UUserWidget;

UCLASS()
class J1_API UJ1UIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/** UI **/
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DungeonStatusWidgetClass;
};
