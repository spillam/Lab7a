#include <iostream>
#include <vector>
#include "packet.h"

using namespace std;

int main()
{
	// Do shit
	vector<Packet> pkts = { Packet(1, 1, 0), Packet(2, 1, 0), Packet(3, 1, 1),
							 Packet(4, 2, 1), Packet(5, 2, 3), Packet(6, 1, 2),
							 Packet(7, 2, 3), Packet(8, 2, 5), Packet(9, 2, 5),
							 Packet(10, 2, 7), Packet(11, 1, 8), Packet(12, 1, 8)
	};

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


	/*for (int i = 0; i < pkts.size(); i++)
	{
		if (pkts[i].arrival > i)
			pkts[i].end = pkts[i].arrival + 1;
		else
		{
			pkts[i].end = i + 2;
		}

		cout << "Packet " << pkts[i].ID << " was sent at frame " << pkts[i].end << "." << endl;
	}*/

	return 0;
}