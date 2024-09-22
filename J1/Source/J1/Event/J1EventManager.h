#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "J1EventManager.generated.h"



UCLASS()
class J1_API UJ1EventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	void OnBroadcastEvent(EEventType InType);

public:
	void OnToggleInventory();
	
public:
	/** information **/
};
