#include "Data/J1InputData.h"
#include "J1LogChannels.h"

const UInputAction* UJ1InputData::FindInputActionByTag(const FGameplayTag& InputTag) const
{
	for (const FJ1InputAction& Action : InputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	UE_LOG(LogJ1, Error, TEXT("Can't find InputAction for InputTag [%s]"), *InputTag.ToString());

	return nullptr;
}

UNiagaraSystem* UJ1InputData::GetFXCursor()
{
	return FXCursor;
}
