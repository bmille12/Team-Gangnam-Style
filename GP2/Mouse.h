#pragma once

class CMouse
{
public:
	CMouse();
	~CMouse();

	void setMouseButtonsDown(bool mouse1,bool mouse2,bool mouse3)
	{
		m_bMouseButtons[0]=mouse1;
		m_bMouseButtons[1]=mouse2;
		m_bMouseButtons[2]=mouse3;
	};

	void setMouseMove(float x,float y)
	{
		m_fMouseDeltaX=x;
		m_fMouseDeltaY=y;
		m_fMouseAbsX+=m_fMouseDeltaX;
		m_fMouseAbsY+=m_fMouseDeltaY;
	};

	bool getMouseDown(int index)
	{
		return m_bMouseButtons[index];
	};

	bool getMouseUp(int index)
	{
		return m_bMouseButtons[index];
	};

	float getRelativeMouseX()
	{
		return m_fMouseDeltaX;
	};

	float getRelativeMouseY()
	{
		return m_fMouseDeltaY;
	};

	float getAbsoluteMouseX()
	{
		return m_fMouseAbsX;
	};

	float getAbsoluteMouseY()
	{
		return m_fMouseAbsY;
	};
private:
	bool m_bMouseButtons[3];
	float m_fMouseDeltaX;
	float m_fMouseDeltaY;
	float m_fMouseAbsX;
	float m_fMouseAbsY;
	float m_fMouseWheel;
};