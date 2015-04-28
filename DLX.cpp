#include "DLX.h"
#include <set>

using namespace dlx;

DLX::DLX()
	: _head(NULL)
	, _row(0)
{

}

DLX::DLX( int col)
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
	_head = new ColNode;
	_head->left = _head;
	_head->right = _head;
	_head->rowCount = 0;

	for(int i = 0; i < col; i ++)
	{
		ColNode *node = new ColNode;

		node->left = _head->left;
		node->right = _head;
		_head->left->right = node;
		_head->left = node;

		node->up = node;
		node->down = node;
		node->col = node;
		node->row = 0;
		node->rowCount = 0;

		_cols.push_back(node);
	}
}

int DLX::appendRow( const std::list<int> &data )
{
	int colCount = _cols.size();
	bool *boolData = new bool[colCount];
	for(int i = 0; i < colCount; i ++)
		boolData[i] = false;

	for(std::list<int>::const_iterator it = data.begin(); it != data.end(); ++ it)
		boolData[*it] = true;

	int ret = appendRow(boolData);

	delete []boolData;

	return ret;
}

int DLX::appendRow( bool *data )
{
	if(data == NULL)
		return -1;

	_row ++;

	Node *leftNode = NULL;
	int colCount = _cols.size();
	for(int i = 0; i < colCount; i ++)
	{
		if(data[i])
		{
			ColNode *colNode = _cols[i];
			Node *node = new Node;

			node->up = colNode->up;
			node->down = colNode;
			colNode->up->down = node;
			colNode->up = node;
			node->col = colNode;
			node->row = _row;
			colNode->rowCount ++;

			if(leftNode != NULL)
			{
				node->left = leftNode->left;
				node->right = leftNode;
				leftNode->left->right = node;
				leftNode->left = node;
			}
			else
			{
				node->left = node;
				node->right = node;
				leftNode = node;
			}
		}
	}

	return _row;
}

std::vector<std::vector<int> >  DLX::dance( bool multiAnswer /*= false*/, bool recover /*= true*/ )
{
	std::vector<std::vector<int> > ret;
	std::vector<int> ans;
	danceInternal(0, ret, ans, multiAnswer, recover);
	return ret;
}

bool DLX::danceInternal( int lev, std::vector<std::vector<int> > &anss, std::vector<int> &ans, bool multiAnswer, bool recover )
{
	if(_head->right == _head)
	{
		ans.resize(lev);
		anss.push_back(ans);
		return true;
	}

	ColNode *c1 = (ColNode *)_head->right;
	ColNode *tmpNode = c1;
	while(tmpNode != _head)
	{
		if(tmpNode->rowCount < c1->rowCount && tmpNode->rowCount > 0)
			c1 = tmpNode;
		tmpNode = (ColNode*)tmpNode->right;
	}

	removeCol(c1);

	bool ret = false;
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

		ret = danceInternal(lev + 1, anss, ans, multiAnswer, recover);
		if(ret && !recover && !multiAnswer)
			return true;

		Node *leftNode = downNode->left;
		while(leftNode != downNode)
		{
			resumeCol(leftNode->col);
			leftNode = leftNode->left;
		}

		if(ret && !multiAnswer)
			break;

		downNode = downNode->down;
	}

	resumeCol(c1);
	return ret;
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
			rightNode->down->up = rightNode->up;
			rightNode->up->down = rightNode->down;
			rightNode->col->rowCount --;
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
			rightNode->down->up = rightNode;
			rightNode->up->down = rightNode;
			rightNode->col->rowCount ++;
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

	for(std::vector<ColNode*>::iterator it = _cols.begin(); it != _cols.end(); ++ it)
	{
		Node *colNode = *it;
		Node *node = colNode->down;
		delete colNode;
		while(node != colNode)
		{
			Node *tmp = node->down;
			delete node;
			node = tmp;
		}
	}

	_row = 0;
}
