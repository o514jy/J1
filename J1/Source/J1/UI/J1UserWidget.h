#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Blueprint/UserWidget.h"
#include "J1UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class J1_API UJ1UserWidget : public UUserWidget
{
	GENERATED_BODY()
public:


public:
	/** information **/
	EUIType UIType = EUIType::None;
};
