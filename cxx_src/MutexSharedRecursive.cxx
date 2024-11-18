#include "ext/MutexSharedRecursive.h"
#pragma
#

namespace nscxx_root = ext::multithread;

/**
/// function - overwrite
/// function - normal
**/

/// function - overwrite
auto                       //
    nscxx_root::           //
    MutexSharedRecursive:: //
    lock() -> void
{
    if (this->m_owner_thread_id != std::this_thread::get_id())
    {
        // normal lock
        this->std::shared_mutex::lock();
        this->fx_hook_post_normal_lock();
    }
    else
    {
        // recursive lock
        ((void)"this->fx_recursive_lock"); // NOOP
        this->fx_hook_post_recursive_lock();
    }
}
auto                       //
    nscxx_root::           //
    MutexSharedRecursive:: //
    unlock() -> void
{
    if (this->m_owner_thread_id != std::this_thread::get_id())
    {
        return;
    }
    if (this->m_owner_lock_times <= 1)
    {
        // normal unlock
        this->fx_hook_prep_normal_unlock();
        this->std::shared_mutex::unlock();
    }
    else
    {
        // recursive unlock
        this->fx_hook_prep_recursive_unlock();
    }
}
auto                       //
    nscxx_root::           //
    MutexSharedRecursive:: //
    try_lock() -> bool
{
    auto is_try_lock_success = bool();
    if (this->std::shared_mutex::try_lock())
    {
        // normal lock
        this->fx_hook_post_normal_lock();
        is_try_lock_success = true;
    }
    else if (this->m_owner_thread_id == std::this_thread::get_id())
    {
        // recursive lock
        this->fx_hook_post_recursive_lock();
        is_try_lock_success = true;
    }
    else
    {
        // no lock
        is_try_lock_success = false;
    }
    return is_try_lock_success;
}

/// function - normal
auto                       //
    nscxx_root::           //
    MutexSharedRecursive:: //
    FX_makerSafety_std_shared_lock(MutexSharedRecursive &mutex) -> std::unique_ptr<std::shared_lock<MutexSharedRecursive>>
{
    if (std::this_thread::get_id() == mutex.m_owner_thread_id)
    {
        return {};
    }
    else
    {
        return std::make_unique<std::shared_lock<std::remove_reference_t<decltype(mutex)>>>(mutex);
    }
}
auto                       //
    nscxx_root::           //
    MutexSharedRecursive:: //
    fx_hook_post_normal_lock() -> void
{
    this->m_owner_thread_id = std::this_thread::get_id();
    this->m_owner_lock_times = 1;
}
auto                       //
    nscxx_root::           //
    MutexSharedRecursive:: //
    fx_hook_prep_normal_unlock() -> void
{
    this->m_owner_thread_id = std::thread::id();
    this->m_owner_lock_times = 0;
}
auto                       //
    nscxx_root::           //
    MutexSharedRecursive:: //
    fx_hook_post_recursive_lock() -> void
{
    this->m_owner_lock_times++;
    if (this->m_owner_lock_times == 0)
    {
        throw std::overflow_error(std::string() + __PRETTY_FUNCTION__ + " # Mutex lock recursively too many times.");
    }
}
auto                       //
    nscxx_root::           //
    MutexSharedRecursive:: //
    fx_hook_prep_recursive_unlock() -> void
{
    if (this->m_owner_lock_times >= 1)
    {
        this->m_owner_lock_times--;
    }
}
