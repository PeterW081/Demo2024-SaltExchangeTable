#pragma once
#
#include <thread>
#include <shared_mutex>
#

namespace ext::multithread
{
/// https://stackoverflow.com/questions/36619715/a-shared-recursive-mutex-in-standard-c/36624355#36624355
class MutexSharedRecursive;
}

class                    //
    ext::multithread::   //
    MutexSharedRecursive //
    : public std::shared_mutex
{
    std::atomic<std::thread::id> m_owner_thread_id;
    std::atomic<std::uint16_t> m_owner_lock_times;

  public: // overwrite
    auto lock() -> void;
    auto unlock() -> void;
    auto try_lock() -> bool;

  public:
    static auto FX_makerSafety_std_shared_lock(MutexSharedRecursive &) -> std::unique_ptr<std::shared_lock<MutexSharedRecursive>>;
    void fx_hook_post_normal_lock();
    void fx_hook_prep_normal_unlock();
    void fx_hook_post_recursive_lock();
    void fx_hook_prep_recursive_unlock();
};
