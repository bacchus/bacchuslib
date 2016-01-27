#pragma once

#include <unistd.h>
#include <exception>
#include <pthread.h>

#define BCC_DISABLE_COPY(classname) \
    classname(const classname&);    \
    classname& operator=(const classname&);

#define bcc_thread_local __thread

namespace bacchus {

class Mutex {
public:
    Mutex() {
        pthread_mutex_init(&mHandle, NULL);
    }

    ~Mutex() {
        pthread_mutex_destroy(&mHandle);
    }

    inline void lock() {
        pthread_mutex_lock(&mHandle);
    }

    inline bool trylock() {
        return !pthread_mutex_trylock(&mHandle);
    }

    inline void unlock() {
        pthread_mutex_unlock(&mHandle);
    }

    BCC_DISABLE_COPY(Mutex)

private:
    pthread_mutex_t mHandle;
};

class LockGuard {
public:
    explicit LockGuard(Mutex& mutex)
        : mMutex(mutex)
    {
        mMutex.lock();
    }

    ~LockGuard() {
        mMutex.unlock();
    }

    BCC_DISABLE_COPY(LockGuard)

private:
    Mutex& mMutex;
};

template <class T>
class LockPtr {
public:
    LockPtr(volatile T& obj, Mutex& mutex)
        : mObj(const_cast<T*>(&obj)), mMutex(&mutex)
    {
        mMutex->lock();
    }

    ~LockPtr() {
        mMutex->unlock();
    }

    T& operator *() {
        return *mObj;
    }

    T* operator ->() {
        return mObj;
    }

    BCC_DISABLE_COPY(LockPtr)

private:
    T* mObj;
    Mutex* mMutex;
};

class Thread {
public:
    Thread(void(*func)(void*), void* arg) {
        LockGuard guard(mDataMutex);
        FuncInfo* fi = new FuncInfo;
        fi->mFunc = func;
        fi->mArg = arg;
        fi->mThread = this;
        mNotThread = false;
        if (pthread_create(&mHandle, NULL, wrap, fi) != 0) {
            mHandle = 0;
            mNotThread = true;
            delete fi;
        }
    }

    ~Thread() {
        if (joinable())
            std::terminate();
    }

    inline void join() {
        if (joinable())
            pthread_join(mHandle, NULL);
    }

    inline bool joinable() const {
        LockGuard guard(mDataMutex);
        bool res = !mNotThread;
        return res;
    }

    inline void detach() {
        LockGuard guard(mDataMutex);
        if (!mNotThread) {
            pthread_detach(mHandle);
            mNotThread = true;
        }
    }

    static unsigned int id() {
        return (unsigned int)pthread_self();
    }

    static unsigned int procnum() {
#if defined(_SC_NPROCESSORS_ONLN)
        return (unsigned int)sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(_SC_NPROC_ONLN)
        return (unsigned int)sysconf(_SC_NPROC_ONLN);
#else
        return 0;
#endif
    }

    BCC_DISABLE_COPY(Thread)

private:
    pthread_t mHandle;
    mutable Mutex mDataMutex;
    bool mNotThread;

    struct FuncInfo {
        void (*mFunc)(void*);
        void* mArg;
        Thread* mThread;
    };

    static void* wrap(void* info) {
        FuncInfo* fi = reinterpret_cast<FuncInfo*>(info);
        try {
            fi->mFunc(fi->mArg);
        } catch (...) {
            std::terminate();
        }
        LockGuard guard(fi->mThread->mDataMutex);
        fi->mThread->mNotThread = true;
        delete fi;
        return NULL;
    }
};

} // namespace bacchus
