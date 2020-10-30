# define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

#define	MAX_LINE		10000
#define DELIM			"    "

int num[2]; // 리소스 타입 수와 프로세스 수를 저장하기 위한 배열; num[0]가 리소스 타입 수, num[1]이 프로세스 수이다.
int allocation[MAX_LINE][MAX_LINE]; // allocation 값을 저장하기 위한 배열
int request[MAX_LINE][MAX_LINE]; // request 값을 저장하기 위한 배열
int available[MAX_LINE]; // available 값을 저장하기 위한 배열
char category[3][MAX_LINE]; // 출력한 배열이 어느 곳에 속하는 값인지를 나타내기 위한 배열
int* Work;
bool* Finish;
int detection = 1;		// finish배열에 false값이 있는지 확인하기 위한 변수
bool isDeadlock; // 데드락인지 판별하기 위한 변수
int* index_detect;

void read_file(char* filename); // 파일을 읽어들이는 함수
void print_result(); // 결과 출력
void deadlock_detection(); // 데드락을 찾아내는 함수

int main(int argc, char** argv)
{
	read_file(argv[1]);

	Work = new int[num[0]]; // 리소스 타입 수 만큼의 크기를 갖는 배열 Work를 선언
	Finish = new bool[num[1]]; // 프로세스 수 만큼의 크기를 갖는 배열 Finish를 선언
	index_detect = new int[num[1]];	//데드락이 발생하는 부분을 찾기 위한 배열

	deadlock_detection();

	print_result();
}

void read_file(char* filename)
{
	char line[MAX_LINE], *tok; // 파일의 내용을 받아올 배열과 해당 배열에서 내용을 찾을 변수 선언
	FILE* fp;

	int iRow = 0, iCol = 0;

	if (!(fp = fopen(filename, "r"))) { printf("ERROR: file open\n"); exit(0); } // 파일이 열리지 않으면 에러 출력

	while (fgets(line, MAX_LINE, fp)) // 파일의 모든 줄에 대해 반복
	{
		tok = strtok(line, DELIM); // 줄 별로 공백을 없앤다
		int iCol = 0;

		do
		{
			if (iRow == 0) // 첫번째 줄일 때
			{
				num[iCol++] = atoi(tok); // 해당 줄의 내용에서 정수를 저장
			}
			if (iRow == 1) // 두번째 줄일 때
			{
				fgets(category[0], MAX_LINE, fp); // 해당 줄의 내용 전체를 category[0]에 저장
			}

			if (iRow < (2 + num[1]) && iRow >= 2) // 3번째 줄 부터 1+num[1]번째 줄까지 (파일 별로 크기가 다르기에 유동적으로 받아오기 위해서 num[1]을 이용하여 받아온다.)
			{
				allocation[iRow - 2][iCol++] = atoi(tok); //해당 줄의 내용에서 정수를 저장
			}

			if (iRow == (2 + num[1])) // 2+num[1]번째 줄일 때 
			{
				fgets(category[1], MAX_LINE, fp); // 해당 줄의 내용 전체를 category[1]에 저장
			}

			if (iRow >= (3 + num[1]) && iRow < (3 + (2 * num[1]))) // 3+num[1]번째 줄 부터 2+(2*num[1])번째 줄까지
			{
				request[iRow - (3 + num[1])][iCol++] = atoi(tok); //해당 줄의 내용에서 정수를 저장
			}

			if (iRow == (3 + (2 * num[1]))) // 3+(2*num[1])번째 줄일 때
			{
				fgets(category[2], MAX_LINE, fp); // 해당 줄의 내용 전체를 category[2]에 저장
			}

			if (iRow >= (4 + (2 * num[1]))) // 4+(2*num[1])번째 줄 이후부터
			{
				available[iCol++] = atoi(tok); //해당 줄의 내용에서 정수를 저장
			}
		} while (tok = strtok(NULL, DELIM)); // 공백이 존재하는 줄을 제외하고 반복문 실행
		iRow++; // 줄 수 증가
	}

	fclose(fp);
}

void print_result()
{
	cout << "------------------------------------------\n";
	cout << "Number of Resource Types :   " << num[0] << endl;
	cout << "Number of Process        :   " << num[1] << endl << endl;
	for (int i = 0; i < (11 + 4 * num[0]); i++)
	{
		cout << category[0][i];
	}
	cout << endl;

	for (int i = 0; i < num[1]; i++)
	{
		cout << "\t      ";
		for (int j = 1; j <= num[0]; j++)
		{
			cout << allocation[i][j] << "   ";
		}
		cout << endl;
	}
	cout << endl;

	for (int i = 0; i < (11 + 4 * num[0]); i++)
	{
		cout << category[1][i];
	}
	cout << endl;

	for (int i = 0; i < num[1]; i++)
	{
		cout << "\t      ";
		for (int j = 1; j <= num[0]; j++)
		{
			cout << request[i][j] << "   ";
		}
		cout << endl;
	}
	cout << endl;

	for (int i = 0; i < (11 + 4 * num[0]); i++)
	{
		cout << category[2][i];
	}

	cout << "\n\t      ";
	for (int i = 0; i < num[0]; i++)
	{
		cout << available[i] << "   ";
	}

	cout << "\n\n------------------------------------------" << endl;
	cout << "Result : ";
	if (isDeadlock == false)
	{
		cout << "No Deadlock";
	}
	else
	{
		cout << "Deadlock --> ";
		for (int i = 0; i < num[1]; i++)
		{
			if (index_detect[i] != 0)
			{
				printf("P_%02d ", index_detect[i] - 1); // 데드락이 발생하는 위치
			}
		}
	}
	cout << "\n------------------------------------------\n";
}

void deadlock_detection()
{
	for (int i = 0; i < num[1]; i++)
	{
		index_detect[i] = 0;	// 0으로 초기화. 0이라면 아무 문제가 없이 계산이 되었다는 것이고 0이 아니라면 문제가 생겼다는 것이다.
	}

	for (int i = 0; i < num[0]; i++)
	{
		Work[i] = available[i];		// Work를 available로 초기화한다.
	}

	for (int i = 0; i < num[1]; i++)
	{
		int isZero = 0;		// allocation의 값이 0인지 확인하는 변수
		for (int j = 1; j <= num[0]; j++)
		{
			if (allocation[i][j] != 0)
				isZero += 1;		// allocation의 값이 0이 아니라면 isZero에 1을 더한다.
		}

		if (isZero != 0)	// 해당 열에서 allocation의 값이 하나라도 0이 아닐 때
		{
			Finish[i] = false;
		}
		else      // 해당 열에서 allocation의 값이 모두 0일 때
		{
			Finish[i] = true;
		}
	}

	for (int i = 0; i < num[1]; i++)
	{
		if (Finish[i] == false)
		{
			int condition = 0; // finish[i]==false와 request_i <= Work를 만족하는지 확인하기 위한 변수
			for (int k = 0; k < num[0]; k++)
			{
				if (request[i][k + 1] <= Work[k])
				{
					condition += 1; // 조건을 만족한다면 1을 더함
				}
			}
			if (condition == num[0]) // 모든 값이 조건을 만족해서 num[0]값 만큼의 수가 되었다면
			{
				Finish[i] = true;	// finish[i]가 참이 되고
				for (int k = 0; k < num[0]; k++)
				{
					Work[k] += allocation[i][k + 1];	// Work에 allocation 값을 더한다.
				}
			}
			else
			{
				index_detect[i] = i + 1;	// 하나의 값이라도 조건을 만족하지 못하면 index_detect에 해당 인덱스 값 + 1을 하여 저장
			}
		}
	}

	for (int i = 0; i < num[1]; i++)	// 순서를 맞추기 위해 index_detext를 여러번 반복
	{
		for (int j = 0; j < num[1]; j++)
		{
			if (index_detect[j] != 0)	// index_detext[j]의 값이 0이 아니라면 다시 디텍션 알고리즘을 시행
			{
				if (Finish[j] == false)
				{
					int condition = 0;
					for (int k = 0; k < num[0]; k++)
					{
						if (request[j][k + 1] <= Work[k])
						{
							condition += 1;
						}
					}
					if (condition == num[0])
					{
						Finish[j] = true;
						for (int k = 0; k < num[0]; k++)
						{
							Work[k] += allocation[j][k + 1];
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < num[1]; i++)
	{
		if (Finish[i] == false) // finish의 값 중에 거짓이 있다면
		{
			detection *= 0; // detection변수에 0을 곱하라
		}
	}

	if (detection == 0) // detection변수가 0이라면
		isDeadlock = true; // 데드락 발생
	else
		isDeadlock = false; // 데드락 없음
}