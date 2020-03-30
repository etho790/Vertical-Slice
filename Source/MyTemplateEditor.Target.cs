// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyTemplateEditorTarget : TargetRules
{
	public MyTemplateEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        bUsesSteam = true;
		ExtraModuleNames.Add("MyTemplate");
	}
}
