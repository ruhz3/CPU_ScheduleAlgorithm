#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#include "Scheduler.h"	// Process, CPU, ReadyQueue Ŭ����
using namespace std;


// * ��� ���� *
int N;					// Process�� ����
Process * Task;			// �Է� ���� Process�� ����
vector<Process> Result; // ���� Process���� �־���� ����


// * ����� ���� �Լ� *
void PrintResult();	
void FCFS();			
void SJF();
void PRIORITY();
void RR();


int main() {
	cout << "======================================================" << endl;
	cout << "=                  OS Assignment #2                  =" << endl;
	cout << "=            : CPU Scheduling Algorithm              =" << endl;
	cout << "=                                 2016112650 ������  =" << endl;
	cout << "======================================================" << endl;
	
	// Process���� ������ �Է�
	cout << "<Input>" << endl;
	cout << "Number of Process : ";
	cin >> N;
	Task = new Process[N];
	for (int i = 0; i < N; i++) {
		Task[i].SetNum(i);
	}

	cout << "\n* Input in order �� Process[0] to Process[N-1] *\n" << endl;
	int get;
	cout << "Arrival Time : ";
	for (int i = 0; i < N; i++) {
		cin >> get;
		Task[i].SetArrivalTime(get);
	}
	cout << "Burst Time : ";
	for (int i = 0; i < N; i++) {
		cin >> get;
		Task[i].SetBurstTime(get);
	}
	cout << "Priority : ";
	for (int i = 0; i < N; i++) {
		cin >> get;
		Task[i].SetPriority(get);
	}
	
	// CPU �����층�� �����ϰ� ����� ���
	cout << "\n\n<Output>" << endl;
	cout << "***<FCFS>*********************************************" << endl;
	FCFS();
	PrintResult();
	Result.clear();

	cout << "***<SJF>**********************************************" << endl;
	SJF();
	PrintResult();
	Result.clear();

	cout << "***<PRIORITY>*****************************************" << endl;
	PRIORITY();
	PrintResult();
	Result.clear();
	
	cout << "***<RR>***********************************************" << endl;
	RR();
	PrintResult();
	Result.clear();

	cout << "Type Anything -> Enter to Exit!" << endl;
	cin >> get;

	return 0;
}

void PrintResult() {
	double average_WaitingTime = 0;
	double average_CompleteTime = 0;
	for (int i = 0; i < N; i++) {
		cout << "======================================================" << endl;
		cout << "<Process " << Result[i].GetNum() << " > : " << endl;
		Result[i].PrintProcessingTime();

		average_WaitingTime += Result[i].GetWaitingTime();
		average_CompleteTime += Result[i].GetCompleteTime();
		cout << "WaitingTime : " << Result[i].GetWaitingTime() << endl;
		cout << "CompleteTime : " << Result[i].GetCompleteTime() << endl;
		cout << "======================================================" << endl;
	}
	cout << "******************************************************" << endl;
	cout << "* Average Waiting Time : " << (double)(average_WaitingTime / N) << endl;;
	cout << "* Average Complete Time : " << (double)(average_CompleteTime / N) << endl;;
	cout << "******************************************************" << endl;
	cout << "\n\n\n\n"<< endl;
}

void FCFS() {
	ReadyQueue R(Task, N);
	CPU C;

	int clock = 0;
	int start, end;
	int count = 0;

	bool isRunning = false;
	bool stop = false;
	while (true) {
		R.Refresh(clock);			// Ready ť�� Arrive�� Process�߰� �� ���ð� ����

		// CPU�� ��������� Process�� �Է�
		if (!isRunning) {
			C.Input(R.front());
			R.pop();
			start = clock;
			isRunning = true;
		}

		// �ð� ���, Process ���� (BurstTime�� �����ٸ� stop)
		clock++;
		stop = !C.Running();

		if (stop) {
			end = clock;						
			C.Finish(start, end, end);			// Process�� ������
			Result.push_back(C.GetProcess());	// Process�� ��� ���Ϳ� �ְ�
			isRunning = false;					// CPU�� ������� ǥ��
			
			if (++count == N)	// ��� Process�� BurstTime�� �����ٸ� �Լ� ����
				return;
		}
	}
	return;
}

void SJF() {
	ReadyQueue R(Task, N);
	CPU C;

	int clock = 0;
	int start, end;
	int count = 0;

	bool isRunning = false;
	bool stop = false;

	while (true) {
		R.Refresh(clock);			// Ready ť�� Arrive�� Process�߰� �� ���ð� ����
		R.SortInBurstTime();		// Ready ť�� BurstTime �������� ����

		// CPU�� ��������� Process�� �Է�
		if (!isRunning) {
			C.Input(R.front());
			R.pop();
			start = clock;
			isRunning = true;
		}
		// CPU�� �������� Preemption���� üũ
		else {
			bool preemption = false;
			if (R.size() > 0) {
				// Ready ť �� �� ���ҿ�, CPU�� Process �� ������ �� ShortJob���� ��
				if (R.front().GetBurstTime() < C.GetProcess().GetBurstTime()) {
					preemption = true;
				}
			}
			// preemption �߻�
			if (preemption) {
				end = clock;
				C.Finish(start, end, end);	// Process�� ������

				R.push(C.GetProcess());		// Ready ť�� ���� ���̴� Process�� �ٽ� �ְ�
				C.Input(R.front());			// CPU�� ������ Process �ִ´�
				R.pop();

				start = clock;
			}
		}

		// �ð� ���, Process ���� (BurstTime�� �� ������ stop)
		stop = !C.Running();
		clock++;

		if (stop) {
			end = clock;
			C.Finish(start, end, end);			// Process�� ������
			Result.push_back(C.GetProcess());	// Process�� ��� ���Ϳ� �ְ�
			isRunning = false;					// CPU�� ������� ǥ��

			if (++count == N)	// ��� Process�� BurstTime�� �����ٸ� �Լ� ����
				return;
		}
	}
	return;
}

void PRIORITY() {
	ReadyQueue R(Task, N);
	CPU C;

	int clock = 0;
	int start, end;
	int count = 0;

	bool isRunning = false;
	bool stop = false;

	while (true) {
		R.Refresh(clock);			// Ready ť�� Arrive�� Process�߰� �� ���ð� ����
		R.SortInPriority();			// Ready ť�� BurstTime �������� ����

		// CPU�� ��������� Process�� �Է�
		if (!isRunning) {
			C.Input(R.front());
			R.pop();
			start = clock;
			isRunning = true;
		}
		// CPU�� �������� Preemption���� üũ
		else {
			bool preemption = false;
			if (R.size() > 0) {
				// Ready ť �� �� ���ҿ�, CPU�� Process �� ������ �� ShortJob���� ��
				if (R.front().GetPriority() < C.GetProcess().GetPriority()) {
					preemption = true;
				}
			}
			// preemption �߻�
			if (preemption) {
				end = clock;
				C.Finish(start, end, end);	// Process�� ������

				R.push(C.GetProcess());		// Ready ť�� ���� ���̴� Process�� �ٽ� �ְ�
				C.Input(R.front());			// CPU�� ������ Process �ִ´�
				R.pop();

				start = clock;
			}
		}

		// �ð� ���, Process ���� (BurstTime�� �� ������ stop)
		clock++;
		stop = !C.Running();

		if (stop) {
			end = clock;
			C.Finish(start, end, end);			// Process�� ������
			Result.push_back(C.GetProcess());	// Process�� ��� ���Ϳ� �ְ�
			isRunning = false;					// CPU�� ������� ǥ��

			if (++count == N) {			// ��� Process�� BurstTime�� �����ٸ� return
				return;
			}
		}
	}
	return;
}

void RR() {
	// Quantum ��� (BurstTime�� �ּ� 80% ����)
	vector<int> BrstTime;
	for (int i = 0; i < N; i++) {
		BrstTime.push_back(Task[i].GetBurstTime());
	}
	sort(BrstTime.begin(), BrstTime.end());			// BurstTime�� ��� ��� ����

	int quantum = BrstTime.at((int)(N * 0.8) - 1);	// ������ ���Ϳ��� 80% ��° �� quntum���� ����
	cout << "Quantum : " << quantum << " ( 80% )" << endl;

	ReadyQueue R(Task, N);
	CPU C;

	int idx = 0;
	int clock = 0;
	int start, end;
	int count = 0;

	bool isRunning = false;
	bool stop = false;

	while (true) {
		R.Refresh(clock);			// Ready ť�� Arrive�� Process�߰� �� ���ð� ����

		// CPU�� ��������� Process�� �Է�
		if (!isRunning) {
			C.Input(R.front());
			R.pop();
			start = clock;
			isRunning = true;
		}
		// CPU�� �������� Preemption���� üũ
		else {
			bool preemption = false;
			if (R.size() > 0) {
				// �ð��� ������ quantum�� ����� �� ������ preemption
				if ((clock - start) % quantum == 0) {
					preemption = true;
				}
			}
			// preemption �߻�
			if (preemption) {
				end = clock;
				C.Finish(start, end, end);	// Process�� ������

				R.push(C.GetProcess());		// Ready ť�� ���� ���̴� Process�� �ٽ� �ְ�
				C.Input(R.front());			// CPU�� ������ Process �ִ´�
				R.pop();

				start = clock;
			}
		}

		// �ð� ���, Process ���� (BurstTime�� �� ������ stop)
		clock++;
		stop = !C.Running();

		if (stop) {
			end = clock;
			C.Finish(start, end, end);			// Process�� ������
			Result.push_back(C.GetProcess());	// Process�� ��� ���Ϳ� �ְ�
			isRunning = false;					// CPU�� ������� ǥ��

			if (++count == N) {			// ��� Process�� BurstTime�� �����ٸ� return
				return;
			}
		}
	}
	return;
}

