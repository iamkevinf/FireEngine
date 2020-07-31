#include "mesh.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace RC
{
	bgfx::VertexLayout AnimatedPosColorTexVertex::ms_layout;

	Mesh::Mesh()
	{
		AnimatedPosColorTexVertex::init();
	}

	Mesh::Mesh(const std::vector<bx::Vec3>& model_vertices,
		const std::vector<bx::Vec3>& model_colors,
		const std::vector<bx::Vec3>& model_normals,
		const std::vector<bx::Vec3>& model_uvs,
		const std::vector<int>& model_indices):Mesh()
	{
		for (int i = 0; i < model_vertices.size(); i++)
		{
			vertices[i].x = model_vertices[i].x;
			vertices[i].y = model_vertices[i].y;
			vertices[i].z = model_vertices[i].z;
			vertices[i].r = model_colors[i].x;
			vertices[i].g = model_colors[i].y;
			vertices[i].b = model_colors[i].z;
			vertices[i].normal_x = model_normals[i].x;
			vertices[i].normal_y = model_normals[i].y;
			vertices[i].normal_z = model_normals[i].z;
			vertices[i].texcoord_x1 = model_uvs[i].x;
			vertices[i].texcoord_y1 = model_uvs[i].y;
		}

		for (int i = 0; i < model_indices.size(); i++)
		{
			indices[i] = model_indices[i];
		}
	}

}

