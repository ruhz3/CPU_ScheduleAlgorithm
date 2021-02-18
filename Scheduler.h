#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class Process {
private:
	int num;

	// �Է� ����
	int arrivalTime;
	int burstTime;
	int priority;

	//��� ����
	vector<int> ProcessingTime;
	int waitingTime;
	int completeTime;

public:
	// ������
	Process() {
		waitingTime = 0;
		completeTime = 0;
	}

	// ������� �� ����
	void SetNum(int n) {
		this->num = n;
	}
	void SetArrivalTime(int t) {
		this->arrivalTime = t;
	}
	void SetBurstTime(int t) {
		this->burstTime = t;
	}
	void SetPriority(int n) {
		this->priority = n;
	}
	void SetProcessingTime(int a, int b) {
		this->ProcessingTime.push_back(a);
		this->ProcessingTime.push_back(b);
	}
	void SetWaitingTime(int t) {
		this->waitingTime = t;
	}
	void SetCompleteTime(int t) {
		this->completeTime = t;
	}

	// ������� ��� �� ��ȯ
	int GetNum() {
		return this->num;
	}
	int GetArrivalTime() {
		return arrivalTime;
	}
	int GetBurstTime() {
		return burstTime;
	}
	int GetPriority() {
		return priority;
	}
	void PrintProcessingTime() {
		cout << "ProcessingTime : ";
		for (int i = 0; i < ProcessingTime.size(); i += 2) {
			cout << ProcessingTime[i] << " to " << ProcessingTime[i + 1] << ", ";
		}
		cout << endl;
	}
	int GetWaitingTime() {
		return waitingTime;
	}
	int GetCompleteTime() {
		return completeTime;
	}
};

class ReadyQueue {
private:
	vector<Process> Ready;
	Process * ProcessList;
	int N;
public:
	ReadyQueue(Process * Task, int N) {
		this->ProcessList = Task;
		this->N = N;
	}

	void Refresh(int clock) {
		// ť ���� Process�� WaitingTime ����
		for (int i = 0; i < Ready.size(); i++) {
			Ready[i].SetWaitingTime(Ready[i].GetWaitingTime() + 1);
		}
		// Arrive�� Process�� �ִ��� �˻� �� push
		for (int i = 0; i < N; i++) {
			if (ProcessList[i].GetArrivalTime() == clock)
				Ready.push_back(ProcessList[i]);
		}
	}
	void SortInBurstTime() {
		// BurstTime ����, �������� ����(Bubble Sort)
		for (int i = Ready.size() - 1; i > 0; i--) {
			for (int j = 0; j < i; j++) {
				if (Ready[j].GetBurstTime() > Ready[j + 1].GetBurstTime()) {
					swap(Ready[j], Ready[j + 1]);
				}
			}
		}
	}
	void SortInPriority() {
		for (int i = Ready.size() - 1; i > 0; i--) {
			for (int j = 0; j < i; j++) {
				if (Ready[j].GetPriority() > Ready[j + 1].GetPriority()) {
					swap(Ready[j], Ready[j + 1]);
				}
			}
		}
	}
	void push(Process P) {
		Ready.push_back(P);
	}
	void pop() {
		Ready.erase(Ready.begin());
	}
	Process front() {
		return Ready.front();
	}
	int size() {
		return Ready.size();
	}
};

class CPU {
private:
	Process P;

public:
	CPU() {}
	void Input(Process P) {
		this->P = P;
	}
	Process GetProcess() {
		return P;
	}
	bool Running() {
		if (P.GetBurstTime() == 1) {
			P.SetBurstTime(P.GetBurstTime() - 1);
			return false;
		}
		else {
			P.SetBurstTime(P.GetBurstTime() - 1);
			return true;
		}
	}
	void Finish(int process_start, int process_end, int complete_time) {
		P.SetProcessingTime(process_start, process_end);
		P.SetCompleteTime(complete_time);
	}
};
