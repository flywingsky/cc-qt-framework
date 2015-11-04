//
//  MeshTools.cpp
//  demo
//
//  Created by C218-pc on 15/10/14.
//
//

#include "MeshTools.h"

#include <3d/CCMesh.h>
#include <base/CCDirector.h>
#include <renderer/CCTextureCache.h>
#include <renderer/CCMaterial.h>

USING_NS_CC;

namespace Editor
{
    
    Sprite3D* createSquareModel(float width, float height, const std::string & texture, float repeatStep)
    {
        float halfWidth = width * 0.5f;
        float halfHeight = height * 0.5f;

        float uvScaleX = 1.0f;
        float uvScaleY = 1.0f;
        if(repeatStep > 0.0f)
        {
            uvScaleX = halfWidth / repeatStep;
            uvScaleY = halfHeight / repeatStep;
        }
        
        /* create quad vertices.
         * 2 - 3
         * | + |
         * 0 - 1
         * NOTICE: the vertical texture coords is inverted in shader.
         */
        std::vector<float> vertices(sizeof(V3F_T2F_Quad) / sizeof(float));
        V3F_T2F_Quad *quad = (V3F_T2F_Quad *)vertices.data();
        quad->tl.vertices = Vec3(-halfWidth, 0.0f, -halfHeight);
        quad->tl.texCoords = Tex2F(0, uvScaleY);
        
        quad->tr.vertices = Vec3(halfWidth, 0.0f, -halfHeight);
        quad->tr.texCoords = Tex2F(uvScaleX, uvScaleY);
        
        quad->bl.vertices = Vec3(-halfWidth, 0.0f, halfHeight);
        quad->bl.texCoords = Tex2F(0, 0);
        
        quad->br.vertices = Vec3(halfWidth, 0.0f, halfHeight);
        quad->br.texCoords = Tex2F(uvScaleX, 0);
        
        // generate vertex indices. in counterclockwise.
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
        Mesh *mesh = Mesh::create(vertices, sizeof(V3F_T2F_Quad) / sizeof(float), indices, attribs);
        Texture2D *tex = Director::getInstance()->getTextureCache()->addImage(texture);
        if(tex)
        {
            if(repeatStep > 0.0f)
            {
                tex->generateMipmap();

                Texture2D::TexParams param;
                param.minFilter = GL_LINEAR_MIPMAP_LINEAR;
                param.magFilter = GL_LINEAR;
                param.wrapS = GL_REPEAT;
                param.wrapT = GL_REPEAT;

                tex->setTexParameters(param);
            }
            mesh->setTexture(tex);
        }
        GLProgramState *glProgram = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_3D_POSITION_TEXTURE);
        mesh->setGLProgramState(glProgram);
        
        Sprite3D *model = Sprite3D::create();
        model->addMesh(mesh);
        
        return model;
    }

    cocos2d::Sprite3D* createGridModel(int rows, int cols,
                                       float gridWidth, float gridHeight,
                                       const std::string &texture)
    {
        int rowVertices = rows + 1;
        int colVertices = cols + 1;

        float halfWidth = cols * gridWidth * 0.5f;
        float halfHeight = rows * gridHeight * 0.5f;

        typedef V3F_T2F VertexType;

        /* grid order
         * (0,1) | (1,1)
         * -     -     - x+
         * (0,0) | (0,1)
         *       z+
        */

        // generate vertices.
        std::vector<float> vertices(rowVertices * colVertices * (sizeof(VertexType) / sizeof(float)));
        VertexType *pv = (VertexType*)vertices.data();
        for(int r = 0; r < rowVertices; ++r)
        {
            for(int c = 0; c < colVertices; ++c)
            {
                pv->vertices.x = c * gridWidth - halfWidth;
                pv->vertices.y = 0.0f;
                pv->vertices.z = halfHeight - r * gridHeight;

                pv->texCoords.u = c;
                pv->texCoords.v = r;

                ++pv;
            }
        }

        // generate indices.
        Mesh::IndexArray indices(rows * cols * 2 * 3);
        Mesh::IndexArray::value_type *pi = indices.data();
        for(int r = 0; r < rows; ++r)
        {
            for(int c = 0; c < cols; ++c)
            {
                /* index order.
                 * p2 -> p3
                 * p0 -> p1
                */
                pi[0] = r * colVertices + c;
                pi[1] = pi[0] + 1;
                pi[2] = pi[0] + colVertices;

                pi[3] = pi[2] + 1;
                pi[4] = pi[2];
                pi[5] = pi[1];

                pi += 6;
            }
        }

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
        Mesh *mesh = Mesh::create(vertices, sizeof(VertexType) / sizeof(float), indices, attribs);
        if(!texture.empty())
        {
            Texture2D *tex = Director::getInstance()->getTextureCache()->addImage(texture);
            if(tex != nullptr)
            {
                tex->generateMipmap();

                Texture2D::TexParams param;
                param.minFilter = GL_LINEAR_MIPMAP_LINEAR;
                param.magFilter = GL_LINEAR;
                param.wrapS = GL_REPEAT;
                param.wrapT = GL_REPEAT;

                tex->setTexParameters(param);
                mesh->setTexture(tex);
            }
        }

        GLProgramState *glProgram = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_3D_POSITION_TEXTURE);
        mesh->setGLProgramState(glProgram);

        Sprite3D *model = Sprite3D::create();
        model->addMesh(mesh);
        return model;
    }

    cocos2d::Sprite3D* createCube(const cocos2d::Vec3 &radius, const cocos2d::Color4B &color)
    {
        /*
         *   6 - 7
         *  /|  /|
         * 2 - 3 |
         * | 4-|-5
         * |/  |/
         * 0 - 1
        */

        // generate vertices.
        std::vector<float> vertices = {
            -radius.x, -radius.y, radius.z,
            radius.x, -radius.y, radius.z,
            -radius.x, radius.y, radius.z,
            radius.x, radius.y, radius.z,

            -radius.x, -radius.y, -radius.z,
            radius.x, -radius.y, -radius.z,
            -radius.x, radius.y, -radius.z,
            radius.x, radius.y, -radius.z,
        };

        // generate indices.
        Mesh::IndexArray indices = {
            0, 1, 2, 3, 2, 1, //front
            4, 6, 5, 7, 5, 6, //back
            2, 3, 6, 7, 6, 3, //top
            0, 4, 1, 5, 1, 4, //bottom
            1, 5, 3, 7, 3, 5, //right
            0, 2, 4, 6, 4, 2, //left
        };

        // generate vertex attributes.
        std::vector<MeshVertexAttrib> attribs;
        MeshVertexAttrib attr;
        attr.size = 3;
        attr.type = GL_FLOAT;
        attr.vertexAttrib = GLProgram::VERTEX_ATTRIB_POSITION;
        attr.attribSizeBytes = sizeof(Vec3);
        attribs.push_back(attr);

        // create mesh.
        Mesh *mesh = Mesh::create(vertices, sizeof(Vec3) / sizeof(float), indices, attribs);

        GLProgramState *glProgram = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_U_COLOR);
        mesh->setGLProgramState(glProgram);

        Sprite3D *model = Sprite3D::create();
        model->addMesh(mesh);
        model->setColor(Color3B(color));
        model->setOpacity(color.a);
        return model;
    }
}
