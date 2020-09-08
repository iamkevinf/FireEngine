﻿#ifndef __STRING_TOOL_H__
#define __STRING_TOOL_H__

#include <string>
#include <vector>
#include <iostream>

namespace FireEngine
{
	typedef std::vector<std::string> StringVector;

	const std::string EmptyString;


	bool IsWhiteSpace(char ch);

	void TrimString(std::string& str);
	void LowerString(std::string& str);
	void UpperString(std::string& str);

	bool StringFromFormat(std::string& out, const char* format, ...);
	bool StringFromVAList(std::string& out, const char* format, va_list va);
	bool StringBeginWith(const std::string& str, const std::string& prefix);
	bool StringEndWith(const std::string& str, const std::string& postfix);
	bool StringSplit(const std::string& res, const std::string& sep, StringVector& ret, bool exclude_empty = false);
	
	bool U16StringFromVAList(std::u16string& out, const char16_t* format, va_list va);

	// 格式化斜线
	void FormatPathSlash(std::string& path);

	// 格式化'/'，传回值末尾含'/'
	void FormatFilePath(std::string& path);

	// 返回值末尾含'/'
	std::string GetFilePath(const std::string& filename);
	std::string GetFileName(const std::string& filename);

	// 删除'.'
	std::string RemoveFileExt(const std::string& name);

	// 返回值包含'.'
	std::string GetFileExt(const std::string& name);

	class OutputNChar
	{
		char    _ch;
		int     _nRepeat;

	public:
		OutputNChar(char ch, int nRepeat);

		friend std::ostream& operator<<(std::ostream& stream, const OutputNChar& tab);
	};


	// 输出n个制表符
	class TabProxy
	{
		int _n;

	public:

		explicit TabProxy(int n);

		friend std::ostream& operator<<(std::ostream& stream, const TabProxy& tab);
	};


}// end namespace FireEngine

#endif // __STRING_TOOL_H__
