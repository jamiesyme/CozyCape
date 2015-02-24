#pragma once

#include <vector>
#include <string>
#include "drawable.hpp"
class Camera;

class DrawLayer : public Drawable {
public:
	DrawLayer(const std::string& name);
	~DrawLayer();
	
	std::string getName() const;
	
	void onDraw();
	
	void    add(Drawable* d, const float depth = 0.0f);
	void remove(Drawable* d);
	void removeAll();
	
	void    setCamera(Camera* c);
	Camera* getCamera();

private:
	struct DrawNode {
		Drawable* drawer;
		float     depth;
	};

	std::string mName;
	std::vector<DrawNode> mDrawNodes;
	Camera* mCamera;
};
