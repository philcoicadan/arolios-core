/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>

namespace arolios {




namespace common {

   
    
template <typename T > class Singleton  {
public:
    
    
    static Singleton<T> & instance();

    std::shared_ptr<T> object() const ;
    void object (T* p_object);
    void object (const std::shared_ptr<T>& p_object);
    


private:


    /**
     * Constructor
     */
    Singleton();
    
    /**
     * Destructor
     */
    virtual ~Singleton();

    
private:

    std::shared_ptr<T> obj_;  // the lifecycle of this object must be almost identical to the application

    
};

}
}

