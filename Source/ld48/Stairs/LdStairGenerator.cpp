// Fill out your copyright notice in the Description page of Project Settings.

#include "LdStairGenerator.h"

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
		AddStep();
		break;
	case 'T':
		{
			auto o = Obstacles.Find("T");
			if (o) AddStep(*o);
		}
	case 'H':
		break;
	default:
		{
			AddStep();
			auto o = Obstacles.Find(FName(&currChar));
			if (o)
			{
				FActorSpawnParameters spawnParams;
				FRotator rot = FRotator(0, CurrentRotation+90, 0);

				AActor* step = GetWorld()->SpawnActor<AActor>(*o, FVector(0, 0, 0) + rot.Vector()* 600, rot, spawnParams);
			}
			break;
		}
	}

	CurrentHeight -= StepHeight;
	CurrentRotation += 360.f/NumStepsPer360;

}

void ALdStairGenerator::AddStep(UClass* Class)
{
	FActorSpawnParameters spawnParams;
	AStaticMeshActor* step = GetWorld()->SpawnActor<AStaticMeshActor>(Class, FVector(0, 0, CurrentHeight), FRotator(0, CurrentRotation - 90, 0), spawnParams);
	step->GetStaticMeshComponent()->SetStaticMesh(StepMesh);

	AStaticMeshActor* wall = GetWorld()->SpawnActor<AStaticMeshActor>(Class, FVector(0, 0, CurrentHeight), FRotator(0, 0, 0), spawnParams);
	wall->GetStaticMeshComponent()->SetStaticMesh(WallMesh);

	Steps.Add(step);

	wall->AttachToActor(step, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
}

