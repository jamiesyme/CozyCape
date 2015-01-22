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
		if (!ent->isSolid())
			continue;
		
		Vec2    pos  = ent->getPos();
		const float radius    = ent->getRadius();
		const float radiusSqr = radius * radius;
		
		// Check right tile
		const Vec2 rTile = Vec2(std::floor(pos.x + radius),
		                        std::floor(pos.y));
		if (Map::getTile((int)rTile.x, (int)rTile.y) != 0)
			pos.x = rTile.x - radius;
			
		// Check left tile
		const Vec2 lTile = Vec2(std::floor(pos.x - radius),
		                        std::floor(pos.y));
		if (Map::getTile((int)lTile.x, (int)lTile.y) != 0)
			pos.x = lTile.x + radius + 1;
		
		// Check top tile
		const Vec2 tTile = Vec2(std::floor(pos.x),
		                        std::floor(pos.y + radius));
		if (Map::getTile((int)tTile.x, (int)tTile.y) != 0)
			pos.y = tTile.y - radius;
		                        
		// Check bottom tile
		const Vec2 bTile = Vec2(std::floor(pos.x),
		                        std::floor(pos.y - radius));
		if (Map::getTile((int)bTile.x, (int)bTile.y) != 0)
			pos.y = bTile.y + radius + 1;
		
		// Check top right
		const Vec2 trTile = Vec2(std::floor(pos.x + radius),
		                         std::floor(pos.y + radius));
		if (Map::getTile((int)trTile.x, (int)trTile.y) != 0) {
			const float distSqr = (pos - trTile).getSqrMag();
			if (distSqr < radiusSqr) {
				const float diff = radius - std::sqrt(distSqr);
				pos -= (trTile - pos).getNormalized() * diff;
			}
		}
		
		// Check top left
		const Vec2 tlTile = Vec2(std::floor(pos.x - radius) + 1.0f,
		                         std::floor(pos.y + radius));
		if (Map::getTile((int)tlTile.x - 1, (int)tlTile.y) != 0) {
			const float distSqr = (pos - tlTile).getSqrMag();
			if (distSqr < radiusSqr) {
				const float diff = radius - std::sqrt(distSqr);
				pos -= (tlTile - pos).getNormalized() * diff;
			}
		}
		
		// Check bottom right
		const Vec2 brTile = Vec2(std::floor(pos.x + radius),
		                         std::floor(pos.y - radius) + 1.0f);
		if (Map::getTile((int)brTile.x, (int)brTile.y - 1) != 0) {
			const float distSqr = (pos - brTile).getSqrMag();
			if (distSqr < radiusSqr) {
				const float diff = radius - std::sqrt(distSqr);
				pos -= (brTile - pos).getNormalized() * diff;
			}
		}
		
		// Check bottom left
		const Vec2 blTile = Vec2(std::floor(pos.x - radius) + 1.0f,
		                         std::floor(pos.y - radius) + 1.0f);
		if (Map::getTile((int)blTile.x - 1, (int)blTile.y - 1) != 0) {
			const float distSqr = (pos - blTile).getSqrMag();
			if (distSqr < radiusSqr) {
				const float diff = radius - std::sqrt(distSqr);
				pos -= (blTile - pos).getNormalized() * diff;
			}
		}
		
		// Set the position
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
