// Copyright 2020 Dan Kestranek.


#include "Actor/Router/GRouter.h"

#include "Components/SplineComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AGRouter::AGRouter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("SceneComponent"));
	SceneComponent->SetVisibility(false, false);
	SetRootComponent(SceneComponent);
}

USplineComponent* AGRouter::GetSplineComponent() const
{
	return SplineComponent.Get();
}


// Called when the game starts or when spawned
void AGRouter::BeginPlay()
{
	Super::BeginPlay();
}