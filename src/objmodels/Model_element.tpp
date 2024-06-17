/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include <model/Model_recorder.h>
#include <util/String.h>
#include <omm/Model_factory.h>
#include <omm/Persistent_object.h>







using namespace arolios;
using namespace arolios::model;

template <typename T1, typename T2 > Model_element<T1,T2 >::Model_element(const T1& p_elem, const std::string& p_std_name,  const std::shared_ptr<T2>& p_stored, const Base_element& p_parent, const Element_action p_action): Base_element (p_std_name, p_action), xml_obj_ (p_elem), stored_ (p_stored), parent_element_ (p_parent) {


	}


template <typename T1, typename T2 > Model_element<T1,T2 >::~Model_element() {
}



template < typename T1, typename T2  > const T1& Model_element<T1,T2 >::xml_object () const {
    return xml_obj_;
}


template < typename T1, typename T2  > const std::shared_ptr<T2>& Model_element<T1,T2 >::stored () const {
    return stored_;
}

template < typename T1, typename T2  > const std::shared_ptr<omm::Packageable_element> Model_element<T1,T2 >::stored_element () const {
    return stored_;
}

template < typename T1, typename T2  >   void Model_element<T1,T2 >::stored( const std::shared_ptr<T2>& p_elt) {
    stored_ = p_elt;
}
template < typename T1, typename T2  > const Base_element& Model_element<T1,T2 >::parent () const {
    return parent_element_;
}


template < typename T1, typename T2  > void Model_element<T1,T2 >::execute ()  {
    omm::Element_attributes attrs;
    
    this->internal_execute(attrs);
    
	this->execute_children();
}



template < typename T1, typename T2  > void Model_element<T1,T2 >::fill_children ()  {
//  default: nothing to do
}
