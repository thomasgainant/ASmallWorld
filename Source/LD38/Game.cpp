// Fill out your copyright notice in the Description page of Project Settings.

#include "LD38.h"
#include "Game.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"


// Sets default values for this component's properties
UGame::UGame()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	currentYear = 2017;
	waitForMovingAI = false;
	waitForMovingPlayer = false;

	playerCanMove = true;
	gamewon = false;
	gameover = false;
}


// Called when the game starts
void UGame::BeginPlay()
{
	Super::BeginPlay();

	playerController = GetWorld()->GetFirstPlayerController();

	playerController->bShowMouseCursor = true;
	playerController->bEnableClickEvents = true;
	playerController->bEnableMouseOverEvents = true;

	int length = 0;
	for (TObjectIterator<UTile> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		//USkeletalMeshCompoment *Component = *Itr;
		//ClientMessage(Itr->GetName());
		length++;
	}

	//tiles = new UTile*[length];
	//tiles = TArray<UTile*>();

	//int current = 0;
	for (TActorIterator<AStaticMeshActor> It(GetWorld()); It; ++It)
	{
		AActor* actor = *It;
		UTile* tile = actor->FindComponentByClass<UTile>();
		if(tile != NULL){
			tile->uGame = this;
			tiles.Add(tile);
		}

		UASWCharacter* character = actor->FindComponentByClass<UASWCharacter>();
		if(character != NULL){
			character->uGame = this;
			if(character->ai){
				ai.Add(character);
			}
			else {
				player = character;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Tiles count: %d"), tiles.Num());

	for (int i = 0; i < tiles.Num(); i++){
		tiles[i]->coorX = tiles[i]->GetOwner()->GetActorLocation().X / 100;
		tiles[i]->coorY = (tiles[i]->GetOwner()->GetActorLocation().Y / 100) - 1;

		/*UTile* potentialNeighbour0 = GetTileAt(tiles[i]->coorX - 1, tiles[i]->coorY);
		if(potentialNeighbour0 != NULL){
			tiles[i]->neighbours[0] = potentialNeighbour0;
		}

		UTile* potentialNeighbour1 = GetTileAt(tiles[i]->coorX, tiles[i]->coorY - 1);
		if (potentialNeighbour1 != NULL) {
			tiles[i]->neighbours[1] = potentialNeighbour1;
		}

		UTile* potentialNeighbour2 = GetTileAt(tiles[i]->coorX + 1, tiles[i]->coorY);
		if (potentialNeighbour2 != NULL) {
			tiles[i]->neighbours[2] = potentialNeighbour2;
		}

		UTile* potentialNeighbour3 = GetTileAt(tiles[i]->coorX, tiles[i]->coorY + 1);
		if (potentialNeighbour3 != NULL) {
			tiles[i]->neighbours[3] = potentialNeighbour3;
		}*/
	}

	UE_LOG(LogTemp, Warning, TEXT("Tiles count: %d"), tiles.Num());

	computingNeighbours = true;
	currentTileToRefreshIndex = 0;

	//NeighboursComputer::JoyInit(this, currentTileToRefreshIndex);
	/*computer = NeighboursComputer();
	computer.uGame = this;*/
	//FRunnableThread::Create(this, TEXT("UGame.NeighboursComputer"), 0, TPri_Normal, neighboursThread); //windows default = 8mb for thread, could specify more
	
}


// Called every frame
void UGame::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...

	//UE_LOG(LogTemp, Warning, TEXT("Your message"));
	//UE_LOG(LogTemp, Warning, TEXT("Tiles length: %d"), sizeof(tiles)/sizeof(tiles[0]));
	//UE_LOG(LogTemp, Warning, TEXT("currentTileToRefreshIndex: %d"), currentTileToRefreshIndex);

	if(currentTileToRefreshIndex != -1){
		refreshNeighboursForTile(currentTileToRefreshIndex);
		//currentTileToRefreshIndex = -1;
	}

	//UE_LOG(LogTemp, Warning, TEXT("waitForMovingAI: %s"), waitForMovingAI);
	if(waitForMovingAI && !waitForMovingPlayer){
		if(movingAI <= 0){
			waitForMovingAI = false;
			ChangeWorld();
		}
	}
}

/*void UGame::DispatchOnClicked(FKey ButtonClicked)
(
	
)*/

void UGame::refreshNeighboursForTile(int index) {
	//UE_LOG(LogTemp, Warning, TEXT("Refreshing tile: %d"), index);
	if(index < tiles.Num()){
		UTile* potentialNeighbour0 = GetTileAt(tiles[index]->coorX - 1, tiles[index]->coorY);
		if (potentialNeighbour0 != NULL) {
			tiles[index]->neighbours[0] = potentialNeighbour0;
		}

		UTile* potentialNeighbour1 = GetTileAt(tiles[index]->coorX, tiles[index]->coorY - 1);
		if (potentialNeighbour1 != NULL) {
			tiles[index]->neighbours[1] = potentialNeighbour1;
		}

		UTile* potentialNeighbour2 = GetTileAt(tiles[index]->coorX + 1, tiles[index]->coorY);
		if (potentialNeighbour2 != NULL) {
			tiles[index]->neighbours[2] = potentialNeighbour2;
		}

		UTile* potentialNeighbour3 = GetTileAt(tiles[index]->coorX, tiles[index]->coorY + 1);
		if (potentialNeighbour3 != NULL) {
			tiles[index]->neighbours[3] = potentialNeighbour3;
		}

		currentTileToRefreshIndex++;
	}
	else {
		currentTileToRefreshIndex = -1;
	}
	//UE_LOG(LogTemp, Warning, TEXT("currentTileToRefreshIndex: %d"), currentTileToRefreshIndex);
}

void UGame::ConfirmMovement() {
	UE_LOG(LogTemp, Warning, TEXT("ConfirmMovement"));
	if(movingAI <= 0 && !waitForMovingAI){
		NewTurn();
	}
}

void UGame::NewTurn() {
	//UE_LOG(LogTemp, Warning, TEXT("Your message"));
	waitForMovingAI = true;

	for (int i = 0; i < ai.Num(); i++) {
		UTile* chosenTile = NULL;
		int nextCoorX = 0;
		int nextCoorY = 0;
		int tries = 0;

		while(chosenTile == NULL || chosenTile->currentType != UTile::TileType::plain){			
			if (FMath::RandRange(0, 10) > 5) {
				nextCoorX = 1;
			}
			else {
				nextCoorX = - 1;
			}
			
			if (FMath::RandRange(0, 10) > 5) {
				nextCoorY = 1;
			}
			else {
				nextCoorY = 1;
			}

			chosenTile = GetTileAt(ai[i]->currentXCoor + nextCoorX, ai[i]->currentYCoor + nextCoorY);
			tries++;
			if(tries > 250) {
				break;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("AI choose to move to %d, %d"), nextCoorX, nextCoorY);
		ai[i]->moveTo(nextCoorX, nextCoorY);
		movingAI++;
	}
}

void UGame::ChangeWorld() {
	bool hasIce = false;
	for (int i = 0; i < tiles.Num(); i++) {
		if(tiles[i]->currentType == UTile::TileType::ice){
			hasIce = true;
			break;
		}
	}

	if(hasIce){
		int randIndex = FMath::RandRange(0, tiles.Num() - 1);
		//tiles[randIndex]->GetOwner()->SetActorLocation(tiles[randIndex]->GetOwner()->GetActorLocation() + FVector(0, 0, 1.0f));
		while (tiles[randIndex]->currentType != UTile::TileType::ice) {
			randIndex = FMath::RandRange(0, tiles.Num() - 1);
		}

		tiles[randIndex]->setTileType(UTile::TileType::ocean);

		int numberOfSoil = 0;
		for (int i = 0; i < tiles.Num(); i++) {
			if (tiles[i]->currentType != UTile::TileType::ice != tiles[i]->currentType != UTile::TileType::ocean) {
				numberOfSoil++;
			}
		}

		if(numberOfSoil > 0){
			if (numberOfSoil > 3) {
				numberOfSoil = 3;
			}

			for (int j = 0; j < numberOfSoil; j++) {
				int randIndexSoil = FMath::RandRange(0, tiles.Num() - 1);
				//tiles[randIndex]->GetOwner()->SetActorLocation(tiles[randIndex]->GetOwner()->GetActorLocation() + FVector(0, 0, 1.0f));
				while (tiles[randIndexSoil]->currentType == UTile::TileType::ice || tiles[randIndexSoil]->currentType == UTile::TileType::ocean || !tiles[randIndexSoil]->hasOceanNeighbourTile()) {
					randIndexSoil = FMath::RandRange(0, tiles.Num() - 1);
				}

				tiles[randIndexSoil]->setTileType(UTile::TileType::ocean);
			}

			UTile* playerTileOnIt = GetTileAt(player->currentXCoor, player->currentYCoor);
			if (playerTileOnIt != NULL && playerTileOnIt->currentType == UTile::TileType::ocean) {
				UE_LOG(LogTemp, Warning, TEXT("game over"));
				gameover = true;
			}

			if (!gameover) {
				int destroyIndex[3] = { -1, -1, -1 };
				for (int i = 0; i < ai.Num(); i++) {
					UTile* tileOnIt = GetTileAt(ai[i]->currentXCoor, ai[i]->currentYCoor);
					if (tileOnIt != NULL && tileOnIt->currentType == UTile::TileType::ocean) {
						UE_LOG(LogTemp, Warning, TEXT("ai %d is going to be destroyed"), i);
						for (int j = 0; j < 3; j++) {
							if (destroyIndex[j] == -1) {
								destroyIndex[j] = i;
								break;
							}
						}
					}
				}
				for (int i = 0; i < 3; i++) {
					UE_LOG(LogTemp, Warning, TEXT("%d"), destroyIndex[i]);
					if (destroyIndex[i] != -1 && destroyIndex[i] < ai.Num()) {
						ai[destroyIndex[i]]->GetOwner()->Destroy();
						numberOfDeadAI++;
						ai.RemoveAt(destroyIndex[i]);
						UE_LOG(LogTemp, Warning, TEXT("removed ai %d"), destroyIndex[i]);
					}
				}

				UE_LOG(LogTemp, Warning, TEXT("ai num %d"), ai.Num());
				if (ai.Num() <= 0 && numberOfDeadAI == 3) {
					UE_LOG(LogTemp, Warning, TEXT("game won"));
					//Game won
					gamewon = true;
				}
			}
		}
		else {
			//Game over
		}

		currentYear++;
		playerCanMove = true;
	}
	else {
		//Game over
	}
}

UTile* UGame::GetTileAt(int coorX, int coorY){
	UTile* result = NULL;

	for (int i = 0; i < tiles.Num(); i++) {
		if(tiles[i]->coorX == coorX
			&& tiles[i]->coorY == coorY){
			result = tiles[i];
			break;
		}
	}

	return result;
}

