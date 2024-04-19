#include "J1AnimNotify_SendGameplayEvent.h"
#include "J1/J1GameplayTags.h"
#include "J1/Game/Skill/J1SkillComponent.h"
#include "J1/Game/Object/J1Creature.h"

UJ1AnimNotify_SendGameplayEvent::UJ1AnimNotify_SendGameplayEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UJ1AnimNotify_SendGameplayEvent::~UJ1AnimNotify_SendGameplayEvent()
{
}

void UJ1AnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	TObjectPtr<AJ1Creature> creature = Cast<AJ1Creature>(MeshComp->GetOwner());
	if (creature)
	{
		creature->HandleGameplayEvent(EventTag);
	}
}
