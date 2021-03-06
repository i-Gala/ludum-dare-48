// Fill out your copyright notice in the Description page of Project Settings.

#include "LdStairGenerator.h"
#include "Camera/CameraActor.h"

// Sets default values
ALdStairGenerator::ALdStairGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FloorDistanceToDestroy = -500.f;
}

void ALdStairGenerator::Start()
{
	if (UpdatePhase(0))
	{
		GenerateModule(FirstModule);
		GenerateSteps();
	}
}

bool ALdStairGenerator::UpdatePhase(int32 phase)
{
	bool success = false;
	auto modules = StepModulesByPhase.Find(phase);
	if (modules)
	{
		success = true;
		FModulePhaseStruct module = *modules;
		StepModules = module.StepModules;
	}
	return success;
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
	if (!Camera) return;
	FVector stepLocation = Steps[0]->GetActorLocation();
	FVector camLocation = Camera->GetActorLocation();
	float dist = camLocation.Z - stepLocation.Z;

	if (dist < FloorDistanceToDestroy)
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
		GenerateModule(StepModules[module]);
	}
}

void ALdStairGenerator::GenerateModule(FString Module)
{
	for (int32 i = 0; i < Module.Len(); i++)
	{
		FString currChar = Module.Mid(i, 1);
		GenerateStep(currChar);
	}
}


void ALdStairGenerator::GenerateStep(FString key)
{

	if (key.Equals("S"))
	{
		AddStep(StepMesh);
	}
	else if (key.Equals("T") || key.Equals("K"))
	{
		const auto obstacleClass = Obstacles.Find(FName(key));
		if (obstacleClass)
		{
			AddStep(EmptyStepMesh, true);
			FActorSpawnParameters spawnParams;
			FVector loc = Steps.Last()->GetActorLocation();
			FRotator rot = Steps.Last()->GetActorRotation();
			AActor* obstacle = GetWorld()->SpawnActor<AActor>(*obstacleClass, loc, rot, spawnParams);
		}
	}
	else if(key.Equals("H"))
	{
		AddStep(EmptyStepMesh);
	}
	else
	{
		AddStep(StepMesh);
		const auto obstacleClass = Obstacles.Find(FName(key));
		if (obstacleClass)
		{
			FActorSpawnParameters spawnParams;
			FRotator rot = FRotator(0, CurrentRotation, 0);

			AActor* obstacle = GetWorld()->SpawnActor<AActor>(*obstacleClass, FVector(0, 0, CurrentHeight + StepHeight / 2) + rot.Vector() * 600, rot, spawnParams);
		}
	}

	CurrentHeight -= StepHeight;
	CurrentRotation += 360.f/NumStepsPer360;
}

void ALdStairGenerator::AddStep(UStaticMesh* mesh, bool GenerateDecoration, UClass* Class)
{
	FActorSpawnParameters spawnParams;
	AStaticMeshActor* step = GetWorld()->SpawnActor<AStaticMeshActor>(Class, FVector(0, 0, CurrentHeight), FRotator(0, CurrentRotation - 90, 0), spawnParams);
	step->SetMobility(EComponentMobility::Movable);
	step->GetStaticMeshComponent()->SetStaticMesh(mesh);
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

	if (!GenerateDecoration) return;

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

