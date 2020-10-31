#pragma once
#include "zarevo_base.h"
#include "BBox.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>

// Thanks to "The DCEL Data Structure for 3D Graphics" from Holmes3D.net
// http://www.holmes3d.net/graphics/dcel/

_ZRV_BEGIN

struct Vertex;
struct Face;
struct Mesh;

struct HalfEdge
{
    Vertex*   orig;
    HalfEdge* twin;
    HalfEdge* next;
    Face*     face;

    HalfEdge() :
        orig(nullptr), twin(nullptr), next(nullptr), face(nullptr)
    {}
    HalfEdge(const HalfEdge& edge);

    /// Leads to
    inline Vertex* dest() const { return next->orig; }

    inline glm::vec3 asVec3() const;

    ~HalfEdge();
};

struct Vertex
{
    glm::vec3 coords, normal;
    HalfEdge* leaving;

    // vertex id
    unsigned int id;

    Vertex
    (
        unsigned int id = 0,
        glm::vec3 coords = { 0,0,0 },
        glm::vec3 normal = { 0,0,0 },
        HalfEdge* leaving = nullptr
    );
    
    Vertex(const Vertex& vert);
    
    inline Vertex* next() const { return leaving->next->orig; }
    inline Vertex* prev() const { return leaving->twin->next->next->orig; }

    inline HalfEdge* entring() const { return leaving->twin->next->twin; }
    HalfEdge* getEdgeTo (const Vertex* v) const;

    ~Vertex();
};

struct Face
{
    glm::vec3 normal;
    HalfEdge* edge;

    Face();
    Face(const Face& face);

    ~Face();
};

/**
 * Mesh for HalfEdge data structure
 */
struct Mesh
{
    std::vector<Vertex>   vertList;
    std::vector<Face>     faceList;
    std::vector<HalfEdge> halfedgeList;

    /// scales from 0 (invisible) to 100 (fully visible) 
    uint8_t opacity;
    inline bool isVisible() const { return opacity != 0; }

    std::vector<GLfloat> _texCoord;
    glm::mat4 _model_matrix;
	AxisAlignedBB _bbox;

    /// TODO: add reserved numbers for objects for automatic name generation
    Mesh(const std::string& label = "Mesh");

    /**
     * Read .obj and create HE mesh object
     * Assumes to recieved data to ve triangulated (aiProcess_Triangulate is used)
     * \returns this
     */
    Mesh* loadFromFile (const std::string& path_to_obj);

    void init
    (
        std::vector<Vertex>&   verts,
        std::vector<Face>&     faces,
        std::vector<HalfEdge>& edges,
        std::vector<GLfloat>&  texCoords,
        const std::string&     tex_path
    );

    std::vector<unsigned int> getIndices();

    void useColor(bool use) { _use_color_flag = use; }

    void drawMesh(const ShaderProgram& program);

    void clear   ();
    bool isEmpty () const;

    ~Mesh(){}

private:
    bool _isInited, _use_color_flag;

    //OGL stuff
    zrv::ArrayObject   _array_object;
    zrv::BufferObject  _array_buffer, _texcoord_buffer, _index_buffer;
    zrv::TextureObject _texture, _default_color;
};

_ZRV_END