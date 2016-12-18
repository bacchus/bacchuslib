#pragma once

namespace android {

template <typename TYPE>
class Singleton {
public:
    static TYPE& getInstance() {
        TYPE* instance = sInstance;
        if (instance == 0) {
            instance = new TYPE();
            sInstance = instance;
        }
        return *instance;
    }

    static bool hasInstance() {
        return sInstance != 0;
    }

protected:
    ~Singleton() { }
    Singleton() { }

private:
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);
    static TYPE* sInstance;
};

#define ANDROID_SINGLETON_STATIC_INSTANCE(TYPE)                 \
    template<> TYPE* Singleton< TYPE >::sInstance(0);           \
    template class Singleton< TYPE >;

} // namespace android
