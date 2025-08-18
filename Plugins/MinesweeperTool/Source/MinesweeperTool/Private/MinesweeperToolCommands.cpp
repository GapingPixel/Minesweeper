// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperToolCommands.h"

#define LOCTEXT_NAMESPACE "FMinesweeperToolModule"

void FMinesweeperToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MinesweeperTool", "Bring up MinesweeperTool window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
