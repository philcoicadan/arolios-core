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






namespace appcfg {
    class App;
}

namespace model {


template < typename T1, typename T2, typename T3, template<typename M1, typename M2, typename M3> typename MOD> class App_config_composite   {
public:
	App_config_composite(Base_element& p_parent, const std::shared_ptr<appcfg::App>& p_app ) ;
    virtual ~App_config_composite();

    
    virtual std::shared_ptr<MOD<T1,T2,T3>> insert_child (const T1& p_type, const std::string& p_name);
     
   


protected:
    
        Base_element& parent_;
        const std::shared_ptr<appcfg::App> app_;

private:



};
}

}

