/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Package.h"
#include "Class.h"
#include "Data_type.h"
#include "Composite_type.h"
#include "Enumeration.h"
#include "Association.h"
#include "Property.h"
#include "Constraint.h"
#include "Generalization.h"
#include "Logical_schema.h"
#include "Table.h"
#include "Logical_enum.h"
#include "Logical_type.h"
#include "Column.h"
#include "Attribute.h"
#include "Column_constraint.h"
#include "Property_by_sub_field.h"
#include "Enum_value.h"
#include "Language.h"
#include "Element_attributes.h"
#include "util/Native_type.h"

namespace arolios {



namespace omm
{

class Specific_type;
class Collection_attributes;

class Model_factory
{
public:
    

    static Model_factory& instance();
    
    template <typename T1>  std::shared_ptr<T1> make(const std::shared_ptr<omm::Persistent_object>& p_pers_elem, const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string() );
    
    
    virtual std::shared_ptr<omm::Package> make_package(const std::string& p_name, const std::shared_ptr<omm::Package>& p_nesting = nullptr, const omm::Lc_status p_status = omm::Lc_status::active , const std::string& p_desc = std::string());
	virtual std::shared_ptr<omm::Class> make_class(const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const bool p_is_abstract = false, const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string() );
	virtual std::shared_ptr<omm::Composite_type> make_composite_type(const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string());
	virtual std::shared_ptr<omm::Primitive_type> make_primitive_type(const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const util::Native_type& p_native_type, const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string());
	virtual std::shared_ptr<omm::Enumeration> make_enumeration(const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string());
	virtual std::shared_ptr<omm::Association> make_association(const std::string& p_name,const std::shared_ptr<omm::Package>& p_package, const bool p_is_abstract = false, const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string());
	virtual std::shared_ptr<omm::Property> make_property(const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_classifier, const std::shared_ptr<const omm::Classifier>& p_type,
			const int p_min_occurs = 0, const int p_max_occurs = 1, const std::string& p_computed = "", const std::string& p_default = "",  /*const bool p_part_of_id = false,*/ const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string());
	virtual std::shared_ptr<omm::Property> make_member_end(const std::string& p_name,const std::shared_ptr<omm::Association>& p_assoc, const std::shared_ptr<const omm::Classifier>& p_target, const std::shared_ptr<omm::Classifier>& p_source,
			const int p_min = 0, const int p_max = -1, const bool p_is_src_class_property = true, const bool p_is_composition = false,  const bool p_is_navigable = true ,/*const bool p_part_of_id = false,*/ const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string());
	virtual std::shared_ptr<omm::Constraint> make_constraint( const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_classifier, const std::string& p_expression, const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string() );
	virtual std::shared_ptr<omm::Specific_type> make_specific_type( const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const std::shared_ptr<omm::Classifier>& p_referenced_type, const std::shared_ptr<omm::Collection_attributes>& p_collection = nullptr, const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string() );
	virtual std::shared_ptr<omm::Selection_type> make_selection_type( const std::string& p_name, const std::shared_ptr<omm::Package>& p_package , const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string() );
	virtual std::shared_ptr<omm::Generalization> make_generalization(const std::shared_ptr<omm::Classifier>& p_general, const std::shared_ptr<omm::Classifier>& p_specific);
	std::shared_ptr<omm::Persistent_object> make_persistent_object(const std::string& p_id, const std::string& p_scope_id);
 
    virtual std::shared_ptr<omm::Logical_schema> make_logical_schema(const std::shared_ptr<omm::Package>& p_domain, const std::string& p_name );
    virtual std::shared_ptr<omm::Logical_enum> make_logical_enum(const std::shared_ptr<omm::Enumeration>& p_enum, const std::string& p_name );
    virtual std::shared_ptr<omm::Logical_type> make_logical_type(const std::shared_ptr<omm::Data_type>& p_type, const std::string& p_name );
    virtual std::shared_ptr<omm::Table> make_table(const std::shared_ptr<omm::Class>& p_class, const std::string& p_name );
    virtual std::shared_ptr<omm::Table> make_table(const std::shared_ptr<omm::Association>& p_assoc, const std::string& p_name );
    virtual std::shared_ptr<omm::Column> make_column(const std::shared_ptr<omm::Property>& p_prop, const std::string& p_name, const std::shared_ptr<omm::Table>& p_table, const int p_native_type=-1 );
    virtual std::shared_ptr<omm::Attribute> make_attribute(const std::shared_ptr<omm::Property>& p_prop, const std::string& p_name,  const int p_native_type=-1  );
    virtual std::shared_ptr<omm::Column_constraint> make_column_constraint(const std::shared_ptr<omm::Column>& p_column, const omm::Column_constraint::Type p_type );
    virtual std::shared_ptr<omm::Property_by_sub_field> make_property_by_sub_field(const std::shared_ptr<omm::Property>& p_result_prop, const std::shared_ptr<omm::Property>& p_filter_prop) ;
    virtual std::shared_ptr<omm::Property_by_sub_field> make_property_by_sub_field(const std::shared_ptr<omm::Property>& p_result_prop, const std::shared_ptr<omm::Column>& p_fk_column) ;
    virtual std::shared_ptr<omm::Property_by_sub_field> make_property_by_sub_field(const std::shared_ptr<omm::Property>& p_result_prop, const std::shared_ptr<omm::Table>& p_filter_table, const std::string& p_filter_column) ;
    virtual std::shared_ptr<omm::Enum_value> make_enum_value (const std::string& p_value, const std::shared_ptr<omm::Enumeration>& p_enum, const omm::Lc_status p_status = omm::Lc_status::active);
    virtual std::shared_ptr<omm::Language> make_language( const std::string& p_code, const std::string& p_name, const std::string& p_fts_config, const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string() );
    
    template <typename T1>  void update(const std::shared_ptr<T1>& p_elem, const std::string& p_new_name,  const std::string& p_new_desc , const omm::Element_attributes& p_attrs, const omm::Lc_status p_status = omm::Lc_status::active  );
    

private:

	Model_factory();
	~Model_factory();
	virtual void package_element_attributes (const std::shared_ptr<omm::Classifier>& p_elem, const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string());
	virtual void property_attributes (const std::shared_ptr<omm::Property>& p_prop, const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_classifier, const std::shared_ptr<const omm::Classifier>& p_type,
			const int p_min_occurs = 0, const int p_max_occurs = 1, const std::string& p_computed = "", const std::string& p_default = "", /* const bool p_part_of_id = false,*/ const omm::Lc_status p_status = omm::Lc_status::active, const std::string& p_desc = std::string());

    virtual void specific_attributes_update (const std::shared_ptr<omm::Packageable_element>& p_elem, const omm::Element_attributes& p_attrs) ;
    
    virtual void specific_attributes_update (const std::shared_ptr<omm::Class>& p_elem, const omm::Element_attributes& p_attrs) ;
    virtual void specific_attributes_update (const std::shared_ptr<omm::Association>& p_elem, const omm::Element_attributes& p_attrs) ;
        
    virtual void specific_attributes_update (const std::shared_ptr<omm::Property>& p_elem, const omm::Element_attributes& p_attrs) ;
    virtual void specific_attributes_update (const std::shared_ptr<omm::Primitive_type>& p_elem, const omm::Element_attributes& p_attrs) ;
    virtual void specific_attributes_update (const std::shared_ptr<omm::Constraint>& p_elem, const omm::Element_attributes& p_attrs) ;


};

}


}

