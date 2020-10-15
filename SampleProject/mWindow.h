#pragma once
#include <zarevo.h>

class mWindow : public zrv::Window
{
public:
	mWindow(uint16_t w, uint16_t h, const char* title): 
		zrv::Window(w, h, title),
		m_prog("myShader"),
		mesh("myMesh")

	{
		initGl();
		sensivity = 0.005f;
		_scroll_coeff = 1.1f;
		glfwSetCursorPos(p_window, _ww / 2.0, _wh / 2.0);
		glfwGetCursorPos(p_window, &x_prev, &y_prev);
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
	zrv::ShaderProgram m_prog;
	zrv::SimpleMesh mesh;

	GLfloat cutOff;
	GLfloat outerCutOff;

	double sensivity, x_prev, y_prev, x_offset, y_offset;
	double _scroll_coeff;
	MouseMoveState mouse_move_state;
	GLfloat _prev_pos_x, _prev_pos_y;

	GLuint spec_tex, emis_tex;
	int t_w, t_h, e_t_w, e_t_h;
};