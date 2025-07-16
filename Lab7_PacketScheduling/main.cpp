#include <iostream>
#include <vector>
#include <queue>
#include "packet.h"

using namespace std;

void FCFS(vector<Packet> pkts) {
	int framesOrdered = 0;
	int currentCheckedFrame = 0;

	vector<Packet> orderedPkts = {};

	while (framesOrdered < pkts.size())
	{
		for (int i = 0; i < pkts.size(); i++)
		{
			if (pkts[i].arrival == currentCheckedFrame)
			{
				orderedPkts.push_back(pkts[i]);
				framesOrdered++;
			}
		}
		currentCheckedFrame++;
	}

	for (int i = 0; i < orderedPkts.size(); i++)
	{
		pkts[i].end = orderedPkts[i].ID;
		cout << "Packet " << pkts[i].ID << " ended at frame " << pkts[i].end << "." << endl;
	}
}

void roundRobin(vector<Packet> pkts) {

}

void WFQ(vector<Packet> pkts) {
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
	int transmissionCount = 0;
	while (transmissionCount < pkts.size()) {
		if (weightCounter < 2 && class1ordered[class1count].arrival <= transmissionCount) {
			transmissionOrder[transmissionCount] = class1ordered[transmissionCount].ID;
			class1count++;
			transmissionCount++;
			weightCounter++;
		}
		else if (weightCounter == 2 && class2ordered[class2count].arrival <= transmissionCount) {
			transmissionOrder[transmissionCount] = class2ordered[transmissionCount].ID;
			class2count++;
			transmissionCount++;
			weightCounter = 0;
		}
		//note: need to send from other class if current class did not have their next packet arrive yet
		else {
			
		}
	}
	
}

void RR(vector<Packet> pkts)
{

	int framesOrdered = 0;
	int currentCheckedFrame = 0;

	vector<queue<Packet>> queues = { {}, {} };
	vector<Packet> orderedPkts = {};

	int queueNumber = 0;

	int queue1Index = 0, queue2Index = 0;

	while (framesOrdered < pkts.size())
	{
		for (int i = 0; i < pkts.size(); i++)
		{
			if (pkts[i].arrival == currentCheckedFrame)
				queues[pkts[i].flow - 1].push(pkts[i]);
		}

		cout << "We are currently examining queue: " << queueNumber << "." << endl;
		if (queues[queueNumber].size() > 0) // if the queue is not empty...
		{
			cout << "Queue " << queueNumber << " has sent packet " << queues[queueNumber].front().ID << "!" << endl;
			orderedPkts.push_back(queues[queueNumber].front());
			queues[queueNumber].front().end = framesOrdered;
			queues[queueNumber].pop();
			framesOrdered++;

			if (queueNumber == 0)
				queueNumber = 1;
			else
				queueNumber = 0;
		}
		else
		{
			cout << "Queue " << queueNumber << " was empty. Checking other queue..." << endl;
			if (queueNumber == 0)
				queueNumber = 1;
			else
				queueNumber = 0;

			if (queues[queueNumber].size() > 0) // if the queue is not empty...
			{
				cout << "Queue " << queueNumber << " has sent packet " << queues[queueNumber].front().ID << "!" << endl;
				orderedPkts.push_back(queues[queueNumber].front());
				queues[queueNumber].front().end = framesOrdered;
				queues[queueNumber].pop();
				framesOrdered++;

				if (queueNumber == 0)
					queueNumber = 1;
				else
					queueNumber = 0;
			}
		}

		currentCheckedFrame++;
	}

	cout << "Ordered Packets has " << orderedPkts.size() << " packets in it!" << endl;

	for (int i = 0; i < orderedPkts.size(); i++)
	{
		cout << "Packet number " << orderedPkts[i].ID << endl;
		orderedPkts[i].end = i;
	}

	for (int i = 0; i < pkts.size(); i++)
	{
		cout << "Packet " << pkts[i].ID << " ended at frame " << pkts[i].end << "." << endl;
	}
}

int main()
{
	// Do shit
	vector<Packet> pkts = { Packet(1, 1, 0), Packet(2, 1, 0), Packet(3, 1, 1),
							 Packet(4, 2, 1), Packet(5, 2, 3), Packet(6, 1, 2),
							 Packet(7, 2, 3), Packet(8, 2, 5), Packet(9, 2, 5),
							 Packet(10, 2, 7), Packet(11, 1, 8), Packet(12, 1, 8)
	};

	



	return 0;
}