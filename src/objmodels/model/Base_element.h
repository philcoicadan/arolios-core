/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <omm/Packageable_element.h>
#include <model/Element_action.h>
#include <omm/Element_attributes.h>

namespace arolios {



//Design Pattern: composite + command
namespace model {

class Visitor;

class Base_element {
public:
	Base_element( const std::string& p_std_name, const Element_action p_action=Element_action::a_create);
	virtual ~Base_element();
	virtual void execute () = 0;
    const std::string& standard_name () const { return std_name_;}
    const Element_action action() const { return action_ ;}
    virtual const std::shared_ptr<omm::Packageable_element> stored_element () const = 0;
    virtual void accept(Visitor& p_visitor) = 0;   

    std::shared_ptr<Base_element> find_child (const std::string& p_name) const;
    void add_child (const std::shared_ptr<Base_element>& p_elem) ;
    const std::vector<std::shared_ptr<Base_element>>& get_children () const { return vec_; }
    
protected:
    const std::string std_name_;
    const Element_action action_;
    std::vector<std::shared_ptr<Base_element>> vec_;
	std::unordered_map<std::string,std::shared_ptr<Base_element>> map_ ;
    
	virtual void fill_children () = 0;
    
    virtual void internal_execute (const omm::Element_attributes& p_attrs) = 0;
    
    void execute_children ();
   

    
private:
};

}

}

