// Ninja Bear Studio Inc., all rights reserved.
#include "AI/StateTree/StateTreeCancelGameplayAbilityTask.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "StateTreeExecutionContext.h"
#include "VisualLogger/VisualLogger.h"

EStateTreeRunStatus FStateTreeCancelGameplayAbilityTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	UAbilitySystemComponent* AbilityComponent = GetAbilitySystemComponent(Context);
	if (!AbilityComponent)
	{
		UE_VLOG(Context.GetOwner(), LogStateTree, Error, TEXT("FStateTreeCancelGameplayAbilityTask failed since Pawn does not have an ASC."));
		return EStateTreeRunStatus::Failed;
	}

	return CancelAbilities(Context, AbilityComponent);
}

UAbilitySystemComponent* FStateTreeCancelGameplayAbilityTask::GetAbilitySystemComponent(const FStateTreeExecutionContext& Context)
{
	const AActor* OwnerActor = Cast<AActor>(Context.GetOwner());
	if (!IsValid(OwnerActor))
	{
		return nullptr;
	}

	if (OwnerActor->IsA<AAIController>())
	{
		const APawn* OwnerPawn = Cast<AAIController>(OwnerActor)->GetPawn();
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerPawn);
	}

	// Simply try to obtain the ASC directly from the actor (probably a pawn/character).
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);	
}

EStateTreeRunStatus FStateTreeCancelGameplayAbilityTask::CancelAbilities(const FStateTreeExecutionContext& Context, UAbilitySystemComponent* AbilityComponent) const
{
	if (!IsValid(AbilityComponent))
	{
		UE_VLOG(Context.GetOwner(), LogStateTree, Error, TEXT("FStateTreeCancelGameplayAbilityTask failed since an invalid ASC was received."));
		return EStateTreeRunStatus::Failed;
	}

	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	const FGameplayTagContainer& CancelAbilitiesWithTags = InstanceData.CancelAbilityWithTags;
	const FGameplayTagContainer& CancelAbilitiesWithoutTags = InstanceData.CancelAbilityWithoutTags;

	AbilityComponent->CancelAbilities(
		CancelAbilitiesWithTags.IsValid() ? &CancelAbilitiesWithTags : nullptr,
		CancelAbilitiesWithoutTags.IsValid() ? &CancelAbilitiesWithoutTags : nullptr
	);

	return EStateTreeRunStatus::Succeeded;
}
