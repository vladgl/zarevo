#include "../include/Mesh.h"
#include "../include/zrv_logger.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>

_ZRV_SOURCE

//HalfEdge
//=============================================================================

HalfEdge::HalfEdge(const HalfEdge& edge)
{
    this->orig = edge.orig;
    this->twin = edge.twin;
    this->next = edge.next;
    this->face = edge.face;
}

inline glm::vec3 HalfEdge::asVec3() const { return dest()->coords - orig->coords; }

HalfEdge::~HalfEdge() {}

//=============================================================================

//Vertex
//=============================================================================

Vertex::Vertex
(
	unsigned int id,
	glm::vec3 coords,
	glm::vec3 normal,
	HalfEdge* leaving_new
) :
	id(id), coords(coords), normal(normal), leaving(leaving) {}

Vertex::Vertex(const Vertex& vert) :
	coords(vert.coords), normal(vert.normal), leaving(vert.leaving) {}
    
/**
* \returns halfedge, pointing to given vertex
*/
HalfEdge* Vertex::getEdgeTo (const Vertex* v) const
{
   HalfEdge* result = nullptr;

   if (leaving)
   {
        if(leaving->twin->orig == v)
            return leaving;
        else
        {
            HalfEdge* next = leaving->twin->next;
            while (result == nullptr && next != leaving)
            {
                if (next->twin->orig == v)
                    return next;
                else
                    next = next->twin->next;
            }
        }
   } 

   return result;
}

Vertex::~Vertex() {}

//=============================================================================

//Face
//=============================================================================

Face::Face() :
    normal({0,0,0}), edge(nullptr)
{}

Face::Face(const Face& face) : normal(face.normal), edge(face.edge) {}

Face::~Face() {}

//=============================================================================

//Mesh
//=============================================================================

Mesh::Mesh (const std::string& label) :
    opacity(100),
    _array_object(label),
    _array_buffer(zrv::BufferObject::BufferType::Array, label),
    _texcoord_buffer(zrv::BufferObject::BufferType::Array, label + "tex_coord"),
    _index_buffer(zrv::BufferObject::BufferType::ElementArray, label),
    _texture(zrv::TextureObject::TextureType::Texture2D, label + "_texture"),
	_default_color(zrv::TextureObject::TextureType::Texture2D, label + "_color"),
    _isInited(false), _use_color_flag(false),
    _model_matrix{ 1.0f }
    {}

/**
* Generate HE mesh from .obj
* \returns this if success, nullptr elsewise
*/
Mesh* Mesh::loadFromFile(const std::string& path_to_obj)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path_to_obj,
        aiProcess_Triangulate);
    if (!scene)
    {
        ZRV_LOG << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    this->clear();
    
	// we have only one frame == only one (root) node and only one model in it's only child
	aiMesh* ai_mesh = scene->mMeshes[scene->mRootNode->mChildren[0]->mMeshes[0]];

    //mesh stuff
    if (scene->HasMeshes())
    {
        //1) getting verts
        for (unsigned int i = 0; i != ai_mesh->mNumVertices; ++i)
        {
            Vertex vert;
            vert.id = i;
            vert.coords =
				{ ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z };
            vert.normal =
				{ ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z };
            vert.leaving = nullptr;
            vertList.push_back(vert);
        }
        
        //2) getting faces and halfedges
        halfedgeList.resize(ai_mesh->mNumFaces * 3);
        faceList.resize(ai_mesh->mNumFaces);
        for (unsigned int i = 0; i != ai_mesh->mNumFaces; ++i)
        {
            /*
            Each face is a riangle (aiProcess_Triangulate is used)

            vertList[ai_mesh->mFaces[i].mIndices[0]]
            |\
            | \ e1
            |  \
         e3 |   >vertList[ai_mesh->mFaces[i].mIndices[1]]
            |  /
            | / e2
            |/
            vertList[ai_mesh->mFaces[i].mIndices[2]]

            */

            //setting origs for edges in this face
            halfedgeList[i * 3].orig = &vertList[ai_mesh->mFaces[i].mIndices[0]];
            halfedgeList[i * 3 + 1].orig = &vertList[ai_mesh->mFaces[i].mIndices[1]];
            halfedgeList[i * 3 + 2].orig = &vertList[ai_mesh->mFaces[i].mIndices[2]];

            //setting next for edges
            halfedgeList[i * 3].next = &halfedgeList[i * 3 + 1];
            halfedgeList[i * 3 + 1].next = &halfedgeList[i * 3 + 2];
            halfedgeList[i * 3 + 2].next = &halfedgeList[i * 3];

            //giving them face
            halfedgeList[i * 3].face = halfedgeList[i * 3 + 1].face = halfedgeList[i * 3 + 2].face = &faceList[i];

            //calculation normal for face as a normalized cross of two edges
            faceList[i].normal = glm::normalize(glm::cross(halfedgeList[i * 3].asVec3(), halfedgeList[i * 3 + 1].asVec3()));

            //giving twins to edges
			if (halfedgeList[i * 3].orig->leaving != nullptr &&
				halfedgeList[i * 3].orig->leaving->next->orig == halfedgeList[i * 3 + 1].orig)
				halfedgeList[i * 3].twin = halfedgeList[i * 3].orig->leaving;

			if (halfedgeList[i * 3 + 1].orig->leaving != nullptr &&
				halfedgeList[i * 3 + 1].orig->leaving->next->orig == halfedgeList[i * 3 + 2].orig)
				halfedgeList[i * 3 + 1].twin = halfedgeList[i * 3 + 1].orig->leaving;

			if (halfedgeList[i * 3 + 2].orig->leaving != nullptr &&
				halfedgeList[i * 3 + 2].orig->leaving->next->orig == halfedgeList[i * 3].orig)
				halfedgeList[i * 3 + 2].twin = halfedgeList[i * 3 + 2].orig->leaving;

            //updating leavings for origs
            halfedgeList[i * 3].orig->leaving = &halfedgeList[i * 3];
            halfedgeList[i * 3 + 1].orig->leaving = &halfedgeList[i * 3 + 1];
            halfedgeList[i * 3 + 2].orig->leaving = &halfedgeList[i * 3 + 2];

            //giving new face an edge
            faceList[i].edge = &halfedgeList[i * 3];

        }
    }

    //if (scene->HasTextures())
    {
		for (size_t i = 0; i < ai_mesh->mNumVertices; ++i)
		{
			_texCoord.push_back(ai_mesh->mTextureCoords[0][i].x);
			_texCoord.push_back(ai_mesh->mTextureCoords[0][i].y);
		}
        _use_color_flag = true;
    }

    this->init("D:\\Prog\\nrtl-master\\obj\\2\\105227.png");
    return this;
}

/**
 * init OGL stuff
 */
void Mesh::init(const std::string& tex_path)
{
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
    _array_buffer.allocate(getVertexCoords());
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertList.size(), &vertList[0], GL_STATIC_DRAW);
    /*
    for (unsigned int i = 0; i != vertList.size(); ++i)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 3 * i * sizeof(float), sizeof(float), &vertList[i].coords.x);
        glBufferSubData(GL_ARRAY_BUFFER, (3 * i + 1) * sizeof(float), sizeof(float), &vertList[i].coords.y);
        glBufferSubData(GL_ARRAY_BUFFER, (3 * i + 2) * sizeof(float), sizeof(float), &vertList[i].coords.z);
    }
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glEnableVertexAttribArray(0);
    _array_buffer.release();

    _texcoord_buffer.bind();
    _texcoord_buffer.allocate(_texCoord);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    _texcoord_buffer.release();

    _index_buffer.bind();
    std::vector<unsigned int> inds = getIndices();
    _index_buffer.allocate(inds);

    _array_object.release();

    _isInited = true;
    _use_color_flag = true;
}

std::vector<float> Mesh::getVertexCoords()
{
    std::vector<float> res;
    for (auto& i : vertList)
    {
        res.push_back(i.coords.x);
        res.push_back(i.coords.y);
        res.push_back(i.coords.z);
    }
    return res;
}

/**
* Get vector of indices for EBO
* triangulates mesh if not triangulated
* \returns vector<unsigned int> of indices
* \todo test faces with holes
*/
std::vector<unsigned int> Mesh::getIndices()
{
    std::vector<unsigned int> result;

    //for each face
    for (size_t i = 0; i != faceList.size(); ++i)
    {
        HalfEdge* tempHE = faceList[i].edge;
        do
        {
            result.push_back(tempHE->orig->id);
            tempHE = tempHE->next;
        }
        while (tempHE != faceList[i].edge);
    }

    return result;
}

/**
 * draw mesh if isVisible==true and _isInited==true
 */
void Mesh::drawMesh(const ShaderProgram& program)
{
    if (!isVisible() or !_isInited) return;
    if (opacity < 100)
    {
        /// TODO
    }
    else
    {
        program.use();
        _array_object.bind();

        if (_use_color_flag)
        {
            _default_color.bind(0);
            glDrawElements(GL_TRIANGLES, faceList.size() * 3, GL_UNSIGNED_INT, nullptr);
            _default_color.release();
        }
        else
        {
            _texture.bind(0);
            glDrawElements(GL_TRIANGLES, faceList.size() * 3, GL_UNSIGNED_INT, nullptr);
            _texture.release();
        }

        _array_object.release();
        program.release();
    }
}

/**
* Make this object as after default construction
*/
void Mesh::clear ()
{
    vertList.clear();
    faceList.clear();
    halfedgeList.clear();
}

/**
* \returns true if this->_numVertices != 0
*/
bool Mesh::isEmpty () const
{
    return vertList.empty();
}

//=============================================================================
