#include "string_tool.h"
#include <algorithm>
#include <cassert>
#include <cstdarg>

namespace FireEngine
{

	bool IsWhiteSpace(char ch)
	{
		return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\0';
	}

	void TrimString(std::string& str)
	{
		if (str.empty()) return;

		int length = int(str.size());

		int p1 = 0;
		for (; p1 < length; ++p1)
		{
			if (!IsWhiteSpace(str[p1])) break;
		}

		int p2 = length - 1;
		for (; p2 >= 0; --p2)
		{
			if (!IsWhiteSpace(str[p2])) break;
		}

		if (p1 > p2)
		{
			str.clear();
			return;
		}

		int n = p2 - p1 + 1;
		if (p1 != 0)
		{
			for (int i = 0; i < n; ++i)
			{
				str[i] = str[p1 + i];
			}
		}

		str.erase(n, str.npos);
	}

	void LowerString(std::string& str)
	{
		std::vector<char> tmp;

		for (auto c : str)
		{
			if (c >= 'A' && c <= 'Z')
			{
				c -= 'A' - 'a';
			}
			tmp.push_back(c);
		}
		tmp.push_back(0);

		str = (&tmp[0]);
	}

	void UpperString(std::string& str)
	{
		std::vector<char> tmp;

		for (auto c : str)
		{
			if (c >= 'A' && c <= 'Z')
			{
				c += 'A' - 'a';
			}
			tmp.push_back(c);
		}
		tmp.push_back(0);

		str = (&tmp[0]);
	}

	std::string RemoveFileExt(const std::string& name)
	{
		size_t pos = name.find_last_of('.');
		if (pos != name.npos)
		{
			return name.substr(0, pos);
		}

		return name;
	}

	std::string GetFileExt(const std::string& name)
	{
		size_t pos = name.find_last_of('.');
		if (pos != name.npos)
		{
			return name.substr(pos);
		}

		return "";
	}

	void FormatPathSlash(std::string& path)
	{
		std::replace(path.begin(), path.end(), '\\', '/');
	}

	void FormatFilePath(std::string& path)
	{
		if (path.empty()) return;

		FormatPathSlash(path);
		if (path.back() != '/') path += '/';
	}

	std::string GetFilePath(const std::string& filename)
	{
		if (filename.empty())
			return std::string();

		std::string name = filename;
		FormatPathSlash(name);
		if (name.back() == '/')
			name.pop_back();

		size_t pos = name.find_last_of('/');
		if (pos == std::string::npos)
			return std::string();

		return name.substr(0, pos + 1);
	}

	std::string GetFileName(const std::string& filename)
	{
		if (filename.empty())
			return std::string();

		std::string name = filename;
		FormatPathSlash(name);
		if (name.back() == '/')
			name.pop_back();

		size_t pos = name.find_last_of('/');
		if (pos == std::string::npos)
			return name;

		return name.substr(pos + 1);
	}

	bool StringFromFormat(std::string& out, const char* format, ...)
	{
		assert(format != nullptr);

		va_list pArgList;
		va_start(pArgList, format);

		bool ret = StringFromVAList(out, format, pArgList);

		va_end(pArgList);

		return ret;
	}

	bool StringFromVAList(std::string& out, const char* format, va_list va)
	{
		int len = 128;
		int ret = -1;

		while (ret < 0)
		{
			len <<= 1;

			out.resize(len);
			ret = vsnprintf(&out[0], len, format, va);

			//mac上，如果长度不够长，会返回需要的长度
			if (ret > len)
			{
				len = (ret >> 1) + 1;
				ret = -1;
			}

			assert(len < 65535 && "stringFromVAList");
		}

		out.erase(ret);
		return true;
	}

	bool U16StringFromVAList(std::u16string& out, const char16_t* format, va_list va)
	{
		int len = 128;
		int ret = -1;

		while (ret < 0)
		{
			len <<= 1;

			out.resize(len);
			ret = _vsnwprintf_s(reinterpret_cast<wchar_t* const>(&out[0]), (size_t)len, _TRUNCATE, reinterpret_cast<const wchar_t*>(format), va);

			//mac上，如果长度不够长，会返回需要的长度
			if (ret > len)
			{
				len = (ret >> 1) + 1;
				ret = -1;
			}

			assert(len < 65535 && "stringFromVAList");
		}

		out.erase(ret);
		return true;
	}

	bool StringBeginWith(const std::string& str, const std::string& prefix)
	{
		if (str.length() < prefix.length()) return false;

		for (size_t i = 0; i < prefix.length(); ++i)
		{
			if (str[i] != prefix[i]) return false;
		}

		return true;
	}

	bool StringEndWith(const std::string& str, const std::string& postfix)
	{
		if (str.length() < postfix.length()) return false;

		size_t offset = str.length() - postfix.length();
		for (size_t i = 0; i < postfix.length(); ++i)
		{
			if (str[i + offset] != postfix[i]) return false;
		}

		return true;
	}


	bool StringSplit(const std::string& res, const std::string& sep, StringVector& ret, bool exclude_empty)
	{
		size_t start = 0;
		while (true)
		{
			size_t index = res.find(sep, start);
			if (index >= 0)
			{
				std::string str = res.substr(start, index - start);
				if (!str.empty() || !exclude_empty)
					ret.push_back(str);
				start = index + sep.size();
			}
			else
			{
				break;
			}
		}

		std::string str = res.substr(start, -1);
		if (!str.empty() || !exclude_empty)
			ret.push_back(str);

		return ret.size() > 0;
	}

	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> _converter;

	std::u16string ToUtf16String(char* strAnsiOrUtf8)
	{
		return _converter.from_bytes(strAnsiOrUtf8);
	}

	std::string ToUtf8String(char16_t* strUtf16)
	{
		return _converter.to_bytes(strUtf16);
	}
	
	std::u16string ToUtf16String(std::string strAnsiOrUtf8)
	{
		return _converter.from_bytes(strAnsiOrUtf8);
	}
	
	std::string ToUtf8String(std::u16string strUtf16)
	{
		return _converter.to_bytes(strUtf16);
	}

	OutputNChar::OutputNChar(char ch, int nRepeat)
		: _ch(ch)
		, _nRepeat(nRepeat)
	{
	}

	std::ostream& operator<<(std::ostream& stream, const OutputNChar& output)
	{
		int n = output._nRepeat;
		while (n-- > 0) stream << output._ch;

		return stream;
	}


	TabProxy::TabProxy(int n)
		: _n(n)
	{}


	std::ostream& operator << (std::ostream& stream, const TabProxy& tab)
	{
		int n = tab._n;
		while (n-- > 0) stream << '\t';

		return stream;
	}
}