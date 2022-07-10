#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TempClass.generated.h"

UCLASS()
class ATempClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATempClass();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = TMesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TempMesh;

private:
	UPROPERTY()
	class UJsonComponent* Comp;
};
