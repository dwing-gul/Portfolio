#include <Windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "MyUtil.h"

POINT CenterPoint(RECT& r)
{
	POINT p;
	p.x = (r.left + r.right) / 2;
	p.y = (r.top + r.bottom) / 2;

	return p;
}

int CheckStrikeX(RECT& r, RECT& bound)
{
	if (r.left <= bound.left)
		return 1;
	else if (r.right >= bound.right)
		return 2;
	else
		return 0;
}

void DrawObject(HDC hdc, RECT& r, COLORREF color, int type, int ropCode)
{
	DrawObject(hdc, r, color, color, type);
}

void DrawObject(HDC hdc, RECT& r, COLORREF penC, COLORREF brushC, int type, int ropCode)
{
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	SetROP2(hdc, ropCode);
	hPen = CreatePen(PS_SOLID, 1, penC);
	hOldPen = (HPEN)SelectObject(hdc, hPen);
	hBrush = CreateSolidBrush(brushC);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	switch (type)
	{
	case 0:
		Rectangle(hdc, r.left, r.top, r.right, r.bottom);
		break;
	case 1:
		Ellipse(hdc, r.left, r.top, r.right, r.bottom);
		break;
	}

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

int HitTest(RECT& bar, RECT& blocks)
{
	RECT temp;
	int w, h;

	if (IntersectRect(&temp, &bar, &blocks))
	{
		w = temp.right - temp.left;
		h = temp.bottom - temp.top;

		if (w < h)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else
		return 0;
}

void InitBlocks(RECT blocks[][5], RECT& bound, int xPos, int yPos)
{
	RECT r, t;
	int w, h;

	r = bound;
	r.right /= 45;

	r.top = yPos;
	r.bottom = r.top + 20;

	w = r.right - r.left - 2;
	h = r.bottom - r.top - 2;

	OffsetRect(&r, xPos, h);
	for (int i = 0; i < 5; i++)
	{
		t = r;
		OffsetRect(&r, 0, h);

		for (int j = 0; j < 5; j++)
		{
			blocks[i][j] = t;
			OffsetRect(&t, w, 0);
		}
	}
}