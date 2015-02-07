#pragma once

#include "entity.hpp"
class PathInfo;
class Tiles;

class PathFinder : public Entity {
public:
	PathFinder();
	~PathFinder();
	
	bool findPath(const Vec2& start, const Vec2& end, PathInfo* info);

private:
	struct Node {
		int x;
		int y;
		float g;
		float h;
		float f;
		Node* p;
	};
	class NodeGrid {
	public:
		NodeGrid(int w, int h);
		~NodeGrid();
		
		void reset();
		int getSizeX() const;
		int getSizeY() const;
		
		void  setGoal(int x, int y);
		void  open(int x, int y, Node* parent);
		Node* closeBest();
		
	private:
		class OpenList {
		public:
			OpenList();
			~OpenList();
			
			void  reset();
			void  add(Node* n);
			void  remove(Node* n);
			Node* popBest();
			
		private:
			struct OpenNode {
				Node*     node;
				OpenNode* next;
			};
			OpenNode* mHead;
		};
	
		OpenList mOpenList;
		Node* mNodes;
		int*  mStates;
		int   mSizeX;
		int   mSizeY;
		int   mGoalX;
		int   mGoalY;
	};

	Tiles*    mTiles;
	NodeGrid* mNodeGrid;
};
