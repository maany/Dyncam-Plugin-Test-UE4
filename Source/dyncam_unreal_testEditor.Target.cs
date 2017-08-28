// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class dyncam_unreal_testEditorTarget : TargetRules
{
	public dyncam_unreal_testEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "dyncam_unreal_test" } );
	}
}
