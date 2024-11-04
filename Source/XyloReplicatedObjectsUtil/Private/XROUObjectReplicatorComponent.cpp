// Copyright XyloIsCoding 2024


#include "XROUObjectReplicatorComponent.h"

#include "XROUReplicatedObject.h"

UXROUObjectReplicatorComponent::UXROUObjectReplicatorComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * SubObjects Replication
 */

bool UXROUObjectReplicatorComponent::RegisterReplicatedObject(UXROUReplicatedObject* ObjectToRegister)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(ObjectToRegister))
	{
		ReplicatedObjects.AddUnique(ObjectToRegister);
		AddReplicatedSubObject(ObjectToRegister);
		return true;
	}
	return false;
}

bool UXROUObjectReplicatorComponent::UnregisterReplicatedObject(UXROUReplicatedObject* ObjectToUnregister, const bool bDestroyObject)
{
	if (IsUsingRegisteredSubObjectList() && IsValid(ObjectToUnregister))
	{
		ReplicatedObjects.Remove(ObjectToUnregister);
		RemoveReplicatedSubObject(ObjectToUnregister);
		if (bDestroyObject && GetOwner() && GetOwner()->HasAuthority()) ObjectToUnregister->DestroyObject();
		return true;
	}
	return false;
}