#pragma once
#ifdef _DEBUG && !defined(_ZRV_DEBUG)
#define ZRV_DEBUG
#endif

#ifndef PROJECTION_STATES
#define PROJECTION_STATES void initProjectionFunctions(); std::map<Projection, ResizeFunc> _projection_map;
#define PROJECTION_CALL(TYPE) _projection_map[TYPE]();
#define INIT_PROJECTION_FUNC this->initProjectionFunctions();
#define _INIT_PROJECTION_FUNC_BEGIN void mWindow::initProjectionFunctions() {
#define _INIT_PROJECTION_FUNC_END }
#define PROJECTION_MAP_BEGIN(TYPE) _projection_map[TYPE] = [this]() {
#define PROJECTION_MAP_END };
#endif

#include <zarevo.h>
#include <functional>
#include <map>

typedef std::function<void()> ResizeFunc;

struct MouseParam
{
	float sensivity;
	glm::dvec2 pos;
};

class mWindow : public zrv::Window
{
public:
	enum class Projection { Perspective, Ortho };
	mWindow(uint16_t w, uint16_t h, const char* title): 
		zrv::Window(w, h, title),
		m_prog("myShader"),
		mesh("myMesh"),
		key_pressed_time{ 0.4 }

	{
		_proj = Projection::Perspective;
		INIT_PROJECTION_FUNC
		initGl();
		mouse.sensivity = 8.0f;
		_scroll_coeff = 1.1f;
		glfwSetCursorPos(p_window, _ww / 2.0, _wh / 2.0);
		glfwGetCursorPos(p_window, &mouse.pos.x, &mouse.pos.y);
		_draw_backface_as_wireframe_flag = true;
		_roll_flag = false;

#ifdef _DEBUG
		sleep = 0.0;
#endif
	}



	void initGl() override;
	void paintGl() override;
	void keyEvent(int key, int scancode, int action, int mode) override;
	void resizeEvent(int width, int height) override;
	void mouseMoveEvent(double xpos, double ypos) override;
	void mouseButtonEvent(int button, int action, int mods) override;
	void scrollEvent(double xoffset, double yoffset)        override;



//	void drawModel(zrv::ShaderProgram* program, zrv::MeshModel* mm) override;

	enum MouseMoveState { DRAG, ROTATE };

private:
	PROJECTION_STATES
	zrv::ShaderProgram m_prog;
	zrv::SimpleMesh mesh;

	GLfloat cutOff;
	GLfloat outerCutOff;

	double _scroll_coeff;
	MouseMoveState mouse_move_state;
	GLfloat _prev_pos_x, _prev_pos_y;

	GLuint spec_tex, emis_tex;
	int t_w, t_h, e_t_w, e_t_h;

	Projection _proj;

	zrv::TargetCamera _cam;
	MouseParam mouse;
	bool _draw_backface_as_wireframe_flag;
	bool _roll_flag;
//	std::map<Projection, ResizeFunc> _projection_map;
#ifdef _DEBUG
	double sleep;
	
#endif
	double key_pressed_time[1024];
};
