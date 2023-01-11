/*=============================================================================*/
// Copyright 2019-2020
// Authors: Yosha Vandaele
/*=============================================================================*/
// SpacePartitioning.h: Contains Cell and Cellspace which are used to partition a space in segments.
// Cells contain pointers to all the agents within.
// These are used to avoid unnecessary distance comparisons to agents that are far away.

// Heavily based on chapter 3 of "Programming Game AI by Example" - Mat Buckland
/*=============================================================================*/

#pragma once
#include <list>
#include <vector>
#include <iterator>
#include "../inc/EliteMath/EMath.h"


struct Rect final
{
	Rect() = default;

	Elite::Vector2 bottomLeft;
	float width;
	float height;
};

// --- Cell ---
// ------------
struct Cell
{
	Cell(float left, float bottom, float width, float height);
	Cell() = default;
	std::vector<Elite::Vector2> GetRectPoints() const;

	Rect boundingBox;
	bool isCellChecked;
	Elite::Vector2 center;
};

// --- Partitioned Space ---
// -------------------------
class CellSpace
{
public:
	CellSpace(float width, float height, int rows, int cols);
	CellSpace() = default;

	std::vector<Cell> GetCells() const;
	std::vector<Cell> GetPath() const;
	Cell GetNearestCellInPath(const Elite::Vector2& position) const;
	void CheckedCellInCells(int index);
	void CheckedCellInPath(int index);
	void ResetPath();

private:
	// Cells and properties
	std::vector<Cell> m_Cells;
	std::vector<Cell> m_Path;

	float m_SpaceWidth;
	float m_SpaceHeight;

	int m_NrOfRows;
	int m_NrOfCols;

	float m_CellWidth;
	float m_CellHeight;

	// Helper functions


};
