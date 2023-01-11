#include "stdafx.h"
#include "SpacePartitioning.h"

// --- Cell ---
// ------------
Cell::Cell(float left, float bottom, float width, float height)
	: isCellChecked(false)
	, center(left + (width / 2.f), bottom + (height / 2.f))
{
	boundingBox.bottomLeft = { left, bottom };
	boundingBox.width = width;
	boundingBox.height = height;
}

std::vector<Elite::Vector2> Cell::GetRectPoints() const
{
	auto left = boundingBox.bottomLeft.x;
	auto bottom = boundingBox.bottomLeft.y;
	auto width = boundingBox.width;
	auto height = boundingBox.height;

	std::vector<Elite::Vector2> rectPoints =
	{
		{ left , bottom  },
		{ left , bottom + height  },
		{ left + width , bottom + height },
		{ left + width , bottom  },
	};

	return rectPoints;
}

// --- Partitioned Space ---
// -------------------------
CellSpace::CellSpace(float width, float height, int rows, int cols)
	: m_SpaceWidth(width)
	, m_SpaceHeight(height)
	, m_NrOfRows(rows)
	, m_NrOfCols(cols)
	, m_CellWidth(width / cols)
	, m_CellHeight(height / rows)
{
	// add to m_Cells and m_Path.

	int num{ 3 }; // was 6 for him.

	for (int i{ -m_NrOfRows / 2 }; i < m_NrOfRows / 2; ++i)
	{
		for (int j{ -m_NrOfCols / 2 }; j < m_NrOfCols / 2; ++j)
		{
			Cell cell{ m_CellWidth * j, m_CellHeight * i, m_CellWidth, m_CellHeight };
			if (
				(abs(j) == num && i <= num && i >= -num) || // sides
				 abs(i) == num && j <= num && j >= -num  || // top
				 (i == -4 && j <= 3 && j >= -4) || // bottom
			     (i == 2 && j <= 1 && j >= -2)  || // top
			     (j == 2 && i <= 2 && i >= -2)  || // right
			     (j == -4 && i <= 3 && i >= -3) || // left
			     (i >= -1 && i <= 0 && j >= -1 && j <= 0))
			{
				m_Path.push_back(cell);
			}
			else
			{
				m_Cells.push_back(cell);
			}
		}
	}
}

std::vector<Cell> CellSpace::GetCells() const
{
	return m_Cells;
}

std::vector<Cell> CellSpace::GetPath() const
{
	return m_Path;
}

Cell CellSpace::GetNearestCellInPath(const Elite::Vector2& position) const
{
	float smallestDistance{ FLT_MAX };
	Cell nearestCell{m_Cells[ (m_NrOfCols * m_NrOfRows / 2) - 1 ]};

	for (auto cell : m_Path)
	{
		auto distance = Elite::Distance(cell.center, position);
		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			nearestCell = cell;
		}
	}

	return nearestCell;
}

void CellSpace::CheckedCell(const Elite::Vector2& position) const
{
	// mark the cell as checked. could be another effecient better method.
}