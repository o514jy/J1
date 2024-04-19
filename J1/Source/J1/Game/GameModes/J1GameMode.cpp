#include "J1GameMode.h"
#include "J1/System/J1GameInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(J1GameMode)

AJ1GameMode::AJ1GameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void AJ1GameMode::BeginPlay()
{
	Super::BeginPlay();

	Cast<UJ1GameInstance>(GetGameInstance())->BeginPlay();
}
