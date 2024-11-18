#include "def_project/SaltExchangeTable.h"
#pragma
#

namespace nscxx_root = def_project;

///
auto                                       //
    nscxx_root::                           //
    SaltExchangeTable::ManagerSignalSlot:: //
    fx_register(TypeSignalLike_With_OrderId signale_like, TypeSlotFunction &&slot_function) -> TypeSignal
{
    auto signal = this->ext::EzSignalSlotTable::fx_register(std::move(slot_function));
    this->m_mapRe_signal_with_order_id[signale_like] = signal;
    return signal;
}
auto                                       //
    nscxx_root::                           //
    SaltExchangeTable::ManagerSignalSlot:: //
    fx_unregister(TypeSignalLike_With_OrderId signale_like) -> void
{
    auto &container = this->m_mapRe_signal_with_order_id;
    if (not container.contains(signale_like))
    {
        return;
    }
    auto signal = container.at(signale_like); /**/
    container.erase(signale_like);
    this->ext::EzSignalSlotTable::fx_unregister(signal);
}
auto                                       //
    nscxx_root::                           //
    SaltExchangeTable::ManagerSignalSlot:: //
    fx_emit(TypeSignal signal) -> void
{
    auto &container = this->m_mapRe_signal_with_order_id;
    auto is_stay_registered = this->ext::EzSignalSlotTable::fx_emit(signal);
    if (is_stay_registered)
    {
        return;
    }
    auto func = [&signal](std::remove_reference_t<decltype(container)>::value_type var) -> bool { return signal == var.second; };
    std::erase_if(container, func);
}
auto                                       //
    nscxx_root::                           //
    SaltExchangeTable::ManagerSignalSlot:: //
    fx_emit(TypeSignalLike_With_OrderId signale_like) -> void
{
    auto &container = this->m_mapRe_signal_with_order_id;
    if (not container.contains(signale_like))
    {
        return;
    }
    auto signal = container.at(signale_like); /**/
    auto is_stay_registered = this->ext::EzSignalSlotTable::fx_emit(signal);
    if (is_stay_registered)
    {
        return;
    }
    auto func = [&signal](std::remove_reference_t<decltype(container)>::value_type var) -> bool { return signal == var.second; };
    std::erase_if(container, func);
}
