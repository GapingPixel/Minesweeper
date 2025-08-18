// Copyright Epic Games, Inc. All Rights Reserved.
#include "MinesweeperTool.h"
#include "SMinesweeperWidget.h"

#include "Widgets/Docking/SDockTab.h"
#include "Styling/AppStyle.h"

#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ToolMenus.h"

const FName FMinesweeperToolModule::MinesweeperTabName(TEXT("MinesweeperToolTab"));

void FMinesweeperToolModule::StartupModule()
{
	//Tool window 
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		MinesweeperTabName,
		FOnSpawnTab::CreateRaw(this, &FMinesweeperToolModule::SpawnMinesweeperTab))
		.SetDisplayName(FText::FromString(TEXT("Minesweeper")))
		.SetTooltipText(FText::FromString(TEXT("Open the Minesweeper tool window.")))
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));

	//Add toolbar button that opens the tab
	AddLevelEditorToolbarExtender();

	//Menu entry (Tools → Minesweeper) 
	RegisterToolsMenuEntry();
}

void FMinesweeperToolModule::ShutdownModule()
{
	if (ToolbarExtender.IsValid() && FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditor.GetToolBarExtensibilityManager()->RemoveExtender(ToolbarExtender);
		ToolbarExtender.Reset();
	}

	if (UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::UnregisterOwner(this);
	}

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperTabName);
}

TSharedRef<SDockTab> FMinesweeperToolModule::SpawnMinesweeperTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		.Label(FText::FromString(TEXT("Minesweeper")))
		[
			SNew(SMinesweeperWidget) 
		];
}

void FMinesweeperToolModule::OpenMinesweeperTab()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
}

void FMinesweeperToolModule::AddLevelEditorToolbarExtender()
{
	if (!FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	}

	if (ToolbarExtender.IsValid())
		return; // avoid duplicates on hot reload

	FLevelEditorModule& LevelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");

	ToolbarExtender = MakeShared<FExtender>();
	const FToolBarExtensionDelegate Delegate =
		FToolBarExtensionDelegate::CreateRaw(this, &FMinesweeperToolModule::AddToolbarButton);

	// Hook at several anchors 
	ToolbarExtender->AddToolBarExtension("Compile",  EExtensionHook::After, nullptr, Delegate);
	ToolbarExtender->AddToolBarExtension("Play",     EExtensionHook::After, nullptr, Delegate);
	ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, nullptr, Delegate);

	LevelEditor.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FMinesweeperToolModule::AddToolbarButton(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(
		FUIAction(FExecuteAction::CreateStatic(&FMinesweeperToolModule::OpenMinesweeperTab)),
		NAME_None,
		FText::FromString(TEXT("Minesweeper")),
		FText::FromString(TEXT("Open the Minesweeper tool window.")),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.GameSettings")
	);
}

void FMinesweeperToolModule::RegisterToolsMenuEntry()
{
	if (!UToolMenus::IsToolMenuUIEnabled())
		return;

	if (UToolMenus* Menus = UToolMenus::Get())
	{
		if (UToolMenu* Tools = Menus->ExtendMenu("LevelEditor.MainMenu.Tools"))
		{
			FToolMenuSection& Sec = Tools->FindOrAddSection("Minesweeper");
			Sec.AddMenuEntry(
				"Minesweeper_OpenTool",
				FText::FromString(TEXT("Minesweeper")),
				FText::FromString(TEXT("Open the Minesweeper tool window.")),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateStatic(&FMinesweeperToolModule::OpenMinesweeperTab))
			);
		}
		Menus->RefreshAllWidgets();
	}
}

IMPLEMENT_MODULE(FMinesweeperToolModule, MinesweeperTool)