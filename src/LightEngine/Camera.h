#pragma once


class Camera
{

private:
	float m_x;
	float m_y;

	float m_width;
	float m_height;


public:

	Camera(float _x, float _y, float _w, float _h) : m_x(_x), m_y(_y), m_width(_w), m_height(_h)
	{}


	float getScreenX(float _reelX)  const { return _reelX - m_x; }
	float getScreenY(float _reelY) const { return _reelY - m_y; }

};
