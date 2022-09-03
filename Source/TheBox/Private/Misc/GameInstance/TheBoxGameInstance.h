#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TheBoxGameInstance.generated.h"

UCLASS()
class UTheBoxGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTheBoxGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	void LoadOnSpawn(TObjectPtr<UClass>& LoadedBpAsset, FString Path);

protected:
	void Init() override;

public:
	FString ReturnPath;

private:
	UPROPERTY()
	TObjectPtr<class UBPLoaderComponent> Loader;

	UPROPERTY()
	TSoftClassPtr<AActor> ActorBpClass;
};
