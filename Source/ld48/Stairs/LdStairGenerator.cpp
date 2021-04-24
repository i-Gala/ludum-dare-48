// Fill out your copyright notice in the Description page of Project Settings.

#include "LdStairGenerator.h"
#include "Camera/CameraActor.h"

// Sets default values
ALdStairGenerator::ALdStairGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ALdStairGenerator::Start()
{
	GenerateSteps();
}

// Called when the game starts or when spawned
void ALdStairGenerator::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ALdStairGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Started) return;
	if (!Camera) return;
	float dist = FVector::Distance(Steps[0]->GetActorLocation(), Camera->GetActorLocation());

	if (dist < 1000)
	{
		TArray<AActor*> attached;
		Steps[0]->GetAttachedActors(attached, true);
		for (auto c : attached)
			c->Destroy();

		Steps[0]->Destroy();
		Steps.RemoveAt(0);

		GenerateSteps();

		if(Plane)
			Plane->SetActorLocation(FVector(0, 0, CurrentHeight));
	}

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
		AddStep();
		Steps.Last()->SetActorHiddenInGame(true);
		Steps.Last()->SetActorEnableCollision(false);
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
	step->SetMobility(EComponentMobility::Movable);
	step->GetStaticMeshComponent()->SetStaticMesh(StepMesh);
	Steps.Add(step);

	float walln = FMath::FRandRange(0, 1);

	float acc = 0;
	for (auto m : WallMeshes)
	{
		acc += m.Value;
		if (walln < acc)
		{
			AStaticMeshActor* wall = GetWorld()->SpawnActor<AStaticMeshActor>(Class, FVector(0, 0, 970 * 2), FRotator(0, 0, 0), spawnParams);
			wall->SetMobility(EComponentMobility::Movable);
			wall->GetStaticMeshComponent()->SetStaticMesh(m.Key);
			wall->AttachToActor(step, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
			break;
		}
	}

	float decon = FMath::FRandRange(0, 1);

	acc = 0;
	for (auto m : DecoMeshes)
	{
		acc += m.Value;
		if (decon < acc)
		{
			AStaticMeshActor* deco = GetWorld()->SpawnActor<AStaticMeshActor>(Class, FVector(0, 0, CurrentHeight), FRotator(0, 0, 0), spawnParams);
			deco->SetMobility(EComponentMobility::Movable);
			deco->GetStaticMeshComponent()->SetStaticMesh(m.Key);
			deco->AttachToActor(step, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
			break;
		}
	}
}

