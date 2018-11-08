
#include "stdafx.h"
#include "window.h"


window::window(const char* Title, int Width, int Height)
{
	width = Width;
	height = Height;
	title = Title;
	init();

}


window::~window()
{
	glfwTerminate();

}

bool window::init() {

	if (!glfwInit()) {
		std::cout << "Failed!" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!_window) {
		glfwTerminate();
		std::cout << "Failed" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1);
	return true;
}

void window::update() {

	glfwPollEvents();
	glfwGetFramebufferSize(_window, &width, &height);
	//Clear();
	glfwSwapBuffers(_window);
	//std::cout << GetWidth() <<  ", " << GetHeight() << std::endl;

}

bool window::Closed() const {

	return glfwWindowShouldClose(_window);


}

void window::Clear() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

int window::GetWidth() {

	return width;
}

int window::GetHeight() {

	return height;

}