// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"


class FToolBarBuilder;
class SDockTab;

class FMinesweeperToolModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	// Tool window 
	static const FName MinesweeperTabName;
	TSharedRef<SDockTab> SpawnMinesweeperTab(const class FSpawnTabArgs& Args);
	static void OpenMinesweeperTab();

	// Toolbar button 
	void AddLevelEditorToolbarExtender();
	void AddToolbarButton(FToolBarBuilder& Builder);

	// Menu item under Tools 
	void RegisterToolsMenuEntry();

	TSharedPtr<class FExtender> ToolbarExtender;
};