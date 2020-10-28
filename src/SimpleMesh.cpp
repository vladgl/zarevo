#include "../include/SimpleMesh.h"
#include "../include/zrv_logger.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <set>
_ZRV_SOURCE

SimpleMesh::SimpleMesh(const std::string& label)	:
	_array_object(label),
	_array_buffer(zrv::BufferObject::BufferType::Array, label),
	_texcoord_buffer(zrv::BufferObject::BufferType::Array, label + "tex_coord"),
	_index_buffer(zrv::BufferObject::BufferType::ElementArray, label),
	_texture(zrv::TextureObject::TextureType::Texture2D, label + "_texture"),
	_default_color(zrv::TextureObject::TextureType::Texture2D, label + "_color"),
	_model_matrix{ 1.0f },
	_bbox{},
	_use_color_flag{ false }
{
}

void SimpleMesh::initMesh(std::vector<GLfloat>&& vertices,
	std::vector<GLuint>&& indices,
	std::vector<GLfloat>&& texCoord,
	const std::string& tex_path)
{
	_vertices = std::exchange(vertices, {});
	_indices = std::exchange(indices, {});
	_texCoord = std::exchange(texCoord, {});

	Image2D img{ tex_path };
	if (img.format() == Image2D::Format::UNSUPPORTED)
	{
		ZRV_LOG << "Image format is unsupported \n";
	}
	setTextureImg(img.flipY());
	setColor({ 0.5f, 0.5f, 0.5f, 1.0f });
	init_gl();

}

void SimpleMesh::drawMesh(const ShaderProgram& program)
{
	program.use();

	glUniformMatrix4fv(program.getUnifLoc("zrv_UModel"), 1, GL_FALSE, glm::value_ptr(_model_matrix));

	_array_object.bind();

	if (_use_color_flag)
	{
		_default_color.bind(0);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
		_default_color.release();
	}
	else
	{
		_texture.bind(0);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
		_texture.release();
	}

	_array_object.release();

	program.release();
}

void SimpleMesh::loadFromFile(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
	if (!scene)
	{
		ZRV_LOG << "Failed to load file " << path << std::endl;
		return;
	}
	aiMesh* mesh = scene->mMeshes[0];
	//vertex position
	for (size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		_vertices.push_back(mesh->mVertices[i].x);
		_vertices.push_back(mesh->mVertices[i].y);
		_vertices.push_back(mesh->mVertices[i].z);
	}
	//texture coordinates
	if (mesh->mTextureCoords)
	{
		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			_texCoord.push_back(mesh->mTextureCoords[0][i].x);
			_texCoord.push_back(mesh->mTextureCoords[0][i].y);
		}
	}
	//indices
	//
	std::set<GLuint> ind;
	for (size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		for (size_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
		{
			_indices.push_back(mesh->mFaces[i].mIndices[j]);
			ind.insert(mesh->mFaces[i].mIndices[j]);
		}
	}
	std::cout << std::endl;
	std::cout << (ind.size() == _vertices.size() / 3) << std::endl;

	std::cout << std::endl;

	//materials
	std::string tex_path = "";
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial*  mat = scene->mMaterials[mesh->mMaterialIndex];
		//diffuse
		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString str;
			mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
			tex_path = str.C_Str();
		}
		//specular
	}

	if (tex_path.size() != 0)
	{
		size_t pos = path.find_last_of("/\\");
		if (pos < path.length())
		{
			tex_path = path.substr(0, pos) + "\\" + tex_path;
		}
		Image2D img{ tex_path };
		if (img.format() == Image2D::Format::UNSUPPORTED)
		{
			ZRV_LOG << "Image format is unsupported \n";
		}
		setTextureImg(img.flipY());
		
	}

	setColor({ 0.5f, 0.5f, 0.5f, 1.0f });
	//normals
	////

	init_gl();
}

void SimpleMesh::setTextureImg(const Image2D& img)
{
	_texture.bind();

	_texture.setImage(img);

	_texture.setMinFilter(zrv::TextureObject::TextureFilter::Linear);
	_texture.setMagFilter(zrv::TextureObject::TextureFilter::Linear);
	_texture.setWrapping(zrv::TextureObject::Wrapping::Repeat);
	_texture.generateMipmap();

	_texture.release();
}


void SimpleMesh::setColor(const glm::vec4& color)
{
	Image2D clr{ 64, 64, color };
	_default_color.bind();
	_default_color.setImage(clr);
	_default_color.setMinFilter(zrv::TextureObject::TextureFilter::Linear);
	_default_color.setMagFilter(zrv::TextureObject::TextureFilter::Linear);
	_default_color.setWrapping(zrv::TextureObject::Wrapping::Repeat);
	_default_color.generateMipmap();
	_default_color.release();
}


void SimpleMesh::useColor(bool usage)
{
	_use_color_flag = usage;
}

void SimpleMesh::init_gl()
{
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
