/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <model/Base_element.h>

namespace arolios {






namespace model {


template < typename T1, typename T2, template<typename M1, typename M2> typename MOD> class Base_composite   {
public:
	Base_composite(Base_element& p_parent ) ;
    virtual ~Base_composite();

    
    virtual std::shared_ptr<MOD<T1,T2>> insert_child (const T1& p_type) ;
     
   


protected:
    
        Base_element& parent_;

private:



};
}

}

