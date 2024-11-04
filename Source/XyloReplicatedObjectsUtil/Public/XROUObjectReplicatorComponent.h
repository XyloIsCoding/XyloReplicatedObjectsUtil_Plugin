// Copyright XyloIsCoding 2024

#pragma once

#include "CoreMinimal.h"
#include "XROUObjectReplicatorComponent.generated.h"

class UXROUReplicatedObject;
/**
 * 
 */
UCLASS()
class XYLOREPLICATEDOBJECTSUTIL_API UXROUObjectReplicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UXROUObjectReplicatorComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	 * SubObjects Replication
	 */
	
public:
	/** Register a Replicated UObjects to replicate
	 * @param ObjectToRegister The Replicated UObject to register. */
	virtual bool RegisterReplicatedObject(UXROUReplicatedObject* ObjectToRegister);

	/** Unregister a Replicated UObject from replication
	 * @param ObjectToUnregister The Replicated UObject to unregister.
	 * @param bDestroyObject Whether the DestroyObject should be called
	 */
	virtual bool UnregisterReplicatedObject(UXROUReplicatedObject* ObjectToUnregister, const bool bDestroyObject = false);


public:
	//Get all the replicated objects that are registered on this component.
	virtual TArray<UXROUReplicatedObject*> GetRegisteredReplicatedObjects() { return ReplicatedObjects; }
private:
	//All the currently replicated objects
	UPROPERTY()
	TArray<UXROUReplicatedObject*> ReplicatedObjects;
	
	
};
