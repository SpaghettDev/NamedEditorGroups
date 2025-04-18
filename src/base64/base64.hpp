#pragma once

#include <string>
#include <Geode/Result.hpp>

// cocos2d::ZipUtils::base64Decode sucks so much it errors out on a valid base64 string :sob:
namespace ng::base64
{
	namespace impl
	{
		constexpr std::string_view BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
	}

	geode::Result<std::string> base64URLEncode(const std::string_view input);
	geode::Result<std::string> base64URLDecode(const std::string_view input);

	bool isBase64URLLike(const std::string_view input);
}
