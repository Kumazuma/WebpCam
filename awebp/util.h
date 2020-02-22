#pragma once
#include<wx/wx.h>
enum class DirectionState
{
	None = 0,
	North = 4,
	West = 8,
	East = 2,
	South = 1,
	NorthEast = North | East,
	NorthWest = North | West,
	SouthEast = South | East,
	SouthWest = South | West,
	All = North | South | East | West
};
DirectionState& operator |= (DirectionState& obj, DirectionState state);
bool operator && (DirectionState& obj, DirectionState state);
DirectionState GetDirection(const wxRect& rect, const wxPoint& position);
wxCursor GetSizingCursor(DirectionState state);