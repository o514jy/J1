#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "UI/J1PopupWidget.h"
#include "J1DungeonClearWidget.generated.h"

class UButton;

UCLASS()
class J1_API UJ1DungeonClearWidget : public UJ1PopupWidget
{
	GENERATED_BODY()
public:
	UJ1DungeonClearWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~UJ1DungeonClearWidget();

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnClickedQuitButton();

public:
	/** information **/
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UButton> QuitButton;
};
