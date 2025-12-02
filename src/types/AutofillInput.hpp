#pragma once

#include <functional>
#include <string>

#include <Geode/utils/cocos.hpp>

#include <Geode/ui/TextInput.hpp>

#include <NIDEnum.hpp>

#include "../popups/AutofillNamedIDsPreview.hpp"

struct AutofillInput
{
	AutofillInput() = default;
	explicit AutofillInput(NID, geode::TextInput*, std::function<void(const std::string&)>&&, std::function<void(NID, short)>&&);
	AutofillInput(const AutofillInput&);

	void onEditInput(const std::string&);

	geode::TextInput* getInputNode() { return textInput; }
	AutofillNamedIDsPreview* getAutofillPreview() { return autofillPreview; }

	void setEditInputCallback(std::function<void(const std::string&)>&& cb) { editInputCallback = std::move(cb); }
	void setSelectCallback(std::function<void(NID, short)>&& cb) { selectCallback = std::move(cb); }

	AutofillInput& operator=(const AutofillInput&) noexcept;
	AutofillInput& operator=(AutofillInput&&) noexcept;
	geode::TextInput* operator->() { return textInput; }
	operator bool() { return autofillPreview; }

	NID nid;
	geode::TextInput* textInput;
	geode::Ref<AutofillNamedIDsPreview> autofillPreview;
	std::function<void(const std::string&)> editInputCallback;
	std::function<void(NID, short)> selectCallback;
};
