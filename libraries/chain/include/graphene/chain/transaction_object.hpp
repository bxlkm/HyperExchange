/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#include <fc/io/raw.hpp>
#include <graphene/chain/protocol/operations.hpp>
#include <graphene/db/generic_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <graphene/chain/protocol/transaction.hpp>
#include <fc/uint128.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace graphene { namespace chain {
   using namespace graphene::db;
   using boost::multi_index_container;
   using namespace boost::multi_index;
   /**
    * The purpose of this object is to enable the detection of duplicate transactions. When a transaction is included
    * in a block a transaction_object is added. At the end of block processing all transaction_objects that have
    * expired can be removed from the index.
    */
   class transaction_object : public abstract_object<transaction_object>
   {
      public:
         static const uint8_t space_id = implementation_ids;
         static const uint8_t type_id  = impl_transaction_object_type;

         signed_transaction  trx;
         transaction_id_type trx_id;

         time_point_sec get_expiration()const { return trx.expiration; }
   };

   struct by_expiration;
   struct by_id;
   struct by_trx_id;
   typedef multi_index_container<
      transaction_object,
      indexed_by<
         ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
         hashed_unique< tag<by_trx_id>, BOOST_MULTI_INDEX_MEMBER(transaction_object, transaction_id_type, trx_id), std::hash<transaction_id_type> >,
         ordered_non_unique< tag<by_expiration>, const_mem_fun<transaction_object, time_point_sec, &transaction_object::get_expiration > >
      >
   > transaction_multi_index_type;

   typedef generic_index<transaction_object, transaction_multi_index_type> transaction_index;

   class multisig_asset_transfer_object : public abstract_object<multisig_asset_transfer_object>
   {
   public:
	   static const uint8_t space_id = protocol_ids;
	   static const uint8_t type_id = multisig_transfer_object_type;
	   enum tranaction_status
	   {
		   success = 0,
		   failure,
		   waiting_signtures,
		   waiting
	   };
	   std::string		 chain_type;
	   tranaction_status status;
	   fc::variant_object  trx;
	   vector<string> signatures;
	   //std::string get_tunnel_account()const { return bind_account; }
   };

   /**
   * @ingroup object_index
   */
   struct by_status {};
   typedef multi_index_container<
	   multisig_asset_transfer_object,
	   indexed_by<
	   ordered_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
	   ordered_non_unique< tag<by_status>, member<multisig_asset_transfer_object, multisig_asset_transfer_object::tranaction_status, &multisig_asset_transfer_object::status> >
	   >
   > multi_multisig_index_type;

   /**
   * @ingroup object_index
   */
   typedef generic_index<multisig_asset_transfer_object, multi_multisig_index_type> crosschain_transfer_index;


} }

FC_REFLECT_DERIVED( graphene::chain::transaction_object, (graphene::db::object), (trx)(trx_id) )
FC_REFLECT_ENUM(graphene::chain::multisig_asset_transfer_object::tranaction_status, (success)(failure)(waiting_signtures)(waiting))
FC_REFLECT_DERIVED(graphene::chain::multisig_asset_transfer_object, (graphene::db::object), (chain_type)(status)(trx)(signatures))