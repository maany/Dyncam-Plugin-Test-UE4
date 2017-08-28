// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/WebcamRenderer.h"
PRAGMA_DISABLE_OPTIMIZATION
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWebcamRenderer() {}
// Cross Module References
	OPENCVUE4_API UFunction* Z_Construct_UFunction_AWebcamRenderer_OnNextVideoFrame();
	OPENCVUE4_API UClass* Z_Construct_UClass_AWebcamRenderer();
	OPENCVUE4_API UClass* Z_Construct_UClass_AWebcamRenderer_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_OpenCVUE4();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FColor();
	ENGINE_API UClass* Z_Construct_UClass_UTexture2D_NoRegister();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
// End Cross Module References
	static FName NAME_AWebcamRenderer_OnNextVideoFrame = FName(TEXT("OnNextVideoFrame"));
	void AWebcamRenderer::OnNextVideoFrame()
	{
		ProcessEvent(FindFunctionChecked(NAME_AWebcamRenderer_OnNextVideoFrame),NULL);
	}
	void AWebcamRenderer::StaticRegisterNativesAWebcamRenderer()
	{
	}
	UFunction* Z_Construct_UFunction_AWebcamRenderer_OnNextVideoFrame()
	{
		UObject* Outer = Z_Construct_UClass_AWebcamRenderer();
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			ReturnFunction = new(EC_InternalUseOnlyConstructor, Outer, TEXT("OnNextVideoFrame"), RF_Public|RF_Transient|RF_MarkAsNative) UFunction(FObjectInitializer(), nullptr, (EFunctionFlags)0x08020800, 65535);
			ReturnFunction->Bind();
			ReturnFunction->StaticLink();
#if WITH_METADATA
			UMetaData* MetaData = ReturnFunction->GetOutermost()->GetMetaData();
			MetaData->SetValue(ReturnFunction, TEXT("Category"), TEXT("Webcam"));
			MetaData->SetValue(ReturnFunction, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
			MetaData->SetValue(ReturnFunction, TEXT("ToolTip"), TEXT("Blueprint Event called every time the video frame is updated"));
#endif
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_AWebcamRenderer_NoRegister()
	{
		return AWebcamRenderer::StaticClass();
	}
	UClass* Z_Construct_UClass_AWebcamRenderer()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_AActor();
			Z_Construct_UPackage__Script_OpenCVUE4();
			OuterClass = AWebcamRenderer::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= (EClassFlags)0x20900080u;

				OuterClass->LinkChild(Z_Construct_UFunction_AWebcamRenderer_OnNextVideoFrame());

				UProperty* NewProp_Data = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("Data"), RF_Public|RF_Transient|RF_MarkAsNative) UArrayProperty(CPP_PROPERTY_BASE(Data, AWebcamRenderer), 0x0010000000000014);
				UProperty* NewProp_Data_Inner = new(EC_InternalUseOnlyConstructor, NewProp_Data, TEXT("Data"), RF_Public|RF_Transient|RF_MarkAsNative) UStructProperty(FObjectInitializer(), EC_CppProperty, 0, 0x0000000000000000, Z_Construct_UScriptStruct_FColor());
				UProperty* NewProp_VideoTexture = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("VideoTexture"), RF_Public|RF_Transient|RF_MarkAsNative) UObjectProperty(CPP_PROPERTY_BASE(VideoTexture, AWebcamRenderer), 0x0010000000000014, Z_Construct_UClass_UTexture2D_NoRegister());
				UProperty* NewProp_VideoSize = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("VideoSize"), RF_Public|RF_Transient|RF_MarkAsNative) UStructProperty(CPP_PROPERTY_BASE(VideoSize, AWebcamRenderer), 0x0010000000000004, Z_Construct_UScriptStruct_FVector2D());
				CPP_BOOL_PROPERTY_BITMASK_STRUCT(isStreamOpen, AWebcamRenderer);
				UProperty* NewProp_isStreamOpen = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("isStreamOpen"), RF_Public|RF_Transient|RF_MarkAsNative) UBoolProperty(FObjectInitializer(), EC_CppProperty, CPP_BOOL_PROPERTY_OFFSET(isStreamOpen, AWebcamRenderer), 0x0010000000000014, CPP_BOOL_PROPERTY_BITMASK(isStreamOpen, AWebcamRenderer), sizeof(bool), true);
				UProperty* NewProp_RefreshTimer = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("RefreshTimer"), RF_Public|RF_Transient|RF_MarkAsNative) UFloatProperty(CPP_PROPERTY_BASE(RefreshTimer, AWebcamRenderer), 0x0010000000000004);
				UProperty* NewProp_RefreshRate = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("RefreshRate"), RF_Public|RF_Transient|RF_MarkAsNative) UFloatProperty(CPP_PROPERTY_BASE(RefreshRate, AWebcamRenderer), 0x0010000000000005);
				UProperty* NewProp_ResizeDeminsions = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("ResizeDeminsions"), RF_Public|RF_Transient|RF_MarkAsNative) UStructProperty(CPP_PROPERTY_BASE(ResizeDeminsions, AWebcamRenderer), 0x0010000000000005, Z_Construct_UScriptStruct_FVector2D());
				CPP_BOOL_PROPERTY_BITMASK_STRUCT(ShouldResize, AWebcamRenderer);
				UProperty* NewProp_ShouldResize = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("ShouldResize"), RF_Public|RF_Transient|RF_MarkAsNative) UBoolProperty(FObjectInitializer(), EC_CppProperty, CPP_BOOL_PROPERTY_OFFSET(ShouldResize, AWebcamRenderer), 0x0010000000000005, CPP_BOOL_PROPERTY_BITMASK(ShouldResize, AWebcamRenderer), sizeof(bool), true);
				UProperty* NewProp_CameraID = new(EC_InternalUseOnlyConstructor, OuterClass, TEXT("CameraID"), RF_Public|RF_Transient|RF_MarkAsNative) UIntProperty(CPP_PROPERTY_BASE(CameraID, AWebcamRenderer), 0x0010000000000005);
				OuterClass->AddFunctionToFunctionMapWithOverriddenName(Z_Construct_UFunction_AWebcamRenderer_OnNextVideoFrame(), "OnNextVideoFrame"); // 1634480913
				static TCppClassTypeInfo<TCppClassTypeTraits<AWebcamRenderer> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("WebcamRenderer.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
				MetaData->SetValue(NewProp_Data, TEXT("Category"), TEXT("Webcam"));
				MetaData->SetValue(NewProp_Data, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
				MetaData->SetValue(NewProp_Data, TEXT("ToolTip"), TEXT("The current data array"));
				MetaData->SetValue(NewProp_VideoTexture, TEXT("Category"), TEXT("Webcam"));
				MetaData->SetValue(NewProp_VideoTexture, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
				MetaData->SetValue(NewProp_VideoTexture, TEXT("ToolTip"), TEXT("The current video frame's corresponding texture"));
				MetaData->SetValue(NewProp_VideoSize, TEXT("Category"), TEXT("Webcam"));
				MetaData->SetValue(NewProp_VideoSize, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
				MetaData->SetValue(NewProp_VideoSize, TEXT("ToolTip"), TEXT("The videos width and height (width, height)"));
				MetaData->SetValue(NewProp_isStreamOpen, TEXT("Category"), TEXT("Webcam"));
				MetaData->SetValue(NewProp_isStreamOpen, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
				MetaData->SetValue(NewProp_isStreamOpen, TEXT("ToolTip"), TEXT("If the stream has succesfully opened yet"));
				MetaData->SetValue(NewProp_RefreshTimer, TEXT("Category"), TEXT("Webcam"));
				MetaData->SetValue(NewProp_RefreshTimer, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
				MetaData->SetValue(NewProp_RefreshTimer, TEXT("ToolTip"), TEXT("The refresh timer"));
				MetaData->SetValue(NewProp_RefreshRate, TEXT("Category"), TEXT("Webcam"));
				MetaData->SetValue(NewProp_RefreshRate, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
				MetaData->SetValue(NewProp_RefreshRate, TEXT("ToolTip"), TEXT("The rate at which the color data array and video texture is updated (in frames per second)"));
				MetaData->SetValue(NewProp_ResizeDeminsions, TEXT("Category"), TEXT("Webcam"));
				MetaData->SetValue(NewProp_ResizeDeminsions, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
				MetaData->SetValue(NewProp_ResizeDeminsions, TEXT("ToolTip"), TEXT("The targeted resize width and height (width, height)"));
				MetaData->SetValue(NewProp_ShouldResize, TEXT("Category"), TEXT("Webcam"));
				MetaData->SetValue(NewProp_ShouldResize, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
				MetaData->SetValue(NewProp_ShouldResize, TEXT("ToolTip"), TEXT("If the webcam images should be resized every frame"));
				MetaData->SetValue(NewProp_CameraID, TEXT("Category"), TEXT("Webcam"));
				MetaData->SetValue(NewProp_CameraID, TEXT("ModuleRelativePath"), TEXT("Public/WebcamRenderer.h"));
				MetaData->SetValue(NewProp_CameraID, TEXT("ToolTip"), TEXT("The device ID opened by the Video Stream"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(AWebcamRenderer, 4123085870);
	static FCompiledInDefer Z_CompiledInDefer_UClass_AWebcamRenderer(Z_Construct_UClass_AWebcamRenderer, &AWebcamRenderer::StaticClass, TEXT("/Script/OpenCVUE4"), TEXT("AWebcamRenderer"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AWebcamRenderer);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
PRAGMA_ENABLE_OPTIMIZATION
