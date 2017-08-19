// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/PackageNameGetter.h"
#include "Misc/PackageName.h"

// Sets default values for this component's properties
UPackageNameGetter::UPackageNameGetter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPackageNameGetter::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPackageNameGetter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FString UPackageNameGetter::GetLongPackageName(const FString & shortName)
{
	FString string;
	FPackageName::SearchForPackageOnDisk(shortName, &string);
	return string;
}

ULevelStreamingKismet* UPackageNameGetter::LoadLevelInstance(UObject* WorldContextObject, const FString& LevelName, const FVector& Location, const FRotator& Rotation, bool& bOutSuccess)
{
	bOutSuccess = false;
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, false);
	if (!World)
	{
		return nullptr;
	}

	// Check whether requested map exists, this could be very slow if LevelName is a short package name
	FString LongPackageName;
	bOutSuccess = FPackageName::SearchForPackageOnDisk(LevelName, &LongPackageName);
	if (!bOutSuccess)
	{
		return nullptr;
	}

	// Create Unique Name for sub-level package
	const FString ShortPackageName = FPackageName::GetShortName(LongPackageName);
	const FString PackagePath = FPackageName::GetLongPackagePath(LongPackageName);
	FString UniqueLevelPackageName = PackagePath + TEXT("/") + World->StreamingLevelsPrefix + ShortPackageName;
	UniqueLevelPackageName += TEXT("_LevelInstance_") + FString::FromInt(++UniqueLevelInstanceId);

	// Setup streaming level object that will load specified map
	ULevelStreamingKismet* StreamingLevel = NewObject<ULevelStreamingKismet>(World, ULevelStreamingKismet::StaticClass(), NAME_None, RF_Transient, NULL);
	StreamingLevel->SetWorldAssetByPackageName(FName(*UniqueLevelPackageName));
	StreamingLevel->LevelColor = FColor::MakeRandomColor();
	StreamingLevel->bShouldBeLoaded = false;
	StreamingLevel->bShouldBeVisible = false;
	StreamingLevel->bShouldBlockOnLoad = false;
	StreamingLevel->bInitiallyLoaded = false;
	StreamingLevel->bInitiallyVisible = false;
	// Transform
	StreamingLevel->LevelTransform = FTransform(Rotation, Location);
	// Map to Load
	StreamingLevel->PackageNameToLoad = FName(*LongPackageName);

	// Add the new level to world.
	World->StreamingLevels.Add(StreamingLevel);

	bOutSuccess = true;
	return StreamingLevel;
}



