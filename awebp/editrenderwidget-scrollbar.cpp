#include<wx/wxprec.h>
#include "editrenderwidget.h"
void Edit::EditRenderWidget::AdjustScrollbars()
{
	Scroll(m_viewStart);
}

int Edit::EditRenderWidget::AdjectScroll(const int value, const  float factorInImage, const  float factorInScreen)
{
 	int res = value;
	//const int reg = -value;
	if (factorInImage > factorInScreen)//이미지가 화면보다 클 때
	{
		if (value > factorInScreen / 2)
		{
			return factorInScreen / 2;
		}
		if (factorInImage + value < factorInScreen / 2)//이미지 우하단 위치가 화면 축 크기의 반보다 크면
		{
			return factorInScreen / 2 - factorInImage;//역시 축 크기 반에 위치한 곳에 고정한다.
		}
	}
	else//이미지가 화면보다 작을 때
	{
		if (factorInImage / 2.f + value < 0)//이미지의 절반조차 화면에 안 뿌려지면 좌상단에서
		{
			return -factorInImage / 2;
		}
		if (factorInScreen - factorInImage / 2.f - value < 0)
		{
			return factorInScreen - factorInImage / 2.f;
		}
	}
	return res;
}

void Edit::EditRenderWidget::SetScrollRate(int x, int y)
{

}

wxPoint Edit::EditRenderWidget::GetViewStart()
{
	return m_viewStart;
}

void Edit::EditRenderWidget::Scroll(int x, int y)
{
	if (!m_selectedImage.IsOk())
	{
		return;
	}
	float imageWidth = m_selectedImage.GetWidth() * m_scale;
	float imageHeight = m_selectedImage.GetHeight() * m_scale;
	auto clientSize = GetClientSize();
	x = AdjectScroll(x, imageWidth, clientSize.GetWidth());
	y = AdjectScroll(y, imageHeight, clientSize.GetHeight());
	int xThumbPos = -x;
	int yThumbPos = -y;
	int verticalScrollRange = imageHeight;
	int horizontalScrollRange = imageWidth;
	//만약 음수를 넘어서면 스크롤바의 범위를 음수를 포함할 수 있도록 만든다.
 	if (xThumbPos < 0)
	{
		horizontalScrollRange -= xThumbPos;
		xThumbPos = 0;
	}
	if (yThumbPos < 0)
	{
		verticalScrollRange -= yThumbPos;
		yThumbPos = 0;
	}
	if (verticalScrollRange< yThumbPos + clientSize.y)
	{
		verticalScrollRange = yThumbPos + clientSize.y;
	}
	if (horizontalScrollRange < xThumbPos + clientSize.x)
	{
		horizontalScrollRange = xThumbPos + clientSize.x;
	}
	
	//만약에 
  	SetScrollbar(wxVERTICAL, yThumbPos, clientSize.y, verticalScrollRange);
	SetScrollbar(wxHORIZONTAL, xThumbPos, clientSize.x, horizontalScrollRange);
	SetScrollRate(5, 5);
	m_viewStart.x = x;
	m_viewStart.y = y;
}
