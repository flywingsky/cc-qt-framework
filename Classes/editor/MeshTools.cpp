//
//  MeshTools.cpp
//  demo
//
//  Created by C218-pc on 15/10/14.
//
//

#include "MeshTools.h"

#include <3d/CCMesh.h>

USING_NS_CC;

namespace mygame
{
    
    Sprite3D* createSquareModel(float halfWidth, float halfHeight, const std::string & texture)
    {
        std::vector<float> vertices(sizeof(V3F_T2F_Quad));
        
        /* create quad vertices.
         * 2 - 3
         * | + |
         * 0 - 1
         * NOTICE: the vertical texture coords is inverted in shader.
         */
        V3F_T2F_Quad *quad = (V3F_T2F_Quad *)vertices.data();
        quad->tl.vertices = Vec3(-halfWidth, 0.0f, -halfHeight);
        quad->tl.texCoords = Tex2F(0, 1);
        
        quad->tr.vertices = Vec3(halfWidth, 0.0f, -halfHeight);
        quad->tr.texCoords = Tex2F(1, 1);
        
        quad->bl.vertices = Vec3(-halfWidth, 0.0f, halfHeight);
        quad->bl.texCoords = Tex2F(0, 0);
        
        quad->br.vertices = Vec3(halfWidth, 0.0f, halfHeight);
        quad->br.texCoords = Tex2F(1, 0);
        
        // generate vertex indices.
        Mesh::IndexArray indices = {0, 1, 2, 3, 2, 1};
        
        // generate vertex attributes.
        std::vector<MeshVertexAttrib> attribs;
        MeshVertexAttrib attr;
        attr.size = 3;
        attr.type = GL_FLOAT;
        attr.vertexAttrib = GLProgram::VERTEX_ATTRIB_POSITION;
        attr.attribSizeBytes = sizeof(Vec3);
        attribs.push_back(attr);
        
        attr.size = 2;
        attr.type = GL_FLOAT;
        attr.vertexAttrib = GLProgram::VERTEX_ATTRIB_TEX_COORD;
        attr.attribSizeBytes = sizeof(Tex2F);
        attribs.push_back(attr);
        
        // create mesh.
        Mesh *mesh = Mesh::create(vertices, sizeof(V3F_T2F_Quad), indices, attribs);
        mesh->setTexture(texture);
        GLProgramState *glProgram = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_3D_POSITION_TEXTURE);
        mesh->setGLProgramState(glProgram);
        
        Sprite3D *model = Sprite3D::create();
        model->addMesh(mesh);
        
        return model;
    }
}
