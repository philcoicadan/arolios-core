/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <model/Base_element.h>

namespace arolios {




namespace model {
template < typename T1, typename T2> class Model_element: public Base_element {
public:
	Model_element(const T1& p_elem, const std::string& p_std_name, const std::shared_ptr<T2>& p_stored, const Base_element& p_parent_element, const Element_action p_action);
	virtual ~Model_element();
    const std::shared_ptr<T2>& stored() const ;
    void stored( const std::shared_ptr<T2>& p_elt) ;
    virtual const std::shared_ptr<omm::Packageable_element> stored_element () const override;
    const T1& xml_object() const ;
    const Base_element& parent () const;


    virtual void execute () override ;
protected:
	virtual void fill_children () override ;
    
    const T1& xml_obj_;
    std::shared_ptr<T2> stored_;
    const Base_element& parent_element_;

private:


};
}

}

