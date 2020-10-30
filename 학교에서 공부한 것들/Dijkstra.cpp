#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <chrono>

using namespace std;

#define MAX_VERTICES 60

int INF = 1000000000;

int graph[MAX_VERTICES][MAX_VERTICES];

void matrix_all_pairs(int W[MAX_VERTICES][MAX_VERTICES])
{
    int L[MAX_VERTICES][MAX_VERTICES][MAX_VERTICES]; // ��� �� ������ ���� ���ο� ��� ����
    int m = 1; // ����� �������� ����� Ƚ��

    for (int i = 0; i < MAX_VERTICES; i++)
    {
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            L[1][i][j] = W[i][j];
        }
    } // m = 1 �϶� ��� �ʱ�ȭ

    while(m <= (MAX_VERTICES / 2))
    { 
        for (int i = 0; i < MAX_VERTICES; i++)
        {
            for (int j = 0; j < MAX_VERTICES; j++)
            {
                L[2 * m][i][j] = INF; // �ּҰ� �񱳸� ���� �������� �ʱ�ȭ ��Ŵ
                for (int k = 0; k < MAX_VERTICES; k++)
                {
                    L[2 * m][i][j] = min(L[2 * m][i][j], L[m][i][k] + L[m][k][j]); // ���� ���� ���� �ִ´�.
                }
            }
        }
        m *= 2;
    } 

    /*for (int i = 0; i < MAX_VERTICES; i++)
    {
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            cout << L[m][i][j] << " ";
        }
        cout << endl;
    }*/
}

void dijkstra_all_pairs()
{
    int vnear = 0; // ����� ������ ��ȣ
    int min;
    bool visit[MAX_VERTICES]; // �湮�� Ȯ���ϱ� ���� ����
    int dist[MAX_VERTICES]; // ���� �Ÿ� ���� �����ϱ� ���� ����

    for (int index = 0; index < MAX_VERTICES; index++) // ��ü ������ ���� ����ϱ� ���� �ݺ���
    {
        for (int ind = 0; ind < MAX_VERTICES; ind++)
        {
            visit[ind] = false; // �湮 ���θ� �ʱ�ȭ
        }
        for (int ind = 0; ind < MAX_VERTICES; ind++)
        {
            dist[ind] = graph[index][ind]; // �Ÿ��� ����ϱ� �� �ʱ�ȭ
        }

        visit[index] = true; // ������� �湮�� ������ ���

        for (int i = 0; i < MAX_VERTICES - 2; i++)
        {
            min = INF;
            for (int j = 0; j < MAX_VERTICES; j++)
            {
                if (!visit[j] && dist[j] < min)
                {
                    min = dist[j];
                    vnear = j;
                }
            }

            visit[vnear] = true; // ������ �湮 ����

            for (int k = 0; k < MAX_VERTICES; k++)
            {
                if (!visit[k])
                {
                    if (dist[k] > dist[vnear] + graph[vnear][k])
                    {
                        dist[k] = dist[vnear] + graph[vnear][k];
                    }
                }
            }
        }

        /*for (int i = 0; i < MAX_VERTICES; i++)
        {
            cout << dist[i] << " ";
        }
        cout << endl;*/
    }
}

int main()
{
    srand((int)time(NULL));
    for (int i = 0; i < MAX_VERTICES; i++)
    {
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            if (i == j)
            {
                graph[i][j] = 0;
            }
            else
            {
                graph[i][j] = rand() % 20;
            }

        }
    }

    chrono::system_clock::time_point start_d = chrono::system_clock::now();
    dijkstra_all_pairs();
    chrono::duration<double> sec_d = chrono::system_clock::now() - start_d;

    printf("%.10lf \n", sec_d.count());

    chrono::system_clock::time_point start_m = chrono::system_clock::now();
    matrix_all_pairs(graph);
    chrono::duration<double> sec_m = chrono::system_clock::now() - start_m;

    printf("%.10lf \n", sec_m.count());
}