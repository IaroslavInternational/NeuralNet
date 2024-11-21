#pragma once

class Object2D;

class UI
{
public:
	UI();
public:
	void Render(Object2D* obj);
private:
	void SetPanelSizeAndPosition(int corner, float width, float height, float x_offset, float y_offset);
};

