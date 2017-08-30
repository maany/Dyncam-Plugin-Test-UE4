// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Runtime/UMG/Public/UMG.h" 
#include "Runtime/UMG/Public/UMGStyle.h" 
#include "Runtime/UMG/Public/Slate/SObjectWidget.h" 
#include "Runtime/UMG/Public/IUMGModule.h" 
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

THIRD_PARTY_INCLUDES_START
//#include "AllowWindowsPlatformTypes.h"
#include "TCPClient.h"
#include "BaseCam.h"
#include "boost/asio.hpp"
#include "Observer.h"
//#include "HideWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_END

#include "NetCamPawn.generated.h"

UCLASS()
class OPENCVUE4_API ANetCamPawn : public APawn, public BaseCam, public Observer
{
	GENERATED_BODY()

public:
	TCPClient::pointer client;

	// Sets default values for this pawn's properties
	ANetCamPawn();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	boost::asio::io_service io_service;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//
	void Connect(char const * IPAddress, int port, std::vector<BaseCam::StreamCapabilities> requestStreams);
};
