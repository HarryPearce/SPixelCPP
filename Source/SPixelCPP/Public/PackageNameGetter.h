// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/LevelStreamingKismet.h"
#include "EngineGlobals.h"
#include "PackageNameGetter.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPIXELCPP_API UPackageNameGetter : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPackageNameGetter();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	FString GetLongPackageName(const FString& shortName);
	UFUNCTION(BlueprintCallable)
	ULevelStreamingKismet* LoadLevelInstance(UObject* WorldContextObject, const FString& LevelName, const FVector& Location, const FRotator& Rotation, bool& bOutSuccess);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int UniqueLevelInstanceId;
};
