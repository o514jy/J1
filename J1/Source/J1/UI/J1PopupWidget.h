#pragma once

#include "CoreMinimal.h"
#include "UI/J1UserWidget.h"
#include "J1PopupWidget.generated.h"

/**
 * 
 */
UCLASS()
class J1_API UJ1PopupWidget : public UJ1UserWidget
{
	GENERATED_BODY()
public:
	void SetbIsOpened(bool InFlag);
	bool GetbIsOpened();

	void ClosePopupWidget();

public:
	/** information **/
	bool bIsOpened = false;

	FVector2D defaultOpenPos = FVector2D();
};
