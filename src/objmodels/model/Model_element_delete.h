/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <model/Model_element.h>

namespace arolios {



namespace model {
template < typename T1, typename T2 > class Model_element_delete: public Model_element<T1,T2> {
public:
	Model_element_delete(const T1& p_enum, const std::shared_ptr<T2>& p_stored, const Base_element& p_parent_element);
	virtual ~Model_element_delete();
	virtual void execute () override;
    virtual void accept (Visitor& p_visitor) override ;
    
    bool has_children () const;


protected:

        void internal_execute ( const omm::Element_attributes& p_attrs) override ;

private:


};
}

}

