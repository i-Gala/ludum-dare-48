// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../ObstacleActor.h"
#include "Engine/StaticMeshActor.h"
#include "LdStairGenerator.generated.h"

USTRUCT(BlueprintType)
struct FModulePhaseStruct
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> StepModules;
};

UCLASS()
class LD48_API ALdStairGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALdStairGenerator();

	UFUNCTION(BlueprintCallable)
	void Start();
	UFUNCTION(BlueprintCallable)
	bool UpdatePhase(int32 phase);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString FirstModule;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int32, FModulePhaseStruct> StepModulesByPhase;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StepHeight = 80;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float NumStepsPer360 = 12;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxSteps = 64;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* StepMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* EmptyStepMesh;
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
	float FloorDistanceToDestroy;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float CurrentHeight = 0;

private:
	void GenerateSteps();
	void GenerateModule(FString Module);
	void GenerateStep(FString key);
	void AddStep(UStaticMesh* mesh, bool generateDecoration = true, UClass* Class = AStaticMeshActor::StaticClass());

	TArray<AStaticMeshActor*> Steps;
	float CurrentRotation = 0;
	TArray<FString> StepModules;

};
