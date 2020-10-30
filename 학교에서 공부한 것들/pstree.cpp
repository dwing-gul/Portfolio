#include "iostream"
#include "windows.h"
#include "TlHelp32.h"

using namespace std;

int processSequence = 1; // ���μ��� ���� ��ȣ

class ProcessTree
{
private :
	ProcessTree* child;
	ProcessTree* sibling;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // ���� ��� ���μ������� ���¸� ����
	int newParentProcessID; // ������ �θ� ���α׷��� �����ϱ� ���� ����

public:
	PROCESSENTRY32 data;
	ProcessTree() {}
	ProcessTree(PROCESSENTRY32 data)
	{
		child = NULL;
		sibling = NULL;
		this->data = data;
		this->newParentProcessID = this->data.th32ParentProcessID; // newParentProcessID�� �⺻������ ���� �ִ� �θ� ���μ����� ID�� �����Ѵ�.
	}

	void Orphan() // ��� ���μ����� ã�� ���� �Լ�
	{
		PROCESSENTRY32 pe32_search;
		pe32_search.dwSize = sizeof(PROCESSENTRY32);
		bool existParent = false; // �θ����μ����� ��� �ִ��� Ȯ���� ���� ����
		
		Process32First(hProcessSnap, &pe32_search);
	
		do
		{
			ProcessTree* target = new ProcessTree(pe32_search); // ��� ���μ����� ã�� ���� Ŭ���� ����
			if (target->data.th32ProcessID == data.th32ParentProcessID) // target�� ���μ��� id�� �θ��� ���μ��� id�� ���ٸ�
				existParent = true; // existParent�� ���� �ȴ�.
		} while (Process32Next(hProcessSnap, &pe32_search));

		if (!existParent) // �θ� ���μ����� ����Ǿ��ٰ� �ǴܵǾ��� ��
			newParentProcessID = 0; // ���ο� �θ����μ��� id�� 0�� �����Ѵ�.
	}

	void appendChildNode(ProcessTree* Child) // ��� ã�� �Լ� ���� ���� �񱳸� ���� Ŭ������ �Ű� ������ �޾ƿ´�
	{
		if (Child->newParentProcessID == data.th32ProcessID) // �ڽ��� �θ� ���μ���ID�� �ڽ��� ���μ��� ID�� ������ ��
		{
			if (child == NULL) // �ڽ� ��尡 ���� �����϶�
				child = Child; // �ڽ��� ������ ���� �ּҸ� �ڽ� ��忡 �ִ´�.
			else //�ڽ� ��尡 ���� �� ���� ��忡 �ִ´�
			{
				ProcessTree* temp = child;
				while (temp->sibling != NULL)
					temp = temp->sibling;
				temp->sibling = Child;
			}
		}

		if (child != NULL) child->appendChildNode(Child); // �ڽ��� �ҷ��ͼ� �ڽ� ��带 �߰��Ѵ�.
		if (sibling != NULL) sibling->appendChildNode(Child); // ���� ��带 �ҷ��ͼ� �ڽ� ��带 �߰��Ѵ�.
	}

	void printTree(int Depth)
	{
		printf("%03d ", processSequence);

		for (int i = 0; i < Depth; i++)
			cout << "|\t";
		printf("+-%ws<%d><%d:%d> \n", data.szExeFile, data.th32ProcessID, data.th32ParentProcessID, newParentProcessID);
		// ���ϸ�, ���μ���ID, �θ����μ���ID ���;
		// ���ϸ��� ��� cout�� �̿��ϸ� �ּҰ��� ����ϱ⿡ printf�� ����Ѵ�.

		++processSequence; //���� ����

		if (child != NULL) child->printTree(Depth + 1); // �ڽ� ��带 �ҷ����� ���� ����Լ�
		if (sibling != NULL) sibling->printTree(Depth); // ���� ��带 �ҷ����� ���� ����Լ�
	}
};

int main() 
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // ���� ��� ���μ������� ���¸� ����

	PROCESSENTRY32 pe32_root;
	PROCESSENTRY32 pe32_child;
	pe32_root.dwSize = sizeof(PROCESSENTRY32); // ProcessFirst32 �Լ��� ����ϱ� ���ؼ� dwSize�� ���� ����ü�� ũ��� ����
	pe32_child.dwSize = sizeof(PROCESSENTRY32);
	int totalprocessNum = 0; // ���μ��� �� ����

	if (INVALID_HANDLE_VALUE != hProcessSnap)
	{
		Process32First(hProcessSnap, &pe32_root);
		Process32First(hProcessSnap, &pe32_child);
		ProcessTree* root_tree = new ProcessTree(pe32_root); // ���μ��� Ʈ���� ���� Ʈ�� Ŭ���� ����

		do
		{
			totalprocessNum++; // �� ���μ��� ���� ����
		} while (Process32Next(hProcessSnap, &pe32_root));
		Process32First(hProcessSnap, &pe32_root); // ���μ��� ��ġ �ʱ�ȭ

		cout << "############## process tree ############## \n";
		cout << "Number Of Running Process = " << totalprocessNum << "\n";

		Process32Next(hProcessSnap, &pe32_child); // �ι�° ���μ��� ���� ����ϱ� ���� �ѱ�� ����
		do 
		{
			ProcessTree* child = new ProcessTree(pe32_child); // ��� ���μ��� Ž���� �ڽ� Ŭ���� ������ ���� Ʈ�� Ŭ���� ����
			child->Orphan(); // ��� ���μ��� Ž��
			root_tree->appendChildNode(child); // �ڽ� ��� ����
		} while (Process32Next(hProcessSnap, &pe32_child));

		root_tree->printTree(0); // �� ó�� Ʈ������ ���ʷ� ���

		CloseHandle(hProcessSnap);
	}

	system("pause");

	return 0;
}