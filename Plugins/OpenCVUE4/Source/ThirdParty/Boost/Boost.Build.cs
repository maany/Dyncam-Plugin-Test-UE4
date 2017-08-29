// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class Boost : ModuleRules
{
	private string ThirdPartyPath
	{ 
    	get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); } 
	}
	public Boost(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			LoadBoost(Target);
		}
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            //PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libExampleLibrary.dylib"));
        }
	}
	public bool LoadBoost(ReadOnlyTargetRules Target)
	{
    	// Start OpenCV linking here!
    	bool isLibrarySupported = false;
 
    	// Create OpenCV Path 
	    string BoostPath = Path.Combine(ThirdPartyPath, "Boost");
 
    	// Get Library Path 
    	string LibPath = "";
    	bool isdebug = Target.Configuration == UnrealTargetConfiguration.Debug && BuildConfiguration.bDebugBuildsActuallyUseDebugCRT;
    	if (Target.Platform == UnrealTargetPlatform.Win64)
    	{
        	LibPath = Path.Combine(BoostPath, "lib");
	        isLibrarySupported = true;
    	}
    	else
    	{
            //TODO add to Wiki
            string Err = string.Format("{0} dedicated server is made to depend on {1}. We want to avoid this, please correct module dependencies.", Target.Platform.ToString(), this.ToString()); System.Console.WriteLine(Err);
	    }
	
    	if (isLibrarySupported)
    	{
        	//Add Include path 
        	PublicIncludePaths.AddRange(new string[] { Path.Combine(BoostPath, "include") });
 
        	// Add Library Path 
        	PublicLibraryPaths.Add(LibPath);

            //Add Static Libraries
            //PublicAdditionalLibraries.Add("libDyncamUnreal.lib");
            PublicAdditionalLibraries.Add("boost_date_time-vc140-mt-gd-1_60.lib");
            PublicAdditionalLibraries.Add("boost_regex-vc140-mt-gd-1_60.lib");
            PublicAdditionalLibraries.Add("boost_system-vc140-mt-gd-1_60.lib");
            //PublicAdditionalLibraries.Add("CppReact.lib");

            //Add Dynamic Libraries
            //PublicDelayLoadDLLs.Add("libDyncamUnreal.dll");
            //PublicDelayLoadDLLs.Add("freenect2d.dll");
        	//PublicDelayLoadDLLs.Add("glfw3.dll");
        	//PublicDelayLoadDLLs.Add("libusb-1.0.dll");
        	//PublicDelayLoadDLLs.Add("turbojpeg.dll");
            PublicDelayLoadDLLs.Add("boost_date_time-vc140-mt-gd-1_60.dll");
            PublicDelayLoadDLLs.Add("boost_regex-vc140-mt-gd-1_60.dll");
            PublicDelayLoadDLLs.Add("boost_system-vc140-mt-gd-1_60.dll");
        }
        //TODO add to Wiki
    	//Definitions.Add(string.Format("WITH_OPENCV_BINDING={0}", isLibrarySupported ? 1 : 0));
        Definitions.Add(string.Format("WIN32_LEAN_AND_MEAN"));
	    return isLibrarySupported;
	}
}
