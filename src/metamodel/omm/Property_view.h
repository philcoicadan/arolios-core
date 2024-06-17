/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Property.h"

namespace arolios {


namespace omm {
    
class Property_view {
public:
    
    enum Metatype { _primitive = 0, _enumeration = 1, _composite_type = 2, _class = 3} ;
    
	Property_view(const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_id_name);
	virtual ~Property_view();

    virtual const std::string&  id_name() const;

    virtual void metatype (const Metatype p_metatype) ;
    virtual const Metatype metatype () const;
    virtual void is_inherited (const bool p_is_inherited) ;
    virtual const bool is_inherited () const;
    virtual const std::shared_ptr<const omm::Property>& property () const;
    virtual const std::vector<std::shared_ptr<const omm::Property_view>>& nested_property_views () const ;
    virtual const std::vector<std::shared_ptr<const omm::Property_view>>& linked_property_views ()const ;
    virtual const std::vector<std::shared_ptr<const omm::Property_view>>& associated_property_views () const ;
    virtual void insert_linked_property_view (const std::shared_ptr<const Property_view>& p_prop_v);
    virtual void insert_nested_property_view (const std::shared_ptr<const Property_view>& p_prop_v);
    virtual void insert_associated_property_view( const std::shared_ptr<const Property_view>& p_prop_v);
    

    
    virtual const std::string& metatype_to_string () const ;
   
private:
	const std::shared_ptr<const omm::Property> prop_;
    std::vector<std::shared_ptr<const omm::Property_view>> nested_prop_views_;
    std::vector<std::shared_ptr<const omm::Property_view>> linked_prop_views_;
    std::vector<std::shared_ptr<const omm::Property_view>> associated_prop_views_;
    std::string id_name_;

    Metatype metatype_;
    bool is_inherited_;

};

}
}

