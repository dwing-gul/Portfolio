# define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <processthreadsapi.h>
#include <synchapi.h>
#include <threadpoolapiset.h>
using namespace std;

#define	MAX_LINE			1000
#define DELIM			" \r\t"

#define PUZZLE_SIZE 		9
#define MAX_THREAD 			11


/* example valid puzzle */
int g_puzzle[PUZZLE_SIZE + 1][PUZZLE_SIZE + 1] = {
	{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
	{ -1, 5, 3, 4, 6, 7, 8, 9, 1, 2 },
	{ -1, 6, 7, 2, 1, 9, 5, 3, 4, 8 },
	{ -1, 1, 9, 8, 3, 4, 2, 5, 6, 7 },
	{ -1, 8, 5, 9, 7, 6, 1, 4, 2, 3 },
	{ -1, 4, 2, 6, 8, 5, 3, 7, 9, 1 },
	{ -1, 7, 1, 3, 9, 2, 4, 8, 5, 6 },
	{ -1, 9, 6, 1, 5, 3, 7, 2, 8, 4 },
	{ -1, 2, 8, 7, 4, 1, 9, 6, 3, 5 },
	{ -1, 3, 4, 5, 2, 8, 6, 1, 7, 9 }
};

int rowOverlap; // 열 부분에서 겹치는 부분을 체크하기 위한 전역 변수
int columnOverlap; // 행 부분에서 겹치는 부분을 체크하기 위한 전역 변수
int sectionOverlap; // 섹션 부분에서 겹치는 부분을 체크하기 위한 전역 변수

void read_puzzle(int p_puzzle[PUZZLE_SIZE + 1][PUZZLE_SIZE + 1], char* filename);
void PrintPuzzle();
DWORD WINAPI RowCheck(LPVOID lpParam); // 스도쿠의 열 부분을 확인하기 위한 함수
DWORD WINAPI ColumnCheck(LPVOID lpParam); // 스토쿠의 행 부분을 확인하기 위한 함수
DWORD WINAPI section_check(LPVOID lpParam); // 스도쿠의 섹션 부분을 확인하기 위한 함수

int main(int argc, char** argv)
{
	DWORD countOfThread = 0; // 스레드 행렬의 인덱스를 파악하기 위한 변수이다.
	DWORD dwThreadID[MAX_THREAD]; // 각 스레드의 주소를 얻는 변수이다.
	HANDLE hThread[MAX_THREAD]; // 스레드를 실행시키기 위해 받는 행렬 변수이다.
	int rightCheck = 1; // 각 부분이 다 맞는 지 확인된 결과를 연산하기 위한 변수
	bool bIsRight = false; // rightCheck에서 연산된 결과를 저장하는 변수

	printf(argv[1]);
	//#####################################################
	//################ Read Puzzle #######################
	//#####################################################
	if (argc < 2) { printf("ERROR: input file! ...\n"); return 1; }
	read_puzzle(g_puzzle, argv[1]);

	//#####################################################
	//################ Print Puzzle #######################
	//#####################################################
	printf("[Sudoku puzzle]:");
	PrintPuzzle();
	printf("\n");

	//#####################################################
	//################ Rows Check #########################
	//#####################################################
	hThread[0] = CreateThread(NULL, 0, RowCheck, NULL, 0, &dwThreadID[countOfThread]); // 스레드를 통해 해당 함수를 실행 시킨다.
	WaitForSingleObject(hThread[0], 3); // 해당 스레드의 함수 실행후 지정된 시간만큼 대기 후 다음 줄을 실행한다.
	rightCheck *= rowOverlap; // 위의 함수에 따른 결과에 따라 저장된 값을 rightCheck에 곱 연산 후 저장한다.
	countOfThread++; // 다음 스레드로 넘어가기 위한 카운트

	//#####################################################
	//################ Columns Check ######################
	//#####################################################
	hThread[1] = CreateThread(NULL, 0, ColumnCheck, NULL, 0, &dwThreadID[countOfThread]); // 스레드를 통해 해당 함수를 실행 시킨다.
	WaitForSingleObject(hThread[1], 3);
	countOfThread++; // 다음 스레드로 넘어가기 위한 카운트
	rightCheck *= columnOverlap; // 위의 함수에 따른 결과에 따라 저장된 값을 rightCheck에 곱 연산 후 저장한다.

	//#####################################################
	//################ 9 Sections Check #####################
	//#####################################################
	while (countOfThread < MAX_THREAD)
	{
		hThread[countOfThread] = CreateThread(NULL, 0, section_check, (LPVOID)countOfThread, 0, &dwThreadID[countOfThread]); // 스레드를 통해 해당 함수를 실행 시킨다.
		WaitForSingleObject(hThread[countOfThread], 3);
		rightCheck *= sectionOverlap;  // 위의 함수에 따른 결과에 따라 저장된 값을 rightCheck에 곱 연산 후 저장한다.
		countOfThread++;
	}

	bIsRight = rightCheck; // 최종결과를 bool 변수에 저장한다.

	//##############################################################
	//################ Print Checking Result #######################
	//##############################################################
	if (bIsRight == true)
		printf("[Sudoku puzzle Check Result]: Right\n\n");
	else
		printf("[Sudoku puzzle Check Result]: Wrong\n\n");

	for (DWORD i = 0; i < MAX_THREAD; i++)
	{
		ExitThread(dwThreadID[i]); // 생성된 스레드를 모두 종료한다.
	}

	return 0;
}
//#########################################################################################
void read_puzzle(int p_puzzle[PUZZLE_SIZE + 1][PUZZLE_SIZE + 1], char* filename)
{
	char line[MAX_LINE], * tok;
	FILE* fp;

	int iRow = 1, iCol = 1;

	if (!(fp = fopen(filename, "r"))) { printf("ERROR: file open\n"); exit(0); }

	while (fgets(line, MAX_LINE, fp))
	{
		tok = strtok(line, DELIM);
		iCol = 1;
		do
		{
			p_puzzle[iRow][iCol++] = atoi(tok);

		} while (tok = strtok(NULL, DELIM));
		iRow++;
	}
	fclose(fp);
}
//#########################################################################################
void PrintPuzzle()
{
	int i, j;

	printf("\n%c", 1);
	for (i = 0; i < 8; i++) printf("%c%c%c%c", 6, 6, 6, 22);
	printf("%c%c%c%c\n", 6, 6, 6, 2);


	for (i = 1; i < PUZZLE_SIZE; i++)
	{
		printf("%c", 5);
		for (j = 1; j < PUZZLE_SIZE + 1; j++)	printf("%2d %c", g_puzzle[i][j], 5);
		printf("\n%c", 25);
		for (int k = 0; k < 8; k++) printf("%c%c%c%c", 6, 6, 6, 16);
		printf("%c%c%c%c\n", 6, 6, 6, 23);
	}


	printf("%c", 5);
	for (j = 1; j < PUZZLE_SIZE + 1; j++)	printf("%2d %c", g_puzzle[i][j], 5);
	printf("\n%c", 3);
	for (i = 0; i < 8; i++) printf("%c%c%c%c", 6, 6, 6, 21);
	printf("%c%c%c%c\n", 6, 6, 6, 4);

}
//#########################################################################################
DWORD WINAPI RowCheck(LPVOID lpParam)
{
	rowOverlap = 1; // 계산을 위해 1로 변수에 값을 저장

	for (int i = 1; i < PUZZLE_SIZE + 1; i++)
	{
		int check[10] = { 0, }; // 스도쿠가 중복되는지 확인하기 위한 배열. 열이 바뀔 때마다 초기화. 

		for (int j = 1; j < PUZZLE_SIZE + 1; j++)
		{
			check[g_puzzle[i][j] - 1] += 1; // 퍼즐 값을 인덱스로 받아 그 수 만큼 1을 더함
		}

		for (int k = 0; k < 9; k++)
		{
			if (check[k] != 1)
				rowOverlap *= 0; // 해당 배열에서 1-9 까지의 인덱스에서 중복된 값이 있다면 0을 곱함
		}
	}
	return 0;
}

DWORD WINAPI ColumnCheck(LPVOID lpParam)
{
	columnOverlap = 1;

	for (int i = 1; i < PUZZLE_SIZE + 1; i++)
	{
		int check[11] = { 0, }; // 스도쿠가 중복되는지 확인하기 위한 배열. 행이 바뀔 때마다 초기화.

		for (int j = 1; j < PUZZLE_SIZE + 1; j++)
		{
			check[g_puzzle[j][i] - 1] += 1; // 퍼즐 값을 인덱스로 받아 그 수 만큼 1을 더함
		}

		for (int k = 0; k < 9; k++)
		{
			if (check[k] != 1)
				columnOverlap *= 0; // 해당 배열에서 1-9 까지의 인덱스에서 중복된 값이 있다면 0을 곱함
		}
	}
	return 0;
}

DWORD WINAPI section_check(LPVOID lpParam)
{
	int sectionIndex_column = ((DWORD)lpParam - 2) / 3; // 섹션 별로 나눌 때 행으로 구분하기 위한 변수이다.
	int sectionIndex_row = ((DWORD)lpParam - 2) % 3; // 섹션 별로 나눌 때 열로 구분하기 위한 변수이다.
	sectionOverlap = 1;

	int check[11] = { 0, }; // 섹션 별로 확인하기 위해 이 함수의 반복문 밖에서 초기화 한다.

	// 스위치 문을 통해 각 섹션이 행과 열로 이루어져 있다고 했을 때 해당 행과 열에 맞는 부분을 실행 시킨다.
	switch (sectionIndex_column)
	{
	case 0 : // 첫번째 행에 있는 섹션일 때 실행한다.
		for (int i = 1; i <= 3; i++)
		{
			switch (sectionIndex_row)
			{
			case 0 :// 첫번째 열에 있는 섹션일 때 실행한다.
				for (int j = 1; j <= 3; j++)
				{
					check[g_puzzle[i][j] - 1] += 1; // 퍼즐 값을 인덱스로 받아 그 수 만큼 1을 더함
				}
				break;
			case 1:// 두번째 열에 있는 섹션일 때 실행한다.
				for (int j = 4; j <= 6; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			case 2 :// 세번째 열에 있는 섹션일 때 실행한다.
				for (int j = 7; j <= 9; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			}
		}
		for (int k = 0; k < 9; k++)
		{
			if (check[k] != 1)
				sectionOverlap *= 0;
		}
		break;
	case 1 : // 두번째 행에 있는 섹션일 때 실행한다.
		for (int i = 4; i <= 6; i++)
		{
			switch (sectionIndex_row)
			{
			case 0: // 첫번째 열에 있는 섹션일 때 실행한다.
				for (int j = 1; j <= 3; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			case 1:// 두번째 열에 있는 섹션일 때 실행한다.
				for (int j = 4; j <= 6; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			case 2:// 세번째 열에 있는 섹션일 때 실행한다.
				for (int j = 7; j <= 9; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			}
		}
		for (int k = 0; k < 9; k++)
		{
			if (check[k] != 1)
				sectionOverlap *= 0;
		}
		break;
	case 2 : // 세번째 행에 있는 섹션일 때 실행한다.
		for (int i = 7; i <= 9; i++)
		{
			switch (sectionIndex_row)
			{
			case 0:
				for (int j = 1; j <= 3; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			case 1:
				for (int j = 4; j <= 6; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			case 2:
				for (int j = 7; j <= 9; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			}
		}
		for (int k = 0; k < 9; k++)
		{
			if (check[k] != 1)
				sectionOverlap *= 0;
		}
		break;
	}

	return 0;
}