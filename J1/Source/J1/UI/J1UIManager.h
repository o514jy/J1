#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "J1UIManager.generated.h"

class UUserWidget;
class UJ1UserWidget;
class UJ1PopupPosWidget;
/* popup */
class UJ1PopupWidget;
class UJ1InventoryWidget;
/* scene */
class UJ1SceneWidget;
class UJ1StartRoomSceneWidget;

UCLASS()
class J1_API UJ1UIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/** initialize **/
	void SetInfo();

public:
	/** process **/
	void CacheAllPopups();

	TObjectPtr<UJ1PopupWidget> CreatePopupWidget(EUIType InType);

	TObjectPtr<UJ1PopupWidget> ShowPopupWidget(EUIType InType);
	TObjectPtr<UJ1SceneWidget> ShowSceneWidget(EUIType InType);

	TObjectPtr<UJ1UserWidget> CreateWidgetExt(EUIType InType);

	TObjectPtr<UJ1PopupWidget> GetPopupWidget(EUIType InType);

	bool ClosePopupWidget(EUIType InType);

public:
	/** helper **/
	TSubclassOf<UJ1UserWidget> GetWidgetClass(EUIType InType);

public:
	/** information **/
	UPROPERTY()
	TObjectPtr<UJ1PopupPosWidget> PopupPosWidgetInstance;

	/* now opened */
	UPROPERTY()
	TArray<TObjectPtr<UJ1PopupWidget>> PopupStack;

	/* cache */
	UPROPERTY()
	TMap<EUIType, TObjectPtr<UJ1PopupWidget>> Popups;

	/* Scene UI*/
	UPROPERTY()
	TObjectPtr<UJ1SceneWidget> SceneWidget;
};
