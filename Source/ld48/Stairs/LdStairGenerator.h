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

	UFUNCTION(BlueprintCallable)
	void Start();
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
	TMap<UStaticMesh*, float> WallMeshes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<UStaticMesh*, float> DecoMeshes;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, TSubclassOf<AObstacleActor>> Obstacles;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ACameraActor* Camera;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* Plane;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float FloorDistanceToDestroy = 1000;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float CurrentHeight = 0;

private:
	void GenerateSteps();
	void GenerateStep(TCHAR type);
	void AddStep(bool generateDecoration = true, UClass* Class = AStaticMeshActor::StaticClass());

	TArray<AStaticMeshActor*> Steps;
	float CurrentRotation = 0;

};
