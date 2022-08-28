#include "Widgets/Reload/ReloadWidget.h"

UReloadWidget::UReloadWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UReloadWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetDesiredSizeInViewport(FVector2D(400.F, 400.F));

	ScreenSize = FVector2D(GetScreenSize().X / 2, GetScreenSize().Y / 2);

	SetPositionInViewport(ScreenSize);
}

FVector2D UReloadWidget::GetScreenSize()
{
	FVector2D Result = FVector2D(0.F);

	if (GEngine && GEngine->GameViewport)
		GEngine->GameViewport->GetViewportSize(Result);

	return Result;
}
