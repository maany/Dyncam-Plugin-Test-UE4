// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class OpenCV : ModuleRules
{
	private string ThirdPartyPath
	{ 
    	get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); } 
	}
	public OpenCV(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			LoadOpenCV(Target);
		}
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            //PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libExampleLibrary.dylib"));
        }
	}
	public bool LoadOpenCV(ReadOnlyTargetRules Target)
	{
    	// Start OpenCV linking here!
    	bool isLibrarySupported = false;
 
    	// Create OpenCV Path 
	    string OpenCVPath = Path.Combine(ThirdPartyPath, "OpenCV");
 
    	// Get Library Path 
    	string LibPath = "";
    	bool isdebug = Target.Configuration == UnrealTargetConfiguration.Debug && BuildConfiguration.bDebugBuildsActuallyUseDebugCRT;
    	if (Target.Platform == UnrealTargetPlatform.Win64)
    	{
        	LibPath = Path.Combine(OpenCVPath, "x64", "vc14","lib");
	        isLibrarySupported = true;
    	}
    	else
    	{
        	string Err = string.Format("{0} dedicated server is made to depend on {1}. We want to avoid this, please correct module dependencies.", Target.Platform.ToString(), this.ToString()); System.Console.WriteLine(Err);
	    }
	
    	if (isLibrarySupported)
    	{
        	//Add Include path 
        	PublicIncludePaths.AddRange(new string[] { Path.Combine(OpenCVPath, "include") });
 
        	// Add Library Path 
        	PublicLibraryPaths.Add(LibPath);
 
        	//Add Static Libraries
        	PublicAdditionalLibraries.Add("opencv_world330.lib");
 
        	//Add Dynamic Libraries
        	PublicDelayLoadDLLs.Add("opencv_world330.dll");
        	PublicDelayLoadDLLs.Add("opencv_ffmpeg330_64.dll");
    	}
 
    	Definitions.Add(string.Format("WITH_OPENCV_BINDING={0}", isLibrarySupported ? 1 : 0));
 
	    return isLibrarySupported;
	}
}
