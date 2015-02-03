#include "map.hpp"
#include "entity.hpp"
#include "simplepath.hpp"
#include <GL/gl.h>
#include <vector>
#include <list>
#include <cmath>

#define ABS(x) ((x) < 0 ? (-(x)) : (x))

namespace {
	std::vector<Entity*> gEnts;
	int* gTiles;
	int  gWidth;
	int  gHeight;
	
	struct Node {
		int   x;
		int   y;
		float g;
		float h;
		float f() { return g + h; }
		Node* p;
		bool  isOpen;
	};
	
	class OpenList {
		std::list<Node*> nodes;
	public:
		void add(Node* n) 
		{
			n->isOpen = true;
			for (auto it = nodes.begin(); it != nodes.end(); it++) {
				if (n->f() < (*it)->f()) {
					nodes.insert(it, n);
					return;
				}
			}
			nodes.push_back(n);
		}
		Node* popFront()
		{
			Node* n = nodes.front();
			nodes.erase(nodes.begin());
			n->isOpen = false;
			return n;
		}
		bool isEmpty()
		{
			return (nodes.size() == 0);
		}
		void update(Node* n)
		{
			for (auto it = nodes.begin(); it != nodes.end(); it++) {
				if (*it == n) {
					nodes.erase(it);
					break;
				}
			}
			add(n);
		}
	};
	
	class NodeGrid {
		Node** nodes;
		int w;
		int h;
		int ex;
		int ey;
		OpenList* ol;
	public:
		NodeGrid(int w, int h, int ex, int ey, OpenList* ol) 
		{
			nodes = new Node*[w * h];
			for (int i = 0; i < w * h; i++)
				nodes[i] = 0;
			this->w = w;
			this->h = h;
			this->ex = ex;
			this->ey = ey;
			this->ol = ol;
		}
		~NodeGrid() 
		{
			for (int i = 0; i < w * h; i++)
				delete nodes[i];
			delete[] nodes;
		}
		bool isClosed(int x, int y)
		{
			if (nodes[x * h + y] == 0)
				return false;
			return !nodes[x * h + y]->isOpen;
		}
		Node* addOpenNode(int x, int y, Node* parent)
		{
			if (nodes[x * h + y] != 0)
				return nodes[x * h + y];
			Node* n = new Node();
			n->x = x;
			n->y = y;
			if (parent != 0)
				n->g = parent->g + 1;
			else
				n->g = 0;
			n->h = (float)(ABS(ex - x) + ABS(ey - y));
			n->p = parent;
			nodes[x * h + y] = n;
			ol->add(n);
		}
		Node* getNode(int x, int y)
		{
			return nodes[x * h + y];
		}
		bool tryBetterParent(int x, int y, Node* parent)
		{
			Node* n = nodes[x * h + y];
			if (n == 0)     return false; // The node has to exist
			if (!n->isOpen) return false; // The node has to be open
			const float g = parent->g + 1;
			if (n->g <= g)  return false; // The parent needs to be better
			n->g = g;
			n->p = parent;
			ol->update(n);
			return true;
		}
	};
}

void Map::init(int w, int h)
{
	gWidth  = 0;
	gHeight = 0;
	gTiles  = 0;
	setSize(w, h);
}

void Map::kill()
{
	gWidth  = 0;
	gHeight = 0;
	delete[] gTiles;
	gTiles  = 0;
	for (unsigned int i = 0; i < gEnts.size(); i++)
		delete gEnts[i];
	gEnts.clear();
}

void Map::onTick()
{
	// Update the entities
	for (unsigned int i = 0; i < gEnts.size(); i++)
		gEnts[i]->onTick();
		
	// Remove the invalid entities
	for (unsigned int i = 0; i < gEnts.size(); i++) {
		if (!gEnts[i]->isValid()) {
			delete gEnts[i];
			gEnts.erase(gEnts.begin() + i--);
		}
	}
}

void Map::onDraw()
{
	// Draw tiles
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	for (int x = 0; x < gWidth; x++) {
		for (int y = 0; y < gHeight; y++) {
			if (getTile(x, y) == 0) {
				glVertex2i(x    , y    );
				glVertex2i(x + 1, y    );
				glVertex2i(x + 1, y + 1);
				glVertex2i(x + 1, y + 1);
				glVertex2i(x    , y + 1);
				glVertex2i(x    , y    );
			}
		}
	}
	glEnd();
	
	// Draw entities
	for (unsigned int i = 0; i < gEnts.size(); i++)
		gEnts[i]->onDraw();
}

void Map::onPhysics()
{
	for (unsigned int i = 0; i < gEnts.size(); i++)
	{
		Entity* ent = gEnts[i];
		if (!ent->isSolid())
			continue;
		
		Vec2  pos     = ent->getPos();
		float radius  = ent->getRadius();
		float radius2 = radius * radius;
		Vec2  tilePos = Vec2();
		int   tileVal = 0;
		
		// Check right tile
		tilePos = Vec2(std::floor(pos.x + radius),
		               std::floor(pos.y));
		tileVal = Map::getTile((int)tilePos.x, (int)tilePos.y);
		if (tileVal != 0)
		{
			pos.x = tilePos.x - radius;
		}
			
		// Check left tile
		tilePos = Vec2(std::floor(pos.x - radius),
		               std::floor(pos.y));
		tileVal = Map::getTile((int)tilePos.x, (int)tilePos.y);
		if (tileVal != 0)
		{
			pos.x = tilePos.x + radius + 1;
		}
		
		// Check top tile
		tilePos = Vec2(std::floor(pos.x),
		               std::floor(pos.y + radius));
		tileVal = Map::getTile((int)tilePos.x, (int)tilePos.y);
		if (tileVal != 0) 
		{
			pos.y = tilePos.y - radius;
		}
		                        
		// Check bottom tile
		tilePos = Vec2(std::floor(pos.x),
		               std::floor(pos.y - radius));
		tileVal = Map::getTile((int)tilePos.x, (int)tilePos.y);
		if (tileVal != 0)
		{
			pos.y = tilePos.y + radius + 1;
		}
		
		// Check top right
		tilePos = Vec2(std::floor(pos.x + radius),
		               std::floor(pos.y + radius));
		tileVal = Map::getTile((int)tilePos.x, (int)tilePos.y);
		if (tileVal != 0) 
		{
			const float dist2 = pos.distance2(tilePos);
			if (dist2 < radius2) 
			{
				const float diff = radius - pos.distance(tilePos);
				pos += (pos - tilePos).normalized() * diff;
			}
		}
		
		// Check top left
		tilePos = Vec2(std::floor(pos.x - radius) + 1.0f,
		               std::floor(pos.y + radius));
		tileVal = Map::getTile((int)tilePos.x - 1, (int)tilePos.y);
		if (tileVal != 0) 
		{
			const float dist2 = pos.distance2(tilePos);
			if (dist2 < radius2) 
			{
				const float diff = radius - pos.distance(tilePos);
				pos += (pos - tilePos).normalized() * diff;
			}
		}
		
		// Check bottom right
		tilePos = Vec2(std::floor(pos.x + radius),
		               std::floor(pos.y - radius) + 1.0f);
		tileVal = Map::getTile((int)tilePos.x, (int)tilePos.y - 1);
		if (tileVal != 0) 
		{
			const float dist2 = pos.distance2(tilePos);
			if (dist2 < radius2) 
			{
				const float diff = radius - pos.distance(tilePos);
				pos += (pos - tilePos).normalized() * diff;
			}
		}
		
		// Check bottom left
		tilePos = Vec2(std::floor(pos.x - radius) + 1.0f,
		               std::floor(pos.y - radius) + 1.0f);
		tileVal = Map::getTile((int)tilePos.x - 1, (int)tilePos.y - 1);
		if (tileVal != 0) 
		{
			const float dist2 = pos.distance2(tilePos);
			if (dist2 < radius2) 
			{
				const float diff = radius - pos.distance(tilePos);
				pos += (pos - tilePos).normalized() * diff;
			}
		}
		
		// Set the position
		ent->setPos(pos);
	}
}

bool Map::tileRaycast(const Raycast& ray, RaycastHitTile* hitInfo)
{
	// Early out if we don't have a direction
	if (ray.dir.length2() == 0.0f)
		return false;

	// Get the ray's max distance
	const float maxDist2 = (ray.dist < 0.0f ? 0.0f : ray.dist * ray.dist);
		
	// Store some raycast variables
	Vec2 norm = Vec2();
	Vec2 pos  = ray.pos;
	int intX  = (int)std::floor(pos.x);
	int intY  = (int)std::floor(pos.y);
	const int intDx = (ray.dir.x > 0) - (ray.dir.x < 0);
	const int intDy = (ray.dir.y > 0) - (ray.dir.y < 0);
	// ^^ SIGNUM(x) = (x > 0) - (x < 0);
	
	// Raycast
	while (true) 
	{
		// Check the distance to our max
		if (maxDist2 > 0.0f && (pos - ray.pos).length2() > maxDist2)
			break;
		
		// Get the tilePos
		if (getTile(intX, intY) > 0) {
			if (hitInfo != 0) {
				hitInfo->tx     = intX;
				hitInfo->ty     = intY;
				hitInfo->tile   = getTile(intX, intY);
				hitInfo->point  = pos;
				hitInfo->normal = norm;
				hitInfo->dist   = (ray.pos - pos).length();
			}
			return true;
		}
		
		// Check if we've left the map
		if (intX < 0            && ray.dir.x <= 0.0f ||
		    intY < 0            && ray.dir.y <= 0.0f ||
		    intX >= getWidth()  && ray.dir.x >= 0.0f ||
		    intY >= getHeight() && ray.dir.y >= 0.0f)
		  break;
		
		// Move the position
		float dx = 2.0f; // 2.0f because it's > 1.41 (max distance)
		float dy = 2.0f;
		if (intDx < 0)
			dx = ((float)intX - pos.x) / ray.dir.x;
		else if (intDx > 0)
			dx = ((float)intX - pos.x + 1.0f) / ray.dir.x;
		if (intDy < 0)
			dy = ((float)intY - pos.y) / ray.dir.y;
		else if (intDy > 0)
			dy = ((float)intY - pos.y + 1.0f) / ray.dir.y;
		if (dx < dy) {
			intX += intDx;
			norm  = Vec2(-(float)intDx, 0.0f);
			pos.x = std::round(pos.x + dx * ray.dir.x);
			pos.y = ray.pos.y + ray.dir.y * (pos.x - ray.pos.x) / ray.dir.x;
		} else {
			intY += intDy;
			norm  = Vec2(0.0f, -(float)intDy);
			pos.y = std::round(pos.y + dy * ray.dir.y);
			pos.x = ray.pos.x + ray.dir.x * (pos.y - ray.pos.y) / ray.dir.y;
		}
	}
	
	return false;
}

bool Map::entityRaycast(const Raycast& ray, RaycastHitEntity* hitInfo)
{
	const float rayDirRadians = std::atan2(ray.dir.y, ray.dir.x);
	const float rayDirCos     = std::cos( rayDirRadians);
	const float rayDirSin     = std::sin( rayDirRadians);
	const float rayDirCosNeg  = std::cos(-rayDirRadians);
	const float rayDirSinNeg  = std::sin(-rayDirRadians);
	
	RaycastHitEntity best;
	best.ent  = 0;
	best.dist = -1.0f;
	
	for (unsigned int i = 0; i < gEnts.size(); i++) 
	{
		// Get a solid entity
		Entity* ent = gEnts[i];
		if (!ent->isSolid())
			continue;
		const Vec2  pos    = ent->getPos();
		const float radius = ent->getRadius();
		
		// Get the transformed pos
		Vec2 tPos = pos - ray.pos;
		tPos = Vec2(tPos.x * rayDirCosNeg - tPos.y * rayDirSinNeg,
		            tPos.x * rayDirSinNeg + tPos.y * rayDirCosNeg);
		
		// Check that it's in front of the ray
		if (tPos.x - radius < 0.0f)
			continue;
			
		// Check that it's overlapping the line
		if (tPos.y - radius > 0.0f ||
		    tPos.y + radius < 0.0f)
		  continue;
		  
		// Calculate the distance to it
		const float dist = tPos.x - std::sqrt(radius * radius - tPos.y * tPos.y);
		
		// Check if we already found a better one
		if (best.dist >= 0.0f && best.dist <= dist)
			continue;
			
		// This is our best so far
		best.ent    = ent;
		best.dist   = dist;
		best.point  = Vec2(dist * rayDirCos, dist * rayDirSin) + ray.pos;
		best.normal = (Vec2(dist, 0.0f) - tPos).normalized();
		best.normal = Vec2(best.normal.x * rayDirCos - best.normal.y * rayDirSin,
		                   best.normal.x * rayDirSin + best.normal.y * rayDirCos);
	}
	
	// Did we hit something
	if (best.ent != 0) {
		if (hitInfo != 0)
			*hitInfo = best;
		return true;
	}
	
	return false;
}

bool Map::pathfind(const Vec2& start, const Vec2& end, SimplePath* pathInfo)
{
	const int sx = (int)std::floor(start.x);
	const int sy = (int)std::floor(start.y);
	const int ex = (int)std::floor(end.x);
	const int ey = (int)std::floor(end.y);
	if (sx < 0 || sx >= getWidth() ||
	    sy < 0 || sy >= getHeight() ||
	    ex < 0 || ex >= getWidth() ||
	    ey < 0 || ey >= getHeight() ||
	    pathInfo == 0)
	  return false;
	
	if (getTile(sx, sy) != 0 ||
			getTile(ex, ey) != 0)
		return false;
		
	OpenList openList;
	NodeGrid nodeGrid(getWidth(), getHeight(), ex, ey, &openList);
	nodeGrid.addOpenNode(sx, sy, 0);
	while (!openList.isEmpty()) 
	{
		// Get the best node from the open list
		Node* cur = openList.popFront();
		const int x = cur->x;
		const int y = cur->y;
		
		// Did we just finish?
		if (cur->x == ex && cur->y == ey)
			break;
		
		// Get adjacent movement possibilities
		bool canMoveL = true;
		bool canMoveR = true;
		bool canMoveT = true;
		bool canMoveB = true;
		if (getTile(x - 1, y) != 0 || nodeGrid.isClosed(x - 1, y))
			canMoveL = false;
		if (getTile(x + 1, y) != 0 || nodeGrid.isClosed(x + 1, y))
			canMoveR = false;
		if (getTile(x, y - 1) != 0 || nodeGrid.isClosed(x, y - 1))
			canMoveB = false;
		if (getTile(x, y + 1) != 0 || nodeGrid.isClosed(x, y + 1))
			canMoveT = false;
		
		// Add nodes to open list
		if (canMoveL || canMoveR || canMoveT || canMoveB) {
		
			// Check if it's already on there
			//   If it is, just correct it with the better parent
			if (canMoveL && nodeGrid.tryBetterParent(x - 1, y, cur))
				canMoveL = false;
			if (canMoveR && nodeGrid.tryBetterParent(x + 1, y, cur))
				canMoveR = false;
			if (canMoveB && nodeGrid.tryBetterParent(x, y - 1, cur))
				canMoveB = false;
			if (canMoveT && nodeGrid.tryBetterParent(x, y + 1, cur))
				canMoveT = false;
			
			// Add new nodes
			if (canMoveL)
				nodeGrid.addOpenNode(x - 1, y, cur);
			if (canMoveR)
				nodeGrid.addOpenNode(x + 1, y, cur);
			if (canMoveB)
				nodeGrid.addOpenNode(x, y - 1, cur);
			if (canMoveT)
				nodeGrid.addOpenNode(x, y + 1, cur);
		}
	}
	
	// Get the final node
	Node* finalNode = nodeGrid.getNode(ex, ey);
	if (finalNode == 0)
		return false;
	
	// Go through and construct the route
	SimplePath* path = pathInfo;
	Node* n = finalNode;
	while (n != 0) {
		const Vec2 pos = Vec2((float)n->x + 0.5f, (float)n->y + 0.5f);
		if (n->p == 0)
			path->setPos(pos);
		else {
			SimplePath* newPath = new SimplePath(pos);
			newPath->setNext(path->getNext());
			path->setNext(newPath);
		}
		n = n->p;
	}
	
	return true;
}

void Map::manageEntity(Entity* e)
{
	// Skip null entities
	if (e == 0)
		return;
		
	// Check for duplicates
	for (unsigned int i = 0; i < gEnts.size(); i++)
		if (gEnts[i] == e)
			return;
			
	// Manage the entity
	gEnts.push_back(e);
}

void Map::setTile(int x, int y, int t)
{
	if (x < 0 || x >= gWidth || y < 0 || y >= gHeight)
		return;
	gTiles[x * gHeight + y] = t;
}

int Map::getTile(int x, int y)
{
	if (x < 0 || x >= gWidth || y < 0 || y >= gHeight)
		return -1;
	return gTiles[x * gHeight + y];
}

void Map::setSize(int w, int h)
{
	delete[] gTiles;
	if (w <= 0 || h <= 0) {
		gWidth  = 0;
		gHeight = 0;
		gTiles  = 0;
	} else {
		gWidth  = w;
		gHeight = h;
		gTiles  = new int[w * h];
		for (int i = 0; i < w * h; i++)
			gTiles[i] = 0;
	}
}

int Map::getWidth()
{
	return gWidth;
}

int Map::getHeight()
{
	return gHeight;
}
