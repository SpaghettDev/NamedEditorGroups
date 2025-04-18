#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace ng::parser
{
	enum class TokenType
	{
		LITERAL,
		NAME,
		ID
	};

	struct Token
	{
		TokenType type;
		std::string value;
	};

	struct FormatPair
	{
		std::string name;
		std::string id;
	};

	using Tokens = std::vector<Token>;

	geode::Result<Tokens> parseFormat(const std::string_view format);
	geode::Result<FormatPair> extract(const std::string& str, const Tokens& tokens);
	geode::Result<std::string> format(const Tokens& tokens, const FormatPair& fmtPair);
	geode::Result<std::string> format(const std::string_view format, const FormatPair& fmtPair);
}
