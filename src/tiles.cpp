#include "tiles.hpp"
#include "raycast.hpp"
#include "pathinfo.hpp"
#include "image.hpp"
#include "commongl.hpp"
#include <cmath>

Tiles::Tiles()
{
	mTiles = 0;
	mSizeX = 0;
	mSizeY = 0;
	setType("tiles");
	setBodyComplex();
}

Tiles::Tiles(int w, int h)
{
	mTiles = 0;
	mSizeX = 0;
	mSizeY = 0;
	setType("tiles");
	setBodyComplex();
	reset(w, h);
}

Tiles::~Tiles()
{
	delete[] mTiles;
}

void Tiles::onDraw()
{
	CommonGL::setColor(Color::White);
	for (int x = 0; x < mSizeX; x++) {
		for (int y = 0; y < mSizeY; y++) {
			const int tile = getTile(x, y);
			if (tile == 0) {
				const Vec2 p1 = Vec2((float)x, (float)y);
				const Vec2 p2 = p1 + Vec2(1, 1);
				CommonGL::drawRect(p1, p2);
			}
		}
	}
}

void Tiles::load(const std::string& filename)
{
	// Load the map
	Image levelImg;
	if (!levelImg.load(filename))
		return;
		
	// Set the size
	reset(levelImg.getSizeX(), levelImg.getSizeY());
	
	// Save the tiles
	for (int x = 0; x < mSizeX; x++) {
		for (int y = 0; y < mSizeY; y++) {
			if (levelImg.getPixel(x, y) == Color::Black)
				setTile(x, y, 1);
		}
	}
}

void Tiles::reset(int w, int h)
{
	delete[] mTiles;
	mTiles = 0;
	mSizeX = 0;
	mSizeY = 0;
	
	if (w <= 0 || h <= 0)
		return;
		
	mSizeX = w;
	mSizeY = h;
	mTiles = new int[w * h];
	for (int i = 0; i < w * h; i++)
		mTiles[i] = 0;
}

void Tiles::setTile(int x, int y, int t)
{
	if (x < 0 || x >= mSizeX ||
	    y < 0 || y >= mSizeY)
	  return;
	 mTiles[x * mSizeY + y] = t;
}

int Tiles::getTile(int x, int y) const
{
	if (x < 0 || x >= mSizeX ||
	    y < 0 || y >= mSizeY)
	  return -1;
	return mTiles[x * mSizeY + y];
}

int Tiles::getTileAt(const Vec2& p) const
{
	return getTile((int)std::floor(p.x), (int)std::floor(p.y));
}

int Tiles::getSizeX() const
{
	return mSizeX;
}

int Tiles::getSizeY() const
{
	return mSizeY;
}

static bool isPointInSolid(Tiles* t, const Vec2& p)
{
	return (t->getTile((int)std::floor(p.x), (int)std::floor(p.y)) > 0);
}

bool Tiles::fixComplexBodyCollision(Entity* e)
{
	// NOTE: Only deal with circle shapes right now.
	if (!e->isBodyCircle())
		return false;
		
	Vec2  p      = e->getPosition();
	float radius = e->getBodyRadius();
	bool madeFix = false;
	
	// Check adjacent blocks
	const Vec2 el = p - Vec2(radius, 0);
	if (isPointInSolid(this, el)) {
		p = Vec2(std::ceil(el.x) + radius, el.y);
		madeFix = true;
	}
	const Vec2 er = p + Vec2(radius, 0);
	if (isPointInSolid(this, er)) {
		p = Vec2(std::floor(er.x) - radius, el.y);
		madeFix = true;
	}
	const Vec2 eb = p - Vec2(0, radius);
	if (isPointInSolid(this, eb)) {
		p = Vec2(eb.x, std::ceil(eb.y) + radius);
		madeFix = true;
	}
	const Vec2 et = p + Vec2(0, radius);
	if (isPointInSolid(this, et)) {
		p = Vec2(et.x, std::floor(et.y) - radius);
		madeFix = true;
	}
	
	// Check diagonals
	const Vec2 ebl = p + Vec2(-radius, -radius);
	if (isPointInSolid(this, ebl)) 
	{
		const Vec2 t = Vec2(std::ceil(ebl.x), std::ceil(ebl.y));
		if ((p - t).length() < radius) {
			p = t + (p - t).normalized() * radius;
			madeFix = true;
		}
	}
	const Vec2 ebr = p + Vec2(radius, -radius);
	if (isPointInSolid(this, ebr)) 
	{
		const Vec2 t = Vec2(std::floor(ebr.x), std::ceil(ebr.y));
		if ((p - t).length() < radius) {
			p = t + (p - t).normalized() * radius;
			madeFix = true;
		}
	}
	const Vec2 etl = p + Vec2(-radius, radius);
	if (isPointInSolid(this, etl)) 
	{
		const Vec2 t = Vec2(std::ceil(etl.x), std::floor(etl.y));
		if ((p - t).length() < radius) {
			p = t + (p - t).normalized() * radius;
			madeFix = true;
		}
	}
	const Vec2 etr = p + Vec2(radius, radius);
	if (isPointInSolid(this, etr)) 
	{
		const Vec2 t = Vec2(std::floor(etr.x), std::floor(etr.y));
		if ((p - t).length() < radius) {
			p = t + (p - t).normalized() * radius;
			madeFix = true;
		}
	}
	
	if (madeFix)
		e->setPosition(p);
	
	return madeFix;
}

bool Tiles::doComplexBodyRaycast(const Ray& ray, RaycastInfo* info)
{
	// Early out
	if (ray.direction.length2() == 0.0f)
		return false;
		
	// Get the beginning stats
	bool didHit = false;
	float x = ray.position.x;
	float y = ray.position.y;
	int tx  = (int)std::floor(x);
	int ty  = (int)std::floor(y);
	int dtx = (ray.direction.x < 0 ? -1 : (ray.direction.x > 0 ? 1 : 0));
	int dty = (ray.direction.y < 0 ? -1 : (ray.direction.y > 0 ? 1 : 0));
	
	while (true) 
	{
		// Early out if we're out of bounds and heading out
		if (tx < 0 && dtx < 0 ||
		    ty < 0 && dty < 0 ||
		    tx >= getSizeX() && dtx > 0 ||
		    ty >= getSizeY() && dty > 0)
		  break;
	
		// Calculate the deltaX and deltaY
		float dx = -1.0f;
		float dy = -1.0f;
		if (dtx != 0)
		{
			int ntx = tx + dtx;
			float nx = (float)ntx + 0.5f * (float)(-dtx + 1);
			dx = (nx - x) / ray.direction.x;
		}
		if (dty != 0)
		{
			int nty = ty + dty;
			float ny = (float)nty + 0.5f * (float)(-dty + 1);
			dy = (ny - y) / ray.direction.y;
		}
		
		// Pick the lower delta
		float d = -1.0f;
		int mtx = 0;
		int mty = 0;
		if (dx >= 0.0f && dx < dy) {
			d = dx;
			mtx = dtx;
		} else {
			d = dy;
			mty = dty;
		}
			
		// If no delta was picked, we're not moving
		if (d < 0.0f)
			break;
			
		// Move our ray coordinates
		x += ray.direction.x * d;
		y += ray.direction.y * d;
		
		// Move our tile
		tx += mtx;
		ty += mty;
		
		// Check if the current tile is solid
		if (getTile(tx, ty) > 0)
		{
			if (info != 0) {
				info->point  = Vec2(x, y);
				info->normal = Vec2(-(float)mtx, -(float)mty);
			}
			didHit = true;
			break;
		}
	}
	
	return didHit;
}
