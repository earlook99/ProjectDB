#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "PDBInputConfig.h"
#include "PDBInputComponent.generated.h"

UCLASS()
class PROJECTDB_API UPDBInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename PressedFuncType, typename HeldFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UPDBInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, HeldFuncType HeldFunc, ReleasedFuncType ReleasedFunc)
	{
		check(InputConfig);
		
		for (const FPDBInputAction& Action : InputConfig->AbilityInputActions)
		{
			if (Action.InputAction && Action.InputTag.IsValid())
			{
				if (PressedFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
				}
				if (HeldFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
				}
				if (ReleasedFunc)
				{
					BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
				}
			}
		}
	}
};
