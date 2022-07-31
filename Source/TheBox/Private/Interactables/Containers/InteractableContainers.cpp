#include "Interactables/Containers/InteractableContainers.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/Player/PlayerCharacter.h"

AInteractableContainers::AInteractableContainers() : bIsOpen(false), bIsReady(true), RotateValue(1.F), CurveFloatValue(0.F), TimelineValue(0.F), DoorRotation(FRotator(0.F))
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(BaseRootComponent);

	SubMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sub Mesh"));
	SubMesh->SetupAttachment(BaseRootComponent);

	DoorTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	WidgetText = FText::FromString(TEXT("Deez Nuts"));
}

void AInteractableContainers::BeginPlay()
{
	Super::BeginPlay();

	InitializeDoor();
}

void AInteractableContainers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoorTimelineComponent->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
}

void AInteractableContainers::InteractableFound_Implementation()
{
	Super::InteractableFound_Implementation();
}

void AInteractableContainers::InteractWithObject_Implementation()
{
	Super::InteractWithObject_Implementation();
}

void AInteractableContainers::ControlDoor()
{
	TimelineValue = DoorTimelineComponent->GetPlaybackPosition();

	CurveFloatValue = RotateValue * DoorCurve->GetFloatValue(TimelineValue);

	FQuat NewRotation = FQuat(FRotator(0.F, CurveFloatValue, 0.F));

	SubMesh->SetRelativeRotation(NewRotation);
}

void AInteractableContainers::ToggleDoor()
{
	if (bIsReady)
	{
		bIsOpen = !bIsOpen;

		FVector PawnLocation = PlayerRef->GetActorLocation();
		FVector Direction = GetActorLocation() - PawnLocation;

		Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, GetActorRotation());

		DoorRotation = SubMesh->GetRelativeRotation();

		if (bIsOpen)
		{
			if (Direction.X > 0.F)
				RotateValue = 1.F;

			else
				RotateValue = -1.F;

			bIsReady = false;

			DoorTimelineComponent->PlayFromStart();
		}

		else
		{
			bIsReady = false;

			DoorTimelineComponent->Reverse();
		}

	}
}

void AInteractableContainers::InitializeDoor()
{
	if (DoorCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("ControlDoor"));

		TimelineFinishedCallback.BindUFunction(this, FName("SetState"));

		DoorTimelineComponent->AddInterpFloat(DoorCurve, TimelineCallback);
		DoorTimelineComponent->SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void AInteractableContainers::SetState() { bIsReady = true; }
