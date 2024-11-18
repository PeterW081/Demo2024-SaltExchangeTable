#include "def_project/SaltExchangeTable.h"
#pragma
#
#include <mutex>
#include <map>
#include <iomanip>
#include <functional>
#include <algorithm>
#

namespace nscxx_root = def_project;
namespace nscxx
{
template <typename T>
static auto GX_value_equal(T, T) -> bool;
}

/**
/// function - nscxx
/// function - constructor
/// function - elfin
/// function - hook
/// function - bgApi	// background,  api,
/// function - fgApi	// frontground, api,
**/

/// function - nscxx
template <typename T>
auto        //
    nscxx:: //
    GX_value_equal(T var01, T var02) -> bool
{
    return (var01 == var02);
}

/// function - constructor
/*constructor*/         //
nscxx_root::            //
    SaltExchangeTable:: //
    SaltExchangeTable()
{
    this->m_managerOf_signal_slot.second = std::make_unique<ManagerSignalSlot>();
}

/// function - elfin
auto                    //
    nscxx_root::        //
    SaltExchangeTable:: //
    fx_elfin_gc() -> void
{
    auto func = []<typename T>(T &pool) -> void {
        auto &[mutex, container] = pool;
        auto locker_write = std::unique_lock(mutex);
        auto ptrItPair = std::ranges::remove(container, nullptr);
        container.erase(ptrItPair.begin(), ptrItPair.end());
    };
    func(this->m_poolOf_oreder);
    func(this->m_poolOf_suboreder_match_request);
}

/// function - bgApi
auto                    //
    nscxx_root::        //
    SaltExchangeTable:: //
    fx_bgApi_suborder_match_request_make(Order &request_source, Order &request_target) -> std::optional<StubGeneral>
{
    // XXX this like "orderPlacement()".
    auto &[mutex_poolOf_order, container_poolOf_order] = this->m_poolOf_oreder;
    auto &[mutex_poolOf_suborder, container_poolOf_suborder] = this->m_poolOf_suboreder_match_request;

    bool is_ok = true;
    is_ok &= (request_source.m_side != request_target.m_side);
    is_ok &= (request_source.m_user_id != request_target.m_user_id);
    is_ok &= (request_source.m_order_id != request_target.m_order_id);
    if (not is_ok)
    {
        return std::nullopt;
    }

    auto locker_write = std::scoped_lock(mutex_poolOf_order, mutex_poolOf_suborder);
    auto qty_request_as_target = TypeQty();
    /**/ {
        auto s_uint = request_source.m_unit_gramme;
        auto t_uint = request_target.m_unit_gramme;
        auto x = (request_source.m_qty_leave * request_source.m_unit_gramme) / (s_uint * t_uint);
        qty_request_as_target = x * s_uint;
    }
    is_ok &= (qty_request_as_target > 0);
    if (not is_ok)
    {
        return std::nullopt;
    }
    auto suborder = std::make_shared<SubOrderMatchRequest>(SubOrderMatchRequest{
        .m_request_source = request_source,
        .m_request_target = request_target,
        .m_suborder_id = ++(this->m_countOf_suborder_id),
        .m_qty_request_as_target = qty_request_as_target,
        .m_unit_gramme_as_target = request_target.m_unit_gramme,
    });
    container_poolOf_suborder.emplace_back() = suborder;

    auto stub = StubGeneral{
        .m_str__FUNCTION__ = __FUNCTION__,
        .m_str_description = std::string() + __PRETTY_FUNCTION__ + " -- user, order, as_request_target,",
        .m_user_id = suborder->m_request_target.m_user_id,
        .m_order_id = suborder->m_request_target.m_order_id,
        .m_suborder_id = suborder->m_suborder_id,
        .m_qty_request = suborder->m_qty_request_as_target,
    };
    return stub;
}
auto                    //
    nscxx_root::        //
    SaltExchangeTable:: //
    fx_bgApi_suborder_match_request_clear(EnumSubOrderRequestSide request_side, Order &order) -> void
{
    auto &[mutex, container] = this->m_poolOf_suboreder_match_request;

    //
    auto order_id = order.m_order_id;
    auto func00 = std::optional<std::function<auto(decltype(container)::value_type &)->void>>();
    auto func01_order_as_source = [&order_id](decltype(container)::value_type &var) -> void {
        if (var != nullptr || var->m_request_source.m_order_id == order_id)
        {
            var.reset();
        }
    };
    auto func02_order_as_target = [&order_id](decltype(container)::value_type &var) -> void {
        if (var != nullptr || var->m_request_target.m_order_id == order_id)
        {
            var.reset();
        }
    };
    switch (request_side)
    {
    case EnumSubOrderRequestSide::SOURCE:
        func00 = func01_order_as_source;
        break;
    case EnumSubOrderRequestSide::TARGET:
        func00 = func02_order_as_target;
        break;
    [[unlikely]] default:
        func00 = std::nullopt;
        break;
    }
    if (not func00.has_value())
    {
        return;
    }

    // write
    auto locker_write = std::unique_lock(mutex);
    std::ranges::for_each(container, func00.value());
}

/// function - fgApi
auto                    //
    nscxx_root::        //
    SaltExchangeTable:: //
    fx_fgApi_order_add(TypeId user_id, EnumOrderSide side, EnumOrderTimeInForce time_in_force, TypeQty qty, std::uint16_t unit_gramme) -> std::optional<TypeId>
{
    auto &[mutex, container] = this->m_poolOf_oreder;
    bool is_ok;

    auto ptr_order = std::make_shared<Order>(Order{
        .m_user_id = user_id,
        .m_order_id = ++(this->m_countOf_order_id),
        .m_side = side,
        .m_time_in_force = time_in_force,
        .m_qty_total = qty,
        .m_qty_leave = qty,
        .m_qty_execute = 0,
        .m_qty_modify = 0,
        .m_unit_gramme = unit_gramme,
    });

    is_ok = true;
    is_ok &= (ptr_order->m_side != EnumOrderSide::NULL0);
    is_ok &= (ptr_order->m_time_in_force != EnumOrderTimeInForce::NULL0);
    is_ok &= (ptr_order->m_qty_total != 0);
    is_ok &= (ptr_order->m_unit_gramme != 0);
    if (not is_ok)
    {
        return std::nullopt;
    }

    auto locker_write = std::unique_lock(mutex);
    auto func = [&ptr_order](const decltype(container)::value_type &var) -> bool { return (var != nullptr && var->m_order_id == ptr_order->m_order_id && var->m_side != ptr_order->m_side); };
    auto ptrIt_end = container.end(), ptrIt_find = std::ranges::find_if(container, func);
    if (ptrIt_find != ptrIt_end)
    {
        // all orders of same user must have the same side.
        return std::nullopt;
    }

    container.emplace_back() = ptr_order;
    return ptr_order->m_order_id;
}
auto                    //
    nscxx_root::        //
    SaltExchangeTable:: //
    fx_fgApi_suborder_match_request_remake_by_oca_by_auto_redo(TypeId user_id_as_request_source, TypeId order_id_as_request_source, std::unordered_set<TypeId> order_id_as_request_target) -> const std::weak_ptr<const std::vector<StubGeneral>>
{
    auto &[mutex_poolOf_order, container_poolOf_order] = this->m_poolOf_oreder;
    auto &[mutex_poolOf_suborder, container_poolOf_suborder] = this->m_poolOf_suboreder_match_request;
    auto &[mutex_managerOf_signal_slot, table_managerOf_signal_slot] = this->m_managerOf_signal_slot;

    // reade
    auto locker_reade = decltype(mutex_poolOf_suborder)::FX_makerSafety_std_shared_lock(mutex_poolOf_suborder);
    auto func01 = [&order_id_as_request_source](decltype(container_poolOf_order)::value_type &var) -> bool { return (var != nullptr && var->m_order_id == order_id_as_request_source); };
    auto ptrIt_end = container_poolOf_order.end(), ptrIt_find = std::ranges::find_if(container_poolOf_order, func01);
    if (ptrIt_find == ptrIt_end || ptrIt_find.operator*()->m_user_id != user_id_as_request_source || ptrIt_find.operator*()->m_time_in_force != EnumOrderTimeInForce::OCA)
    {
        return {};
    }
    locker_reade.reset();

    // write
    auto locker_write = std::scoped_lock(mutex_poolOf_order, mutex_poolOf_suborder, mutex_managerOf_signal_slot);
    auto ptr_order_as_request_source = std::weak_ptr(ptrIt_find.operator*());
    // write - prepdo
    this->fx_bgApi_suborder_match_request_clear(EnumSubOrderRequestSide::SOURCE, ptr_order_as_request_source.lock().operator*());
    // write - maindo - part01/02
    auto arrOf_stub = std::make_shared<std::vector<StubGeneral>>();
    auto func_slot = decltype(table_managerOf_signal_slot)::element_type::TypeSlotFunction();
    func_slot = [this, ptr_order_as_request_source, order_id_as_request_target, arrOf_stub]() -> bool {
        auto &container = this->m_poolOf_oreder.second;
        auto locker_write = std::scoped_lock(this->m_poolOf_oreder.first, this->m_poolOf_suboreder_match_request.first);
        arrOf_stub->clear();
        //
        auto ptr_order_source = ptr_order_as_request_source.lock();
        if (ptr_order_source == nullptr)
        {
            return false;
        }
        auto func_inner = [this, &ptr_order_source, &order_id_as_request_target, &arrOf_stub](std::remove_reference_t<decltype(container)>::value_type &fp_order) -> void {
            if (std::ranges::any_of(order_id_as_request_target, std::bind(nscxx::GX_value_equal<TypeId>, fp_order->m_order_id, std::placeholders::_1)))
            {
                auto stub = this->fx_bgApi_suborder_match_request_make(ptr_order_source.operator*(), fp_order.operator*());
                if (stub.has_value())
                {
                    arrOf_stub->emplace_back(stub.value());
                }
            }
        };
        std::ranges::for_each(container, func_inner);
        return not arrOf_stub->empty();
    };
    // write - maindo - part02/02
    auto signal = decltype(table_managerOf_signal_slot)::element_type::TypeSignal();
    auto signal_like = decltype(table_managerOf_signal_slot)::element_type::TypeSignalLike_With_OrderId();
    signal_like = {EnumOrderCycleHook::POST_SUBORDER_MATCH_REQUEST_RESPONSE, order_id_as_request_source};
    signal = table_managerOf_signal_slot->fx_register(signal_like, std::move(func_slot));
    table_managerOf_signal_slot->fx_emit(signal);

    this->fx_elfin_gc();
    return arrOf_stub;
}
auto                    //
    nscxx_root::        //
    SaltExchangeTable:: //
    fx_fgApi_suborder_match_request_remake_by_oca_by_auto_redo_0_reverse(TypeId user_id_as_request_source, TypeId order_id_as_request_source) -> void
{
    auto &[mutex_poolOf_order, container_poolOf_order] = this->m_poolOf_oreder;
    auto &[mutex_poolOf_suborder, container_poolOf_suborder] = this->m_poolOf_suboreder_match_request;
    auto &[mutex_managerOf_signal_slot, table_managerOf_signal_slot] = this->m_managerOf_signal_slot;

    // reade
    auto locker_reade = decltype(mutex_poolOf_suborder)::FX_makerSafety_std_shared_lock(mutex_poolOf_suborder);
    auto func01 = [&order_id_as_request_source](decltype(container_poolOf_order)::value_type &var) -> bool { return (var != nullptr && var->m_order_id == order_id_as_request_source); };
    auto ptrIt_end = container_poolOf_order.end(), ptrIt_find = std::ranges::find_if(container_poolOf_order, func01);
    if (ptrIt_find == ptrIt_end || ptrIt_find.operator*()->m_user_id != user_id_as_request_source || ptrIt_find.operator*()->m_time_in_force != EnumOrderTimeInForce::OCA)
    {
        return;
    }
    locker_reade.reset();

    // write
    auto locker_write = std::scoped_lock(mutex_poolOf_order, mutex_poolOf_suborder, mutex_managerOf_signal_slot);
    auto ptr_order_as_request_source = ptrIt_find.operator*();
    // write - 01
    auto signal_like = decltype(table_managerOf_signal_slot)::element_type::TypeSignalLike_With_OrderId();
    signal_like = {EnumOrderCycleHook::POST_SUBORDER_MATCH_REQUEST_RESPONSE, order_id_as_request_source};
    table_managerOf_signal_slot->fx_unregister(signal_like);
    // write - 02
    this->fx_bgApi_suborder_match_request_clear(EnumSubOrderRequestSide::SOURCE, ptr_order_as_request_source.operator*());
    this->fx_elfin_gc();
}
auto                    //
    nscxx_root::        //
    SaltExchangeTable:: //
    fx_fgApi_suborder_match_request_response(TypeId user_id, TypeId suborder_id, TypeQty qty_response) -> bool
{
    // XXX this like "onOrderExecuted()".
    auto &[mutex_poolOf_order, container_poolOf_order] = this->m_poolOf_oreder;
    auto &[mutex_poolOf_suborder, container_poolOf_suborder] = this->m_poolOf_suboreder_match_request;
    auto &[mutex_managerOf_signal_slot, table_managerOf_signal_slot] = this->m_managerOf_signal_slot;
    auto is_ok = bool();
    auto order_id_as_request_source = TypeId();

    // reade
    auto locker_reade = decltype(mutex_poolOf_suborder)::FX_makerSafety_std_shared_lock(mutex_poolOf_suborder);
    auto func01 = [&suborder_id](decltype(container_poolOf_suborder)::value_type &var) -> bool { return (var != nullptr && var->m_suborder_id == suborder_id); };
    auto ptrIt_end = container_poolOf_suborder.end(), ptrIt_find = std::ranges::find_if(container_poolOf_suborder, func01);
    if (ptrIt_find == ptrIt_end)
    {
        return false;
    }
    else
    {
        auto &suborder = ptrIt_find.operator*().operator*();
        is_ok = true;
        is_ok &= (suborder.m_request_target.m_user_id == user_id);
        is_ok &= (suborder.m_qty_request_as_target >= qty_response);
        is_ok &= (((qty_response * suborder.m_unit_gramme_as_target) % suborder.m_request_source.m_unit_gramme) == 0);
        if (not is_ok)
        {
            return false;
        }
    }
    locker_reade.reset();

    // write
    auto locker_write = std::scoped_lock(mutex_poolOf_order, mutex_poolOf_suborder, mutex_managerOf_signal_slot);
    auto &ptr_suborder = ptrIt_find.operator*(); // this is reference. the reference of shared pointer.
    if (ptr_suborder == nullptr)
    {
        return false;
    }
    else
    {
        order_id_as_request_source = ptr_suborder->m_request_source.m_order_id;
    }
    auto qty_execute_as_request_target = qty_response;
    auto qty_execute_as_request_source = (qty_response * ptr_suborder->m_unit_gramme_as_target) / ptr_suborder->m_request_source.m_unit_gramme;
    is_ok = true;
    is_ok &= (qty_execute_as_request_target <= ptr_suborder->m_request_target.m_qty_leave);
    is_ok &= (qty_execute_as_request_source <= ptr_suborder->m_request_source.m_qty_leave);
    if (is_ok)
    {
        // target
        ptr_suborder->m_request_target.m_qty_leave -= qty_execute_as_request_target;
        ptr_suborder->m_request_target.m_qty_execute += qty_execute_as_request_target;
        // source
        ptr_suborder->m_request_source.m_qty_leave -= qty_execute_as_request_source;
        ptr_suborder->m_request_source.m_qty_execute += qty_execute_as_request_source;
        // source - oca
        if (ptr_suborder->m_request_source.m_time_in_force == EnumOrderTimeInForce::OCA)
        {
            this->fx_bgApi_suborder_match_request_clear(EnumSubOrderRequestSide::SOURCE, ptr_suborder->m_request_source);
        }
    }
    ptr_suborder.reset();

    // signal-slot-work
    auto signal_like = decltype(table_managerOf_signal_slot)::element_type::TypeSignalLike_With_OrderId();
    signal_like = {EnumOrderCycleHook::POST_SUBORDER_MATCH_REQUEST_RESPONSE, order_id_as_request_source};
    table_managerOf_signal_slot->fx_emit(signal_like);

    this->fx_elfin_gc();
    return is_ok;
}
auto                    //
    nscxx_root::        //
    SaltExchangeTable:: //
    fx_fgApi_print_user_realtime_position(std::basic_ostream<char> &output, std::string description) -> void
{
    auto &[mutex, container] = this->m_poolOf_oreder;

    auto position = std::map<TypeId, std::int64_t>();
    auto func = [&position](const decltype(container)::value_type &var) -> void {
        if (var == nullptr)
        {
            return;
        }
        switch (var->m_side)
        {
        case EnumOrderSide::BUYER:
            position[var->m_user_id] += (var->m_qty_execute * var->m_unit_gramme) * +1;
            break;
        case EnumOrderSide::SELLER:
            position[var->m_user_id] += (var->m_qty_execute * var->m_unit_gramme) * -1;
            break;
        [[unlikely]] default:
            break;
        }
    };
    auto locker_reade = decltype(mutex)::FX_makerSafety_std_shared_lock(mutex);
    std::ranges::for_each(container, func);
    locker_reade.reset();

    /**/ {
        output << "# " << std::chrono::system_clock::now().time_since_epoch().count() << " #";
        output << description;
        output << std::endl;
        output << "|";
        output << std::setw(8) << std::string("user") << ", ";
        output << std::setw(8) << std::string("side") << ", ";
        output << std::setw(21) << std::string("position(gramme salt)") << ", ";
        output << std::endl;
    }
    for (auto &var : position)
    {
        auto func_toString_side = [](std::int64_t &var) -> std::string {
            if (var == 0)
            {
                return "---";
            }
            else if (var > 0)
            {
                return "buy";
            }
            else if (var < 0)
            {
                return "sell";
            }
            return {};
        };
        output << "|";
        output << std::setw(8) << var.first << ", ";
        output << std::setw(8) << func_toString_side(var.second) << ", ";
        output << std::setw(21) << std::abs(var.second) << ",";
        output << std::endl;
    }
    /**/ {
        output << std::endl;
    }
}
auto                    //
    nscxx_root::        //
    SaltExchangeTable:: //
    fx_fgApi_print_user_realtime_match_request_as_target(std::basic_ostream<char> &output, std::string description) -> void
{
    auto &[mutex, container] = this->m_poolOf_suboreder_match_request;

    auto locker_reade = decltype(mutex)::FX_makerSafety_std_shared_lock(mutex);
    /**/ {
        output << "# " << std::chrono::system_clock::now().time_since_epoch().count() << " #";
        output << description;
        output << std::endl;
        output << "|";
        output << std::setw(8) << std::string("user") << ", ";
        output << std::setw(8) << std::string("suborder") << ", ";
        output << std::setw(11) << std::string("qty_request") << ", ";
        output << std::setw(11) << std::string("unit_gramme") << ", ";
        output << std::endl;
    }
    for (const auto &var : container)
    {
        if (var == nullptr)
        {
            return;
        }
        if (var->m_qty_request_as_target > var->m_request_target.m_qty_leave)
        {
            return;
        }
        auto &suborder = var.operator*();
        output << "|";
        output << std::setw(8) << std::to_string(suborder.m_request_target.m_user_id) << ", ";
        output << std::setw(8) << suborder.m_suborder_id << ", ";
        output << std::setw(11) << suborder.m_qty_request_as_target << ", ";
        output << std::setw(11) << suborder.m_unit_gramme_as_target << ", ";
        output << std::endl;
    }
    /**/ {
        output << std::endl;
    }
}
