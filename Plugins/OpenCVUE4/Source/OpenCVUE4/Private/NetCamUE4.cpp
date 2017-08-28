// Fill out your copyright notice in the Description page of Project Settings.

#include "NetCamUE4.h"

DEFINE_LOG_CATEGORY(DynCamUnreal);
// Sets default values
ANetCamUE4::ANetCamUE4()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANetCamUE4::BeginPlay()
{
	Super::BeginPlay();
	/*wrapper.SubscribeToColorEvents(this);
	wrapper.SubscribeToColorEvents(this);
	std::vector<BaseCam::StreamCapabilities> requestStreams;
	requestStreams.push_back(BaseCam::StreamCapabilities::DEPTH);
	requestStreams.push_back(BaseCam::StreamCapabilities::COLOR);
	UE_LOG(LogTemp, Warning, TEXT("Trying to Start NetCam"));
	wrapper.Connect("127.0.0.1", 31401, requestStreams);*/
}

// Called every frame
void ANetCamUE4::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANetCamUE4::OnReceive(dyncam_unreal::Event event) {
	/*if (event.m_StreamType = BaseCam::StreamCapabilities::DEPTH) {
		UE_LOG(DynCamUnreal, Warning, TEXT("Depth Received"));
	}
	else if (event.m_StreamType = BaseCam::StreamCapabilities::COLOR) {
		UE_LOG(DynCamUnreal, Warning, TEXT("Color Received"));
	}*/
}


