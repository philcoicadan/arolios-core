/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <model/Model_element.h>
#include <omm/Element_attributes.h>

namespace arolios {



namespace model {
template < typename T1, typename T2>  class Model_element_create: public Model_element<T1, T2> {
public:
	Model_element_create(const T1& p_elem, const std::string& p_std_name, const Base_element& p_parent);
	virtual ~Model_element_create();
	virtual void execute () override;
    virtual void accept (Visitor& p_visitor) override ;


protected:
    
    void internal_execute ( const omm::Element_attributes& p_attrs) override ;

private:

};
}

}

