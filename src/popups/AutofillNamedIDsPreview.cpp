#include "AutofillNamedIDsPreview.hpp"

#include <NIDManager.hpp>

#include "cells/NamedIDCell.hpp"

#include "fuzzy_match.hpp"

using namespace geode::prelude;

AutofillNamedIDsPreview* AutofillNamedIDsPreview::create(NID nid, const std::string_view query)
{
	auto ret = new AutofillNamedIDsPreview();

	if (ret && ret->init(nid, query))
		ret->autorelease();
	else
	{
		delete ret;
		ret = nullptr;
	}

	return ret;
}

bool AutofillNamedIDsPreview::init(NID nid, const std::string_view query)
{
	if (!CCLayer::init()) return false;

	this->setID("AutofillNamedIDsPreview");

	m_ids_type = nid;

	this->setContentSize(PREVIEW_SIZE);
	this->setTouchEnabled(true);
	this->setKeypadEnabled(true);

	m_bg_sprite = cocos2d::extension::CCScale9Sprite::create("GJ_square01.png", { .0f, .0f, 80.f, 80.f });
	m_bg_sprite->setContentSize(PREVIEW_SIZE);
	m_bg_sprite->setZOrder(-1);
	this->addChildAtPosition(m_bg_sprite, Anchor::Center, { .0f, .0f });

	m_layer_bg = CCLayerColor::create({ 0, 0, 0, 75 });
	m_layer_bg->setContentSize(SCROLL_LAYER_SIZE);
	m_layer_bg->ignoreAnchorPointForPosition(false);
	this->addChildAtPosition(m_layer_bg, Anchor::Center, { -3.5f, -2.f });

	m_list = ScrollLayer::create(SCROLL_LAYER_SIZE);
	m_list->setTouchEnabled(true);
	m_list->m_contentLayer->setLayout(
		ColumnLayout::create()
			->setAxisReverse(true)
			->setAutoGrowAxis(m_list->getContentHeight())
			->setCrossAxisOverflow(false)
			->setAxisAlignment(AxisAlignment::End)
			->setGap(.0f)
	);
	m_list->m_contentLayer->setPositionY(-2.f);
	m_list->moveToTop();

	updateList(m_ids_type, query);

	m_layer_bg->addChildAtPosition(m_list, Anchor::BottomLeft);

	auto listBorders = geode::ListBorders::create();
	listBorders->setContentSize(SCROLL_LAYER_SIZE);
	this->addChildAtPosition(listBorders, Anchor::Center, { -2.5f, .0f });

	m_scroll_bar = geode::Scrollbar::create(m_list);
	this->addChildAtPosition(m_scroll_bar, Anchor::Center, { m_layer_bg->getContentWidth() / 2.f + 3.f, .0f });

	return true;
}

void AutofillNamedIDsPreview::attachToInput(geode::TextInput* input)
{
	CCPoint inputWorldPos = input->getParent()->convertToWorldSpace(input->getPosition());

	const CCSize& screenSize = CCDirector::sharedDirector()->getWinSize();
	const CCPoint& anchor = this->getAnchorPoint();
	const CCSize& inputSize = input->getScaledContentSize();
	const float screenLeft = .0f;
	const float screenRight = screenSize.width;
	const float screenBottom = .0f;
	const float screenTop = screenSize.height;

	// initial pos is under the input
	CCPoint targetPos{
		inputWorldPos.x,
		inputWorldPos.y - (PREVIEW_SIZE.height / 2.f) - (inputSize.height / 2.f)
	};
	CCRect previewRect{
		{
			targetPos.x - anchor.x * PREVIEW_SIZE.width,
			targetPos.y - anchor.y * PREVIEW_SIZE.height
		},
		PREVIEW_SIZE
	};

	// horizontal adjustment
	if (previewRect.getMinX() < screenLeft)
		targetPos.x += screenLeft - previewRect.getMinX();
	else if (previewRect.getMaxX() > screenRight)
		targetPos.x -= previewRect.getMaxX() - screenRight;

	// vertical adjustment
	if (previewRect.getMinY() < screenBottom)
		targetPos.y = inputWorldPos.y + (inputSize.height / 2.f) + (PREVIEW_SIZE.height / 2.f) + 10.f * input->getScale();

	this->setPosition(targetPos);
}

void AutofillNamedIDsPreview::show()
{
	if (!this->getParent())
		CCScene::get()->addChild(this, CCScene::get()->getHighestChildZ());
}

void AutofillNamedIDsPreview::updateList(const std::string_view query)
{
	m_query = query;

	m_list->m_contentLayer->removeAllChildren();

	{
		const std::unordered_map<std::string, short>& namedIDs = NIDManager::getNamedIDs(m_ids_type);

		std::vector<std::pair<std::string, short>> elements{ namedIDs.begin(), namedIDs.end() };
		std::sort(elements.begin(), elements.end(), [](auto& a, auto& b) { return a.second < b.second; });

		std::array<std::uint8_t, 256> indices{};
		bool bg = false;
		const bool queryEmpty = m_query.empty();

		for (auto& [name, id] : elements)
		{
			indices.fill(0u);

			if (!queryEmpty && !ng::utils::fuzzy_match::matchesQuery(m_query, { name, id }, indices))
				continue;

			auto item = NamedIDCell<true>::create(m_ids_type, id, std::move(name), PREVIEW_SIZE.width);
			item->setDefaultBGColor({ 0, 0, 0, static_cast<GLubyte>(bg ? 60 : 20) });
			item->setSelectCallback([&](NID nid, short id) {
				this->selectCallback(nid, id);
			});
			item->highlightQuery(m_query, indices);
			m_list->m_contentLayer->addChild(item);

			bg = !bg;
		}
	}

	m_list->m_contentLayer->updateLayout();
}

void AutofillNamedIDsPreview::updateList(NID nid, const std::string_view query)
{
	m_ids_type = nid;
	m_query = query;

	updateList(m_query);

	m_list->moveToTop();
}

void AutofillNamedIDsPreview::keyBackClicked()
{
	this->removeFromParent();
}

void AutofillNamedIDsPreview::onEnterTransitionDidFinish()
{
	auto TD = CCTouchDispatcher::get();
	auto layerTouchHandlerPrio = TD->findHandler(this)->getPriority();

	TD->setPriority(layerTouchHandlerPrio - 1, m_list);
	TD->setPriority(layerTouchHandlerPrio - 1, m_scroll_bar);

	CCLayer::onEnterTransitionDidFinish();
}

void AutofillNamedIDsPreview::onExit()
{
	CCTouchDispatcher::get()->unregisterForcePrio(this);
	CCTouchDispatcher::get()->removeDelegate(this);

	CCLayer::onExit();
}

bool AutofillNamedIDsPreview::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (
		this->isVisible() &&
		this->boundingBox().containsPoint(this->getParent()->convertTouchToNodeSpace(touch))
	) {
		return true;
	}

	this->removeFromParent();

	return false;
}

void AutofillNamedIDsPreview::registerWithTouchDispatcher()
{
	// touch prio is set in 2 steps for this layer,
	// in registerWithTouchDispatcher (to increment the prio of the layer)
	// and in onEnterTransitionDidFinish (to increment the prio of the nodes
	// (scroll list & scroll bar)

	auto TD = CCTouchDispatcher::get();

	TD->addTargetedDelegate(this, cocos2d::kCCMenuHandlerPriority, true);
	TD->registerForcePrio(this, 2);
	TD->setPriority(TD->findHandler(this)->getPriority() - 3, this);
}

void AutofillNamedIDsPreview::selectCallback(NID nid, short id)
{
	m_select_callback(nid, id);

	this->removeFromParent();
}
