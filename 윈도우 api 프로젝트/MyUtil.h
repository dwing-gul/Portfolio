#pragma once
#ifndef _MYUTIL
#define _MYUTIL

POINT CenterPoint(RECT& r);
int CheckStrikeX(RECT&, RECT&);
void DrawObject(HDC, RECT&, COLORREF, int, int ropCode = R2_XORPEN);
void DrawObject(HDC, RECT&, COLORREF, COLORREF, int, int ropCode = R2_XORPEN);
int HitTest(RECT&, RECT&);
void InitBlocks(RECT blocks[][5], RECT& bound, int xPos, int yPos);
#endif