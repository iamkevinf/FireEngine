#include "cube.h"
#include "loader.h"

std::map<long, Cube*> Cube::s_cubePool;
long Cube::s_cube_counter = 0;

Cube::Cube()
{
	vertices_count = vertices_per_cube_count;
	indices_count = vertices_per_cube_count;

	vertices = new RC::AnimatedPosColorTexVertex[vertices_count];
	indices = new uint16_t[indices_count];

    bx::Vec3 pos = {0, 0, 0};
    bx::Vec3 col = {1, 1, 1};

    for (int i = 0; i < vertices_per_cube_count; ++i) {
        bx::Vec3 end_pos = bx::add(pos_vertices[i], pos);

        vertices[i].x = end_pos.x;
        vertices[i].y = end_pos.y;
        vertices[i].z = end_pos.z;
        vertices[i].r = col.x;
        vertices[i].g = col.y;
        vertices[i].b = col.z;
    }

    for (int i = 0; i < vertices_per_cube_count; i += 4) {
        bx::Vec3 a = bx::Vec3(
            vertices[i + 0].x,
            vertices[i + 0].y,
            vertices[i + 0].z
        );
        bx::Vec3 b = bx::Vec3(
            vertices[i + 1].x,
            vertices[i + 1].y,
            vertices[i + 1].z
        );
        bx::Vec3 c = bx::Vec3(
            vertices[i + 3].x,
            vertices[i + 3].y,
            vertices[i + 3].z
        );

        bx::Vec3 normal = bx::normalize(
            bx::cross(
                bx::sub(a, b),
                bx::sub(a, c)
            )
        );

        vertices[i + 0].normal_x =
            vertices[i + 1].normal_x =
            vertices[i + 2].normal_x =
            vertices[i + 3].normal_x =
            normal.x;

        vertices[i + 0].normal_y =
            vertices[i + 1].normal_y =
            vertices[i + 2].normal_y =
            vertices[i + 3].normal_y =
            normal.y;

        vertices[i + 0].normal_z =
            vertices[i + 1].normal_z =
            vertices[i + 2].normal_z =
            vertices[i + 3].normal_z =
            normal.z;
    }

    for (int i = 0; i < indices_count / indices_per_face_count; ++i)
    {
        indices[i * indices_per_face_count + 0] = i * vertices_per_face_count + 0;
        indices[i * indices_per_face_count + 1] = i * vertices_per_face_count + 1;
        indices[i * indices_per_face_count + 2] = i * vertices_per_face_count + 3;
        indices[i * indices_per_face_count + 3] = i * vertices_per_face_count + 1;
        indices[i * indices_per_face_count + 4] = i * vertices_per_face_count + 2;
        indices[i * indices_per_face_count + 5] = i * vertices_per_face_count + 3;
    }

    m_vbh = bgfx::createDynamicVertexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(vertices, vertices_count * sizeof(vertices[0])),
        RC::AnimatedPosColorTexVertex::ms_layout
    );

    m_ibh = bgfx::createDynamicIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(indices, indices_per_cube_count * sizeof(indices[0]))
        // bgfx::makeRef(indices, 6 * sizeof(indices, 6 * sizeof(indices[0])
    );

    bgfx::ShaderHandle vsh = RC::loadShader("vs_buildin");
    bgfx::ShaderHandle fsh = RC::loadShader("fs_buildin");

    m_program = bgfx::createProgram(vsh, fsh, false);
}

Cube::~Cube()
{
    if (vertices)
    {
        delete vertices;
        vertices = nullptr;
    }

    if (indices)
    {
        delete indices;
        indices = nullptr;
    }
}

long Cube::Create()
{
    Cube* cube = new Cube();
    s_cubePool.insert(std::pair<long, Cube*>(s_cube_counter, cube));
    s_cube_counter++;
    return s_cube_counter;
}

void Cube::Destroy(long handle)
{
    auto iter = s_cubePool.find(handle);
    if (iter != s_cubePool.end())
    {
        Cube* cube = iter->second;
        s_cubePool.erase(iter);

        delete cube;
        cube = nullptr;
    }
}

void Cube::DrawCube(bgfx::ViewId viewId, long handle)
{
    for (auto iter : s_cubePool)
    {
        Cube* cube = iter.second;
        if (cube)
            cube->Draw(viewId);
    }
}

void Cube::Draw(bgfx::ViewId viewId)
{
    uint64_t state = 0
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CCW
        | BGFX_STATE_MSAA;

    bgfx::setTransform(mtx);
    bgfx::setState(state);
    bgfx::setIndexBuffer(m_ibh);
    bgfx::setVertexBuffer(0, m_vbh);
    bgfx::submit(viewId, m_program);
}
