#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <bx/bx.h>
#include <bx/file.h>

#include "../FireEngine.h"

namespace FireEngine
{
	static bx::FileReaderI* s_fileReader = nullptr;

	bx::FileReaderI* getFileReader();
	bx::AllocatorI* getAllocator();
	bx::AllocatorI* getDefaultAllocator();

	static bx::AllocatorI* g_allocator = getDefaultAllocator();

	typedef bx::StringT<&g_allocator> String;
	static String s_currentDir;

	class FileReader : public bx::FileReader
	{
		typedef bx::FileReader super;

	public:
		virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
		{
			String filePath(s_currentDir);
			filePath.append(_filePath);
			return super::open(filePath.getPtr(), _err);
		}
	};

	class FileWriter : public bx::FileWriter
	{
		typedef bx::FileWriter super;

	public:
		virtual bool open(const bx::FilePath& _filePath, bool _append, bx::Error* _err) override
		{
			String filePath(s_currentDir);
			filePath.append(_filePath);
			return super::open(filePath.getPtr(), _append, _err);
		}
	};


	static void setCurrentDir(const char* _dir);

	class FileSystem
	{
	private:
		FileSystem();
		virtual ~FileSystem();
	public:

		void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size);
		void* load(const char* _filePath, uint32_t* _size = NULL);
		void unload(void* _ptr);

		static FileSystem* getInstance();

	private:
		static FileSystem s_instance;
	};
}; // end of namespace RC

#endif //__FILE_SYSTEM_H__
