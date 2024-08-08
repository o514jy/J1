#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "J1DungeonStatusWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class J1_API UJ1DungeonStatusWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UJ1DungeonStatusWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~UJ1DungeonStatusWidget();

public:
	virtual void NativeConstruct() override;

public:
	void SetClearRatio(float InRatio);

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UProgressBar> ClearRatioProgressBar;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> ClearRatioText;
};
