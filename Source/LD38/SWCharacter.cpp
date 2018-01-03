// Fill out your copyright notice in the Description page of Project Settings.

#include "LD38.h"
#include "SWCharacter.h"


// Sets default values
ASWCharacter::ASWCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASWCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASWCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

