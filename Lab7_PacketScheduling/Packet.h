#pragma once
class Packet
{
public:
	//attributes
	int ID;
	int flow;
	int arrival;
	int end;
	int delay;

	//constructor
	Packet(int in_ID, int in_flow, int in_arrival);

	//destructor
	~Packet();

};

