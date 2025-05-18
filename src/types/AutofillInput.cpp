#include "AutofillInput.hpp"

using namespace geode::prelude;

AutofillInput::AutofillInput(
	NID nid,
	geode::TextInput* input,
	std::function<void(const std::string&)>&& editCb,
	std::function<void(NID, short)>&& selectCb
)
	: nid(nid), autofillPreview(AutofillNamedIDsPreview::create(nid, "")),
		textInput(input), editInputCallback(std::move(editCb)),
		selectCallback(std::move(selectCb))
{
	autofillPreview->attachToInput(textInput);
	autofillPreview->setSelectCallback(std::move(selectCallback));

	input->setCallback([&](const std::string& str) {
		this->onEditInput(str);
	});
}

AutofillInput::AutofillInput(const AutofillInput& other)
{
	this->nid = other.nid;
	this->textInput = other.textInput;
	this->autofillPreview.swap(other.autofillPreview);
	this->editInputCallback = std::move(other.editInputCallback);
	this->selectCallback = std::move(other.selectCallback);

	this->textInput->setCallback([&](const std::string& str) {
		this->onEditInput(str);
	});
}

void AutofillInput::onEditInput(const std::string& str)
{
	editInputCallback(str);

	autofillPreview->updateList(nid, str);
	autofillPreview->show();

	textInput->getInputNode()->onClickTrackNode(true);
}

AutofillInput& AutofillInput::operator=(const AutofillInput& other) noexcept
{
	this->nid = other.nid;
	this->textInput = other.textInput;
	this->autofillPreview.swap(other.autofillPreview);
	this->editInputCallback = std::move(other.editInputCallback);
	this->selectCallback = std::move(other.selectCallback);

	// capturing by reference uses other's this pointer
	this->textInput->setCallback([&](const std::string& str) {
		this->onEditInput(str);
	});

	return *this;
}

AutofillInput& AutofillInput::operator=(AutofillInput&& other) noexcept
{
	this->nid = other.nid;
	this->textInput = other.textInput;
	this->autofillPreview.swap(other.autofillPreview);
	this->editInputCallback = std::move(other.editInputCallback);
	this->selectCallback = std::move(other.selectCallback);

	// capturing by reference uses other's this pointer
	this->textInput->setCallback([&](const std::string& str) {
		this->onEditInput(str);
	});

	return *this;
}
