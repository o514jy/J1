#pragma once

#include "CoreMinimal.h"
#include "UI/J1UserWidget.h"
#include "J1PopupPosWidget.generated.h"

class UTextBlock;

UCLASS()
class J1_API UJ1PopupPosWidget : public UJ1UserWidget
{
	GENERATED_BODY()
public:
	/** helper **/
	FVector2D GetPopupWidgetPos(EUIType InType);

public:
	/** information **/
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	TObjectPtr<UTextBlock> InventoryPosText;


};
