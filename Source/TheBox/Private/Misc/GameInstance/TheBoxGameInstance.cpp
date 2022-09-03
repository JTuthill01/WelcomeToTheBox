#include "Misc/GameInstance/TheBoxGameInstance.h"
#include "JsonComponents/BPLoader/BPLoaderComponent.h"

UTheBoxGameInstance::UTheBoxGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
	Loader = CreateDefaultSubobject<UBPLoaderComponent>(TEXT("Loader"));
}

void UTheBoxGameInstance::LoadOnSpawn(TObjectPtr<UClass>& LoadedBpAsset, FString Path)
{
	if (IsValid(Loader))
	{
		ReturnPath = Loader->LoadParser(Path);

		ActorBpClass = TSoftClassPtr<AActor>(FSoftObjectPath(ReturnPath));

		LoadedBpAsset = ActorBpClass.LoadSynchronous();
	}

	else
		GEngine->AddOnScreenDebugMessage(-1, 6.F, FColor::Yellow, L"FAILED");
}

void UTheBoxGameInstance::Init()
{
	Super::Init();
}

