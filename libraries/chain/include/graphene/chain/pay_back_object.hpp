#pragma once
#include <graphene/db/object.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/chain/protocol/asset.hpp>
#include <map>
namespace graphene {
	namespace chain {
		using namespace graphene::db;
		class pay_back_object;
		class pay_back_object : public abstract_object<pay_back_object>
		{
		public:
			pay_back_object() {};
			static const uint8_t space_id = protocol_ids;
			static const uint8_t type_id = pay_back_object_type;
			address pay_back_owner;
			map<std::string, asset> owner_balance;
		};
		class bonus_object : public abstract_object<bonus_object>
		{
		public:
			bonus_object() {}
			static const uint8_t space_id = protocol_ids;
			static const uint8_t type_id = bonus_object_type;
			address owner;
			map<asset_id_type, asset> bonus;
		};

		struct by_payback_address;
		using pay_back_multi_index_type = multi_index_container <
			pay_back_object,
			indexed_by <
			ordered_unique< tag<by_id>,
			member<object, object_id_type, &object::id>
			>,
			ordered_unique<
			tag<by_payback_address>,
			member<pay_back_object, address, &pay_back_object::pay_back_owner>
			>
			>
		>;
		using payback_index = generic_index<pay_back_object, pay_back_multi_index_type>;


		struct by_addr;
		using bonus_multi_index_type = multi_index_container <
			bonus_object, indexed_by < 
			ordered_unique< tag<by_id>, member<object, object_id_type, &object::id> >,
			ordered_unique< tag<by_addr>, member<bonus_object, address, &bonus_object::owner> >
			>
		>;
		using bonus_index = generic_index<bonus_object, bonus_multi_index_type>;
	}
}
FC_REFLECT_DERIVED(graphene::chain::pay_back_object, (graphene::db::object),
(pay_back_owner)
(owner_balance)
)
FC_REFLECT_DERIVED(graphene::chain::bonus_object, (graphene::db::object),
(owner)
(bonus)
)