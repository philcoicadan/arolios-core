/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <common/Singleton.h>
#include <exception/Exceptions.h>


using namespace arolios;
using namespace arolios::common;

template <typename T> Singleton<T>::Singleton () 
{
    obj_ = nullptr;
}

template <typename T> Singleton<T>::~Singleton () {}

template <typename T> void Singleton<T>::object (const std::shared_ptr<T>& p_object) 
{
    obj_ = p_object;
}

template <typename T> void Singleton<T>::object (T* p_object) 
{
    std::shared_ptr<T> shared (p_object);
    object(shared);
}

template <typename T> std::shared_ptr<T> Singleton<T>::object () const
{
    if (obj_ == nullptr) {
        throw exception::Failure ("Shared object for User Access Control not initialized");
    }
    return obj_;
}

template <typename T> Singleton<T>& Singleton<T>::instance ()
{
    static Singleton<T> instance;
    return instance;
}