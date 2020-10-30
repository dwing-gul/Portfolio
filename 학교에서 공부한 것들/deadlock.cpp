# define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace std;

#define	MAX_LINE		10000
#define DELIM			"    "

int num[2]; // ���ҽ� Ÿ�� ���� ���μ��� ���� �����ϱ� ���� �迭; num[0]�� ���ҽ� Ÿ�� ��, num[1]�� ���μ��� ���̴�.
int allocation[MAX_LINE][MAX_LINE]; // allocation ���� �����ϱ� ���� �迭
int request[MAX_LINE][MAX_LINE]; // request ���� �����ϱ� ���� �迭
int available[MAX_LINE]; // available ���� �����ϱ� ���� �迭
char category[3][MAX_LINE]; // ����� �迭�� ��� ���� ���ϴ� �������� ��Ÿ���� ���� �迭
int* Work;
bool* Finish;
int detection = 1;		// finish�迭�� false���� �ִ��� Ȯ���ϱ� ���� ����
bool isDeadlock; // ��������� �Ǻ��ϱ� ���� ����
int* index_detect;

void read_file(char* filename); // ������ �о���̴� �Լ�
void print_result(); // ��� ���
void deadlock_detection(); // ������� ã�Ƴ��� �Լ�

int main(int argc, char** argv)
{
	read_file(argv[1]);

	Work = new int[num[0]]; // ���ҽ� Ÿ�� �� ��ŭ�� ũ�⸦ ���� �迭 Work�� ����
	Finish = new bool[num[1]]; // ���μ��� �� ��ŭ�� ũ�⸦ ���� �迭 Finish�� ����
	index_detect = new int[num[1]];	//������� �߻��ϴ� �κ��� ã�� ���� �迭

	deadlock_detection();

	print_result();
}

void read_file(char* filename)
{
	char line[MAX_LINE], *tok; // ������ ������ �޾ƿ� �迭�� �ش� �迭���� ������ ã�� ���� ����
	FILE* fp;

	int iRow = 0, iCol = 0;

	if (!(fp = fopen(filename, "r"))) { printf("ERROR: file open\n"); exit(0); } // ������ ������ ������ ���� ���

	while (fgets(line, MAX_LINE, fp)) // ������ ��� �ٿ� ���� �ݺ�
	{
		tok = strtok(line, DELIM); // �� ���� ������ ���ش�
		int iCol = 0;

		do
		{
			if (iRow == 0) // ù��° ���� ��
			{
				num[iCol++] = atoi(tok); // �ش� ���� ���뿡�� ������ ����
			}
			if (iRow == 1) // �ι�° ���� ��
			{
				fgets(category[0], MAX_LINE, fp); // �ش� ���� ���� ��ü�� category[0]�� ����
			}

			if (iRow < (2 + num[1]) && iRow >= 2) // 3��° �� ���� 1+num[1]��° �ٱ��� (���� ���� ũ�Ⱑ �ٸ��⿡ ���������� �޾ƿ��� ���ؼ� num[1]�� �̿��Ͽ� �޾ƿ´�.)
			{
				allocation[iRow - 2][iCol++] = atoi(tok); //�ش� ���� ���뿡�� ������ ����
			}

			if (iRow == (2 + num[1])) // 2+num[1]��° ���� �� 
			{
				fgets(category[1], MAX_LINE, fp); // �ش� ���� ���� ��ü�� category[1]�� ����
			}

			if (iRow >= (3 + num[1]) && iRow < (3 + (2 * num[1]))) // 3+num[1]��° �� ���� 2+(2*num[1])��° �ٱ���
			{
				request[iRow - (3 + num[1])][iCol++] = atoi(tok); //�ش� ���� ���뿡�� ������ ����
			}

			if (iRow == (3 + (2 * num[1]))) // 3+(2*num[1])��° ���� ��
			{
				fgets(category[2], MAX_LINE, fp); // �ش� ���� ���� ��ü�� category[2]�� ����
			}

			if (iRow >= (4 + (2 * num[1]))) // 4+(2*num[1])��° �� ���ĺ���
			{
				available[iCol++] = atoi(tok); //�ش� ���� ���뿡�� ������ ����
			}
		} while (tok = strtok(NULL, DELIM)); // ������ �����ϴ� ���� �����ϰ� �ݺ��� ����
		iRow++; // �� �� ����
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
				printf("P_%02d ", index_detect[i] - 1); // ������� �߻��ϴ� ��ġ
			}
		}
	}
	cout << "\n------------------------------------------\n";
}

void deadlock_detection()
{
	for (int i = 0; i < num[1]; i++)
	{
		index_detect[i] = 0;	// 0���� �ʱ�ȭ. 0�̶�� �ƹ� ������ ���� ����� �Ǿ��ٴ� ���̰� 0�� �ƴ϶�� ������ ����ٴ� ���̴�.
	}

	for (int i = 0; i < num[0]; i++)
	{
		Work[i] = available[i];		// Work�� available�� �ʱ�ȭ�Ѵ�.
	}

	for (int i = 0; i < num[1]; i++)
	{
		int isZero = 0;		// allocation�� ���� 0���� Ȯ���ϴ� ����
		for (int j = 1; j <= num[0]; j++)
		{
			if (allocation[i][j] != 0)
				isZero += 1;		// allocation�� ���� 0�� �ƴ϶�� isZero�� 1�� ���Ѵ�.
		}

		if (isZero != 0)	// �ش� ������ allocation�� ���� �ϳ��� 0�� �ƴ� ��
		{
			Finish[i] = false;
		}
		else      // �ش� ������ allocation�� ���� ��� 0�� ��
		{
			Finish[i] = true;
		}
	}

	for (int i = 0; i < num[1]; i++)
	{
		if (Finish[i] == false)
		{
			int condition = 0; // finish[i]==false�� request_i <= Work�� �����ϴ��� Ȯ���ϱ� ���� ����
			for (int k = 0; k < num[0]; k++)
			{
				if (request[i][k + 1] <= Work[k])
				{
					condition += 1; // ������ �����Ѵٸ� 1�� ����
				}
			}
			if (condition == num[0]) // ��� ���� ������ �����ؼ� num[0]�� ��ŭ�� ���� �Ǿ��ٸ�
			{
				Finish[i] = true;	// finish[i]�� ���� �ǰ�
				for (int k = 0; k < num[0]; k++)
				{
					Work[k] += allocation[i][k + 1];	// Work�� allocation ���� ���Ѵ�.
				}
			}
			else
			{
				index_detect[i] = i + 1;	// �ϳ��� ���̶� ������ �������� ���ϸ� index_detect�� �ش� �ε��� �� + 1�� �Ͽ� ����
			}
		}
	}

	for (int i = 0; i < num[1]; i++)	// ������ ���߱� ���� index_detext�� ������ �ݺ�
	{
		for (int j = 0; j < num[1]; j++)
		{
			if (index_detect[j] != 0)	// index_detext[j]�� ���� 0�� �ƴ϶�� �ٽ� ���ؼ� �˰����� ����
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
		if (Finish[i] == false) // finish�� �� �߿� ������ �ִٸ�
		{
			detection *= 0; // detection������ 0�� ���϶�
		}
	}

	if (detection == 0) // detection������ 0�̶��
		isDeadlock = true; // ����� �߻�
	else
		isDeadlock = false; // ����� ����
}