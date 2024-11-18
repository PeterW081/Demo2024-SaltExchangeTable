#pragma once
#
#include <map>
#include <shared_mutex>
#include <functional>
#include <atomic>
#

namespace ext::inline thread_unsafety
{
class EzSignalSlotTable;
}

class                 //
    ext::             //
    EzSignalSlotTable //
{
  public:
    using TypeSignal = std::uint64_t;
    using TypeSlotFunction = std::function<auto()->bool>; // fx() -> is_stay_registered

  private:
    std::atomic<TypeSignal> m_seedOf_signal = 0;
    std::map<TypeSignal, TypeSlotFunction> m_map_kSignal_vSlot;

  public:
    auto fx_register(TypeSlotFunction &&) -> TypeSignal; // fx(slot,) -> signal
    auto fx_unregister(TypeSignal) -> void;              // fx(signal,)
    auto fx_emit(TypeSignal) -> bool;                    // fx(signal,) -> is_stay_registered
};
