#pragma once
#include "zarevo_base.h"
#include "GLFW/glfw3.h"

_ZRV_BEGIN

class EventHandler
{
public:
	EventHandler()
	{
	}
	virtual ~EventHandler()
	{
	}
	void initEventHandler(GLFWwindow* wnd)
	{
		glfwSetWindowUserPointer(wnd, (void*)(this));
		glfwSetKeyCallback
		(
			wnd,
			[](GLFWwindow* window, int key, int scancode, int action, int mode)
			{
				EventHandler* h;
				h = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
				h->keyEvent(key, scancode, action, mode);
			}
		);
		glfwSetWindowSizeCallback
		(
			wnd,
			[](GLFWwindow* window, int width, int height)
			{
				EventHandler* h;
				h = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
				h->resizeEvent(width, height);
			}
		);
		glfwSetCursorPosCallback
		(
			wnd,
			[](GLFWwindow* window, double xpos, double ypos)
			{
				EventHandler* h;
				h = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
				h->mouseMoveEvent(xpos, ypos);
			}
		);
		glfwSetMouseButtonCallback
		(
			wnd,
			[](GLFWwindow* window, int button, int action, int mods)
			{
				EventHandler* h;
				h = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
				h->mouseButtonEvent(button, action, mods);
			}
		);
		glfwSetScrollCallback
		(
			wnd,
			[](GLFWwindow* window, double xoffset, double yoffset)
			{
				EventHandler* h;
				h = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
				h->scrollEvent(xoffset, yoffset);
			}
		);
	}
	virtual void keyEvent(int key, int scancode, int action, int mode) { /*std::cout << __func__;*/ };
	virtual void resizeEvent(int width, int height) { /*std::cout << __func__; */};
	virtual void mouseMoveEvent(double xpos, double ypos) {/* std::cout << __func__;*/ };
	virtual void mouseButtonEvent(int button, int action, int mods) {/* std::cout << __func__; */};
	virtual void scrollEvent(double xoffset, double yoffset) { /*std::cout << __func__; */};

private:
};

class Window : public EventHandler
{
public:
	Window(uint16_t w, uint16_t h, const char* title);
	virtual ~Window() {};
	virtual int exec();
	virtual void paintGl() { };
protected:
	virtual void initGl() {};
//	virtual void drawModel(zrv::ShaderProgram* program, zrv::MeshModel* mm) {};
	GLFWwindow* p_window;
	GLfloat _frame_interval, _cur_time;
	int _ww, _wh, _fw, _fh;
private:
	Window() = delete;
	Window(Window&) = delete;
	Window(Window&&) = delete;
	void operator=(Window&) = delete;
	void operator=(Window&&) = delete;
};


_ZRV_END