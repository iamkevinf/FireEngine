#include "parseshader.h"
#include "filesystem/FileSystem.h"

#include "utils/log.h"
#include "utils/string_tool.h"

#include "app/project.h"

#include <bx/process.h>

#include <iostream>
#include <sstream>
#include <map>

namespace FireEngine
{
	char TranslateChar(char ch)
	{
		switch (ch)
		{
		case 'n':return '\n';
		case 'r':return '\r';
		case 'f':return '\f';
		case 'b':return '\b';
		case 't':return '\t';
		case '0':return '\0';
		default:return ch;
		}
	}

	std::string BLANK_CHARS = "\t \r\n\b\f";
	std::string NUMBER_CHARS = "0123456789";
	std::string ALPHABET_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string IDENTITY_CHARS = ALPHABET_CHARS + NUMBER_CHARS + "_";

	enum class ShaderLexerToken
	{
		T_NULL,
		T_IDENTITY,
		T_STRING,
		T_NUMBER,
		// {
		T_BRACE_L,
		// }
		T_BRACE_R,
		// ,
		T_DOT,
		// ;
		T_SEMICOLON,
		// (
		T_BRACKET_L,
		// )
		T_BRACKET_R,
		// =
		T_EQUIL,
	};

	static std::string ShaderLexerTokenToString(ShaderLexerToken token)
	{
		switch (token)
		{
		case ShaderLexerToken::T_NULL:return "T_NULL";
		case ShaderLexerToken::T_IDENTITY:return "T_IDENTITY";
		case ShaderLexerToken::T_STRING:return "T_STRING";
		case ShaderLexerToken::T_NUMBER:return "T_NUMBER";
		case ShaderLexerToken::T_BRACE_L:return "T_BRACE_L";
		case ShaderLexerToken::T_BRACE_R:return "T_BRACE_R";
		case ShaderLexerToken::T_DOT:return "T_DOT";
		case ShaderLexerToken::T_SEMICOLON:return "T_SEMICOLON";
		case ShaderLexerToken::T_BRACKET_L:return "T_BRACKET_L";
		case ShaderLexerToken::T_BRACKET_R:return "T_BRACKET_R";
		case ShaderLexerToken::T_EQUIL:return "T_EQUIL";
		default:return "";
		}
	}

	struct ShaderLexerValue
	{
		std::string str = "";
		double num = 0;
	};

	struct ValueNode
	{
		ShaderLexerValue value;
		bool ret = false;

		ValueNode()
		{
			value.str = "";
			value.num = 0;

			ret = false;
		}

		ValueNode(const std::string& str)
		{
			value.str = str;
			ret = true;
		}

		ValueNode(double num)
		{
			value.num = num;
			ret = true;
		}
	};

	struct TokenInfo
	{
		ShaderLexerToken token;
		ValueNode value;
		std::string lastError;

		int line;
		int column;

		TokenInfo(const std::string& _lastError)
		{
			lastError = _lastError;
		}

		TokenInfo()
		{
			token = ShaderLexerToken::T_NULL;
			line = column = 0;
			value.ret = false;
		}

		TokenInfo(ShaderLexerToken _token, int _line, int _column)
		{
			token = _token; line = _line; column = _column;
			value.ret = true;
		}


		TokenInfo(ShaderLexerToken _token, const ValueNode& _value, int _line, int _column)
		{
			token = _token; line = _line; column = _column;
			value = _value;
		}

	};

	struct ShaderLexer
	{
	public:
		ShaderLexer()
		{
		}

		void SetContent(const std::string& content)
		{
			this->content = content;
		}

		TokenInfo NextToken()
		{
			return Next();
		}

		std::string GetLastError()const { return lastError; }

	private:
		void Error(const std::string& msg)
		{
			std::basic_ostringstream<char> msgStr;
			msgStr << msg << ". at line" << line << column;
			lastError = msgStr.str();
			FIREENGINE_LOG_ERROR(u"%s", ToUtf16String(lastError).c_str());
		}

		char getch()
		{
			if (index >= content.size())
				return 0;

			char ch = content[index];
			index++;
			column++;
			if (ch == '\n')
			{
				line++;
				column = 1;
			}

			return ch;
		}

		char LookAhead()
		{
			if (index >= content.size())
				return 0;

			return content[index];
		}

		std::string tmp_str;
		ValueNode ReadString(char endCh, char newLineCh)
		{
			int _line = line;
			int _column = column;
			tmp_str.clear();

			while (true)
			{
				auto ch = getch();
				if (ch == 0)
					break;
				else if (ch == endCh)
				{
					return ValueNode(tmp_str);
				}
				else if (ch == newLineCh)
					break;
				else if (ch == '\\')
				{
					ch = getch();
					ch = TranslateChar(ch);
				}

				tmp_str.push_back(ch);
			}

			line = _line;
			column = _column;
			Error("unclosed string");
			return ValueNode();
		}

		void ReadLineComment()
		{
			while (true)
			{
				char ch = getch();
				if (ch == 0 || ch == '\n')
					return;
			}
		}

		bool ReadLongComment()
		{
			int _line = line;
			int _column = column;
			int depth = 1;
			while (true)
			{
				char ch = getch();
				if (ch == 0)
					break;
				else if (ch == '*')
				{
					ch = getch();
					if (ch == '/')
					{
						depth--;
						if (depth == 0)
							return true;
					}
				}
				else if (ch == '/')
				{
					ch = getch();
					if (ch == '*')
						depth++;
				}
			}

			line = _line;
			column = _column;
			Error("unclosed comment");
			return false;
		}


		ValueNode ReadNumber(char ch)
		{
			std::string cache = "";
			cache.push_back(ch);
			bool isFloat = false;

			auto _ReadInt = [&]
			{
				while (ch != 0)
				{
					ch = LookAhead();
					if (NUMBER_CHARS.find(ch) == NUMBER_CHARS.npos)
						break;
					cache.push_back(getch());
				}
			};

			if (ch == '+' || ch == '-')
				cache.push_back(getch());

			if (ch == '0')
			{
				ch = LookAhead();
				if (ch == 'x' || ch == 'X' || ch == 'b' || ch == 'B')
					cache.push_back(getch());
				else if (ch == '.')
					isFloat = true;
			}

			_ReadInt();

			if (ch == '.')
			{
				isFloat = true;
				cache.push_back(getch());

				_ReadInt();
			}

			if (ch == 'e' || ch == 'E')
			{
				cache.push_back(getch());

				ch = LookAhead();
				if (ch == '+' || ch == '-')
					cache.push_back(getch());

				_ReadInt();
			}

			if (ch == 'l' || ch == 'L' || ch == 'u' || ch == 'U')
			{
				if (isFloat)
				{
					Error("invalid number");
					return ValueNode();
				}

				getch();
				cache.push_back(ch);
			}

			return ValueNode(std::stod(cache));
		}

		ValueNode ReadIdentity(char ch)
		{
			tmp_str.clear();
			tmp_str.push_back(ch);

			while (true)
			{
				ch = LookAhead();
				if (IDENTITY_CHARS.find(ch) == IDENTITY_CHARS.npos)
					break;

				tmp_str.push_back(getch());
			}

			return ValueNode(tmp_str);
		}

		TokenInfo Next()
		{
			while (true)
			{
				char ch = getch();
				if (ch == 0)
					return TokenInfo(ShaderLexerToken::T_NULL, line, column);

				if (BLANK_CHARS.find(ch) != BLANK_CHARS.npos)
					continue;

				if (ch == '{')
					return TokenInfo(ShaderLexerToken::T_BRACE_L, line, column);
				if (ch == '}')
					return TokenInfo(ShaderLexerToken::T_BRACE_R, line, column);
				if (ch == ',')
					return TokenInfo(ShaderLexerToken::T_DOT, line, column);
				if (ch == ';')
					return TokenInfo(ShaderLexerToken::T_SEMICOLON, line, column);
				if (ch == '(')
					return TokenInfo(ShaderLexerToken::T_BRACKET_L, line, column);
				if (ch == ')')
					return TokenInfo(ShaderLexerToken::T_BRACKET_R, line, column);
				if (ch == '=')
					return TokenInfo(ShaderLexerToken::T_EQUIL, line, column);

				if (ch == '"')
				{
					ValueNode node = ReadString('"', '\n');
					return TokenInfo(ShaderLexerToken::T_STRING, node, line, column);
				}
				if (ch == '\'')
				{
					ValueNode node = ReadString('\'', '\n');
					return TokenInfo(ShaderLexerToken::T_STRING, node, line, column);
				}
				if (ch == '`')
				{
					ValueNode node = ReadString('`', '\n');
					return TokenInfo(ShaderLexerToken::T_STRING, node, line, column);
				}

				if (ch == '/')
				{
					ch = getch();
					if (ch == '/')
					{
						ReadLineComment();
						continue;
					}
					else if (ch == '*')
					{
						ReadLineComment();
						continue;
					}
					else
					{
						Error("invalid symbo '/'");
						return TokenInfo(lastError);
					}
				}

				if (NUMBER_CHARS.find(ch) != NUMBER_CHARS.npos)
				{
					ValueNode node = ReadNumber(ch);
					return TokenInfo(ShaderLexerToken::T_NUMBER, node, line, column);
				}

				if (ch == '+' || ch == '-')
				{
					char nextCh = LookAhead();
					if (NUMBER_CHARS.find(ch) == NUMBER_CHARS.npos)
					{
						ValueNode node = ReadNumber(ch);
						return TokenInfo(ShaderLexerToken::T_NUMBER, node, line, column);
					}
				}

				if (ch == '_' || ALPHABET_CHARS.find(ch) != ALPHABET_CHARS.npos)
				{
					ValueNode node = ReadIdentity(ch);
					return TokenInfo(ShaderLexerToken::T_IDENTITY, node, line, column);
				}

				Error("invalid symbol " + ch);
				return TokenInfo(lastError);
			}
		}

	private:
		std::string content;

		int index = 0;
		int line = 1;
		int column = 1;
		std::string lastError;
	};



	struct ShaderParser
	{
	private:
		struct PropertyNode
		{
			std::string name;
			std::string type;
			std::string desc;
			std::vector<ValueNode> defaultValue;
		};

		struct PassNode
		{
			std::string type;
			std::vector<ValueNode> value;
		};

	public:
		bool Parse(const std::string& content)
		{
			lexer.SetContent(content);
			return ParseShader();
		}

	private:
		bool MatchToken(const std::string& desc, ShaderLexerToken token)
		{
			if (tokenInfo.token != token)
			{
				Error(desc, "token \"" + ShaderLexerTokenToString(token) + "\" expected, but \"" +
					ShaderLexerTokenToString(tokenInfo.token) + "\" was gived.");
				return false;
			}

			return true;
		}

		bool MatchNext(const std::string& desc, ShaderLexerToken token)
		{
			if (!Next().value.ret)
				return false;
			return MatchToken(desc, token);
		}

		std::vector<ValueNode> ParseValue(const std::string& desc)
		{
			std::vector<ValueNode> ret;
			if (tokenInfo.token == ShaderLexerToken::T_NUMBER
				|| tokenInfo.token == ShaderLexerToken::T_STRING
				|| tokenInfo.token == ShaderLexerToken::T_IDENTITY)
			{
				ret.push_back(tokenInfo.value);
				return ret;
			}

			if (tokenInfo.token == ShaderLexerToken::T_BRACKET_L)
			{
				Next();
				while (tokenInfo.token != ShaderLexerToken::T_BRACKET_R)
				{
					auto tmp = ParseValue(desc);
					ret.insert(ret.end(), tmp.begin(), tmp.end());

					Next();
					if (tokenInfo.token == ShaderLexerToken::T_BRACKET_R)
						break;

					MatchToken(desc, ShaderLexerToken::T_DOT);
					Next();
				}

				MatchToken(desc, ShaderLexerToken::T_BRACKET_R);
				return ret;
			}
			else
			{
				Error(desc, "value expected, but \"" + ShaderLexerTokenToString(tokenInfo.token) + "\" was given.");
				return ret;
			}
		}

		bool ParseProperties()
		{
			std::string desc = "Properties";
			if (!MatchNext(desc, ShaderLexerToken::T_BRACE_L))
				return false;

			if (!Next().value.ret)
				return false;

			properties.clear();

			while (tokenInfo.token != ShaderLexerToken::T_BRACE_R)
			{
				PropertyNode property;

				if (!MatchToken(desc, ShaderLexerToken::T_IDENTITY))
					return false;
				std::string _name = tokenInfo.value.value.str;
				property.name = _name;

				if (!MatchNext(desc, ShaderLexerToken::T_BRACKET_L))
					return false;
				if (!MatchNext(desc, ShaderLexerToken::T_STRING))
					return false;
				property.desc = tokenInfo.value.value.str;

				if (!MatchNext(desc, ShaderLexerToken::T_DOT))
					return false;
				if (!MatchNext(desc, ShaderLexerToken::T_IDENTITY))
					return false;
				property.type = tokenInfo.value.value.str;

				if (!MatchNext(desc, ShaderLexerToken::T_BRACKET_R))
					return false;
				if (!MatchNext(desc, ShaderLexerToken::T_EQUIL))
					return false;

				if (!Next().value.ret)
					return false;
				property.defaultValue = ParseValue(desc);
				if (!MatchNext(desc, ShaderLexerToken::T_SEMICOLON))
					return false;

				auto iter = properties.find(_name);
				if (iter != properties.end())
					properties[_name] = property;
				else
					properties.insert(std::make_pair(_name, property));

				if (!Next().value.ret)
					return false;
			}

			return MatchToken(desc, ShaderLexerToken::T_BRACE_R);
		}

		bool ParsePass()
		{
			std::string desc = "Pass";
			if (!MatchNext(desc, ShaderLexerToken::T_BRACE_L))
				return false;

			if (!Next().value.ret)
				return false;

			passes.clear();

			while (tokenInfo.token != ShaderLexerToken::T_BRACE_R)
			{
				PassNode pass;
				
				if (!MatchToken(desc, ShaderLexerToken::T_IDENTITY))
					return false;
				std::string _name = tokenInfo.value.value.str;
				pass.type = _name;
				if (!MatchNext(desc, ShaderLexerToken::T_EQUIL))
					return false;

				if (!Next().value.ret)
					return false;
				std::vector<ValueNode> values = ParseValue(desc);
				pass.value = values;
				if (!MatchNext(desc, ShaderLexerToken::T_SEMICOLON))
					return false;

				auto iter = passes.find(_name);
				if (iter != passes.end())
					passes[_name] = pass;
				else
					passes.insert(std::make_pair(_name, pass));

				if (!Next().value.ret)
					return false;
			}

			if (!MatchToken(desc, ShaderLexerToken::T_BRACE_R))
				return false;

			return true;
		}

		bool ParseFallback()
		{
			std::string desc = "Fallback";
			if (!MatchNext(desc, ShaderLexerToken::T_STRING))
				return false;
			fallback = tokenInfo.value.value.str;
			return MatchToken(desc, ShaderLexerToken::T_SEMICOLON);
		}

		bool ParseShader()
		{
			std::string desc = "Shader";
			if (!Next().value.ret)
				return false;

			if (tokenInfo.token != ShaderLexerToken::T_IDENTITY ||
				tokenInfo.value.value.str != "Shader")
			{
				Error(desc, "keyword \"Shader\" expected but "
					+ ShaderLexerTokenToString(tokenInfo.token) + " was gived");
				return false;
			}

			if (!Next().value.ret)
				return false;
			if (tokenInfo.token == ShaderLexerToken::T_STRING)
			{
				name = tokenInfo.value.value.str;
				if (!Next().value.ret)
					return false;
			}

			if (!MatchToken(desc, ShaderLexerToken::T_BRACE_L))
				return false;

			if (!Next().value.ret)
				return false;

			while (tokenInfo.token != ShaderLexerToken::T_BRACE_R)
			{
				if (tokenInfo.token == ShaderLexerToken::T_NULL)
					break;
				if (tokenInfo.token == ShaderLexerToken::T_SEMICOLON)
					continue;
				if (tokenInfo.token == ShaderLexerToken::T_IDENTITY)
				{
					if (tokenInfo.value.value.str == "Properties")
					{
						if (!ParseProperties())
							return false;
					}
					else if (tokenInfo.value.value.str == "Pass")
					{
						if (!ParsePass())
							return false;
					}
					else if (tokenInfo.value.value.str == "Fallback")
					{
						if (!ParseFallback())
							return false;
					}
				}
				else
				{
					Error(desc, "Invalid token " + ShaderLexerTokenToString(tokenInfo.token));
					return false;
				}

				if (!Next().value.ret)
					return false;
			}

			if (!MatchToken(desc, ShaderLexerToken::T_BRACE_R))
				return false;

			return true;
		}

		TokenInfo Next()
		{
			tokenInfo = lexer.NextToken();
			if (!tokenInfo.value.ret)
				lastError = tokenInfo.lastError;
			return tokenInfo;
		}

		void Error(const std::string& desc, const std::string& msg)
		{
			std::basic_ostringstream<char> msgStr;
			msgStr << desc << ": " << msg << ". at line" << tokenInfo.line << tokenInfo.column;
			lastError = msgStr.str();
			FIREENGINE_LOG_ERROR(u"%s", ToUtf16String(lastError).c_str());
		}


	private:
		ShaderLexer lexer;

		TokenInfo tokenInfo;

	public:
		std::string name;
		std::map<std::string, PropertyNode> properties;
		std::map<std::string, PassNode> passes;
		std::string fallback;

		std::string lastError;
	};

	static ShaderParser parser = ShaderParser();
	static bool parseRet = false;

	bool _CompileShader(std::string args)
	{
		std::string exe = "shadercRelease";
		bx::Error error;
		bx::ProcessReader processReader;
		if (!processReader.open(exe.c_str(), args.c_str(), &error) )
		{
			FIREENGINE_LOG_ERROR(u"Unable to load shader file");
			return false;
		}

		char buffer[2048];
		bx::memSet(buffer, 0, 2048);
		int32_t numCharactersWritten = processReader.read(buffer, 2048, &error);
		if (!error.isOk())
		{
			if (error.get().code != bx::kErrorReaderWriterEof.code)
			{
				std::basic_ostringstream<char16_t> msg;
				msg << u"Unable to load shader file:" << error.getMessage().getPtr();
				std::u16string strMsg = msg.str();
				FIREENGINE_LOG_ERROR(strMsg.c_str());
				return false;
			}
		}


		if (buffer[0] != '\0')
		{
			std::basic_ostringstream<char16_t> msg;
			msg << u"" << buffer;
			std::u16string strMsg = msg.str();

			FIREENGINE_LOG_ERROR(u"shader compile error %s", strMsg.c_str());
			return false;
		}

		int32_t result = processReader.getExitCode();
		processReader.close();

		if (0 != result)
		{

			std::basic_ostringstream<char16_t> msg;
			msg << u"Unable to compile shader file: :" << error.getMessage().getPtr();
			std::u16string strMsg = msg.str();
			FIREENGINE_LOG_ERROR(strMsg.c_str());
			return false;
		}

		return true;
	}

	bool ParseShader::CompileShader(const std::string& path, std::string& nameVS, std::string& nameFS)
	{
		uint32_t size = 0;
		auto data = FileSystem::getInstance()->load(
			getFileReader(), getAllocator(), path.c_str(), &size);

		auto content = std::string((const char*)data, size);

		parseRet = parser.Parse(content);

		if (!parseRet)
		{
			FIREENGINE_LOG_ERROR(u"Unable to load shader file");
			return false;
		}

		std::string prefix = "";

		size_t idx = path.find_last_of('/');
		if (idx != path.npos)
			prefix = path.substr(0, idx);

		std::string projectPath = Project::getInstance()->GetPath();
		std::string libararyPath = projectPath + "/Library";

		std::string shader_pathi = prefix;
		std::string shader_patho = libararyPath + "/shaderCache/shaders/dx11";
		std::string vs_profile = "vs_4_0";
		std::string plantform = "windows";

		FileSystem::getInstance()->createPath(shader_patho.c_str());

		auto iter = parser.passes.find("varying");
		if (iter == parser.passes.end())
		{
			FIREENGINE_LOG_ERROR(u"must have \"varying\" in pass");
			return false;
		}
		
		if (iter->second.value.empty())
		{
			FIREENGINE_LOG_ERROR(u" \"varying\" value can not be empty");
			return false;
		}

		std::string nameVarying = iter->second.value[0].value.str;

		iter = parser.passes.find("vertex");
		if (iter == parser.passes.end())
		{
			FIREENGINE_LOG_ERROR(u"must have \"veretex\" in pass");
			return false;
		}

		if (iter->second.value.empty())
		{
			FIREENGINE_LOG_ERROR(u" \"veretex\" value can not be empty");
			return false;
		}

		std::string _nameVS = iter->second.value[0].value.str;
		nameVS = RemoveFileExt(_nameVS);
		std::string argsVS = "-f " + shader_pathi + "/" + _nameVS + " -o " + shader_patho + "/" + nameVS + ".bin --varying.def " + shader_pathi + "/" + nameVarying + " --type vertex --platform " + plantform + " --profile " + vs_profile + " -O 3";
		if (!_CompileShader(argsVS))
			return false;

		iter = parser.passes.find("fragment");
		if (iter == parser.passes.end())
		{
			FIREENGINE_LOG_ERROR(u"must have \"fragment\" in pass");
			return false;
		}

		if (iter->second.value.empty())
		{
			FIREENGINE_LOG_ERROR(u" \"fragment\" value can not be empty");
			return false;
		}

		std::string _nameFS = iter->second.value[0].value.str;
		nameFS = RemoveFileExt(_nameFS);
		std::string fs_profile = "ps_4_0";
		std::string argsFS = "-f " + shader_pathi + "/" + _nameFS + " -o " + shader_patho + "/" + nameFS + ".bin --varying.def " + shader_pathi + "/" + nameVarying + " --type fragment --platform " + plantform + " --profile " + fs_profile + " -O 3";
		return _CompileShader(argsFS);
	}
}