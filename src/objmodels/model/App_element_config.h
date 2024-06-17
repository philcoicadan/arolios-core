/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <model/Model_element.h>

namespace arolios {


namespace appcfg {
    class App;
}

namespace model {
template < typename T1, typename T2, typename T3> class App_element_config: public Model_element<T1,T2> {
public:
	App_element_config(const T1& p_elem,  const std::shared_ptr<T2>& p_stored, const Base_element& p_parent_element, const std::shared_ptr<appcfg::App>& p_app);
	virtual ~App_element_config();
	virtual void execute () override;

    virtual void accept (Visitor& p_visitor) override ;
    
    const std::shared_ptr<T3> app_element() const  { return app_elt_; }


protected:

        void internal_execute ( const omm::Element_attributes& p_attrs) override ;
        
        virtual void fill_children () override ;
        
               
        
private:

    const std::shared_ptr<appcfg::App> app_;
    std::shared_ptr<T3> app_elt_;
};
}

}

