#include<wx/wxprec.h>
#include"util.h"
DirectionState& operator |= (DirectionState& obj, DirectionState state)
{
	obj = (DirectionState)((uint8_t)obj | (uint8_t)state);
	return obj;
}
bool operator && (DirectionState& obj, DirectionState state)
{
	return ((uint8_t)obj & (uint8_t)state) != 0;
}
DirectionState GetDirection(const wxRect& rect, const wxPoint& position)
{
	DirectionState res = DirectionState::None;
	wxRect area = rect;
	wxPoint pos = position;
	pos.x -= area.x;
	pos.y -= area.y;
	area.x = 0;
	area.y = 0;
	
	float degree;
	degree = float(area.height) / float(area.width);
	//기울기 계산. \방향
	float calc1 = degree * pos.x;
	//기울기 계산. /방향
	float calc2 = area.height - degree * pos.x;
	char state = (calc2 < pos.y) << 1 | (calc1 < pos.y);
	
	if (area.Contains(pos))
	{
		switch (state)
		{
		case 0:
			res |= DirectionState::North;
			break;
		case 1:
			res |= DirectionState::West;
			break;
		case 2:
			res |= DirectionState::East;
			break;
		case 3:
			res |= DirectionState::South;
			break;
		}
	}
	return res;
}

wxCursor GetSizingCursor(DirectionState state)
{
	switch (state)
	{
	case DirectionState::NorthWest:
	case DirectionState::SouthEast:
		return wxCursor(wxCURSOR_SIZENWSE);
	case DirectionState::NorthEast:
	case DirectionState::SouthWest:
		return wxCursor(wxCURSOR_SIZENESW);
	case DirectionState::West:
	case DirectionState::East:
		return wxCursor(wxCURSOR_SIZEWE);
	case DirectionState::North:
	case DirectionState::South:
		return wxCursor(wxCURSOR_SIZENS);
	case DirectionState::All:
		return wxCursor(wxCURSOR_SIZING);
	default:
		return wxCursor(wxCURSOR_ARROW);
	}
}
