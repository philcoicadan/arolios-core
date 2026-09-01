/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Visitor.h>
#include <omm/Association.h>
#include <omm/Class.h>
#include <omm/Data_type.h>
#include <omm/Generalization.h>
#include <omm/Package.h>
#include <omm/Property.h>
#include <omm/Constraint.h>
#include <omm/Enumeration.h>
#include <omm/Table.h>
#include <omm/Logical_enum.h>
#include <omm/Logical_schema.h>
#include <omm/Logical_type.h>
#include <omm/Column.h>
#include <omm/Attribute.h>
#include <omm/Property_by_sub_field.h>
#include <omm/Enum_value.h>
#include <omm/Language.h>
#include <omm/Restriction_type.h>
#include <omm/Collection_type.h>
#include <omm/Select_type.h>
#include <omm/Primitive_type.h>
#include <omm/Composite_type.h>
#include <iostream>
#include <typeinfo>


using namespace arolios;
using namespace arolios::omm;

void Visitor::visit( const Package& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Association& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Class& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Enumeration& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Data_type& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Property& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Constraint& p_elem)  { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Generalization& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Logical_schema& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Table& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Logical_type& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Logical_enum& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Column& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Attribute& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Property_by_sub_field& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Enum_value& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Language& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Restriction_type& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Collection_type& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Select_type& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Primitive_type& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
void Visitor::visit( const Composite_type& p_elem) { std::cout << "Visitor " << typeid(*this).name() << " not implemented for " << typeid(p_elem).name() << std::endl ; }
