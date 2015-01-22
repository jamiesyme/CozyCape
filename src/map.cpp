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
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	for (int x = 0; x < gWidth; x++) {
		for (int y = 0; y < gHeight; y++) {
			if (getTile(x, y) != 0) {
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
		Entity* ent  = gEnts[i];
		Vec2  pos    = ent->getPos();
		float radius = ent->getRadius();
		
		// Check right tile
		Vec2 tile = Vec2(std::floor(pos.x + radius), std::floor(pos.y));
		if (Map::getTile((int)tile.x, (int)tile.y) != 0)
			pos.x = tile.x - radius;
		// Check left tile
		tile = Vec2(std::floor(pos.x - radius), std::floor(pos.y));
		if (Map::getTile((int)tile.x, (int)tile.y) != 0)
			pos.x = tile.x + radius + 1;
		// Check top tile
		tile = Vec2(std::floor(pos.x), std::floor(pos.y + radius));
		if (Map::getTile((int)tile.x, (int)tile.y) != 0)
			pos.y = tile.y - radius;
		// Check bottom tile
		tile = Vec2(std::floor(pos.x), std::floor(pos.y - radius));
		if (Map::getTile((int)tile.x, (int)tile.y) != 0)
			pos.y = tile.y + radius + 1;
			
		ent->setPos(pos);
	}
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
