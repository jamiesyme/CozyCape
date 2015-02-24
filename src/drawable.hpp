#pragma once

class Drawable {
public:
	Drawable() {}
	virtual ~Drawable() {}
	
	virtual void onDraw() = 0;

private:
	
};
