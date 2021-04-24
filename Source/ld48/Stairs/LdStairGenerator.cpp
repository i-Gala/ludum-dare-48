// Fill out your copyright notice in the Description page of Project Settings.

#include "LdStairGenerator.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
ALdStairGenerator::ALdStairGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALdStairGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateSteps();
}


// Called every frame
void ALdStairGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALdStairGenerator::GenerateSteps()
{
	while (Steps.Num() < MaxSteps)
	{
		int module = FMath::RandRange(0, StepModules.Num() - 1);
		FString moduleString = StepModules[module];
		for (int32 i = 0; i < moduleString.Len(); i++)
		{
			FString currChar = moduleString.Mid(i, 1);

			GenerateStep(currChar[0]);
		}
	}
}

void ALdStairGenerator::GenerateStep(TCHAR currChar)
{
	switch (currChar)
	{
	case 'S':
		{
			FActorSpawnParameters spawnParams;
			AStaticMeshActor* step = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), FVector(0, 0, CurrentHeight), FRotator(0, CurrentRotation, 0), spawnParams);
			step->GetStaticMeshComponent()->SetStaticMesh(StepMesh);
			Steps.Add(step);
			break;
		}
	case 'H':
		break;
	}

	CurrentHeight += StepHeight;
	CurrentRotation += 360.f/NumStepsPer360;

}

