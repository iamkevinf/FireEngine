#pragma once

//#include <glm.hpp>

#include <bx/math.h>

#include <string>
#include <vector>

#include <bgfx/bgfx.h>

namespace RC
{
    struct AnimatedPosColorTexVertex
    {
        float x;
        float y;
        float z;
        float r;
        float g;
        float b;
        float normal_x;
        float normal_y;
        float normal_z;

        float x2;
        float y2;
        float z2;
        float r2;
        float g2;
        float b2;
        float normal_x2;
        float normal_y2;
        float normal_z2;

        float pos_x1;
        float pos_y1;
        float pos_z1;

        float pos_x2;
        float pos_y2;
        float pos_z2;

        float model_from;
        float color_from;
        float pos_from;
        float model_to;
        float color_to;
        float pos_to;

        float texcoord_x1;
        float texcoord_y1;
        float texcoord_x2;
        float texcoord_y2;


        static void init()
        {
            ms_layout
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)

                .add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color1, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)

                .add(bgfx::Attrib::Color2, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color3, 3, bgfx::AttribType::Float)

                .add(bgfx::Attrib::Indices, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Weight, 3, bgfx::AttribType::Float)

                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord1, 2, bgfx::AttribType::Float)

                .end();
        };

        static bgfx::VertexLayout ms_layout;
    };
	class Mesh
	{
    public:
        Mesh();
        Mesh(
            const std::vector<bx::Vec3>& model_vertices,
            const std::vector<bx::Vec3>& model_colors,
            const std::vector<bx::Vec3>& model_normals,
            const std::vector<bx::Vec3>& model_uvs,
            const std::vector<int>& model_indices);

        AnimatedPosColorTexVertex* vertices;
        uint16_t* indices;

	};

}; // end of namespace RC