#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class window
{
private:
	GLFWwindow * _window;
	int width, height;
	const char* title;
	bool closed;
public:
	window(const char* Title, int Width, int Height);
	~window();
public:
	bool init();
	void update();
	bool Closed() const;
	void Clear();
	int GetWidth();
	int GetHeight();

};
