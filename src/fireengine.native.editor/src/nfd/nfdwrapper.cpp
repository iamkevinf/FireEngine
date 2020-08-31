#include "exportapi.h"
#include "nfd.h"

#include <string>
#include <codecvt>

namespace FireEngine
{
		/* select folder dialog */
	EXPORT_API nfdresult_t feNFD_PickFolder(const char* defaultPath,
		char** outPath)
	{

		auto ret = NFD_PickFolder(defaultPath, outPath);

		return ret;
	}

}