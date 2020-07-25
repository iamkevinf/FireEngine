#include "FileSystem.h"

namespace RC
{
	FileSystem FileSystem::s_instance;

	bx::AllocatorI* getDefaultAllocator()
	{
		BX_PRAGMA_DIAGNOSTIC_PUSH();
		BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
		BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
		static bx::DefaultAllocator s_allocator;
		return &s_allocator;
		BX_PRAGMA_DIAGNOSTIC_POP();
	}

	bx::FileReaderI* getFileReader()
	{
		return s_fileReader;
	}

	bx::AllocatorI* getAllocator()
	{
		if (NULL == g_allocator)
		{
			g_allocator = getDefaultAllocator();
		}

		return g_allocator;
	}

	static void setCurrentDir(const char* _dir)
	{
		s_currentDir.set(_dir);
	}

	void* FileSystem::load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
	{
		if (bx::open(_reader, _filePath))
		{
			uint32_t size = (uint32_t)bx::getSize(_reader);
			void* data = BX_ALLOC(_allocator, size);
			bx::read(_reader, data, size);
			bx::close(_reader);
			if (NULL != _size)
			{
				*_size = size;
			}
			return data;
		}
		else
		{
			DBG("Failed to open: %s.", _filePath);
		}

		if (NULL != _size)
		{
			*_size = 0;
		}

		return NULL;
	}

	FileSystem::FileSystem()
	{
		if (!s_fileReader)
			s_fileReader = BX_NEW(g_allocator, FileReader);
	}

	FileSystem::~FileSystem()
	{
		if(s_fileReader)
		{
			BX_DELETE(g_allocator, s_fileReader);
			s_fileReader = NULL;
		}
	}

	void* FileSystem::load(const char* _filePath, uint32_t* _size)
	{
		return FileSystem::load(getFileReader(), getAllocator(), _filePath, _size);
	}

	void FileSystem::unload(void* _ptr)
	{
		BX_FREE(getAllocator(), _ptr);
	}

	FileSystem* FileSystem::getInstance()
	{
		return &s_instance;
	}

};// end of namespace RC
