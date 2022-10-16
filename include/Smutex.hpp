#include <shared_mutex>
#include <mutex>
//simple type def for shared and unique mutex
typedef std::shared_mutex Lock;
typedef std::unique_lock<Lock> WriteLock;
typedef std::shared_lock<Lock> ReadLock;