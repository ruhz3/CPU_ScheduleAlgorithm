#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#include "Scheduler.h"	// Process, CPU, ReadyQueue 클래스
using namespace std;


// * 멤버 변수 *
int N;					// Process의 갯수
Process * Task;			// 입력 받은 Process의 정보
vector<Process> Result; // 끝난 Process들을 넣어놓을 벡터


// * 사용자 정의 함수 *
void PrintResult();	
void FCFS();			
void SJF();
void PRIORITY();
void RR();


int main() {
	cout << "======================================================" << endl;
	cout << "=                  OS Assignment #2                  =" << endl;
	cout << "=            : CPU Scheduling Algorithm              =" << endl;
	cout << "=                                 2016112650 류형주  =" << endl;
	cout << "======================================================" << endl;
	
	// Process들의 정보를 입력
	cout << "<Input>" << endl;
	cout << "Number of Process : ";
	cin >> N;
	Task = new Process[N];
	for (int i = 0; i < N; i++) {
		Task[i].SetNum(i);
	}

	cout << "\n* Input in order ㅡ Process[0] to Process[N-1] *\n" << endl;
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
	
	// CPU 스케쥴링을 실행하고 결과를 출력
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
		R.Refresh(clock);			// Ready 큐에 Arrive한 Process추가 및 대기시간 증가

		// CPU가 비어있으면 Process를 입력
		if (!isRunning) {
			C.Input(R.front());
			R.pop();
			start = clock;
			isRunning = true;
		}

		// 시간 경과, Process 진행 (BurstTime이 끝났다면 stop)
		clock++;
		stop = !C.Running();

		if (stop) {
			end = clock;						
			C.Finish(start, end, end);			// Process를 마무리
			Result.push_back(C.GetProcess());	// Process를 결과 벡터에 넣고
			isRunning = false;					// CPU가 비었음을 표시
			
			if (++count == N)	// 모든 Process의 BurstTime이 끝났다면 함수 종료
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
		R.Refresh(clock);			// Ready 큐에 Arrive한 Process추가 및 대기시간 증가
		R.SortInBurstTime();		// Ready 큐를 BurstTime 기준으로 정렬

		// CPU가 비어있으면 Process를 입력
		if (!isRunning) {
			C.Input(R.front());
			R.pop();
			start = clock;
			isRunning = true;
		}
		// CPU가 차있으면 Preemption한지 체크
		else {
			bool preemption = false;
			if (R.size() > 0) {
				// Ready 큐 맨 앞 원소와, CPU의 Process 중 무엇이 더 ShortJob인지 비교
				if (R.front().GetBurstTime() < C.GetProcess().GetBurstTime()) {
					preemption = true;
				}
			}
			// preemption 발생
			if (preemption) {
				end = clock;
				C.Finish(start, end, end);	// Process를 마무리

				R.push(C.GetProcess());		// Ready 큐에 진행 중이던 Process를 다시 넣고
				C.Input(R.front());			// CPU에 선점할 Process 넣는다
				R.pop();

				start = clock;
			}
		}

		// 시간 경과, Process 진행 (BurstTime이 다 끝나면 stop)
		stop = !C.Running();
		clock++;

		if (stop) {
			end = clock;
			C.Finish(start, end, end);			// Process를 마무리
			Result.push_back(C.GetProcess());	// Process를 결과 벡터에 넣고
			isRunning = false;					// CPU가 비었음을 표시

			if (++count == N)	// 모든 Process의 BurstTime이 끝났다면 함수 종료
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
		R.Refresh(clock);			// Ready 큐에 Arrive한 Process추가 및 대기시간 증가
		R.SortInPriority();			// Ready 큐를 BurstTime 기준으로 정렬

		// CPU가 비어있으면 Process를 입력
		if (!isRunning) {
			C.Input(R.front());
			R.pop();
			start = clock;
			isRunning = true;
		}
		// CPU가 차있으면 Preemption한지 체크
		else {
			bool preemption = false;
			if (R.size() > 0) {
				// Ready 큐 맨 앞 원소와, CPU의 Process 중 무엇이 더 ShortJob인지 비교
				if (R.front().GetPriority() < C.GetProcess().GetPriority()) {
					preemption = true;
				}
			}
			// preemption 발생
			if (preemption) {
				end = clock;
				C.Finish(start, end, end);	// Process를 마무리

				R.push(C.GetProcess());		// Ready 큐에 진행 중이던 Process를 다시 넣고
				C.Input(R.front());			// CPU에 선점할 Process 넣는다
				R.pop();

				start = clock;
			}
		}

		// 시간 경과, Process 진행 (BurstTime이 다 끝나면 stop)
		clock++;
		stop = !C.Running();

		if (stop) {
			end = clock;
			C.Finish(start, end, end);			// Process를 마무리
			Result.push_back(C.GetProcess());	// Process를 결과 벡터에 넣고
			isRunning = false;					// CPU가 비었음을 표시

			if (++count == N) {			// 모든 Process의 BurstTime이 끝났다면 return
				return;
			}
		}
	}
	return;
}

void RR() {
	// Quantum 계산 (BurstTime의 최소 80% 포함)
	vector<int> BrstTime;
	for (int i = 0; i < N; i++) {
		BrstTime.push_back(Task[i].GetBurstTime());
	}
	sort(BrstTime.begin(), BrstTime.end());			// BurstTime을 모두 모아 정렬

	int quantum = BrstTime.at((int)(N * 0.8) - 1);	// 정렬한 벡터에서 80% 번째 값 quntum으로 설정
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
		R.Refresh(clock);			// Ready 큐에 Arrive한 Process추가 및 대기시간 증가

		// CPU가 비어있으면 Process를 입력
		if (!isRunning) {
			C.Input(R.front());
			R.pop();
			start = clock;
			isRunning = true;
		}
		// CPU가 차있으면 Preemption한지 체크
		else {
			bool preemption = false;
			if (R.size() > 0) {
				// 시간이 설정한 quantum의 배수가 될 때마다 preemption
				if ((clock - start) % quantum == 0) {
					preemption = true;
				}
			}
			// preemption 발생
			if (preemption) {
				end = clock;
				C.Finish(start, end, end);	// Process를 마무리

				R.push(C.GetProcess());		// Ready 큐에 진행 중이던 Process를 다시 넣고
				C.Input(R.front());			// CPU에 선점할 Process 넣는다
				R.pop();

				start = clock;
			}
		}

		// 시간 경과, Process 진행 (BurstTime이 다 끝나면 stop)
		clock++;
		stop = !C.Running();

		if (stop) {
			end = clock;
			C.Finish(start, end, end);			// Process를 마무리
			Result.push_back(C.GetProcess());	// Process를 결과 벡터에 넣고
			isRunning = false;					// CPU가 비었음을 표시

			if (++count == N) {			// 모든 Process의 BurstTime이 끝났다면 return
				return;
			}
		}
	}
	return;
}

