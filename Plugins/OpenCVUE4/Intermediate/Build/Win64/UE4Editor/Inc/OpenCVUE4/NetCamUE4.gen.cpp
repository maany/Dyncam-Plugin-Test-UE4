// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/NetCamUE4.h"
PRAGMA_DISABLE_OPTIMIZATION
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNetCamUE4() {}
// Cross Module References
	OPENCVUE4_API UClass* Z_Construct_UClass_ANetCamUE4_NoRegister();
	OPENCVUE4_API UClass* Z_Construct_UClass_ANetCamUE4();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_OpenCVUE4();
// End Cross Module References
	void ANetCamUE4::StaticRegisterNativesANetCamUE4()
	{
	}
	UClass* Z_Construct_UClass_ANetCamUE4_NoRegister()
	{
		return ANetCamUE4::StaticClass();
	}
	UClass* Z_Construct_UClass_ANetCamUE4()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_AActor();
			Z_Construct_UPackage__Script_OpenCVUE4();
			OuterClass = ANetCamUE4::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= (EClassFlags)0x20900080u;


				static TCppClassTypeInfo<TCppClassTypeTraits<ANetCamUE4> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("NetCamUE4.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/NetCamUE4.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(ANetCamUE4, 1457271654);
	static FCompiledInDefer Z_CompiledInDefer_UClass_ANetCamUE4(Z_Construct_UClass_ANetCamUE4, &ANetCamUE4::StaticClass, TEXT("/Script/OpenCVUE4"), TEXT("ANetCamUE4"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ANetCamUE4);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
PRAGMA_ENABLE_OPTIMIZATION
