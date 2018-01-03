// Fill out your copyright notice in the Description page of Project Settings.

#include "LD38.h"
#include "NeighboursComputer.h"
#include "../../Source/LD38/Game.h"
#include "Async.h"

NeighboursComputer* NeighboursComputer::Runnable = NULL;

NeighboursComputer::NeighboursComputer(UGame* gameInstance, int& index)
{
	UE_LOG(LogTemp, Warning, TEXT("Create"));
	//Compute();
	uGame = gameInstance;
	numberOfTilesToScan = uGame->tiles.Num();
	tilesScanned = 0;
	currentTileToRefreshIndex = &index;
	neighboursThread = FRunnableThread::Create(this, TEXT("UGame.NeighboursComputer"), 0, TPri_Normal); //windows default = 8mb for thread, could specify more
}

NeighboursComputer::~NeighboursComputer()
{
	delete neighboursThread;
	neighboursThread = NULL;
}

/*void NeighboursComputer::Compute() {
	neighboursThread = FRunnableThread::Create(this, TEXT("UGame.NeighboursComputer"), 0, TPri_Normal); //windows default = 8mb for thread, could specify more
}*/

//Init
bool NeighboursComputer::Init()
{
	return true;
}

//Run
uint32 NeighboursComputer::Run()
{
	UE_LOG(LogTemp, Warning, TEXT("Run"));
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	UE_LOG(LogTemp, Warning, TEXT("Run"));

	for (int i = 0; i < uGame->tiles.Num(); i++){
		UE_LOG(LogTemp, Warning, TEXT("Tile number: %d"), i);

		/*UTile* potentialNeighbour0 = uGame->GetTileAt(uGame->tiles[i]->coorX - 1, uGame->tiles[i]->coorY);
		if(potentialNeighbour0 != NULL){
			uGame->tiles[i]->neighbours[0] = potentialNeighbour0;
		}

		UTile* potentialNeighbour1 = uGame->GetTileAt(uGame->tiles[i]->coorX, uGame->tiles[i]->coorY - 1);
		if (potentialNeighbour1 != NULL) {
			uGame->tiles[i]->neighbours[1] = potentialNeighbour1;
		}

		UTile* potentialNeighbour2 = uGame->GetTileAt(uGame->tiles[i]->coorX + 1, uGame->tiles[i]->coorY);
		if (potentialNeighbour2 != NULL) {
			uGame->tiles[i]->neighbours[2] = potentialNeighbour2;
		}

		UTile* potentialNeighbour3 = uGame->GetTileAt(uGame->tiles[i]->coorX, uGame->tiles[i]->coorY + 1);
		if (potentialNeighbour3 != NULL) {
			uGame->tiles[i]->neighbours[3] = potentialNeighbour3;
		}*/

		/*AsyncTask(ENamedThreads::GameThread, [](int* currentTileToRefreshIndex, int i) {
			// code to execute on game thread here
			*currentTileToRefreshIndex = i;
			//UE_LOG(LogTemp, Warning, TEXT("currentTileToRefreshIndex: %d"), currentTileToRefreshIndex);
		});*/

		//*currentTileToRefreshIndex = i;
		//UE_LOG(LogTemp, Warning, TEXT("currentTileToRefreshIndex: %d"), currentTileToRefreshIndex);

		tilesScanned++;
		FPlatformProcess::Sleep(1.0);
	}

	//While not told to stop this thread 
	//		and not yet finished finding Prime Numbers
	/*while (StopTaskCounter.GetValue() == 0 && !IsFinished())
	{
		//PrimeNumbers->Add(FindNextPrimeNumber());
		//PrimesFoundCount++;

		//***************************************
		//Show Incremental Results in Main Game Thread!

		//	Please note you should not create, destroy, or modify UObjects here.
		//	  Do those sort of things after all thread are completed.

		//	  All calcs for making stuff can be done in the threads
		//	     But the actual making/modifying of the UObjects should be done in main game thread.
		//ThePC->ClientMessage(FString::FromInt(PrimeNumbers->Last()));
		//UE_LOG(LogTemp, Warning, TEXT("Your message"));
		//***************************************

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//prevent thread from using too many resources
		FPlatformProcess::Sleep(0.01);
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	}*/

	//Run FPrimeNumberWorker::Shutdown() from the timer in Game Thread that is watching
	//to see when FPrimeNumberWorker::IsThreadFinished()

	return 0;
}

//stop
void NeighboursComputer::Stop()
{
	StopTaskCounter.Increment();
}

NeighboursComputer* NeighboursComputer::JoyInit(UGame* gameInstance, int& indexToRefresh)
{
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (Runnable == NULL && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new NeighboursComputer(gameInstance, indexToRefresh);
	}
	return Runnable;
}

void NeighboursComputer::EnsureCompletion()
{
	Stop();
	neighboursThread->WaitForCompletion();
}

void NeighboursComputer::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

bool NeighboursComputer::IsThreadFinished()
{
	if (Runnable) return Runnable->IsFinished();
	return true;
}

//Done?
bool NeighboursComputer::IsFinished()
{
	return tilesScanned >= numberOfTilesToScan;
}
