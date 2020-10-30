#include "iostream"
#include "windows.h"
#include "TlHelp32.h"

using namespace std;

int processSequence = 1; // 프로세스 순서 번호

class ProcessTree
{
private :
	ProcessTree* child;
	ProcessTree* sibling;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 현재 모든 프로세스들의 상태를 스냅
	int newParentProcessID; // 현재의 부모 프로그램을 설정하기 위한 변수

public:
	PROCESSENTRY32 data;
	ProcessTree() {}
	ProcessTree(PROCESSENTRY32 data)
	{
		child = NULL;
		sibling = NULL;
		this->data = data;
		this->newParentProcessID = this->data.th32ParentProcessID; // newParentProcessID에 기본적으로 갖고 있던 부모 프로세스의 ID를 저장한다.
	}

	void Orphan() // 고아 프로세스를 찾기 위한 함수
	{
		PROCESSENTRY32 pe32_search;
		pe32_search.dwSize = sizeof(PROCESSENTRY32);
		bool existParent = false; // 부모프로세스가 살아 있는지 확인을 위한 변수
		
		Process32First(hProcessSnap, &pe32_search);
	
		do
		{
			ProcessTree* target = new ProcessTree(pe32_search); // 고아 프로세스를 찾기 위한 클래스 생성
			if (target->data.th32ProcessID == data.th32ParentProcessID) // target의 프로세스 id와 부모의 프로세스 id가 같다면
				existParent = true; // existParent가 참이 된다.
		} while (Process32Next(hProcessSnap, &pe32_search));

		if (!existParent) // 부모 프로세스가 종료되었다고 판단되었을 때
			newParentProcessID = 0; // 새로운 부모프로세스 id로 0을 설정한다.
	}

	void appendChildNode(ProcessTree* Child) // 고아 찾는 함수 실행 이후 비교를 위해 클래스를 매개 변수로 받아온다
	{
		if (Child->newParentProcessID == data.th32ProcessID) // 자식의 부모 프로세스ID와 자신의 프로세스 ID가 같은지 비교
		{
			if (child == NULL) // 자식 노드가 없는 상태일때
				child = Child; // 자식의 정보에 대한 주소를 자식 노드에 넣는다.
			else //자식 노드가 있을 때 형제 노드에 넣는다
			{
				ProcessTree* temp = child;
				while (temp->sibling != NULL)
					temp = temp->sibling;
				temp->sibling = Child;
			}
		}

		if (child != NULL) child->appendChildNode(Child); // 자식을 불러와서 자식 노드를 추가한다.
		if (sibling != NULL) sibling->appendChildNode(Child); // 형제 노드를 불러와서 자식 노드를 추가한다.
	}

	void printTree(int Depth)
	{
		printf("%03d ", processSequence);

		for (int i = 0; i < Depth; i++)
			cout << "|\t";
		printf("+-%ws<%d><%d:%d> \n", data.szExeFile, data.th32ProcessID, data.th32ParentProcessID, newParentProcessID);
		// 파일명, 프로세스ID, 부모프로세스ID 출력;
		// 파일명의 경우 cout을 이용하면 주소값을 출력하기에 printf를 사용한다.

		++processSequence; //순서 증가

		if (child != NULL) child->printTree(Depth + 1); // 자식 노드를 불러내기 위한 재귀함수
		if (sibling != NULL) sibling->printTree(Depth); // 형제 노드를 불러내기 위한 재귀함수
	}
};

int main() 
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 현재 모든 프로세스들의 상태를 스냅

	PROCESSENTRY32 pe32_root;
	PROCESSENTRY32 pe32_child;
	pe32_root.dwSize = sizeof(PROCESSENTRY32); // ProcessFirst32 함수를 사용하기 위해서 dwSize의 값을 구조체의 크기로 설정
	pe32_child.dwSize = sizeof(PROCESSENTRY32);
	int totalprocessNum = 0; // 프로세스 총 개수

	if (INVALID_HANDLE_VALUE != hProcessSnap)
	{
		Process32First(hProcessSnap, &pe32_root);
		Process32First(hProcessSnap, &pe32_child);
		ProcessTree* root_tree = new ProcessTree(pe32_root); // 프로세스 트리를 위한 트리 클래스 생성

		do
		{
			totalprocessNum++; // 총 프로세스 개수 세기
		} while (Process32Next(hProcessSnap, &pe32_root));
		Process32First(hProcessSnap, &pe32_root); // 프로세스 위치 초기화

		cout << "############## process tree ############## \n";
		cout << "Number Of Running Process = " << totalprocessNum << "\n";

		Process32Next(hProcessSnap, &pe32_child); // 두번째 프로세스 부터 계산하기 위해 넘기는 과정
		do 
		{
			ProcessTree* child = new ProcessTree(pe32_child); // 고아 프로세스 탐색과 자식 클래스 주입을 위한 트리 클래스 생성
			child->Orphan(); // 고아 프로세스 탐색
			root_tree->appendChildNode(child); // 자식 노드 삽입
		} while (Process32Next(hProcessSnap, &pe32_child));

		root_tree->printTree(0); // 맨 처음 트리부터 차례로 출력

		CloseHandle(hProcessSnap);
	}

	system("pause");

	return 0;
}