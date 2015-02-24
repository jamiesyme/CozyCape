#include "pathfinder.hpp"
#include "tiles.hpp"
#include "gameobjectgod.hpp"
#include <cmath>

#define ABS(x) ((x) < 0 ? -(x) : (x))

PathFinder::PathFinder()
{
	mTiles    = 0;
	mNodeGrid = 0;
	setType("pathfinder");
}

PathFinder::~PathFinder()
{
}

bool PathFinder::findPath(const Vec2& s, const Vec2& e, PathInfo* info)
{
	// Get the tiles
	if (mTiles == 0) {
		mTiles = (Tiles*)getGod()->findByType("tiles");
		if (mTiles == 0)
			return false;
	}

	// Check that the start and end points are open
	const int stx = (int)std::floor(s.x);
	const int sty = (int)std::floor(s.y);
	const int etx = (int)std::floor(e.x);
	const int ety = (int)std::floor(e.y);
	if (mTiles->getTile(stx, sty) != 0 ||
	    mTiles->getTile(etx, ety) != 0)
	  return false;
	  
	// Initialize our grid
	if (mNodeGrid == 0 || 
	    mNodeGrid->getSizeX() != mTiles->getSizeX() ||
	    mNodeGrid->getSizeY() != mTiles->getSizeY()) {
	  delete mNodeGrid;
	  mNodeGrid = new NodeGrid(mTiles->getSizeX(), mTiles->getSizeY());
	} else
		mNodeGrid->reset();
	mNodeGrid->setGoal(etx, ety);
	mNodeGrid->open(stx, sty, 0);
		
	// Find the path
	Node* n = mNodeGrid->closeBest();
	while (n != 0)
	{
		// Are we at the destination
		if (n->x == etx && n->y == ety)
			break;
			
		// Open new neighbors
		if (mTiles->getTile(n->x - 1, n->y) == 0)
			mNodeGrid->open(n->x - 1, n->y, n);
		if (mTiles->getTile(n->x + 1, n->y) == 0)
			mNodeGrid->open(n->x + 1, n->y, n);
		if (mTiles->getTile(n->x, n->y - 1) == 0)
			mNodeGrid->open(n->x, n->y - 1, n);
		if (mTiles->getTile(n->x, n->y + 1) == 0)
			mNodeGrid->open(n->x, n->y + 1, n);
			
		// Get the next node
		n = mNodeGrid->closeBest();
	}
	
	// If we quit before reaching the end, quit
	if (n == 0 || n->x != etx || n->y != ety)
		return false;
	
	// Build the path
	if (info != 0) {
		info->clear();
		while (n != 0) {
			info->push(Vec2((float)n->x + 0.5f, (float)n->y + 0.5f));
			n = n->p;
		}
	}
	return true;
}

// ====================
// PathFinder::NodeGrid
// ====================
PathFinder::NodeGrid::NodeGrid(int w, int h)
{
	mSizeX = w;
	mSizeY = h;
	mGoalX = -1;
	mGoalY = -1;
	mNodes  = new Node[w * h];
	mStates = new  int[w * h];
	
	// Setup the nodes
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++) {
			mNodes[x * h + y].x = x;
			mNodes[x * h + y].y = y;
		}
	
	// Reset everything
	reset();
}

PathFinder::NodeGrid::~NodeGrid()
{
	delete[] mNodes;
	delete[] mStates;
}

void PathFinder::NodeGrid::reset()
{
	for (int i = 0; i < mSizeX * mSizeY; i++)
		mStates[i] = 0;
	mOpenList.reset();
}

int PathFinder::NodeGrid::getSizeX() const
{
	return mSizeX;
}

int PathFinder::NodeGrid::getSizeY() const
{
	return mSizeY;
}

void PathFinder::NodeGrid::setGoal(int x, int y)
{
	mGoalX = x;
	mGoalY = y;
}

void PathFinder::NodeGrid::open(int x, int y, Node* parent)
{
	// Early out
	if (x < 0 || x >= mSizeX || y < 0 || y >= mSizeY)
		return;
	
	// Calculate the index
	const int index = x * mSizeY + y;

	// We can't touch closed nodes
	if (mStates[index] < 0)
		return;
		
	// If it's already open, the parent has to be better
	if (mStates[index] > 0) {
		if (parent == 0)
			return;
		if (mNodes[index].g <= parent->g + 1)
			return;
		mOpenList.remove(&mNodes[index]);
	}
	
	// Calculate the scores
	float g = 0;
	if (parent != 0)
		g = parent->g + 1;
	float h = (float)(ABS(mGoalX - x) + ABS(mGoalY - y));
	
	// Open the node
	Node* node = &mNodes[index];
	node->g = g;
	node->h = h;
	node->f = node->g + node->h;
	node->p = parent;
	mStates[index] = 1;
	mOpenList.add(node);
}

PathFinder::Node* PathFinder::NodeGrid::closeBest()
{
	// Get the best open node
	Node* node = mOpenList.popBest();
	
	// Close it
	if (node != 0)
		mStates[node->x * mSizeY + node->y] = -1;
		
	return node;
}

// ==============================
// PathFinder::NodeGrid::OpenList
// ==============================
PathFinder::NodeGrid::OpenList::OpenList()
{
	mHead = 0;
}

PathFinder::NodeGrid::OpenList::~OpenList()
{
	reset();
}

void PathFinder::NodeGrid::OpenList::reset()
{
	OpenNode* oNode = mHead;
	while (oNode != 0) {
		OpenNode* oNext = oNode->next;
		delete oNode;
		oNode = oNext;
	}
	mHead = 0;
}

void PathFinder::NodeGrid::OpenList::add(Node* node)
{
	// Base case
	if (mHead == 0) {
		mHead = new OpenNode();
		mHead->node = node;
		mHead->next = 0;
		
	// Every other case
	} else {
		OpenNode* oNode = mHead;
		while (oNode != 0) {
			OpenNode* oNext = oNode->next;
			if (oNext == 0 || oNext->node->f > node->f) {
				OpenNode* newONode = new OpenNode();
				newONode->node = node;
				newONode->next = oNext;
				oNode->next = newONode;
				break;
			}
			oNode = oNext;
		}
	}
}

void PathFinder::NodeGrid::OpenList::remove(Node* node)
{
	// Base case
	if (mHead == 0 &&
	    mHead->node == node) {
	  OpenNode* oNode = mHead->next;
	  delete mHead;
	  mHead = oNode;  
	  
	// Every other case
	} else {
		OpenNode* oNode = mHead;
		while (oNode != 0) {
			OpenNode* oNext = oNode->next;
			if (oNext != 0 && oNext->node == node) {
				OpenNode* oNextNext = oNext->next;
				delete oNext;
				oNode->next = oNextNext;
				break;
			}
			oNode = oNode->next;
		}
	}
}

PathFinder::Node* PathFinder::NodeGrid::OpenList::popBest()
{
	Node* node = 0;
	if (mHead != 0) {
		node = mHead->node;
		OpenNode* oNode = mHead->next;
		delete mHead;
		mHead = oNode;
	}
	return node;
}
