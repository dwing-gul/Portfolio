#include <windows.h>
#include <TCHAR.H>
#include <stdio.h>
#include "MyUtil.h"
#include "resource.h"

static int m_Patterns[8][5][5] = { 
	{ {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 1, 1, 1, 1} },
	{ {1, 1, 1, 0, 0}, {1, 1, 1, 0, 0}, {1, 1, 1, 0, 0}, {1, 1, 1, 0, 0}, {1, 1, 1, 0, 0} }, 
	{ {0, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 1, 1, 0, 0}, {1, 1, 1, 1, 0} }, 
	{ {1, 1, 1, 1, 1}, {0, 1, 1, 1, 1}, {0, 0, 1, 1, 1}, {0, 0, 0, 1, 1}, {0, 0, 0, 0, 1} }, 
	{ {1, 1, 1, 1, 1}, {1, 1, 0, 1, 1}, {1, 0, 0, 0, 1}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} },
	{ {1, 1, 1, 1, 1}, {1, 1, 0, 1, 1}, {1, 0, 0, 1, 1}, {0, 0, 0, 1, 1}, {0, 0, 0, 1, 0} }, 
	{ {1, 1, 1, 1, 1}, {0, 1, 1, 1, 1}, {0, 1, 0, 1, 1}, {0, 1, 0, 1, 0}, {0, 1, 0, 1, 0} }, 
	{ {1, 1, 1, 1, 1}, {1, 1, 0, 1, 1}, {1, 0, 0, 1, 1}, {0, 0, 0, 1, 1}, {0, 0, 0, 1, 1} } };
static int patternID[8];
static int xPos[890];

int HitTest(HDC hdc, RECT& ball, RECT blocks[][5], int index);
void DrawBlocks(HDC hdc, RECT blocks[][5], RECT&, int yPosition, int xPosition, int index);
void PlayEffectSound(UINT Sound);

enum tag_Status { GAMEOVER, RUNNING, PAUSE };
tag_Status GameStatus;
BOOL bQuiet = FALSE;
int xPosIndex[8]; // 0 ~ 890
int yPos = 0; // -20 ~ 642

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = _T("Meteor");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		300, 100, 1000, 700,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd; // hWnd 정보도 전역변수에 저장!

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	static RECT Blocks[8][5][5];

	static COLORREF barColor;

	static POINT p;
	static RECT barR, clientR;
	static BOOL dragFlag = FALSE;
	static TCHAR sTime[128];

	static int sec = 0;
	static int alphaX;
	int hitResult;
	static int yPosition[8];

	switch (iMessage) {
	case WM_CREATE:
		barColor = RGB(255, 0, 0);
		GetClientRect(hWnd, &clientR);
		srand(GetTickCount64());
		p = CenterPoint(clientR);
		for (int i = 0; i < 8; i++)
		{
			xPosIndex[i] = rand() % 890;
			yPosition[i] = yPos;
			patternID[i] = rand() % 8;
		}
		SetRect(&barR, p.x - 15, p.y - 15, p.x + 15, p.y + 15);
		OffsetRect(&barR, 0, 300);
		for (int i = 0; i < 890; i++)
		{
			xPos[i] = i;
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_GAME_START:
			if (GameStatus != GAMEOVER) {
				break;
			}
			GameStatus = RUNNING;
			SetTimer(hWnd, 1, 10, NULL);
			SetTimer(hWnd, 9, 1000, NULL);
			SendMessage(hWnd, WM_TIMER, 9, 0);
			break;
		case IDM_GAME_PAUSE:
			if (GameStatus == RUNNING) {
				GameStatus = PAUSE;
				SetTimer(hWnd, 1, 10, NULL);
				SetTimer(hWnd, 9, 1000, NULL);
			}
			else if (GameStatus == PAUSE) {
				GameStatus = RUNNING;
				SetTimer(hWnd, 1, 10, NULL);
				SetTimer(hWnd, 9, 1000, NULL);
			}
			break;
		case IDM_GAME_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_GAME_QUIET:
			bQuiet = !bQuiet;
			break;
		}
		return 0;
	case WM_INITMENU:
		CheckMenuItem((HMENU)wParam, IDM_GAME_QUIET,
			MF_BYCOMMAND | (bQuiet ? MF_CHECKED : MF_UNCHECKED));
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawObject(hdc, barR, barColor, 0);
		TextOut(hdc, 930, 0, sTime, _tcslen(sTime));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		if (GameStatus != RUNNING)
			return 0;
		switch (wParam)
		{
		case VK_LEFT:
			alphaX = -1;
			if (CheckStrikeX(barR, clientR) == 1)
			{
				break;
			}
			else
			{
				PlayEffectSound(IDR_WAVE1);
				OffsetRect(&barR, 20 * alphaX, 0);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case VK_RIGHT:
			alphaX = 1;
			if (CheckStrikeX(barR, clientR) == 2)
			{
				break;
			}
			else
			{
				PlayEffectSound(IDR_WAVE1);
				OffsetRect(&barR, 20 * alphaX, 0);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			hdc = GetDC(hWnd);
			if (GameStatus == RUNNING)
			{
				hitResult = HitTest(hdc, barR, Blocks[0], patternID[0]);

				if (yPosition[0] < 643)
				{
					yPosition[0] += 5;
					DrawBlocks(hdc, Blocks[0], clientR, yPosition[0], xPosIndex[0], patternID[0]);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				else
				{
					yPosition[0] = yPos;
					xPosIndex[0] = rand() % 890;
					patternID[0] = rand() % 8;
				}

				if (yPosition[0] == 150)
				{
					SetTimer(hWnd, 2, 10, NULL);
				}

				if (hitResult)
				{
					PlayEffectSound(IDR_WAVE2);
					InvalidateRect(hWnd, &barR, TRUE);
					GameStatus = GAMEOVER;
				}
			}
			ReleaseDC(hWnd, hdc);
			break;
		case 2:
			hdc = GetDC(hWnd);
			if (GameStatus == RUNNING)
			{
				hitResult = HitTest(hdc, barR, Blocks[1], patternID[1]);

				if (yPosition[1] < 643)
				{
					yPosition[1] += 5;
					DrawBlocks(hdc, Blocks[1], clientR, yPosition[1], xPosIndex[1], patternID[1]);
				}
				else
				{
					yPosition[1] = yPos;
					xPosIndex[1] = rand() % 890;
					patternID[1] = rand() % 8;
				}

				if (yPosition[1] == 150)
				{
					SetTimer(hWnd, 3, 10, NULL);
				}

				if (hitResult)
				{
					PlayEffectSound(IDR_WAVE2);
					InvalidateRect(hWnd, &barR, TRUE);
					GameStatus = GAMEOVER;
				}
			}
			ReleaseDC(hWnd, hdc);
			break;
		case 3 :
			hdc = GetDC(hWnd);
			if (GameStatus == RUNNING)
			{
				hitResult = HitTest(hdc, barR, Blocks[2], patternID[2]);

				if (yPosition[2] < 643)
				{
					yPosition[2] += 5;
					DrawBlocks(hdc, Blocks[2], clientR, yPosition[2], xPosIndex[2], patternID[2]);
				}
				else
				{
					yPosition[2] = yPos;
					xPosIndex[2] = rand() % 890;
					patternID[2] = rand() % 8;
				}

				if (yPosition[2] == 150)
				{
					SetTimer(hWnd, 4, 10, NULL);
				}

				if (hitResult)
				{
					PlayEffectSound(IDR_WAVE2);
					InvalidateRect(hWnd, &barR, TRUE);
					GameStatus = GAMEOVER;
				}
			}
			ReleaseDC(hWnd, hdc);
			break;
		case 4 :
			hdc = GetDC(hWnd);
			if (GameStatus == RUNNING)
			{
				hitResult = HitTest(hdc, barR, Blocks[3], patternID[3]);

				if (yPosition[3] < 643)
				{
					yPosition[3] += 5;
					DrawBlocks(hdc, Blocks[3], clientR, yPosition[3], xPosIndex[3], patternID[3]);
				}
				else
				{
					yPosition[3] = yPos;
					xPosIndex[3] = rand() % 890;
					patternID[0] = rand() % 8;
				}

				if (yPosition[3] == 150)
				{
					SetTimer(hWnd, 5, 10, NULL);
				}

				if (hitResult)
				{
					PlayEffectSound(IDR_WAVE2);
					InvalidateRect(hWnd, &barR, TRUE);
					GameStatus = GAMEOVER;
				}
			}
			ReleaseDC(hWnd, hdc);
			break;
		case 5 :
			hdc = GetDC(hWnd);
			if (GameStatus == RUNNING)
			{
				hitResult = HitTest(hdc, barR, Blocks[4], patternID[4]);

				if (yPosition[4] < 643)
				{
					yPosition[4] += 5;
					DrawBlocks(hdc, Blocks[4], clientR, yPosition[4], xPosIndex[4], patternID[4]);
				}
				else
				{
					yPosition[4] = yPos;
					xPosIndex[4] = rand() % 890;
					patternID[4] = rand() % 8;
				}

				if (yPosition[4] == 150)
				{
					SetTimer(hWnd, 6, 10, NULL);
				}

				if (hitResult)
				{
					PlayEffectSound(IDR_WAVE2);
					InvalidateRect(hWnd, &barR, TRUE);
					GameStatus = GAMEOVER;
				}
			}
			ReleaseDC(hWnd, hdc);
			break;
		case 6:
			hdc = GetDC(hWnd);
			if (GameStatus == RUNNING)
			{
				hitResult = HitTest(hdc, barR, Blocks[5], patternID[5]);

				if (yPosition[5] < 643)
				{
					yPosition[5] += 5;
					DrawBlocks(hdc, Blocks[5], clientR, yPosition[5], xPosIndex[5], patternID[5]);
				}
				else
				{
					yPosition[5] = yPos;
					xPosIndex[5] = rand() % 890;
					patternID[5] = rand() % 8;
				}

				if (yPosition[5] == 150)
				{
					SetTimer(hWnd, 7, 10, NULL);
				}

				if (hitResult)
				{
					PlayEffectSound(IDR_WAVE2);
					InvalidateRect(hWnd, &barR, TRUE);
					GameStatus = GAMEOVER;
				}
			}
			ReleaseDC(hWnd, hdc);
			break;
		case 7:
			hdc = GetDC(hWnd);
			if (GameStatus == RUNNING)
			{
				hitResult = HitTest(hdc, barR, Blocks[6], patternID[6]);

				if (yPosition[6] < 643)
				{
					yPosition[6] += 5;
					DrawBlocks(hdc, Blocks[6], clientR, yPosition[6], xPosIndex[6], patternID[6]);
				}
				else
				{
					yPosition[6] = yPos;
					xPosIndex[6] = rand() % 890;
					patternID[6] = rand() % 8;
				}

				if (yPosition[6] == 150)
				{
					SetTimer(hWnd, 8, 10, NULL);
				}

				if (hitResult)
				{
					PlayEffectSound(IDR_WAVE2);
					InvalidateRect(hWnd, &barR, TRUE);
					GameStatus = GAMEOVER;
				}
			}
			ReleaseDC(hWnd, hdc);
			break;
		case 8:
			hdc = GetDC(hWnd);
			if (GameStatus == RUNNING)
			{
				hitResult = HitTest(hdc, barR, Blocks[7], patternID[7]);

				if (yPosition[7] < 643)
				{
					yPosition[7] += 5;
					DrawBlocks(hdc, Blocks[7], clientR, yPosition[7], xPosIndex[7], patternID[7]);
				}
				else
				{
					yPosition[7] = yPos;
					xPosIndex[7] = rand() % 890;
					patternID[7] = rand() % 8;
				}

				if (hitResult)
				{
					PlayEffectSound(IDR_WAVE2);
					InvalidateRect(hWnd, &barR, TRUE);
					GameStatus = GAMEOVER;
				}
			}
			ReleaseDC(hWnd, hdc);
			break;
		case 9 :
			if (GameStatus == RUNNING)
			{
				_stprintf_s(sTime, _T("%04d초"), sec);
				sec++;
			}
		}
		return 0;
	case WM_DESTROY:
		for (int i = 1; i <= 9; i++)
		{
			KillTimer(hWnd, i);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

int HitTest(HDC hdc, RECT& bar, RECT blocks[][5], int index)
{
	int result;
	COLORREF color = 255;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (m_Patterns[index][i][j])
			{
				result = HitTest(bar, blocks[i][j]);

				if (result)
				{
					m_Patterns[index][i][j] = 0;
					DrawObject(hdc, blocks[i][j], RGB(0, 0, 0), RGB(0, color, color), 0);
					DrawObject(hdc, bar, RGB(0, 0, 0), RGB(0, color, color), 0);
					return result;
				}
			}
		}
		color -= 25;
	}
	return 0;
}

void DrawBlocks(HDC hdc, RECT blocks[][5], RECT& bound, int yPosition, int xPosition, int index)
{
	InitBlocks(blocks, bound, xPos[xPosition], yPosition);

	COLORREF color = 255;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (m_Patterns[index][i][j])
			{
				DrawObject(hdc, blocks[i][j], RGB(0, 0, 0), RGB(0, color, color), 0);
			}
		}
		color -= 25;
	}
}

void PlayEffectSound(UINT Sound)
{
	if (!bQuiet) {
		PlaySound(MAKEINTRESOURCE(Sound), g_hInst, SND_RESOURCE | SND_ASYNC);
	}
}