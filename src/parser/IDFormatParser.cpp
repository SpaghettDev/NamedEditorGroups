#include "IDFormatParser.hpp"

using namespace ng::parser;

geode::Result<Tokens> ng::parser::parseFormat(const std::string_view format)
{
	Tokens tokens;
	std::size_t pos = 0;
	bool hasName = false;
	bool hasID = false;

	while (pos < format.size())
	{
		if (format[pos] == '{')
		{
			std::size_t end = format.find('}', pos);
			if (end == std::string::npos)
				return geode::Err("");

			std::string_view placeholder = format.substr(pos + 1, end - pos - 1);

			if (placeholder == "name")
			{
				tokens.emplace_back(Token{ TokenType::NAME, "" });
				hasName = true;
			}
			else if (placeholder == "id")
			{
				tokens.emplace_back(Token{ TokenType::ID, "" });
				hasID = true;
			}
			else
				return geode::Err("");

			pos = end + 1;
		}
		else
		{
			std::size_t start = pos;
			while (pos < format.size() && format[pos] != '{')
				pos++;

			auto literal = std::string{ format.substr(start, pos - start) };

			if (!literal.empty())
				tokens.emplace_back(Token{ TokenType::LITERAL, literal });
		}
	}

	if (!hasName || !hasID)
		return geode::Err("");

	return geode::Ok(tokens);
}

geode::Result<FormatPair> ng::parser::extract(const std::string& str, const Tokens& tokens)
{
	FormatPair res;
	std::size_t pos = 0;

	for (std::size_t i = 0; i < tokens.size(); i++)
	{
		const Token& token = tokens[i];

		if (token.type == TokenType::LITERAL)
		{
			const std::string& lit = token.value;
			if (str.substr(pos, lit.size()) != lit)
				return geode::Err("Literal {} not found in input", lit);

			pos += lit.size();
		}
		else
		{
			std::string nextLiteral;
			std::size_t nextLiteralPos = i + 1;
			while (nextLiteralPos < tokens.size())
			{
				if (tokens[nextLiteralPos].type == TokenType::LITERAL)
				{
					nextLiteral = tokens[nextLiteralPos].value;
					break;
				}
				nextLiteralPos++;
			}

			std::string value;
			if (nextLiteral.empty())
			{
				if (pos > str.size())
					return geode::Err("Input too short");

				value = str.substr(pos);
				pos = str.size();
			}
			else
			{
				std::size_t found = str.find(nextLiteral, pos);

				if (found == std::string::npos)
					return geode::Err("Next literal '{}' not found", nextLiteral);

				value = str.substr(pos, found - pos);
				pos = found;
			}

			if (token.type == TokenType::NAME)
				res.name = value;
			else
				res.id = value;
		}
	}

	if (pos != str.size())
		return geode::Err("Input not fully consumed");

	return geode::Ok(res);
}

geode::Result<std::string> ng::parser::format(const Tokens& tokens, const FormatPair& fmtPair)
{
	std::string output;
	for (const Token& token : tokens)
	{
		switch (token.type)
		{
			case TokenType::LITERAL:
				output += token.value;
			break;

			case TokenType::NAME:
				output += fmtPair.name;
			break;

			case TokenType::ID:
				output += fmtPair.id;
			break;
		}
	}

	return geode::Ok(output);
}

geode::Result<std::string> ng::parser::format(const std::string_view format, const FormatPair& fmtPair)
{
	auto tokensRes = parseFormat(format);
	if (tokensRes.isErr())
		return geode::Err(tokensRes.unwrapErr());

	return ::format(tokensRes.unwrap(), fmtPair);
}
