// Ninja Bear Studio Inc., all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ApplyLooseGameplayTags.generated.h"

class UAbilitySystemComponent;

/**
 * Adds and removes loose gameplay tags.
 */
UCLASS(meta = (DisplayName = "Apply Loose Gameplay Tags"))
class NINJAGAS_API UAnimNotifyState_ApplyLooseGameplayTags : public UAnimNotifyState
{
	
	GENERATED_BODY()
	
public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTagContainer GameplayTags;
	
	void RemoveGameplayTags(const USkeletalMeshComponent* MeshComp);
	
private:

	struct FAppliedLooseGameplayTagsInfo
	{
		TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;
		FGameplayTagContainer Tags;
	};
	
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, FAppliedLooseGameplayTagsInfo> ActiveGameplayTags;
	
};
