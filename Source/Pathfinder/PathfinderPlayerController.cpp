// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Pathfinder.h"
#include "PathfinderPlayerController.h"
#include "Hexagon.h"
#include <unordered_map>
#include "EngineUtils.h"
#include "PathfinderMovementComponent.h"

APathfinderPlayerController::APathfinderPlayerController()
{
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bShowMouseCursor = true;
	MovementController = CreateDefaultSubobject<UPathfinderMovementComponent>("MovementController");
	MovementController->bVisualizePath = true;
	MovementController->bLerpMovement = false;
}

void APathfinderPlayerController::SetNewDestination(AHexagon* hex)
{
	if(hex && hex->IsWalkable())
	{
		MovementController->MoveFromCurrentPositionTo(hex);
	}
}

void APathfinderPlayerController::BeginPlay()
{
	MovementController->ActorToMove = Cast<AActor>(GetPawn());
	Super::BeginPlay();

	bool startFound = false;
	for (TActorIterator<AHexagon> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AHexagon* startHex = *ActorItr;
		if (startHex->ActorHasTag("START"))
		{
			MovementController->MoveInstantly(startHex);
			startFound = true;
			break;
		}
	}
	if(!startFound)
		UE_LOG(LogPathfinder, Error, TEXT("NO STARTPOINT FOUND!! Define a start Hexagon with Actortag = START"));
}

void APathfinderPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void APathfinderPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("DecreaseCharacterSpeed", IE_Pressed, MovementController, &UPathfinderMovementComponent::IncreaseOneMoveTime);
	InputComponent->BindAction("IncreaseCharacterSpeed", IE_Pressed, MovementController, &UPathfinderMovementComponent::DecreaseOneMoveTime);
}
