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

int rowOverlap; // �� �κп��� ��ġ�� �κ��� üũ�ϱ� ���� ���� ����
int columnOverlap; // �� �κп��� ��ġ�� �κ��� üũ�ϱ� ���� ���� ����
int sectionOverlap; // ���� �κп��� ��ġ�� �κ��� üũ�ϱ� ���� ���� ����

void read_puzzle(int p_puzzle[PUZZLE_SIZE + 1][PUZZLE_SIZE + 1], char* filename);
void PrintPuzzle();
DWORD WINAPI RowCheck(LPVOID lpParam); // �������� �� �κ��� Ȯ���ϱ� ���� �Լ�
DWORD WINAPI ColumnCheck(LPVOID lpParam); // �������� �� �κ��� Ȯ���ϱ� ���� �Լ�
DWORD WINAPI section_check(LPVOID lpParam); // �������� ���� �κ��� Ȯ���ϱ� ���� �Լ�

int main(int argc, char** argv)
{
	DWORD countOfThread = 0; // ������ ����� �ε����� �ľ��ϱ� ���� �����̴�.
	DWORD dwThreadID[MAX_THREAD]; // �� �������� �ּҸ� ��� �����̴�.
	HANDLE hThread[MAX_THREAD]; // �����带 �����Ű�� ���� �޴� ��� �����̴�.
	int rightCheck = 1; // �� �κ��� �� �´� �� Ȯ�ε� ����� �����ϱ� ���� ����
	bool bIsRight = false; // rightCheck���� ����� ����� �����ϴ� ����

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
	hThread[0] = CreateThread(NULL, 0, RowCheck, NULL, 0, &dwThreadID[countOfThread]); // �����带 ���� �ش� �Լ��� ���� ��Ų��.
	WaitForSingleObject(hThread[0], 3); // �ش� �������� �Լ� ������ ������ �ð���ŭ ��� �� ���� ���� �����Ѵ�.
	rightCheck *= rowOverlap; // ���� �Լ��� ���� ����� ���� ����� ���� rightCheck�� �� ���� �� �����Ѵ�.
	countOfThread++; // ���� ������� �Ѿ�� ���� ī��Ʈ

	//#####################################################
	//################ Columns Check ######################
	//#####################################################
	hThread[1] = CreateThread(NULL, 0, ColumnCheck, NULL, 0, &dwThreadID[countOfThread]); // �����带 ���� �ش� �Լ��� ���� ��Ų��.
	WaitForSingleObject(hThread[1], 3);
	countOfThread++; // ���� ������� �Ѿ�� ���� ī��Ʈ
	rightCheck *= columnOverlap; // ���� �Լ��� ���� ����� ���� ����� ���� rightCheck�� �� ���� �� �����Ѵ�.

	//#####################################################
	//################ 9 Sections Check #####################
	//#####################################################
	while (countOfThread < MAX_THREAD)
	{
		hThread[countOfThread] = CreateThread(NULL, 0, section_check, (LPVOID)countOfThread, 0, &dwThreadID[countOfThread]); // �����带 ���� �ش� �Լ��� ���� ��Ų��.
		WaitForSingleObject(hThread[countOfThread], 3);
		rightCheck *= sectionOverlap;  // ���� �Լ��� ���� ����� ���� ����� ���� rightCheck�� �� ���� �� �����Ѵ�.
		countOfThread++;
	}

	bIsRight = rightCheck; // ��������� bool ������ �����Ѵ�.

	//##############################################################
	//################ Print Checking Result #######################
	//##############################################################
	if (bIsRight == true)
		printf("[Sudoku puzzle Check Result]: Right\n\n");
	else
		printf("[Sudoku puzzle Check Result]: Wrong\n\n");

	for (DWORD i = 0; i < MAX_THREAD; i++)
	{
		ExitThread(dwThreadID[i]); // ������ �����带 ��� �����Ѵ�.
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
	rowOverlap = 1; // ����� ���� 1�� ������ ���� ����

	for (int i = 1; i < PUZZLE_SIZE + 1; i++)
	{
		int check[10] = { 0, }; // ������ �ߺ��Ǵ��� Ȯ���ϱ� ���� �迭. ���� �ٲ� ������ �ʱ�ȭ. 

		for (int j = 1; j < PUZZLE_SIZE + 1; j++)
		{
			check[g_puzzle[i][j] - 1] += 1; // ���� ���� �ε����� �޾� �� �� ��ŭ 1�� ����
		}

		for (int k = 0; k < 9; k++)
		{
			if (check[k] != 1)
				rowOverlap *= 0; // �ش� �迭���� 1-9 ������ �ε������� �ߺ��� ���� �ִٸ� 0�� ����
		}
	}
	return 0;
}

DWORD WINAPI ColumnCheck(LPVOID lpParam)
{
	columnOverlap = 1;

	for (int i = 1; i < PUZZLE_SIZE + 1; i++)
	{
		int check[11] = { 0, }; // ������ �ߺ��Ǵ��� Ȯ���ϱ� ���� �迭. ���� �ٲ� ������ �ʱ�ȭ.

		for (int j = 1; j < PUZZLE_SIZE + 1; j++)
		{
			check[g_puzzle[j][i] - 1] += 1; // ���� ���� �ε����� �޾� �� �� ��ŭ 1�� ����
		}

		for (int k = 0; k < 9; k++)
		{
			if (check[k] != 1)
				columnOverlap *= 0; // �ش� �迭���� 1-9 ������ �ε������� �ߺ��� ���� �ִٸ� 0�� ����
		}
	}
	return 0;
}

DWORD WINAPI section_check(LPVOID lpParam)
{
	int sectionIndex_column = ((DWORD)lpParam - 2) / 3; // ���� ���� ���� �� ������ �����ϱ� ���� �����̴�.
	int sectionIndex_row = ((DWORD)lpParam - 2) % 3; // ���� ���� ���� �� ���� �����ϱ� ���� �����̴�.
	sectionOverlap = 1;

	int check[11] = { 0, }; // ���� ���� Ȯ���ϱ� ���� �� �Լ��� �ݺ��� �ۿ��� �ʱ�ȭ �Ѵ�.

	// ����ġ ���� ���� �� ������ ��� ���� �̷���� �ִٰ� ���� �� �ش� ��� ���� �´� �κ��� ���� ��Ų��.
	switch (sectionIndex_column)
	{
	case 0 : // ù��° �࿡ �ִ� ������ �� �����Ѵ�.
		for (int i = 1; i <= 3; i++)
		{
			switch (sectionIndex_row)
			{
			case 0 :// ù��° ���� �ִ� ������ �� �����Ѵ�.
				for (int j = 1; j <= 3; j++)
				{
					check[g_puzzle[i][j] - 1] += 1; // ���� ���� �ε����� �޾� �� �� ��ŭ 1�� ����
				}
				break;
			case 1:// �ι�° ���� �ִ� ������ �� �����Ѵ�.
				for (int j = 4; j <= 6; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			case 2 :// ����° ���� �ִ� ������ �� �����Ѵ�.
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
	case 1 : // �ι�° �࿡ �ִ� ������ �� �����Ѵ�.
		for (int i = 4; i <= 6; i++)
		{
			switch (sectionIndex_row)
			{
			case 0: // ù��° ���� �ִ� ������ �� �����Ѵ�.
				for (int j = 1; j <= 3; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			case 1:// �ι�° ���� �ִ� ������ �� �����Ѵ�.
				for (int j = 4; j <= 6; j++)
				{
					check[g_puzzle[i][j] - 1] += 1;
				}
				break;
			case 2:// ����° ���� �ִ� ������ �� �����Ѵ�.
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
	case 2 : // ����° �࿡ �ִ� ������ �� �����Ѵ�.
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