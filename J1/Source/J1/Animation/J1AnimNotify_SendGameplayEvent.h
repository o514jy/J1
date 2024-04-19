#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "J1AnimNotify_SendGameplayEvent.generated.h"


UCLASS()
class J1_API UJ1AnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()
public:
	UJ1AnimNotify_SendGameplayEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	~UJ1AnimNotify_SendGameplayEvent();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;

};
