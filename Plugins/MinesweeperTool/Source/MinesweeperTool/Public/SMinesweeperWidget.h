// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SUniformGridPanel;

class SMinesweeperWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMinesweeperWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	//Config
	int32 GridW = 10;
	int32 GridH = 10;
	int32 Bombs = 10;

	//Game State
	bool bGameOver = false;
	bool bStarted  = false;

	//-1 = bomb, else [0..8] 
	TArray<int8>  Board;
	TArray<uint8> Revealed;

	TSharedPtr<SUniformGridPanel> GridPanel;
	
	TSharedRef<SWidget> BuildToolbar();
	TSharedRef<SWidget> BuildGrid();
	void RedrawTiles();

	FReply OnNewGameClicked();
	FReply OnTileClicked(const int32 X, const int32 Y);

	void RebuildBoard();
	void PlaceBombs();
	void ComputeNumbers();
	void RevealRecursive(const int32 X, const int32 Y);
	int32 AdjacentBombs(const int32 X, const int32 Y) const;

	FORCEINLINE int32 Index(const int32 X, const int32 Y) const { return Y * GridW + X; }
	FORCEINLINE bool  InBounds(const int32 X, const int32 Y) const { return X >= 0 && X < GridW && Y >= 0 && Y < GridH; }
};