// Copyright XyloIsCoding 2024


#include "XROUReplicatedObject.h"

UXROUReplicatedObject::UXROUReplicatedObject(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

AActor* UXROUReplicatedObject::GetOwningActor() const
{
	return GetTypedOuter<AActor>();
}

ENetRole UXROUReplicatedObject::GetOwnerRole() const
{
	const AActor* MyOwner = GetOwningActor();
	return (MyOwner ? MyOwner->GetLocalRole() : ROLE_None);
}

bool UXROUReplicatedObject::HasAuthority() const
{
	return GetOwningActor() && GetOwningActor()->HasAuthority();
}

bool UXROUReplicatedObject::IsNetSimulating() const
{
	return GetIsReplicated() && GetOwnerRole() != ROLE_Authority;
}

void UXROUReplicatedObject::DestroyObject()
{
	if (bIsBeingDestroyed)
    {
    	return;
    }
    	
	if (IsValid(this))
	{
		bIsBeingDestroyed = true;
		OnDestroyed();
		K2_OnDestroyed();
		
		MarkAsGarbage();
	}
}

void UXROUReplicatedObject::OnDestroyed()
{
	bHasBeenCreated = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * UObject Interface
 */

void UXROUReplicatedObject::BeginDestroy()
{
	if (bHasBeenCreated)
	{
		OnDestroyed();
	}
	
	Super::BeginDestroy();
}

void UXROUReplicatedObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	if (const UBlueprintGeneratedClass* BPCClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPCClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

UWorld* UXROUReplicatedObject::GetWorld() const
{
	if (GetOuter() == nullptr)
	{
		return nullptr;
	}
		
	if (Cast<UPackage>(GetOuter()) != nullptr)
	{
		return Cast<UWorld>(GetOuter()->GetOuter());
	}
		
	return GetOwningActor()->GetWorld();
}

int32 UXROUReplicatedObject::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	if (HasAnyFlags(RF_ClassDefaultObject) || !IsSupportedForNetworking())
	{
		return GEngine->GetGlobalFunctionCallspace(Function, this, Stack);
	}
	
	return GetOuter()->GetFunctionCallspace(Function, Stack);
}

bool UXROUReplicatedObject::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	AActor* Owner = GetOwningActor();
	if (UNetDriver* NetDriver = Owner->GetNetDriver())
	{
		NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * IInterface_ActorSubobject Interface
 */

void UXROUReplicatedObject::OnCreatedFromReplication()
{
}

void UXROUReplicatedObject::OnDestroyedFromReplication()
{
	if (bHasBeenCreated)
	{
		OnDestroyed();
	}
}