// Fill out your copyright notice in the Description page of Project Settings.

#include "NetCamPawn.h"
// Sets default values
ANetCamPawn::ANetCamPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	client= TCPClient::Create(io_service, this);
}

// Called when the game starts or when spawned
void ANetCamPawn::BeginPlay()
{
	
	Super::BeginPlay();
	Observe(&this->DepthEventsSource, [](cv::Mat mat) {
		UE_LOG(LogTemp, Warning, TEXT("depth event works!"));
	});
	boost::asio::ip::tcp::endpoint serverEndpoint(boost::asio::ip::address::from_string(std::string("127.0.0.1")), 31401);
	client->GetSocket().connect(serverEndpoint);
	client->ReceiveHandshakeRequest();
	std::vector<BaseCam::StreamCapabilities> requestStreams;
	requestStreams.push_back(BaseCam::StreamCapabilities::DEPTH);
	requestStreams.push_back(BaseCam::StreamCapabilities::COLOR);
	client->SendHandshakeAck(requestStreams);
	
	
}

// Called every frame
void ANetCamPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	client->Execute();
}

// Called to bind functionality to input
void ANetCamPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ANetCamPawn::Connect(char const * IPAddress, int port, std::vector<BaseCam::StreamCapabilities> requestStreams) {
	
}

