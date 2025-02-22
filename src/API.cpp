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
	new geode::EventListener(+[](geode::Result<std::string>& res, NID nid, short id) {
		res = NIDManager::getNameForID(nid, id);
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventGetNameForID>("v1/get-name-for-id"_spr));

	new geode::EventListener(+[](geode::Result<short>& res, NID nid, const std::string& id) {
		res = NIDManager::getIDForName(nid, id);
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventGetIDForName>("v1/get-id-for-name"_spr));

	new geode::EventListener(+[](std::unordered_map<std::string, short>& nids, NID nid) {
		nids = NIDManager::getNamedIDs(nid);
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventGetNamedIDs>("v1/get-named-ids"_spr));


	new geode::EventListener(+[](geode::Result<>& res, NID nid, std::string name, short id) {
		res = NIDManager::saveNamedID(nid, std::move(name), id);
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventSaveNamedID>("v1/save-named-id"_spr));

	new geode::EventListener(+[](geode::Result<>& res, NID nid, std::string name) {
		res = NIDManager::removeNamedID(nid, std::move(name));
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventRemoveNamedID1>("v1/remove-named-id"_spr));

	new geode::EventListener(+[](geode::Result<>& res, NID nid, short id) {
		res = NIDManager::removeNamedID(nid, id);
		return geode::ListenerResult::Stop;
	}, ToFilter<NIDManager::event::EventRemoveNamedID2>("v1/remove-named-id"_spr));
}
