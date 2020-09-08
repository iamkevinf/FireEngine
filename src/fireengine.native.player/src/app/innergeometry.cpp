#include "exportapi.h"

#include "core/gameobject.h"

#include "io/memorystream.h"
#include "io/parseshader.h"

#include "graphics/material.h"
#include "graphics/meshrenderer.h"
#include "graphics/texture2d.h"

#include "loader/loader.h"

namespace FireEngine
{
	struct myvec
	{
		float x;
		float y;
		float z;
		uint32_t m_argb;
	};

	static myvec s_cubeVertices[] =
	{
		//{-1.0f,  1.0f,  1.0f, 0x01010101 },
		//{ 1.0f,  1.0f,  1.0f, 0xff0101ff },
		//{-1.0f, -1.0f,  1.0f, 0x0101ff01 },
		//{ 1.0f, -1.0f,  1.0f, 0xff01ffff },
		//{-1.0f,  1.0f, -1.0f, 0x01ff0101 },
		//{ 1.0f,  1.0f, -1.0f, 0xffff01ff },
		//{-1.0f, -1.0f, -1.0f, 0xffffff01 },
		//{ 1.0f, -1.0f, -1.0f, 0xffffffff },


		//{1.5f + -1.0f,  1.0f,  1.0f, 0xffffffff },
		//{1.5f +  1.0f,  1.0f,  1.0f, 0xffffffff },
		//{1.5f + -1.0f, -1.0f,  1.0f, 0xffffffff },
		//{1.5f +  1.0f, -1.0f,  1.0f, 0xffffffff },
		//{1.5f + -1.0f,  1.0f, -1.0f, 0xffffffff },
		//{1.5f +  1.0f,  1.0f, -1.0f, 0xffffffff },
		//{1.5f + -1.0f, -1.0f, -1.0f, 0xffffffff },
		//{1.5f +  1.0f, -1.0f, -1.0f, 0xffffffff },

		{ 1.0f,  1.0f, -1.0f, 0x01010101 },
		{ 1.0f, -1.0f, -1.0f, 0xff0101ff },
		{ 1.0f,  1.0f,  1.0f, 0x0101ff01 },
		{ 1.0f, -1.0f,  1.0f, 0xff01ffff },
		{-1.0f,  1.0f, -1.0f, 0x01ff0101 },
		{-1.0f, -1.0f, -1.0f, 0xffff01ff },
		{-1.0f,  1.0f,  1.0f, 0xffffff01 },
		{-1.0f, -1.0f,  1.0f, 0xffffffff },


		{2 +  1.0f,  1.0f, -1.0f, 0x01010101/2 },
		{2 +  1.0f, -1.0f, -1.0f, 0xff0101ff/2 },
		{2 +  1.0f,  1.0f,  1.0f, 0x0101ff01/2 },
		{2 +  1.0f, -1.0f,  1.0f, 0xff01ffff/2 },
		{2 + -1.0f,  1.0f, -1.0f, 0x01ff0101/2 },
		{2 + -1.0f, -1.0f, -1.0f, 0xffff01ff/2 },
		{2 + -1.0f,  1.0f,  1.0f, 0xffffff01/2 },
		{2 + -1.0f, -1.0f,  1.0f, 0xffffffff/2 },
	};


	static const uint16_t s_cubeTriList[] =
	{
		0, 1, 2, // 0
		1, 3, 2,
		4, 6, 5, // 2
		5, 6, 7,
		0, 2, 4, // 4
		4, 2, 6,
		1, 5, 3, // 6
		5, 7, 3,
		0, 4, 1, // 8
		4, 5, 1,
		2, 3, 6, // 10
		6, 3, 7,

		0 + 8, 1 + 8, 2 + 8, // 0
		1 + 8, 3 + 8, 2 + 8,
		4 + 8, 6 + 8, 5 + 8, // 2
		5 + 8, 6 + 8, 7 + 8,
		0 + 8, 2 + 8, 4 + 8, // 4
		4 + 8, 2 + 8, 6 + 8,
		1 + 8, 5 + 8, 3 + 8, // 6
		5 + 8, 7 + 8, 3 + 8,
		0 + 8, 4 + 8, 1 + 8, // 8
		4 + 8, 5 + 8, 1 + 8,
		2 + 8, 3 + 8, 6 + 8, // 10
		6 + 8, 3 + 8, 7 + 8,
	};

	EXPORT_API GameObject* InnerGeoCreateCube(Transform* parent, const char16_t* name)
	{
		ObjectPtr objPtr = ObjectManager::GetRef(parent->objectID);
		TransformPtr transform = std::static_pointer_cast<Transform>(objPtr);

		std::string _name = toUTF8(std::u16string(name));

		GameObjectPtr gameObject = GameObject::Create(transform, _name);
		auto meshRenderer = gameObject->AddComponent<MeshRenderer>();
		ObjectManager::Register(meshRenderer, ObjectType::Component);

		ByteBuffer buffer = ByteBuffer(92 * 16 + sizeof(s_cubeTriList) + sizeof(int) * 3 + sizeof(Mesh::SubMesh) * 2);
		MemoryStream ms = MemoryStream(buffer);
		int vc = sizeof(s_cubeVertices) / sizeof(myvec);
		ms.Write<int>(vc);
		for (auto iter : s_cubeVertices)
		{
			glm::vec3 pos(iter.x, iter.y, iter.z);
			ms.Write<glm::vec3>(pos);
			ms.Write<glm::vec2>({ 0,0 });
			ms.Write<uint32_t>(iter.m_argb);
			ms.Write<glm::vec2>({ 0,0 });
			ms.Write<glm::vec3>({ 0,0,0 });
			ms.Write<glm::vec4>({ 0,0,0,0 });
			ms.Write<glm::vec4>({ 0,0,0,0 });
			ms.Write<glm::vec4>({ 0,0,0,0 });
		}

		int ic = sizeof(s_cubeTriList) / sizeof(uint16_t);
		ms.Write<int>(ic);

		for (auto iter : s_cubeTriList)
			ms.Write<uint16_t>(iter);

		int sc = 2;
		ms.Write<int>(sc);
		ms.Write<Mesh::SubMesh>({  0, 36 });
		ms.Write<Mesh::SubMesh>({ 36, 36 });
		ms.Close();

		std::vector<std::shared_ptr<Material>> mats;

		//auto mesh = Mesh::LoadFromMem(buffer, false);
		auto mesh = Mesh::Create();
		//loadMesh("mesh/Human.fbx", mesh, mats, mats);
		//loadMesh("mesh/untitled.obj", mesh, mats);
		loadMesh("mesh/nanosuit/nanosuit.obj", mesh, mats);
		//loadMesh("mesh/cube/cube.obj", mesh, mats);

		meshRenderer->SetSharedMesh(mesh);
		meshRenderer->SetSharedMaterials(mats);

		return gameObject.get();
	}

}