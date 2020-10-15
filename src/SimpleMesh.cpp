#include "../include/SimpleMesh.h"
#include "../include/zrv_logger.h"
_ZRV_SOURCE

SimpleMesh::SimpleMesh(const std::string& label)	:
	_array_object(label),
	_array_buffer(zrv::BufferObject::BufferType::Array, label),
	_texcoord_buffer(zrv::BufferObject::BufferType::Array, label + "tex_coord"),
	_index_buffer(zrv::BufferObject::BufferType::ElementArray, label),
	_texture(zrv::TextureObject::TextureType::Texture2D, label)

{
}

void SimpleMesh::initMesh(std::vector<GLfloat>& vertices,
	std::vector<GLuint>& indices,
	std::vector<GLfloat>& texCoord,
	const std::string& tex_path)
{
	_vertices = std::move(vertices);
	_indices = std::move(indices);
	_texCoord = std::move(texCoord);

	Image2D img{ tex_path };
	if (img.format() == Image2D::Format::UNSUPPORTED)
	{
		ZRV_LOG << "Image format is unsupported \n";
	}
	_texture.bind();
	_texture.setImage(img.flipY());
	_texture.setMinFilter(zrv::TextureObject::TextureFilter::Linear);
	_texture.setMagFilter(zrv::TextureObject::TextureFilter::Linear);
	_texture.setWrapping(zrv::TextureObject::Wrapping::Repeat);
	_texture.generateMipmap();

	_texture.release();
	_array_object.bind();

	_array_buffer.bind();
	_array_buffer.allocate(_vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	_array_buffer.release();

	_texcoord_buffer.bind();
	_texcoord_buffer.allocate(_texCoord);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	_texcoord_buffer.release();

	_index_buffer.bind();
	_index_buffer.allocate(_indices);


	_array_object.release();
}

void SimpleMesh::drawMesh(const ShaderProgram& program)
{
	program.use();

	_array_object.bind();

	_texture.bind(0);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);

	_array_object.release();

	program.release();
}
