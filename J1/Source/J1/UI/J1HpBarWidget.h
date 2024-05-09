#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "J1HpBarWidget.generated.h"

class UProgressBar;

UCLASS()
class J1_API UJ1HpBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UJ1HpBarWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~UJ1HpBarWidget();

public:
	virtual void NativeConstruct() override;
public:
	void SetHpRatio(float InRatio);

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UProgressBar> HpBar;
};
