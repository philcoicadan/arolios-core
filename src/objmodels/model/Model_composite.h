/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <model/Element_action.h>
#include <util/String.h>
#include <model/Base_composite.h>

namespace arolios {





namespace model {


template < typename T1, typename T2, template<typename M1, typename M2> typename MOD> class Model_composite {
public:
	Model_composite(Base_element& p_parent ) ;
    virtual ~Model_composite();

    
    std::shared_ptr<MOD<T1,T2>> insert_child (const T1& p_type) ;
     
    
    bool check_action_compatibility( const T1& p_type) const ;

    


protected:

private:

    Base_element& parent_;

};
}

}

