/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <model/Element_action.h>
#include <model/Base_element.h>
#include <model/Model_element.h>
#include <model/Model_element_create.h>
#include <model/Model_element_delete.h>
#include <model/Model_element_update.h>
#include <model/Model_element_translation.h>
#include <model/App_element_config.h>
#include <memory>

namespace arolios {


namespace model {

class Element_factory {
public:
    
    static Element_factory& instance();
    


template < typename T1, typename T2, typename T3> const std::shared_ptr<T2> make_create (const T1& p_xml_obj, const std::string& p_name,  const Base_element& p_parent) const  {


 return std::make_shared<Model_element_create<T1,T3>>(p_xml_obj, p_name, p_parent);
}

 
template < typename T1, typename T2, typename T3> const std::shared_ptr<T2> make_update (const T1& p_xml_obj, const std::shared_ptr<T3>& p_stored, const Base_element& p_parent) const  {
    
     return std::make_shared<Model_element_update<T1,T3>>(p_xml_obj, p_stored, p_parent);
}

template < typename T1, typename T2, typename T3> const std::shared_ptr<T2> make_delete (const T1& p_xml_obj, const std::shared_ptr<T3>& p_stored, const Base_element& p_parent) const {
    
     return std::make_shared<Model_element_delete<T1,T3>>(p_xml_obj, p_stored, p_parent);
}
    

template < typename T1, typename T2, typename T3> const std::shared_ptr<T2> make_translation (const T1& p_xml_obj, const std::shared_ptr<T3>& p_stored, const Base_element& p_parent, const std::shared_ptr<const omm::Language>& p_lang) const {
    
     return std::make_shared<Model_element_translation<T1,T3>>(p_xml_obj, p_stored, p_parent, p_lang);
}

template <typename T1, typename T2, typename T3 , typename T4> const std::shared_ptr<T2>  make_element_config  (const T1& p_xml_obj, const std::shared_ptr<T3>& p_stored,  const Base_element& p_parent, const std::shared_ptr<appcfg::App>& p_app ) const {
     
    return std::make_shared<App_element_config<T1,T3,T4 >>(p_xml_obj, p_stored, p_parent, p_app);
}
private:

    /**
     * Constructor
     */
    Element_factory();

    /**
     * Destructor
     */
    ~Element_factory();

};
}
}

