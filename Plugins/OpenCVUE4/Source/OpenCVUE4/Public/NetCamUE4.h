// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynCamUE4Wrapper.h"
#include "NetCamUE4.generated.h"

UCLASS()
class OPENCVUE4_API ANetCamUE4 : public AActor, public dyncam_unreal::EventListener
{
	GENERATED_BODY()
private:
	dyncam_unreal::DynCamUE4Wrapper wrapper;
		
public:	
	// Sets default values for this actor's properties
	ANetCamUE4();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnReceive(dyncam_unreal::Event event);
};
