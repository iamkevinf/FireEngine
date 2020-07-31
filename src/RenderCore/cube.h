#pragma once

#include "bx/math.h"
#include "mesh.h"

#include <map>

class Cube : RC::Mesh
{
private:
	static const int faces_per_cube_count = 6;
	static const int vertices_per_face_count = 4;
	static const int vertices_per_cube_count = faces_per_cube_count * vertices_per_face_count;
	static const int indices_per_face_count = 6;
	static const int indices_per_cube_count = indices_per_face_count * faces_per_cube_count;
	
	static const bx::Vec3 constexpr pos_vertices[vertices_per_cube_count] = {
		// CCW culling, indices: 0, 1, 3, 1, 2, 3, means: 'u' shapes starting on the right

		// front
		{  1.0f,  1.0f, -1.0f,},
		{  1.0f, -1.0f, -1.0f,},
		{ -1.0f, -1.0f, -1.0f,},
		{ -1.0f,  1.0f, -1.0f,},

		// right
		{  1.0f,  1.0f,  1.0f,},
		{  1.0f, -1.0f,  1.0f,},
		{  1.0f, -1.0f, -1.0f,},
		{  1.0f,  1.0f, -1.0f,},

		// back
		{ -1.0f,  1.0f,  1.0f,},
		{ -1.0f, -1.0f,  1.0f,},
		{  1.0f, -1.0f,  1.0f,},
		{  1.0f,  1.0f,  1.0f,},

		// left
		{ -1.0f,  1.0f, -1.0f,},
		{ -1.0f, -1.0f, -1.0f,},
		{ -1.0f, -1.0f,  1.0f,},
		{ -1.0f,  1.0f,  1.0f,},

		//top
		{  1.0f,  1.0f,  1.0f,},
		{  1.0f,  1.0f, -1.0f,},
		{ -1.0f,  1.0f, -1.0f,},
		{ -1.0f,  1.0f,  1.0f,},

		//bottom
		{ -1.0f, -1.0f,  1.0f,},
		{ -1.0f, -1.0f, -1.0f,},
		{  1.0f, -1.0f, -1.0f,},
		{  1.0f, -1.0f,  1.0f,},
	};


public:
	static long Create();
	static void Destroy(long handle);
	static void DrawCube(bgfx::ViewId, long handle);
	void Draw(bgfx::ViewId viewId);

	int vertices_count;
	int indices_count;

	bgfx::DynamicVertexBufferHandle m_vbh;
	bgfx::DynamicIndexBufferHandle m_ibh;
	bgfx::ProgramHandle m_program;

private:
	Cube();
	virtual ~Cube();

private:
	float mtx[16];
	static std::map<long, Cube*> s_cubePool;
	static long s_cube_counter;


};

