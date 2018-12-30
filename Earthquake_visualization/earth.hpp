#ifndef EARTH_HPP
#define EARTH_HPP
#include "config.hpp"
#include "engine.hpp"
#include <vector>

using glm::vec2;
using glm::vec3;

class Earth {
public:
    void initialize(Engine *engine, int slices, int stacks, float spherical);
    float isSpherical();
    float clamp(float x, float a, float b);
    void setSpherical(float spherical);
    vec3 getPosition(float latitude, float longitude);
    vec3 getNormal(float latitude, float longitude);
    void draw(bool textured);
protected:
    int slices, stacks;
    int nVertices, nTriangles;
    float spherical;
    Engine *engine;
    Texture texture;
    VertexBuffer vertexBuffer, normalBuffer, texCoordBuffer;
    ElementBuffer indexBuffer;
    int nIndices;
};

inline void Earth::initialize(Engine *e, int sl, int st, float sp) {
    engine = e;
    slices = sl;
    stacks = st;
    spherical = sp;
    
    nVertices = (sl+1) * (st+1);
    nTriangles = 2 * sl * st;
    vertexBuffer = engine->allocateVertexBuffer(nVertices*sizeof(vec3));
    normalBuffer = engine->allocateVertexBuffer(nVertices*sizeof(vec3));
    texCoordBuffer = engine->allocateVertexBuffer(nVertices*sizeof(vec2));
    texture = engine->loadTexture(Config::textureFile);
}

inline float Earth::clamp(float x, float a, float b) {
    if (x <= a)
        return a;
    else if (b <= x)
        return b;
    else
        return x;
}

inline float Earth::isSpherical() {
    return spherical;
}

inline void Earth::setSpherical(float s) {
    spherical = clamp(s, 0, 1);
}

inline vec3 Earth::getPosition(float latitude, float longitude) {
    vec3 rectangularPosition(0,0,0), sphericalPosition(0,0,0);

    float x,y,z;
    x = cos(latitude) * sin(longitude);
    y = sin(latitude);
    z = cos(latitude) * cos(longitude);
    
    rectangularPosition = vec3 (longitude, latitude, 0);
    
    sphericalPosition = vec3 (x,y,z);
    
    if (spherical == 0)
        return rectangularPosition;
    else if (spherical == 1)
        return sphericalPosition;
    else {
        return vec3(longitude + spherical * (x - longitude), latitude + spherical * (y - latitude), spherical * z );
    }
}


inline vec3 Earth::getNormal(float latitude, float longitude) {
    vec3 rectangularNormal(0,0,0), sphericalNormal(0,0,0);

    float x,y,z;
    x = cos(latitude) * sin(longitude);
    y = sin(latitude);
    z = cos(latitude) * cos(longitude);
    
    rectangularNormal = vec3(0,0,1);
    
    sphericalNormal = vec3(x,y,z);
    if (spherical == 0)
        return rectangularNormal;
    else if (spherical == 1)
        return sphericalNormal;
    else {
        return vec3(longitude + spherical * (x - longitude), latitude + spherical * (y - latitude), spherical * z );
    }
}

inline void Earth::draw(bool textured) {
    std::vector<vec3> vertices, normals;
    std::vector<vec2> texCoords;
    std::vector<int> indices;
    
    for (float latitude = -M_PI/2; latitude < M_PI/2; latitude += M_PI/stacks) {
        for (float longitude = -M_PI; longitude <= M_PI; longitude += 2 * M_PI/slices) {
            vertices.push_back(getPosition(latitude, longitude));
            normals.push_back(getNormal(latitude, longitude));
            texCoords.push_back(vec2(0.5 + longitude/(2*M_PI), 0.5 - latitude/M_PI));
        }
    }
    
    
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j <= (slices-1); j++) {
            indices.push_back((slices+1)*i + j);
            indices.push_back((slices+1)*i + j + 1);
            indices.push_back((slices+1)*(i+1) + j );
            
            indices.push_back((slices+1)*i + j + 1);
            indices.push_back((slices+1)*(i+1) +j +1);
            indices.push_back((slices+1)*(i+1) +j );
        }
    }
    
    nIndices = (int)indices.size();
    indexBuffer = engine->allocateElementBuffer(nIndices*sizeof(int));
    engine->copyVertexData(texCoordBuffer, &texCoords[0], nVertices*sizeof(vec2));
    engine->copyElementData(indexBuffer, &indices[0], nIndices*sizeof(int));
    engine->copyVertexData(vertexBuffer, &vertices[0], nVertices*sizeof(vec3));
    engine->copyVertexData(normalBuffer, &normals[0], nVertices*sizeof(vec3));
    engine->setVertexArray(vertexBuffer);
    engine->setNormalArray(normalBuffer);

    if (textured) {
        engine->setTexCoordArray(texCoordBuffer);
        engine->setTexture(texture);
        engine->drawElements(GL_TRIANGLES, indexBuffer, nIndices);
    } else {
        engine->drawElements(GL_TRIANGLES, indexBuffer, nIndices);
    }
    engine->unsetTexture();
}

#endif
