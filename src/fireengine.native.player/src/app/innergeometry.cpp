#include "exportapi.h"

#include "core/gameobject.h"

#include "io/memorystream.h"

#include "graphics/material.h"
#include "graphics/meshrenderer.h"

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
		{-1.0f,  1.0f,  1.0f, 0x01010101 },
		{ 1.0f,  1.0f,  1.0f, 0xff0101ff },
		{-1.0f, -1.0f,  1.0f, 0x0101ff01 },
		{ 1.0f, -1.0f,  1.0f, 0xff01ffff },
		{-1.0f,  1.0f, -1.0f, 0x01ff0101 },
		{ 1.0f,  1.0f, -1.0f, 0xffff01ff },
		{-1.0f, -1.0f, -1.0f, 0xffffff01 },
		{ 1.0f, -1.0f, -1.0f, 0xffffffff },
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
	};

	EXPORT_API GameObject* InnerGeoCreateCube(Transform* parent, const char16_t* name)
	{
		ObjectPtr objPtr = ObjectManager::GetRef(parent->objectID);
		TransformPtr transform = std::static_pointer_cast<Transform>(objPtr);

		std::string _name = toUTF8(std::u16string(name));

		GameObjectPtr gameObject = GameObject::Create(transform, _name);
		auto meshRenderer = gameObject->AddComponent<MeshRenderer>();
		ObjectManager::Register(meshRenderer, ObjectType::Component);

		ByteBuffer buffer = ByteBuffer(92 * 8 + sizeof(s_cubeTriList) + sizeof(int) * 2);
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
		ms.Close();

		auto mesh = Mesh::LoadFromMem(buffer, false);

		meshRenderer->SetSharedMesh(mesh);

		std::vector<std::shared_ptr<Material>> mats(1);
		mats[0] = Material::Create("Default-Material");
		meshRenderer->SetSharedMaterials(mats);

		return gameObject.get();
	}

}