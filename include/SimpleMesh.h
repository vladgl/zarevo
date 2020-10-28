#pragma once
#include "zarevo_base.h"
#include "BBox.h"
_ZRV_BEGIN

struct SimpleMesh
{
	SimpleMesh(const std::string& label = "Mesh");
	void initMesh(std::vector<GLfloat>&& vertices,
		std::vector<GLuint>&& indices,
		std::vector<GLfloat>&& texCoord,
		const std::string& tex_path);
	void drawMesh(const ShaderProgram& program);
	void loadFromFile(const std::string& path);
	void setTextureImg(const Image2D& img);
	void setColor(const glm::vec4& color);
	void useColor(bool usage);
	std::vector<GLfloat> _vertices;
	std::vector<GLfloat> _texCoord;
	std::vector<GLuint> _indices;

	zrv::ArrayObject _array_object;
	zrv::BufferObject _array_buffer, _texcoord_buffer, _index_buffer;
	zrv::TextureObject _texture;
	zrv::TextureObject _default_color;
	AxisAlignedBB _bbox;
	glm::mat4 _model_matrix;


	bool _use_color_flag;

private:
	void init_gl();

	SimpleMesh(const SimpleMesh&) = delete;
	void operator=(const SimpleMesh&) = delete;
};

_ZRV_END