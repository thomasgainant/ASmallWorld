// Fill out your copyright notice in the Description page of Project Settings.

#include "LD38.h"
#include "ASWCharacter.h"
#include "Game.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"


// Sets default values for this component's properties
UASWCharacter::UASWCharacter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	shouldMove = false;
	moveSpeed = 2.0f;
	hasMovedThisTurn = false;
}


// Called when the game starts
void UASWCharacter::BeginPlay()
{
	Super::BeginPlay();

	destination = GetOwner()->GetActorLocation();

	currentXCoor = GetOwner()->GetActorLocation().X / 100;
	currentYCoor = GetOwner()->GetActorLocation().Y / 100 - 1;
	//moveTo(10, 20);
}


// Called every frame
void UASWCharacter::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	if(shouldMove){
		FVector A = GetOwner()->GetActorLocation();
		FVector newPos = destination - A;
		newPos.Normalize();
		newPos *= moveSpeed;
		GetOwner()->SetActorLocation(A + newPos);

		FVector diff = destination - GetOwner()->GetActorLocation();
		if(diff.Size() <= moveSpeed){
			UE_LOG(LogTemp, Warning, TEXT("Arrived at %d, %d"), currentXCoor, currentYCoor);
			shouldMove = false;
			hasMovedThisTurn = true;
			if (ai) {
				uGame->movingAI--;
			}
			else {
				uGame->ConfirmMovement();
				uGame->waitForMovingPlayer = false;
			}
		}
	}
}

bool UASWCharacter::moveTo(int coorX, int coorY){
	currentXCoor += coorX;
	currentYCoor += coorY;
	UE_LOG(LogTemp, Warning, TEXT("Moving to %d, %d"), currentXCoor, currentYCoor);

	FVector A = GetOwner()->GetActorLocation();
	destination = FVector(A.X + coorX * 100.0f, A.Y + coorY * 100.0f, A.Z);
	shouldMove = true;
	return true;
}

