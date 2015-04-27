#include "DLX.h"
#include <map>

using namespace dlx;

struct AnsNode
{
	int col;
	int row;
	int num;
};

void getRowData(const AnsNode &node, std::list<int> &rowData)
{
	int d1 = node.row * 9 + node.col;
	int d2 = 80 + node.row * 9 + node.num;
	int d3 = 161 + node.col * 9 + node.num;

	int x = node.col % 3;
	int y = node.row % 3;
	int idx = y * 3 + x;
	int d4 = 242 + idx * 9 + node.num;
	
	rowData.clear();
	rowData.push_back(d1);
	rowData.push_back(d2);
	rowData.push_back(d3);
	rowData.push_back(d4);
}

int main(int argc, char *argv[])
{
	//bool data[][7] = {
	//	{0, 0, 1, 0, 1, 1, 0},
	//	{1, 0, 0, 1, 0, 0, 1},
	//	{0, 1, 1, 0, 0, 1, 0},
	//	{1, 0, 0, 1, 0, 0, 0},
	//	{0, 1, 0, 0, 0, 0, 1},
	//	{0, 0, 0, 1, 1, 0, 1}
	//};

	//DLX d(7);
	//d.appendRow(data[0]);
	//d.appendRow(data[1]);
	//d.appendRow(data[2]);
	//d.appendRow(data[3]);
	//d.appendRow(data[4]);
	//d.appendRow(data[5]);

	//std::vector<int> ret = d.dance();

//	int shudu[9][9] = {
//		{0,3,0, 5,4,2, 0,1,0},
//		{0,2,8, 0,0,0, 5,6,0},
//		{0,0,0, 0,6,9, 0,0,0},
////----------------------------------
//		{0,5,0, 0,0,3, 0,0,1},
//		{8,9,3, 6,0,1, 0,2,7},
//		{1,0,0, 7,2,8, 0,0,0},
////----------------------------------
//		{0,0,1, 0,7,4, 0,9,0},
//		{0,4,9, 2,8,0, 0,0,0},
//		{6,7,0, 0,0,5, 0,3,4}
//	};

	int shudu[9][9] = {
		{0,6,0, 5,9,3, 0,0,0},
		{9,0,1, 0,0,0, 5,0,0},
		{0,3,0, 4,0,0, 0,9,0},
		//----------------------------------
		{1,0,8, 0,2,0, 0,0,4},
		{4,0,0, 3,0,9, 0,0,1},
		{2,0,0, 0,1,0, 6,0,9},
		//----------------------------------
		{0,8,0, 0,0,6, 0,2,0},
		{0,0,4, 0,0,0, 8,0,7},
		{0,0,0, 7,8,5, 0,1,0}
	};

	dlx::DLX dlx(81 * 4);
	std::map<int, AnsNode> ansMap;

	for(int row = 0; row < 9; row ++)
	{
		for(int col = 0; col < 9; col ++)
		{
			std::list<int> rowData;
			int num = shudu[row][col];
			if(num == 0)
			{
				for(int i = 1; i < 10; i ++)
				{
					AnsNode node = {col, row, i};
					getRowData(node, rowData);
					int rowIdx = dlx.appendRow(rowData);
					ansMap.insert(std::make_pair(rowIdx, node));
				}
			}
			else
			{
				AnsNode node = {col, row, num};
				getRowData(node, rowData);
				int rowIdx = dlx.appendRow(rowData);
				ansMap.insert(std::make_pair(rowIdx, node));
			}
		}
	}

	std::vector<int> r = dlx.dance();

	return 0;
}