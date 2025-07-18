#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include "Packet.h"
 
using std::cout;
using std::endl;

using namespace std;

void FCFS(vector<Packet> pkts) {
	vector<bool> enqueued(pkts.size(), false);

	int framesOrdered = 0;
	int currentCheckedFrame = 0;

	queue<int> queue = {};
	vector<int> orderedPkts = {};

	int queueNumber = 0;

	int queue1Index = 0, queue2Index = 0;

	while (framesOrdered < pkts.size())
	{
		for (int i = 0; i < pkts.size(); i++)
		{
			if (!enqueued[i] && pkts[i].arrival <= currentCheckedFrame)
			{
				queue.push(i);
				enqueued[i] = true;
			}
		}

		if (!queue.empty())
		{
			int pktIndex = queue.front();
			queue.pop();
			orderedPkts.push_back(pktIndex);
			pkts[pktIndex].end = currentCheckedFrame + 1;
			framesOrdered++;
		}
		else
		{
			orderedPkts.push_back(-1);
		}

		if (queueNumber == 0)
			queueNumber = 1;
		else
			queueNumber = 0;

		currentCheckedFrame++;
	}

	int totalDelay = 0;

	int index = 0;

	cout << "Pkt\tFlow\tArrival\t  End\tDelay" << endl;
	for (int i = 0; i < orderedPkts.size(); i++)
	{
		if (orderedPkts[i] == -1)
			continue;

		pkts[index].delay = pkts[index].end - pkts[index].arrival;
		cout << pkts[index].ID << "\t" << pkts[index].flow << "\t" << pkts[index].arrival << "\t  " << pkts[index].end << "\t" << pkts[index].delay << endl;

		totalDelay += pkts[index].delay;
		index++;
	}

	cout << "Timeline: ";

	for (int i = 0; i < orderedPkts.size(); i++)
	{
		if (orderedPkts[i] == -1)
			cout << "[ ]";
		else
			cout << "[" << (orderedPkts[i] + 1) << "]";
	}

	cout << endl;

	cout << "Average Delay: " << fixed << setprecision(2) << (totalDelay / (float)pkts.size()) << endl;
}

void WFQ(vector<Packet> pkts) {
	cout << "WFQ called" << endl;
	vector<Packet> class1 = {};
	vector<Packet> class2 = {};
	vector<Packet> class1ordered = {};
	vector<Packet> class2ordered = {};
	vector<int> transmissionOrder = {}; //transmission results
	int arrivalSort = 0;
	int maxArrival = 0;
	int weightCounter = 0;
	//Find maximum arrival
	for (int i = 0; i < pkts.size(); i++) {
		if (pkts[i].arrival > maxArrival) {
			maxArrival = pkts[i].arrival;
		}
	}
	//Sort packets with flow 1 into class1
	for (int i = 0; i < pkts.size(); i++) {
		if (pkts[i].flow == 1) {
			class1.push_back(pkts[i]);
		}
	}
	//Sort packets with flow 2 into class2
	for (int i = 0; i < pkts.size(); i++) {
		if (pkts[i].flow == 2) {
			class2.push_back(pkts[i]);
		}
	}
	//Sort class1 by arrival
	for (int i = 0; i <= maxArrival; i++) {
		for (int o = 0; o < class1.size(); o++) {
			if (class1[o].arrival == i) {
				class1ordered.push_back(class1[o]);
			}
		}
	}
	//Sort class2 by arrival
	for (int i = 0; i <= maxArrival; i++) {
		for (int o = 0; o < class2.size(); o++) {
			if (class2[o].arrival == i) {
				class2ordered.push_back(class2[o]);
			}
		}
	}

	//Transmission
	int class1count = 0;
	int class2count = 0;
	int transmissionSlot = 0;
	int framesSent = 0;
	while (class1count < class1ordered.size() || class2count < class2ordered.size()) {
		//Class 1's turn (weight of 2)
		if (weightCounter < 2) {
			//Check if Class 1 has sent all frames already
			if (class1count < class1ordered.size()) {
				//Check if Class 1 has a frame ready
				if (class1ordered[class1count].arrival <= transmissionSlot) {
					transmissionOrder.push_back(class1ordered[class1count].ID);
					class1count++;
					transmissionSlot++;
					weightCounter++;
				}
				//Class 1's next frame has not arrived; check Class 2 for a frame to send
				else if (class2ordered[class2count].arrival <= transmissionSlot && class2count < class2ordered.size()) {
					transmissionOrder.push_back(class2ordered[class2count].ID);
					class2count++;
					transmissionSlot++;
					weightCounter = 0;
				}
				//neither classes have a frame ready
				else {
					transmissionOrder.push_back(0);
					transmissionSlot++;
				}
			}
			//Class 1 has sent all frames already; send from Class 2 instead if possible
			else if (class2ordered[class2count].arrival <= transmissionSlot && class2count < class2ordered.size()) {
				transmissionOrder.push_back(class2ordered[class2count].ID);
				class2count++;
				transmissionSlot++;
				weightCounter = 0;
			}
			//neither classes have a frame ready
			else {
				transmissionOrder.push_back(0);
				transmissionSlot++;
			}
		}
		//Class 2's turn (weight of 1)
		else if (weightCounter >= 2) {
			//Check if Class 2 has sent all frames already
			if (class2count < class2ordered.size()) {
				//Check if Class 2 has a frame ready
				if (class2ordered[class2count].arrival <= transmissionSlot) {
					transmissionOrder.push_back(class2ordered[class2count].ID);
					class2count++;
					transmissionSlot++;
					weightCounter = 0;
				}
				//Class 2's next frame has not arrived; check Class 1 for a frame to send
				else if (class1ordered[class1count].arrival <= transmissionSlot && class1count < class1ordered.size()) {
					transmissionOrder.push_back(class1ordered[class1count].ID);
					//cout << class1ordered[class1count].ID << " transmitted in slot " << transmissionSlot << endl;
					class1count++;
					transmissionSlot++;
					weightCounter = 1;
				}
				//neither classes have a frame ready
				else {
					transmissionOrder.push_back(0);
					transmissionSlot++;
				}
			}
			//Class 2 has sent all frames already; send from Class 1 instead if possible
			else if (class1ordered[class1count].arrival <= transmissionSlot && class1count < class1ordered.size()) {
				transmissionOrder.push_back(class1ordered[class1count].ID);
				class1count++;
				transmissionSlot++;
				weightCounter = 1;
			}
			//neither classes have a frame ready
			else {
				transmissionOrder.push_back(0);
				transmissionSlot++;
			}
		}
		
		
	}

	vector<bool> assigned(pkts.size(), false);

	for (int slot = 0; slot < transmissionOrder.size(); slot++) {
		int pktID = transmissionOrder[slot];
		if (pktID == 0) continue;

		for (int i = 0; i < pkts.size(); i++) {
			if (pkts[i].ID == pktID && !assigned[i]) {
				pkts[i].end = slot + 1;
				pkts[i].delay = pkts[i].end - pkts[i].arrival;
				assigned[i] = true;
				break;
			}
		}
	}

	vector<Packet> orderedPkts;
	for (int i = 0; i < pkts.size(); i++) {
		if (assigned[i]) {
			orderedPkts.push_back(pkts[i]);
		}
	}

	float totalDelay = 0;

	cout << "Pkt\tFlow\tArrival\t  End\tDelay" << endl;
	for (int i = 0; i < orderedPkts.size(); i++) {
		cout << orderedPkts[i].ID << "\t" << orderedPkts[i].flow << "\t" << orderedPkts[i].arrival << "\t  " << orderedPkts[i].end << "\t" << orderedPkts[i].delay << endl;
		totalDelay += orderedPkts[i].delay;
	}
	totalDelay = totalDelay / orderedPkts.size();


	
	for (const auto& frame : transmissionOrder) {
		if (frame != 0) { 
			cout << "[" << frame << "] "; 
		}
		else if (frame == 0) {
			cout << "[" << " " << "] ";
		}
		
	}
	cout << endl;
	cout << "Average Delay: " << fixed << setprecision(2) << (totalDelay) << endl;
	
}

void RR(vector<Packet> pkts)
{
	vector<bool> enqueued(pkts.size(), false);

	int framesOrdered = 0;
	int currentCheckedFrame = 0;

	vector<queue<int>> queues = { {}, {} };
	vector<int> orderedPkts = {};

	int queueNumber = 0;

	int queue1Index = 0, queue2Index = 0;

	while (framesOrdered < pkts.size())
	{
		for (int i = 0; i < pkts.size(); i++)
		{
			if (!enqueued[i] && pkts[i].arrival <= currentCheckedFrame)
			{
				queues[pkts[i].flow - 1].push(i);
				enqueued[i] = true;
			}
		}

		if (!queues[queueNumber].empty())
		{
			int pktIndex = queues[queueNumber].front();
			queues[queueNumber].pop();
			orderedPkts.push_back(pktIndex);
			pkts[pktIndex].end = currentCheckedFrame + 1;
			framesOrdered++;
		}
		else if (!queues[1 - queueNumber].empty())
		{
			int pktIndex = queues[1 - queueNumber].front();
			queues[1 - queueNumber].pop();
			orderedPkts.push_back(pktIndex);
			pkts[pktIndex].end = currentCheckedFrame + 1;
			framesOrdered++;
		}
		else
		{
			orderedPkts.push_back(-1);
		}

		if (queueNumber == 0)
			queueNumber = 1;
		else
			queueNumber = 0;

		currentCheckedFrame++;
	}

	int totalDelay = 0;

	int index = 0;

	cout << "Pkt\tFlow\tArrival\t  End\tDelay" << endl;
	for (int i = 0; i < orderedPkts.size(); i++)
	{
		if (orderedPkts[i] == -1)
			continue;

		pkts[index].delay = pkts[index].end - pkts[index].arrival;
		cout << pkts[index].ID << "\t" << pkts[index].flow << "\t" << pkts[index].arrival << "\t  " << pkts[index].end << "\t" << pkts[index].delay << endl;

		totalDelay += pkts[index].delay;
		index++;
	}

	cout << "Timeline: ";

	for (int i = 0; i < orderedPkts.size(); i++)
	{
		if (orderedPkts[i] == -1)
			cout << "[ ]";
		else
			cout << "[" << (orderedPkts[i] + 1) << "]";
	}

	cout << endl;
	
	cout << "Average Delay: " << fixed << setprecision(2) << (totalDelay / (float)pkts.size()) << endl;
}

int main()
{
	vector<Packet> pkts = { Packet(1, 1, 0), Packet(2, 1, 0), Packet(3, 1, 1),
							 Packet(4, 2, 1), Packet(5, 2, 3), Packet(6, 1, 2),
							 Packet(7, 2, 3), Packet(8, 2, 5), Packet(9, 2, 5),
							 Packet(10, 2, 7), Packet(11, 1, 8), Packet(12, 1, 8)
	};
	vector<Packet> pkts2 = { Packet(1, 1, 0), Packet(2, 2, 2), Packet(3, 1, 3),
						 Packet(4, 2, 3), Packet(5, 2, 3), Packet(6, 1, 5),
						 Packet(7, 2, 6), Packet(8, 1, 7), Packet(9, 1, 7),
						 Packet(10, 2, 12), Packet(11, 1, 13), Packet(12, 2, 14),
						 Packet(13, 2, 15), Packet(14, 2, 15), Packet(15, 1, 16),
						 Packet(16, 1, 16)
	};
	FCFS(pkts2); 
	



	return 0;
}