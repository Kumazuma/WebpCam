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
//position의 좌표가 박스 안의 중심점을 기준으로 상하좌우 어디에 있는 지를 구한다. NE, NW, SE, SW, All은 결과값으로 나오지 않는다.
DirectionState GetDirection(const wxRect& rect, const wxPoint& position);
wxCursor GetSizingCursor(DirectionState state);