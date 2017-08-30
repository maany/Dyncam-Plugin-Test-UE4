// Fill out your copyright notice in the Description page of Project Settings.

#include "WebcamRenderer.h"


// Sets default values
AWebcamRenderer::AWebcamRenderer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	client = TCPClient::Create(io_service, this);

	// Initialize OpenCV and webcam properties
	CameraID = 0;
	//RefreshRate = 15;
	isStreamOpen = false;
	VideoSize = FVector2D(0, 0);
	ShouldResize = false;
	ResizeDeminsions = FVector2D(320, 240);
	RefreshTimer = 0.0f;
	//stream = cv::VideoCapture();
	frame = cv::Mat();

}

// Called when the game starts or when spawned
void AWebcamRenderer::BeginPlay()
{
	Super::BeginPlay();
	Observe(&this->ColorEventsSource, [&](cv::Mat mat) {
		if (!isStreamOpen) {
			VideoSize = FVector2D(mat.cols, mat.rows);
			size = cv::Size(ResizeDeminsions.X, ResizeDeminsions.Y);
			VideoTexture = UTexture2D::CreateTransient(VideoSize.X, VideoSize.Y);
			#define UpdateResource UpdateResource
			VideoTexture->UpdateResource();
			VideoUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, VideoSize.X, VideoSize.Y);

			// Initialize data array
			Data.Init(FColor(0, 0, 0, 255), VideoSize.X * VideoSize.Y);
			isStreamOpen = true;
		}
		UE_LOG(LogTemp, Warning, TEXT("depth event works!"));
		UpdateFrame(mat);
		DoProcessing();
		UpdateTexture();
		OnNextVideoFrame();
	});
	boost::asio::ip::tcp::endpoint serverEndpoint(boost::asio::ip::address::from_string(std::string("127.0.0.1")), 31401);
	client->GetSocket().connect(serverEndpoint);
	client->ReceiveHandshakeRequest();
	std::vector<BaseCam::StreamCapabilities> requestStreams;
	requestStreams.push_back(BaseCam::StreamCapabilities::DEPTH);
	requestStreams.push_back(BaseCam::StreamCapabilities::COLOR);
	client->SendHandshakeAck(requestStreams);
	// Open the stream
	//stream.open(CameraID);
	//if (stream.isOpened())
	//{
		// Initialize stream
		//isStreamOpen = false;
		//UpdateFrame();
		//VideoSize = FVector2D(frame.cols, frame.rows);
		//size = cv::Size(ResizeDeminsions.X, ResizeDeminsions.Y);
		//VideoTexture = UTexture2D::CreateTransient(VideoSize.X, VideoSize.Y);
		//#define UpdateResource UpdateResource
		//VideoTexture->UpdateResource();
		//VideoUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, VideoSize.X, VideoSize.Y);

		// Initialize data array
		//Data.Init(FColor(0, 0, 0, 255), VideoSize.X * VideoSize.Y);

		// Do first frame
		//DoProcessing();
		//UpdateTexture();
		//OnNextVideoFrame();
	//}

}

// Called every frame
void AWebcamRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	client->Execute();
	//RefreshTimer += DeltaTime;
	//if (isStreamOpen && RefreshTimer >= 1.0f / RefreshRate)
	//{
	//	RefreshTimer -= 1.0f / RefreshRate;
	//	UpdateFrame();
	//	DoProcessing();
	//	UpdateTexture();
	//	OnNextVideoFrame();
	//}
}

void AWebcamRenderer::UpdateFrame(cv::Mat mat)
{
	frame = mat;
//	if (stream.isOpened())
//	{
		//stream.read(frame);
		if (ShouldResize)
		{
			//cv::resize(frame, frame, size);
		}
//	}
//	else {
//		isStreamOpen = false;
//	}
}

void AWebcamRenderer::DoProcessing()
{
	// TODO: Do any processing here!
}

void AWebcamRenderer::UpdateTexture()
{
	if (isStreamOpen && frame.data)
	{
		// Copy Mat data to Data array
		for (int y = 0; y < VideoSize.Y; y++)
		{
			for (int x = 0; x < VideoSize.X; x++)
			{
				int i = x + (y * VideoSize.X);
				Data[i].R = frame.data[i * 4 + 0];
				Data[i].G = frame.data[i * 4 + 1];
				Data[i].B = frame.data[i * 4 + 2];
				Data[i].A = frame.data[i * 4 + 3];
			}
		}

		// Update texture 2D
		UpdateTextureRegions(VideoTexture, (int32)0, (uint32)1, VideoUpdateTextureRegion, (uint32)(4 * VideoSize.X), (uint32)4, (uint8*)Data.GetData(), false);
	}
}

void AWebcamRenderer::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
						);
					}
				}
		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}

