#include "DLX.h"


using namespace dlx;

int main(int argc, char *argv[])
{
	bool data[][7] = {
		{0, 0, 1, 0, 1, 1, 0},
		{1, 0, 0, 1, 0, 0, 1},
		{0, 1, 1, 0, 0, 1, 0},
		{1, 0, 0, 1, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 1},
		{0, 0, 0, 1, 1, 0, 1}
	};

	DLX d(7);
	d.appendRow(data[0]);
	d.appendRow(data[1]);
	d.appendRow(data[2]);
	d.appendRow(data[3]);
	d.appendRow(data[4]);
	d.appendRow(data[5]);

	std::vector<int> ret = d.dance();

	return 0;
}