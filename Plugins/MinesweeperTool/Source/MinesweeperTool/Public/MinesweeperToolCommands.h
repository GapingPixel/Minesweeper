// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "MinesweeperToolStyle.h"

class FMinesweeperToolCommands : public TCommands<FMinesweeperToolCommands>
{
public:

	FMinesweeperToolCommands()
		: TCommands<FMinesweeperToolCommands>(TEXT("MinesweeperTool"), NSLOCTEXT("Contexts", "MinesweeperTool", "MinesweeperTool Plugin"), NAME_None, FMinesweeperToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};