#include "DLX.h"
#include <set>

using namespace dlx;

DLX::DLX()
	: _head(NULL)
	, _row(0)
{

}

DLX::DLX( int col )
	: _head(NULL)
	, _row(0)
{
	init(col);
}

DLX::~DLX()
{
	clearData();
}

void DLX::init( int col )
{
	clearData();
	_head = new Node;

	for(int i = 0; i < col; i ++)
	{
		Node *node = new Node;
		if(_cols.empty())
		{
			_head->right = node;
			_head->left = node;
			node->left = _head;
			node->right = _head;
		}
		else
		{
			Node *preNode = _cols.back();
			node->right = preNode->right;
			preNode->right = node;
			node->left = preNode;
		}
		node->up = node;
		node->down = node;
		node->col = node;
		node->row = 0;
		_cols.push_back(node);
	}
}

void DLX::appendRow( const std::list<int> &data )
{
	int colCount = _cols.size();
	bool *boolData = new bool[colCount];
	for(int i = 0; i < colCount; i ++)
		boolData[i] = false;

	for(std::list<int>::const_iterator it = data.begin(); it != data.end(); ++ it)
		boolData[*it] = true;

	appendRow(boolData);

	delete []boolData;
}

void DLX::appendRow( bool *data )
{
	if(data == NULL)
		return;

	_row ++;

	Node *leftNode = NULL;
	Node *lefterNode = NULL;
	int colCount = _cols.size();
	for(int i = 0; i < colCount; i ++)
	{
		if(data[i])
		{
			Node *colNode = _cols[i];
			Node *upNode = colNode->up;
			Node *node = new Node;

			node->up = upNode;
			upNode->down = node;
			colNode->up = node;
			node->down = colNode;
			node->col = colNode;
			node->row = _row;

			if(leftNode != NULL)
			{
				node->left = leftNode;
				node->right = leftNode->right;
				leftNode->right = node;
				lefterNode->left = node;
			}
			else
			{
				node->left = node;
				node->right = node;
				lefterNode = node;
			}
			leftNode = node;
		}
	}
}

std::vector<int> dlx::DLX::dance()
{
	std::vector<int> ret;
	if(danceInternal(0, ret))
		return ret;
	return std::vector<int>();
}

bool dlx::DLX::danceInternal( int lev, std::vector<int> &ans )
{
	Node *c1 = _head->right;
	if(c1 == _head)
	{
		ans.resize(lev);
		return true;
	}

	if(c1->up == c1)
		return false;

	removeCol(c1);

	Node *downNode = c1->down;
	while(downNode != c1)
	{
		if(lev >= ans.size())
			ans.resize(lev + 1, -1);
		ans[lev] = downNode->row;
		Node *rightNode = downNode->right;
		while(rightNode != downNode)
		{
			removeCol(rightNode->col);
			rightNode = rightNode->right;
		}

		if(danceInternal(lev + 1, ans))
			return true;

		Node *leftNode = downNode->left;
		while(leftNode != downNode)
		{
			resumeCol(leftNode->col);
			leftNode = leftNode->left;
		}

		downNode = downNode->down;
	}

	resumeCol(c1);
	return false;
}

void DLX::removeCol( Node *colNode )
{
	if(colNode == NULL || colNode->row != 0)
		return;

	colNode->left->right = colNode->right;
	colNode->right->left = colNode->left;
	
	Node *downNode = colNode->down;
	while(downNode != colNode)
	{
		Node *rightNode = downNode->right;
		while(rightNode != downNode)
		{
			rightNode->up->down = rightNode->down;
			rightNode->down->up = rightNode->up;
			rightNode = rightNode->right;
		}
		downNode = downNode->down;
	}
}

void DLX::resumeCol( Node *colNode )
{
	if(colNode == NULL || colNode->row != 0)
		return;

	colNode->right->left = colNode;
	colNode->left->right = colNode;

	Node *upNode = colNode->up;
	while(upNode != colNode)
	{
		Node *rightNode = upNode->right;
		while(rightNode != upNode)
		{
			rightNode->up->down = rightNode;
			rightNode->down->up = rightNode;
			rightNode = rightNode->right;
		}
		upNode = upNode->up;
	}
}

void DLX::clearData()
{
	if(_head != NULL)
	{
		delete _head;
		_head = NULL;
	}

	for(std::vector<Node*>::iterator it = _cols.begin(); it != _cols.end(); ++ it)
	{
		Node *rowNode = *it;
		Node *node = rowNode->down;
		delete rowNode;
		while(node != rowNode)
		{
			Node *tmp = node->down;
			delete node;
			node = tmp;
		}
	}

	_row = 0;
}
