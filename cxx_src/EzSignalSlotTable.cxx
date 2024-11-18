#include "ext/EzSignalSlotTable.h"
#pragma
#

namespace nscxx_root = ext;

///
auto                    //
    nscxx_root::        //
    EzSignalSlotTable:: //
    fx_register(TypeSlotFunction &&slot) -> TypeSignal
{
    auto &map = this->m_map_kSignal_vSlot;
    auto signal = ++this->m_seedOf_signal;
    map[signal] = std::move(slot);
    return signal;
}
auto                    //
    nscxx_root::        //
    EzSignalSlotTable:: //
    fx_unregister(TypeSignal signal) -> void
{
    auto &map = this->m_map_kSignal_vSlot;
    map.erase(signal);
}
auto                    //
    nscxx_root::        //
    EzSignalSlotTable:: //
    fx_emit(TypeSignal signal) -> bool
{
    auto &map = this->m_map_kSignal_vSlot;

    // reade
    if (not map.contains(signal))
    {
        return false;
    }
    auto is_stay_registered = map.at(signal)();
    if (is_stay_registered)
    {
        return true;
    }

    // write
    map.erase(signal);
    return false;
}
