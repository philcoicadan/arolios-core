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
template < typename T1, typename T2> class Model_element_translation: public Model_element<T1,T2> {
public:
	Model_element_translation(const T1& p_elem,  const std::shared_ptr<T2>& p_stored, const Base_element& p_parent_element, const std::shared_ptr<const omm::Language>& p_lang);
	virtual ~Model_element_translation();
	virtual void execute () override;

    virtual void accept (Visitor& p_visitor) override ;
   

protected:

        void internal_execute ( const omm::Element_attributes& p_attrs) override ;
        
        virtual void fill_children () override ;
        
private:

    const std::shared_ptr<const omm::Language> language_;
    std::string translation_;
};
}

}

