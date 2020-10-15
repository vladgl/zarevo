#pragma once
#include "zarevo_base.h"

_ZRV_BEGIN


struct SimpleMesh
{
	SimpleMesh(const std::string& label = "Mesh");
	void initMesh(std::vector<GLfloat>& vertices,
		std::vector<GLuint>& indices,
		std::vector<GLfloat>& texCoord,
		const std::string& tex_path);
	void drawMesh(const ShaderProgram& program);
	std::vector<GLfloat> _vertices;
	std::vector<GLfloat> _texCoord;
	std::vector<GLuint> _indices;

	zrv::ArrayObject _array_object;
	zrv::BufferObject _array_buffer, _texcoord_buffer, _index_buffer;
	zrv::TextureObject _texture;
};

_ZRV_END