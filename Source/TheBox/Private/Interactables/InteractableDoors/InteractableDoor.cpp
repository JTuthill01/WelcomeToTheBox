#include "Interactables/InteractableDoors/InteractableDoor.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AInteractableDoor::AInteractableDoor() : bIsOpen(false), bIsReady(true), RotateValue(1.F), CurveFloatValue(0.F), TimelineValue(0.F), DoorRotation(FRotator(0.F))
{
	PrimaryActorTick.bCanEverTick = true;

	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Doorframe Mesh"));
	DoorFrameMesh->SetupAttachment(BaseRootComponent);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	DoorMesh->SetupAttachment(DoorFrameMesh);

	WidgetText = FText::FromString(TEXT("Press E To Open"));
}

void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();

	InitializeDoor();
}

void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DoorTimeline.TickTimeline(DeltaTime);
}

void AInteractableDoor::ControlDoor()
{
	TimelineValue = DoorTimeline.GetPlaybackPosition();

	CurveFloatValue = RotateValue * DoorCurve->GetFloatValue(TimelineValue);

	FQuat NewRotation = FQuat(FRotator(0.F, CurveFloatValue, 0.F));
	
	DoorMesh->SetRelativeRotation(NewRotation);
}

void AInteractableDoor::ToggleDoor()
{
	if (bIsReady)
	{
		bIsOpen = !bIsOpen;

		FVector PawnLocation = PlayerRef->GetActorLocation();
		FVector Direction = GetActorLocation() - PawnLocation;

		Direction = UKismetMathLibrary::LessLess_VectorRotator(Direction, GetActorRotation());

		DoorRotation = DoorMesh->GetRelativeRotation();

		if (bIsOpen)
		{
			if (Direction.X > 0.F)
				RotateValue = 1.F;

			else
				RotateValue = -1.F;

			bIsReady = false;

			DoorTimeline.PlayFromStart();
		}

		else
		{
			bIsReady = false;

			DoorTimeline.Reverse();
		}
		
	}
}

void AInteractableDoor::InitializeDoor()
{
	if (DoorCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("ControlDoor"));

		TimelineFinishedCallback.BindUFunction(this, FName("SetState"));

		DoorTimeline.AddInterpFloat(DoorCurve, TimelineCallback);
		DoorTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void AInteractableDoor::InteractableFound_Implementation()
{
	Super::InteractableFound_Implementation();
}

void AInteractableDoor::InteractWithObject_Implementation()
{
	Super::InteractWithObject_Implementation();

	ToggleDoor();
}

void AInteractableDoor::SetState() { bIsReady = true; }
