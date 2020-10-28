#pragma once
#include "zarevo_base.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// Thanks to "The DCEL Data Structure for 3D Graphics" from Holmes3D.net
// http://www.holmes3d.net/graphics/dcel/

_ZRV_BEGIN

class Vertex;
class Face;
class Mesh;

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

    /// Comes from
    inline Vertex* orig() const { return orig; }
    /// Leads to
    inline Vertex* dest() const { return twin->orig; }

    operator glm::vec3() const { return dest()->coords - orig->coords; }

    ~HalfEdge()
    {
        delete orig;
        delete twin;
        delete next;
        delete face;
    }
};

struct Vertex
{
    glm::vec3 coords, normal;
    HalfEdge* leaving;

    // vertex id
    unsigned int id;

    Vertex() : coords({ 0,0,0 }), normal({ 0,0,0 }), leaving(nullptr) {}
    Vertex(const Vertex& vert) :
        coords(vert.coords), normal(vert.normal),
        leaving(new HalfEdge(*vert.leaving))
    {}
    
    Vertex
    (
        unsigned int id = 0,
        glm::vec3 coords = {0,0,0},
        glm::vec3 normal = {0,0,0},
        HalfEdge* leaving = nullptr
    ) :
        id(id), coords(coords), normal(normal), leaving(leaving) {}
    
    inline Vertex* next() const { return leaving->next->orig; }
    inline Vertex* prev() const { return leaving->twin->next->next->orig; }

    inline HalfEdge* entring() const { return leaving->twin->next->twin; }
    HalfEdge* getEdgeTo (const Vertex* v) const;

    ~Vertex() { delete leaving; }
};

struct Face
{
    glm::vec3 normal;
    HalfEdge* edge;

    Face() : normal({ 0,0,0 }), edge(nullptr) {}
    Face(const Face& face);

    bool dropVert(const Vertex* v);

    ~Face() { delete edge; }
};

/**
 * Mesh for HalfEdge data structure
 */
struct Mesh
{
    Vertex*   vertList;
    Face*     faceList;
    HalfEdge* halfedgeList;

    /// scales from 0 (invisible) to 100 (fully visible) 
    uint8_t opacity;
    inline bool isVisible() const { return opacity != 0; }

    std::vector<GLfloat> _texCoord;

    /// TODO: add reserved numbers for objects for automatic name generation
    Mesh(const std::string& label = "Mesh");

    /**
     * Read .obj and create HE mesh object
     * Assumes to recieved data to ve triangulated (aiProcess_Triangulate is used)
     * \returns this
     */
    Mesh* readObj (const std::string& path_to_obj);

    void init
    (
        Vertex*   verts,
        Face*     faces,
        HalfEdge* edges,
        std::vector<GLfloat> texCoords,
        const std::string&   tex_path
    );

    bool triangulate();
    std::vector<unsigned int> getIndices();

    void draw(const ShaderProgram& program) const;

    bool center(glm::vec3* vec) const;

    void clear   ();
    bool isEmpty () const;
    inline bool isTriangulated() const { return isTriangulated; }

    void insert (const Vertex& v);
    void insert (const Face& f);
    void insert (const HalfEdge& e);

    ~Mesh()
    {
        delete vertList;
        delete faceList;
        delete halfedgeList;
    }

private:
    unsigned int _numVertices, _numFaces, _numHalfedges;
    bool _isTriangulated, _isInited;

    //OGL stuff
    zrv::ArrayObject   _array_object;
    zrv::BufferObject  _array_buffer, _texcoord_buffer, _index_buffer;
    zrv::TextureObject _texture;
};

_ZRV_END