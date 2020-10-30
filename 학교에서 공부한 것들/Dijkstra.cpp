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
    int L[MAX_VERTICES][MAX_VERTICES][MAX_VERTICES]; // 행렬 곱 연산을 위한 새로운 행렬 선언
    int m = 1; // 행렬의 곱연산이 시행된 횟수

    for (int i = 0; i < MAX_VERTICES; i++)
    {
        for (int j = 0; j < MAX_VERTICES; j++)
        {
            L[1][i][j] = W[i][j];
        }
    } // m = 1 일때 행렬 초기화

    while(m <= (MAX_VERTICES / 2))
    { 
        for (int i = 0; i < MAX_VERTICES; i++)
        {
            for (int j = 0; j < MAX_VERTICES; j++)
            {
                L[2 * m][i][j] = INF; // 최소값 비교를 위해 무한으로 초기화 시킴
                for (int k = 0; k < MAX_VERTICES; k++)
                {
                    L[2 * m][i][j] = min(L[2 * m][i][j], L[m][i][k] + L[m][k][j]); // 작은 값을 집어 넣는다.
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
    int vnear = 0; // 가까운 정점의 번호
    int min;
    bool visit[MAX_VERTICES]; // 방문을 확인하기 위한 변수
    int dist[MAX_VERTICES]; // 계산된 거리 값을 저장하기 위한 변수

    for (int index = 0; index < MAX_VERTICES; index++) // 전체 정점에 대해 계산하기 위한 반복문
    {
        for (int ind = 0; ind < MAX_VERTICES; ind++)
        {
            visit[ind] = false; // 방문 여부를 초기화
        }
        for (int ind = 0; ind < MAX_VERTICES; ind++)
        {
            dist[ind] = graph[index][ind]; // 거리를 계산하기 전 초기화
        }

        visit[index] = true; // 출발점은 방문한 것으로 취급

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

            visit[vnear] = true; // 목적지 방문 여부

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