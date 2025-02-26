#include <NIDManager.hpp>

template <class>
struct ToFilterImpl;

template <class... Args>
struct ToFilterImpl<geode::DispatchEvent<Args...>> {
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
	}, ToFilter<NIDManager::event::EventGetNameForID>("v1/get-name-for-id"_spr));

	new geode::EventListener(+[](short* res, NID nid, std::string id) {
		if (auto result = NIDManager::getIDForName(nid, id))
			*res = result.unwrap();
		else
			res = nullptr;

		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventGetIDForName>("v1/get-id-for-name"_spr));

	new geode::EventListener(+[](std::unordered_map<std::string, short>* nids, NID nid) {
		*nids = NIDManager::getNamedIDs(nid);
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventGetNamedIDs>("v1/get-named-ids"_spr));


	new geode::EventListener(+[](bool* res, NID nid, std::string name, short id) {
		*res = NIDManager::saveNamedID(nid, std::move(name), id).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventSaveNamedID>("v1/save-named-id"_spr));

	new geode::EventListener(+[](bool* res, NID nid, std::string name) {
		*res = NIDManager::removeNamedID(nid, std::move(name)).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventRemoveNamedID1>("v1/remove-named-id"_spr));

	new geode::EventListener(+[](bool* res, NID nid, short id) {
		*res = NIDManager::removeNamedID(nid, id).isOk();
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventRemoveNamedID2>("v1/remove-named-id"_spr));
}
