#pragma once
#
#include <vector>
#include <shared_mutex>
#include <chrono>
#include <optional>
#include <unordered_set>
#include <map>
#include <memory>
#
#include "def_project/_define.h"
#include "ext/MutexSharedRecursive.h"
#include "ext/EzSignalSlotTable.h"
#

namespace def_project
{
class SaltExchangeTable;
}

class             //
    def_project:: //
    SaltExchangeTable
{
    struct Order;
    struct SubOrderMatchRequest;
    struct StubGeneral;
    class ManagerSignalSlot;
    std::atomic<TypeId> m_countOf_order_id = 0;
    std::atomic<TypeId> m_countOf_suborder_id = 0;
    std::pair<ext::multithread::MutexSharedRecursive, std::unique_ptr<ManagerSignalSlot>> m_managerOf_signal_slot;
    std::pair<ext::multithread::MutexSharedRecursive, std::vector<std::shared_ptr<Order>>> m_poolOf_oreder;                                 // [rule] member of pool: only add, modify. and no remove.
    std::pair<ext::multithread::MutexSharedRecursive, std::vector<std::shared_ptr<SubOrderMatchRequest>>> m_poolOf_suboreder_match_request; // [rule] member of pool: only add, remove. and no modify.

  public:                                                                                                                  //
    explicit SaltExchangeTable();                                                                                          //
    auto fx_fgApi_order_add(TypeId, EnumOrderSide, EnumOrderTimeInForce, TypeQty, std::uint16_t) -> std::optional<TypeId>; // fx(user_id, side, time_in_force, qty, unit_gramme,) -> stub
    auto fx_fgApi_suborder_match_request_remake_by_oca_by_auto_redo(TypeId, TypeId, std::unordered_set<TypeId>)            // fx(user_id_as_request_source, order_id_as_request_source, order_id_as_request_target,) -> arrOf_stub
        -> const std::weak_ptr<const std::vector<StubGeneral>>;                                                            //
    auto fx_fgApi_suborder_match_request_remake_by_oca_by_auto_redo_0_reverse(TypeId, TypeId) -> void;                     // fx(user_id_as_request_source, order_id_as_request_source,)
    auto fx_fgApi_suborder_match_request_response(TypeId, TypeId, TypeQty) -> bool;                                        // fx(user_id_as_request_target, suborder_id, qty_response,) -> is_success
    auto fx_fgApi_print_user_realtime_position(std::basic_ostream<char> &, std::string = "") -> void;                      // fx(print_target, description,)
    auto fx_fgApi_print_user_realtime_match_request_as_target(std::basic_ostream<char> &, std::string = "") -> void;       // fx(print_target, description,)
  private:                                                                                                                 //
    auto fx_elfin_gc() -> void;                                                                                            // fx()
    auto fx_bgApi_suborder_match_request_make(Order &, Order &) -> std::optional<StubGeneral>;                             // fx(request_source, request_target,) -> stb
    auto fx_bgApi_suborder_match_request_clear(EnumSubOrderRequestSide, Order &) -> void;                                  // fx(request_side, order,)
                                                                                                                           //
};

struct            //
    def_project:: //
    SaltExchangeTable::Order
{
    const TypeId m_user_id;
    const TypeId m_order_id;
    const EnumOrderSide m_side;
    const EnumOrderTimeInForce m_time_in_force;
    const TypeQty m_qty_total;         // ( m_qty_total + m_qty_modify ) == ( m_qty_leave + m_qty_execut )
    TypeQty m_qty_leave;               // ^
    TypeQty m_qty_execute;             // ^
    TypeQtyModify m_qty_modify;        // ^
    const std::uint16_t m_unit_gramme; // ( m_unit_gramme * m_qty_total ) == "total gramme for buy/sell."
};

struct            //
    def_project:: //
    SaltExchangeTable::SubOrderMatchRequest
{
    Order &m_request_source;
    Order &m_request_target;
    const TypeId m_suborder_id;
    const TypeQty m_qty_request_as_target;
    const std::uint16_t &m_unit_gramme_as_target;
};

struct            //
    def_project:: //
    SaltExchangeTable::StubGeneral
{
    const std::string m_str__FUNCTION__; // __FUNCTION__
    const std::string m_str_description; // __PRETTY_FUNCTION__ ""
    const std::optional<TypeId> m_user_id;
    const std::optional<TypeId> m_order_id;
    const std::optional<TypeId> m_suborder_id;
    const std::optional<TypeId> m_qty_request;
};

class                                    //
    def_project::                        //
    SaltExchangeTable::ManagerSignalSlot //
    : private ext::thread_unsafety::EzSignalSlotTable
{
  public:
    using TypeSlotFunction = ext::thread_unsafety::EzSignalSlotTable::TypeSlotFunction;
    using TypeSignal = ext::thread_unsafety::EzSignalSlotTable::TypeSignal;
    using TypeSignalLike_With_OrderId = std::pair<EnumOrderCycleHook, TypeId>;        // {order_cycle, order_id}
    auto fx_register(TypeSignalLike_With_OrderId, TypeSlotFunction &&) -> TypeSignal; // fx(signale_like, slot_function,) -> signal
    auto fx_unregister(TypeSignalLike_With_OrderId) -> void;                          // fx(signale_like,)
    auto fx_emit(TypeSignal) -> void;                                                 // fx(signal)
    auto fx_emit(TypeSignalLike_With_OrderId) -> void;                                // fx(signale_like)

  private:
    std::map<TypeSignalLike_With_OrderId, TypeSignal> m_mapRe_signal_with_order_id; //
};
