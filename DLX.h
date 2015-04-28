#ifndef __DLX_H__
#define __DLX_H__

#include <vector>
#include <list>

namespace dlx {

class ColNode;

class Node
{
public:
	Node *left;
	Node *right;
	Node *up;
	Node *down;
	ColNode *col;
	int row;
};

class ColNode
	: public Node
{
public:
	int rowCount;
};

class DLX
{
public:
	DLX();
	DLX(int col);
	virtual ~DLX();

	void init(int col);
	int appendRow(const std::list<int> &data);
	int appendRow(bool *data);
	std::vector<std::vector<int> > dance(bool multiAnswer = false, bool recover = true);

private:
	void removeCol(Node *colNode);
	void resumeCol(Node *colNode);
	bool danceInternal(int lev, std::vector<std::vector<int> > &anss, std::vector<int> &ans, bool multiAnswer, bool recover);
	void clearData();

private:
	std::vector<ColNode*> _cols;
	ColNode *_head;
	int _row;
};

}

#endif //__DLX_H__