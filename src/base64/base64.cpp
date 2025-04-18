#include "base64.hpp"

geode::Result<std::size_t> findBase64Index(char c)
{
	if (c == '=') return geode::Ok(0);

	std::size_t index = ng::base64::impl::BASE64_CHARS.find(c);
	if (index == std::string::npos)
		return geode::Err("Invalid Base64 character '{}'", c);

	return geode::Ok(index);
}

geode::Result<std::string> ng::base64::base64URLEncode(const std::string_view input)
{
	std::string encoded;
	encoded.reserve(((input.size() + 2) / 3) * 4);

	int val = 0;
	int valb = -6;

	for (unsigned char c : input)
	{
		val = (val << 8) + c;
		valb += 8;

		while (valb >= 0)
		{
			encoded.push_back(impl::BASE64_CHARS[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}

	if (valb > -6)
		encoded.push_back(impl::BASE64_CHARS[((val << 8) >> (valb + 8)) & 0x3F]);

	while (encoded.size() % 4 != 0)
		encoded.push_back('=');

	return geode::Ok(encoded);
}

geode::Result<std::string> ng::base64::base64URLDecode(const std::string_view input)
{
	std::string decoded;
	decoded.reserve((input.size() / 4) * 3);

	int val = 0;
	int valb = -8;

	for (unsigned char c : input)
	{
		if (c == '=') break;

		auto idx = findBase64Index(c);
		if (idx.isErr())
			return geode::Err(idx.unwrapErr());

		val = (val << 6) + idx.unwrap();
		valb += 6;

		if (valb >= 0)
		{
			decoded.push_back((val >> valb) & 0xFF);
			valb -= 8;
		}
	}

	return geode::Ok(decoded);
}

bool ng::base64::isBase64URLLike(const std::string_view input)
{
	if (input.size() % 4 != 0)
		return false;

	for (char c : input)
		if (impl::BASE64_CHARS.find(c) == std::string::npos)
			return false;

	std::size_t padPos = input.find('=');
	if (padPos != std::string::npos)
	{
		if (input.substr(padPos).find_first_not_of('=') != std::string::npos)
			return false;

		if (input.size() - padPos > 2)
			return false;
	}

	return true;
}
