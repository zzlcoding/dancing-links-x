#include "DLX.h"
#include <map>
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <set>

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

	int x = node.col / 3;
	int y = node.row / 3;
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
	//bool data[][14] = {
	//	{0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0},
	//	{1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1},
	//	{0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0},
	//	{1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
	//	{0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
	//	{0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1},
	//	{0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0},
	//	{1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1},
	//	{0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0},
	//	{1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0},
	//	{0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
	//	{0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1}
	//};

	//DLX d(14);
	//d.appendRow(data[0]);
	//d.appendRow(data[1]);
	//d.appendRow(data[2]);
	//d.appendRow(data[3]);
	//d.appendRow(data[4]);
	//d.appendRow(data[5]);
	//d.appendRow(data[6]);
	//d.appendRow(data[7]);
	//d.appendRow(data[8]);
	//d.appendRow(data[9]);
	//d.appendRow(data[10]);
	//d.appendRow(data[11]);

	//std::vector<std::vector<int> > ret = d.dance(true);
	//std::vector<std::vector<int> > ret2 = d.dance(true, false);

	// sudoku example
	int sudoku[9][9] = {
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

	std::cout << "source:" << std::endl;
	for(int row = 0; row < 9; row ++)
	{
		std::cout << "	";
		for(int col = 0; col < 9; col ++)
		{
			int num = sudoku[row][col];
			std::cout << num << " ";
			if(col % 3 == 2)
				std::cout << " ";
		}
		std::cout << std::endl;
		if(row % 3 == 2)
			std::cout << std::endl;
	}
	std::cout << std::endl;

	dlx::DLX dlx(81 * 4);
	std::map<int, AnsNode> ansMap;

	for(int row = 0; row < 9; row ++)
	{
		for(int col = 0; col < 9; col ++)
		{
			std::list<int> rowData;
			int num = sudoku[row][col];
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

	std::vector<std::vector<int> > r = dlx.dance(false, false);

	if(r.empty())
	{
		std::cout << "No solution" << std::endl;
	}
	else
	{
		for(std::vector<int>::iterator it = r[0].begin(); it != r[0].end(); ++ it)
		{
			AnsNode &an = ansMap[*it];
			sudoku[an.row][an.col] = an.num;
		}

		std::cout << "answer:" << std::endl;
		for(int row = 0; row < 9; row ++)
		{
			std::cout << "	";
			for(int col = 0; col < 9; col ++)
			{
				int num = sudoku[row][col];
				std::cout << num << " ";
				if(col % 3 == 2)
					std::cout << " ";
			}
			std::cout << std::endl;
			if(row % 3 == 2)
				std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	return 0;
}