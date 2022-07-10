#include "Json/Temp/TempClass.h"
#include "Components/JsonComponent/JsonComponent.h"
#include "UObject/UObjectGlobals.h"

// Sets default values
ATempClass::ATempClass()
{
	TempMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Temp"));
	
	Comp = CreateDefaultSubobject<UJsonComponent>(TEXT("Comp"));
}

void ATempClass::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	
}

// Called when the game starts or when spawned
void ATempClass::BeginPlay()
{
	Super::BeginPlay();
	
	Comp->Parser();

	//GEngine->AddOnScreenDebugMessage(-1, 8.F, FColor::Orange, Comp->InString);

	UStaticMesh* I = LoadObject<UStaticMesh>(this, *Comp->InString);

	TempMesh->SetStaticMesh(I);

	if (IsValid(I))
		GEngine->AddOnScreenDebugMessage(-1, 8.F, FColor::Orange, TEXT("SUCCESS!!!"));

	else
		GEngine->AddOnScreenDebugMessage(-1, 8.F, FColor::Black, TEXT("MOTHERFUCKER!!!"));
}

