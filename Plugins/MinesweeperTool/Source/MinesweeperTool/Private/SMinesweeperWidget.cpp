// Fill out your copyright notice in the Description page of Project Settings.
#include "SMinesweeperWidget.h"

#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/CoreStyle.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "SMinesweeper"

void SMinesweeperWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.Padding(8)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.AutoHeight().Padding(0,0,0,8)
			[
				BuildToolbar()
			]

			+ SVerticalBox::Slot()
			.FillHeight(1.f)
			[
				SAssignNew(GridPanel, SUniformGridPanel)
				.SlotPadding(FMargin(2))
			]
		]
	];

	RebuildBoard();
}

TSharedRef<SWidget> SMinesweeperWidget::BuildToolbar()
{
	return SNew(SHorizontalBox)

	// Width
	+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(0,0,8,0)
	[
		SNew(STextBlock).Text(LOCTEXT("Width", "Width"))
	]
	+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(0,0,16,0)
	[
		SNew(SSpinBox<int32>)
		.MinValue(2).MaxValue(64)
		.Value_Lambda([this]{ return GridW; })
		.OnValueChanged_Lambda([this](const int32 V){ GridW = V; RebuildBoard();})
	]

	// Height
	+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(0,0,8,0)
	[
		SNew(STextBlock).Text(LOCTEXT("Height", "Height"))
	]
	+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(0,0,16,0)
	[
		SNew(SSpinBox<int32>)
		.MinValue(2).MaxValue(64)
		.Value_Lambda([this]{ return GridH; })
		.OnValueChanged_Lambda([this](const int32 V){ GridH = V; RebuildBoard();})
	]

	// Bombs
	+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(0,0,8,0)
	[
		SNew(STextBlock).Text(LOCTEXT("Bombs", "Bombs"))
	]
	+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(0,0,16,0)
	[
		SNew(SSpinBox<int32>)
		.MinValue(1)
		.MaxValue_Lambda([this]{ return FMath::Max(1, GridW*GridH - 1); })
		.Value_Lambda([this]{ return Bombs; })
		.OnValueChanged_Lambda([this](const int32 V){ Bombs = V; RebuildBoard();})
	]

	// New Game
	+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
	[
		SNew(SButton)
		.ButtonStyle(&FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"))
		.Text(LOCTEXT("NewGame", "Start / New Game"))
		.OnClicked(this, &SMinesweeperWidget::OnNewGameClicked)
	];
}

FReply SMinesweeperWidget::OnNewGameClicked()
{
	RebuildBoard();
	return FReply::Handled();
}

void SMinesweeperWidget::RebuildBoard()
{
	const int32 CellCount = GridW * GridH;
	Board.SetNum(CellCount);
	Revealed.SetNum(CellCount);
	for (int32 i = 0; i < CellCount; ++i)
	{
		Board[i] = 0;
		Revealed[i] = 0;
	}

	bGameOver = false;
	bStarted  = true;

	PlaceBombs();
	ComputeNumbers();
	RedrawTiles();
}

void SMinesweeperWidget::PlaceBombs()
{
	const int32 CellCount = GridW * GridH;
	const int32 MaxBombs = FMath::Clamp(Bombs, 1, CellCount - 1);

	TArray<int32> Indices;
	Indices.Reserve(CellCount);
	for (int32 i = 0; i < CellCount; ++i) Indices.Add(i);

	const FRandomStream RNG(FDateTime::Now().GetTicks());

	for (int32 b = 0; b < MaxBombs; ++b)
	{
		const int32 Pick = RNG.RandRange(0, Indices.Num() - 1);
		const int32 ChosenIdx = Indices[Pick];
		Indices.RemoveAtSwap(Pick, 1, false);

		Board[ChosenIdx] = -1; // bomb
	}
}

void SMinesweeperWidget::ComputeNumbers()
{
	for (int32 y = 0; y < GridH; ++y)
	for (int32 x = 0; x < GridW; ++x)
	{
		const int32 I = Index(x,y);
		if (Board[I] == -1) continue;
		Board[I] = static_cast<int8>(AdjacentBombs(x, y));
	}
}

int32 SMinesweeperWidget::AdjacentBombs(int32 X, int32 Y) const
{
	int32 Count = 0;
	for (int Dy = -1; Dy <= 1; ++Dy)
	for (int DX = -1; DX <= 1; ++DX)
	{
		if (DX == 0 && Dy == 0) continue;
		const int Nx = X + DX, Ny = Y + Dy;
		if (!InBounds(Nx, Ny)) continue;
		if (Board[Index(Nx, Ny)] == -1) ++Count;
	}
	return Count;
}

TSharedRef<SWidget> SMinesweeperWidget::BuildGrid()
{
	GridPanel->ClearChildren();

	for (int32 y = 0; y < GridH; ++y)
	{
		for (int32 x = 0; x < GridW; ++x)
		{
			TSharedRef<STextBlock> CellText = SNew(STextBlock)
				.Justification(ETextJustify::Center)
				.Text_Lambda([this, x, y]()
				{
					const int32 I = Index(x,y);
					if (Revealed[I] == 0) return FText::GetEmpty();
					const int8 V = Board[I];
					if (V == -1) return FText::FromString(TEXT("*")); // bomb
					if (V == 0)  return FText::FromString(TEXT(""));  // empty
					return FText::AsNumber(V);
				});

			TSharedRef<SButton> CellBtn = SNew(SButton)
				.ContentPadding(FMargin(4))
				.ButtonStyle(&FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"))
				.OnClicked(FOnClicked::CreateSP(this, &SMinesweeperWidget::OnTileClicked, x, y))
				[
					SNew(SBox)
					.WidthOverride(28.f)
					.HeightOverride(28.f)
					[
						CellText
					]
				];

			GridPanel->AddSlot(x, y)[ CellBtn ];
		}
	}

	return GridPanel.ToSharedRef();
}

void SMinesweeperWidget::RedrawTiles()
{
	BuildGrid();
}

FReply SMinesweeperWidget::OnTileClicked(const int32 X, const int32 Y)
{
	if (!bStarted || bGameOver) return FReply::Handled();
	const int32 I = Index(X,Y);
	if (Revealed[I]) return FReply::Handled();

	if (Board[I] == -1)
	{
		bGameOver = true;
		for (int32 Idx = 0; Idx < Board.Num(); ++Idx)
		{
			if (Board[Idx] == -1) Revealed[Idx] = 1;
		}
		RedrawTiles();
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("GameOver", "Boom! Game Over."));
		return FReply::Handled();
	}

	RevealRecursive(X, Y);
	RedrawTiles();
	return FReply::Handled();
}

void SMinesweeperWidget::RevealRecursive(const int32 X, const int32 Y)
{
	if (!InBounds(X, Y)) return;
	const int32 I = Index(X,Y);
	if (Revealed[I]) return;

	Revealed[I] = 1;

	if (Board[I] == 0)
	{
		for (int Dy = -1; Dy <= 1; ++Dy)
		for (int DX = -1; DX <= 1; ++DX)
		{
			if (DX == 0 && Dy == 0) continue;
			const int Nx = X + DX, Ny = Y + Dy;
			if (InBounds(Nx, Ny) && Board[Index(Nx, Ny)] != -1)
			{
				RevealRecursive(Nx, Ny);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
