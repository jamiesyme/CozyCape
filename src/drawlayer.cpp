#include "drawlayer.hpp"
#include "camera.hpp"
#include "commongl.hpp"

DrawLayer::DrawLayer(const std::string& name)
{
	mName = name;
	mCamera = 0;
}

DrawLayer::~DrawLayer()
{
}

std::string DrawLayer::getName() const
{
	return mName;
}

void DrawLayer::onDraw()
{
	// Bind the layer settings
	CommonGL::push();
	if (mCamera != 0)
		mCamera->onBind();
	
	// Draw the drawables
	for (unsigned int i = 0; i < mDrawNodes.size(); i++)
		mDrawNodes[i].drawer->onDraw();
		
	// Unbind our settings
	CommonGL::pop();
}

void DrawLayer::add(Drawable* d, const float depth)
{
	// Skip null drawables
	if (d == 0)
		return;
		
	// Skip recursive draw layers
	if (d == this)
		return;
		
	// Create the node
	DrawNode node;
	node.drawer = d;
	node.depth  = depth;
		
	// Insert the node based on depth
	for (unsigned int i = 0; i < mDrawNodes.size(); i++) {
		if (depth > mDrawNodes[i].depth) {
			mDrawNodes.insert(mDrawNodes.begin() + i, node);
			return;
		}
	}
	
	// It goes at the end of the list
	mDrawNodes.push_back(node);
}

void DrawLayer::remove(Drawable* d)
{
	for (unsigned int i = 0; i < mDrawNodes.size(); i++) {
		if (d == mDrawNodes[i].drawer) {
			mDrawNodes.erase(mDrawNodes.begin() + i);
			return;
		}
	}
}

void DrawLayer::removeAll()
{
	mDrawNodes.clear();
}

void DrawLayer::setCamera(Camera* c)
{
	mCamera = c;
}

Camera* DrawLayer::getCamera()
{
	return mCamera;
}
