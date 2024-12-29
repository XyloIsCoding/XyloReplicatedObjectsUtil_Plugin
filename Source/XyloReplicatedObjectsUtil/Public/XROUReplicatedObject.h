// Copyright XyloIsCoding 2024

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Interface_ActorSubobject.h"
#include "XROUReplicatedObject.generated.h"


UCLASS()
class XYLOREPLICATEDOBJECTSUTIL_API UXROUReplicatedObject : public UObject, public IInterface_ActorSubobject
{
	GENERATED_BODY()

public:	
	UXROUReplicatedObject(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	//Gets the Actor that "owns" this Replicated UObject.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Replicated UObject")
	AActor* GetOwningActor() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Replicated UObject")
	virtual bool HasAuthority() const;

	ENetRole GetOwnerRole() const;

public:
	//Will mark this UObject as garbage and will eventually get cleaned by the garbage collector.
	//Should only execute this on the server.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Replicated UObject", DisplayName = "Destroy Replicated UObject")
	virtual void DestroyObject();
protected:
	//Will get called on the server once the Replicated UObject has been marked as garbage.
	UFUNCTION(BlueprintImplementableEvent, Category = "Replicated UObject", DisplayName = "On Replicated UObject Destroyed")
	void OnDestroyed();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	 * UObject Interface
	 */
	
public:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UWorld* GetWorld() const override;
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	 * IInterface_ActorSubobject Interface
	 */

public:
	virtual void OnCreatedFromReplication() override;
	virtual void OnDestroyedFromReplication() override;
	
		
};
