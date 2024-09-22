#pragma once

#include "CoreMinimal.h"
#include "UI/J1UserWidget.h"
#include "J1SceneWidget.generated.h"

class UJ1DungeonStatusWidget;

UCLASS()
class J1_API UJ1SceneWidget : public UJ1UserWidget
{
	GENERATED_BODY()
public:


	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UJ1DungeonStatusWidget> DungeonStatusWBP;

};
