// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Core/Public/HAL/RunnableThread.h"

class UGame;

/**
 * 
 */
class LD38_API NeighboursComputer : public FRunnable
{
public:
	static NeighboursComputer* Runnable;

	FRunnableThread* neighboursThread;
	UGame* uGame;

	/** Stop this thread? Uses Thread Safe Counter */
	FThreadSafeCounter StopTaskCounter;

	int32 numberOfTilesToScan;
	int32 tilesScanned;
	int* currentTileToRefreshIndex;

	NeighboursComputer(UGame* gameInstance, int& index);
	~NeighboursComputer();
	//void Compute();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	void EnsureCompletion();

	/*
	Start the thread and the worker from static (easy access)!
	This code ensures only 1 Prime Number thread will be able to run at a time.
	This function returns a handle to the newly started instance.
	*/
	static NeighboursComputer* JoyInit(UGame* gameInstance, int32& indexToRefresh);

	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
	static void Shutdown();
	static bool IsThreadFinished();
	bool IsFinished();
};
