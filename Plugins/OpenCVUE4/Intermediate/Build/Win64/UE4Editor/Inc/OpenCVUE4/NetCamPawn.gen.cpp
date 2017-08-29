// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "Public/NetCamPawn.h"
PRAGMA_DISABLE_OPTIMIZATION
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNetCamPawn() {}
// Cross Module References
	OPENCVUE4_API UClass* Z_Construct_UClass_ANetCamPawn_NoRegister();
	OPENCVUE4_API UClass* Z_Construct_UClass_ANetCamPawn();
	ENGINE_API UClass* Z_Construct_UClass_APawn();
	UPackage* Z_Construct_UPackage__Script_OpenCVUE4();
// End Cross Module References
	void ANetCamPawn::StaticRegisterNativesANetCamPawn()
	{
	}
	UClass* Z_Construct_UClass_ANetCamPawn_NoRegister()
	{
		return ANetCamPawn::StaticClass();
	}
	UClass* Z_Construct_UClass_ANetCamPawn()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_APawn();
			Z_Construct_UPackage__Script_OpenCVUE4();
			OuterClass = ANetCamPawn::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= (EClassFlags)0x20900080u;


				static TCppClassTypeInfo<TCppClassTypeTraits<ANetCamPawn> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("HideCategories"), TEXT("Navigation"));
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("NetCamPawn.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("Public/NetCamPawn.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(ANetCamPawn, 3509763896);
	static FCompiledInDefer Z_CompiledInDefer_UClass_ANetCamPawn(Z_Construct_UClass_ANetCamPawn, &ANetCamPawn::StaticClass, TEXT("/Script/OpenCVUE4"), TEXT("ANetCamPawn"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ANetCamPawn);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
PRAGMA_ENABLE_OPTIMIZATION
