#include <NIDManager.hpp>
#include <NIDExtrasManager.hpp>

template <class>
struct ToFilterImpl;

template <class... Args>
struct ToFilterImpl<geode::DispatchEvent<Args...>>
{
	using type = geode::DispatchFilter<Args...>;
};

template <class T>
using ToFilter = typename ToFilterImpl<T>::type;

$execute
{
	new geode::EventListener(+[](std::string* res, NID nid, short id) {
		if (auto result = NIDManager::getNameForID(nid, id))
			*res = result.unwrap();
		else
		 	res = nullptr;

		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventGetNameForID>("v2/get-name-for-id"_spr));

	new geode::EventListener(+[](short* res, NID nid, std::string id) {
		if (auto result = NIDManager::getIDForName(nid, id))
			*res = result.unwrap();
		else
			res = nullptr;

		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventGetIDForName>("v2/get-id-for-name"_spr));

	new geode::EventListener(+[](std::unordered_map<std::string, short>* nids, NID nid) {
		*nids = NIDManager::getNamedIDs(nid);
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventGetNamedIDs>("v2/get-named-ids"_spr));


	new geode::EventListener(+[](bool* res, NID nid, std::string name, short id) {
		*res = NIDManager::saveNamedID(nid, std::move(name), id).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventSaveNamedID>("v2/save-named-id"_spr));

	new geode::EventListener(+[](bool* res, NID nid, std::string name) {
		*res = NIDManager::removeNamedID(nid, std::move(name)).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventRemoveNamedID1>("v2/remove-named-id"_spr));

	new geode::EventListener(+[](bool* res, NID nid, short id) {
		*res = NIDManager::removeNamedID(nid, id).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventRemoveNamedID2>("v2/remove-named-id"_spr));


	// NID Extras

	new geode::EventListener(+[](bool* res, NID nid, short id) {
		*res = NIDExtrasManager::getIsNamedIDPreviewed(nid, id).unwrapOr(false);
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventGetIDIsPreviewed>("v2/get-id-is-previewed"_spr));

	new geode::EventListener(+[](bool* res, NID nid, std::string name) {
		*res = NIDExtrasManager::getIsNamedIDPreviewed(nid, std::move(name)).unwrapOr(false);
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventGetNamedIDIsPreviewed>("v2/get-named-id-is-previewed"_spr));

	new geode::EventListener(+[](bool* res, NID nid, short id, bool state) {
		*res = NIDExtrasManager::setNamedIDIsPreviewed(nid, id, state).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventSetIDIsPreviewed>("v2/set-id-is-previewed"_spr));

	new geode::EventListener(+[](bool* res, NID nid, std::string name, bool state) {
		*res = NIDExtrasManager::setNamedIDIsPreviewed(nid, std::move(name), state).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventSetNamedIDIsPreviewed>("v2/set-named-id-is-previewed"_spr));

	new geode::EventListener(+[](std::string* res, NID nid, short id) {
		if (auto result = NIDExtrasManager::getNamedIDDescription(nid, id))
			*res = result.unwrap();
		else
			res = nullptr;

		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventGetIDDescription>("v2/get-id-description"_spr));

	new geode::EventListener(+[](std::string* res, NID nid, std::string name) {
		if (auto result = NIDExtrasManager::getNamedIDDescription(nid, std::move(name)))
			*res = result.unwrap();
		else
			res = nullptr;

		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventGetNamedIDDescription>("v2/get-named-id-description"_spr));

	new geode::EventListener(+[](bool* res, NID nid, short id, std::string description) {
		*res = NIDExtrasManager::setNamedIDDescription(nid, id, std::move(description)).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventSetIDDescription>("v2/set-id-description"_spr));

	new geode::EventListener(+[](bool* res, NID nid, std::string name, std::string description) {
		*res = NIDExtrasManager::setNamedIDDescription(nid, std::move(name), std::move(description)).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventSetNamedIDDescription>("v2/set-named-id-description"_spr));

	new geode::EventListener(+[](NamedIDExtra* res, NID nid, short id) {
		if (auto result = NIDExtrasManager::getNamedIDExtras(nid, id))
			*res = result.unwrap();
		else
			res = nullptr;

		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventGetIDExtras>("v2/get-id-extras"_spr));

	new geode::EventListener(+[](NamedIDExtra* res, NID nid, std::string name) {
		if (auto result = NIDExtrasManager::getNamedIDExtras(nid, std::move(name)))
			*res = result.unwrap();
		else
			res = nullptr;

		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventGetNamedIDExtras>("v2/get-named-id-extras"_spr));

	new geode::EventListener(+[](bool* res, NID nid, short id, NamedIDExtra extras) {
		*res = NIDExtrasManager::setNamedIDExtras(nid, id, std::move(extras)).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventSetIDExtras>("v2/set-id-extras"_spr));

	new geode::EventListener(+[](bool* res, NID nid, std::string name, NamedIDExtra extras) {
		*res = NIDExtrasManager::setNamedIDExtras(nid, std::move(name), std::move(extras)).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventSetNamedIDExtras>("v2/set-named-id-extras"_spr));

	new geode::EventListener(+[](bool* res, NID nid, short id) {
		*res = NIDExtrasManager::removeNamedIDExtras(nid, id).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventRemoveIDExtras>("v2/remove-id-extras"_spr));

	new geode::EventListener(+[](bool* res, NID nid, std::string name) {
		*res = NIDExtrasManager::removeNamedIDExtras(nid, std::move(name)).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventRemoveNamedIDExtras>("v2/remove-named-id-extras"_spr));


	new geode::EventListener(+[](NamedIDsExtras* res, NID nid) {
		if (auto nidExtras = NIDExtrasManager::getNIDExtras(nid))
			*res = nidExtras.unwrap();
		else
			res = nullptr;

		return geode::ListenerResult::Stop;
	}, ToFilter<NIDExtrasManager::event::EventGetAllNIDExtras>("v2/get-all-id-extras"_spr));
}
