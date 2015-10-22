//
//  Singleton.h
//  client
//
//  Created by C218-pc on 15/3/24.
//
//

#ifndef client_Singleton_h
#define client_Singleton_h

#include <cassert>

template<typename T>
class Singleton
{
public:
    Singleton()
    {
        assert(s_instance == NULL);
    }

    ~Singleton()
    {

    }

    static void initInstance()
    {
        assert(s_instance == NULL);
        s_instance = new T();
    }

    static void finiInstance()
    {
        if(s_instance != NULL)
        {
            delete s_instance;
            s_instance = NULL;
        }
    }

    static T * instance()
    {
        assert(s_instance != NULL);
        return s_instance;
    }

    static bool hasInstance()
    {
        return s_instance != NULL;
    }

private:
    static T * s_instance;
};

#define IMPLEMENT_SINGLETON(CLASS) \
    template<> CLASS * Singleton<CLASS>::s_instance = NULL

#endif
