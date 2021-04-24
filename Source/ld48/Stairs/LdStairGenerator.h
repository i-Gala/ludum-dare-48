// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../ObstacleActor.h"
#include "Engine/StaticMeshActor.h"
#include "LdStairGenerator.generated.h"

UCLASS()
class LD48_API ALdStairGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALdStairGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> StepModules;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StepHeight = 80;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float NumStepsPer360 = 12;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxSteps = 64;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* StepMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* WallMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, TSubclassOf<AObstacleActor>> Obstacles;
private:
	void GenerateSteps();
	void GenerateStep(TCHAR type);
	void AddStep(UClass* Class = AStaticMeshActor::StaticClass());

	TArray<AStaticMeshActor*> Steps;
	float CurrentHeight = 0;
	float CurrentRotation = 0;
};
