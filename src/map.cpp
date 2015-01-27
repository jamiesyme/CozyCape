#include "map.hpp"
#include "entity.hpp"
#include <GL/gl.h>
#include <vector>
#include <cmath>

namespace {
	std::vector<Entity*> gEnts;
	int* gTiles;
	int  gWidth;
	int  gHeight;
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
	for (unsigned int i = 0; i < gEnts.size(); i++)
		gEnts[i]->onTick();
}

void Map::onDraw()
{
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
			norm  = Vec2((float)intDx, 0.0f);
			pos.x = std::round(pos.x + dx * ray.dir.x);
			pos.y = ray.pos.y + ray.dir.y * (pos.x - ray.pos.x) / ray.dir.x;
		} else {
			intY += intDy;
			norm  = Vec2(0.0f, (float)intDy);
			pos.y = std::round(pos.y + dy * ray.dir.y);
			pos.x = ray.pos.x + ray.dir.x * (pos.y - ray.pos.y) / ray.dir.y;
		}
	}
	
	return false;
}

bool Map::entityRaycast(const Raycast& ray, RaycastHitEntity* hitInfo)
{
	return false;
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

void Map::destroyEntity(Entity* e)
{
	// Skip null entities
	if (e == 0)
		return;
		
	// Locate and destroy the entity
	for (unsigned int i = 0; i < gEnts.size(); i++)
	{
		if (gEnts[i] == e) 
		{
			delete gEnts[i];
			gEnts.erase(gEnts.begin() + i);
			break;
		}
	}
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
