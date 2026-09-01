/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include "util/Native_type.h"
#include <omm/Model_factory.h>
#include <omm/Column.h>
#include <omm/Class_attributes.h>
#include <omm/Property_attributes.h>
#include <omm/Primitive_attributes.h>
#include <omm/Primitive_type.h>
#include <iostream>




using namespace arolios;
using namespace arolios::omm;

Model_factory::Model_factory()
{

}



Model_factory::~Model_factory()
{

}

Model_factory& Model_factory::instance() {
    static Model_factory inst = Model_factory();
    return inst;
}


template <>  std::shared_ptr<omm::Package> Model_factory::make<omm::Package>(const std::shared_ptr<omm::Persistent_object>& p_pers_elem, const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status, const std::string& p_desc ) {
    
    auto parent = std::dynamic_pointer_cast<omm::Package>(p_parent);
    
    auto result = make_package (p_name, parent, p_status, p_desc);
    result->persistent_attributes (p_pers_elem->id(), p_pers_elem->id_scope());
    return result;
}

template <>  std::shared_ptr<omm::Class> Model_factory::make<omm::Class>(const std::shared_ptr<omm::Persistent_object>& p_pers_elem, const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status, const std::string& p_desc ) {
    
    const omm::Class_attributes& attrs =dynamic_cast<const omm::Class_attributes&> (p_attrs);
    auto parent = std::dynamic_pointer_cast<omm::Package>(p_parent);

    auto result = make_class (p_name, parent, attrs.is_abstract_,  p_status, p_desc);
    if (attrs.base_ != nullptr) {
		std::shared_ptr<omm::Generalization> cur_general = omm::Model_factory::instance().make_generalization(attrs.base_, result);
    }
    result->persistent_attributes (p_pers_elem->id(), p_pers_elem->id_scope());
    return result;
}

template <>  std::shared_ptr<omm::Composite_type> Model_factory::make<omm::Composite_type>(const std::shared_ptr<omm::Persistent_object>& p_pers_elem, const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status, const std::string& p_desc ) {
    
    auto parent = std::dynamic_pointer_cast<omm::Package>(p_parent);
    
    auto result = make_composite_type (p_name, parent, p_status, p_desc);
    result->persistent_attributes (p_pers_elem->id(), p_pers_elem->id_scope());
    return result;
}

template <>  std::shared_ptr<omm::Primitive_type> Model_factory::make<omm::Primitive_type>(const std::shared_ptr<omm::Persistent_object>& p_pers_elem, const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status, const std::string& p_desc ) {
    
    auto parent = std::dynamic_pointer_cast<omm::Package>(p_parent);
    const omm::Primitive_attributes& attrs =dynamic_cast<const omm::Primitive_attributes&> (p_attrs);

    auto result = make_primitive_type (p_name, parent, attrs.native_type_, p_status, p_desc);
    result->persistent_attributes (p_pers_elem->id(), p_pers_elem->id_scope());
    return result;
}


template <>  std::shared_ptr<omm::Enumeration> Model_factory::make<omm::Enumeration>(const std::shared_ptr<omm::Persistent_object>& p_pers_elem, const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status, const std::string& p_desc ) {
    
    auto parent = std::dynamic_pointer_cast<omm::Package>(p_parent);
    
    auto result = make_enumeration (p_name, parent, p_status, p_desc);
    result->persistent_attributes (p_pers_elem->id(), p_pers_elem->id_scope());
    return result;
}

template <>  std::shared_ptr<omm::Association> Model_factory::make<omm::Association>(const std::shared_ptr<omm::Persistent_object>& p_pers_elem, const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status, const std::string& p_desc ) {
    
    auto parent = std::dynamic_pointer_cast<omm::Package>(p_parent);
    
    auto result = make_association (p_name, parent, p_status, p_desc);
    result->persistent_attributes (p_pers_elem->id(), p_pers_elem->id_scope());
    return result;
}


template <>  std::shared_ptr<omm::Property> Model_factory::make<omm::Property>(const std::shared_ptr<omm::Persistent_object>& p_pers_elem, const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status, const std::string& p_desc ) {
    
    
    const omm::Property_attributes& attrs =dynamic_cast<const omm::Property_attributes&> (p_attrs);

    
    std::shared_ptr<omm::Property> result;
    
    if (attrs.source_member_end_) {
        const auto target = std::dynamic_pointer_cast<omm::Class>(attrs.type_);
        const auto parent = std::dynamic_pointer_cast<omm::Association>(p_parent);
        result = make_member_end ( p_name, parent, target, attrs.source_member_end_, attrs.min_, attrs.max_, attrs.is_scp_, (attrs.aggregation_ == omm::Aggregation_kind::composite) , /*attrs.part_of_id_,*/ p_status, p_desc);
        
    } else {
        const auto parent = std::dynamic_pointer_cast<omm::Classifier>(p_parent);

        result = make_property (p_name, parent, attrs.type_, attrs.min_, attrs.max_, std::string(), std::string(), /*attrs.part_of_id_,*/ p_status, p_desc);
    }
    
    result->persistent_attributes (p_pers_elem->id(), p_pers_elem->id_scope());
    return result;
}

template <>  std::shared_ptr<omm::Enum_value> Model_factory::make<omm::Enum_value>(const std::shared_ptr<omm::Persistent_object>& p_pers_elem, const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status, const std::string& p_desc ) {
    
    auto parent = std::dynamic_pointer_cast<omm::Enumeration>(p_parent);
    
    auto result = make_enum_value (p_name, parent, p_status);
    result->persistent_attributes (p_pers_elem->id(), p_pers_elem->id_scope());
    return result;
}


std::shared_ptr<omm::Package> Model_factory::make_package(const std::string& p_name, const std::shared_ptr<omm::Package>& p_nesting, const omm::Lc_status p_status, const std::string& p_desc) {
	std::shared_ptr<omm::Package> result = std::make_shared<omm::Package>();
	result->name(p_name);
    result->lc_status(p_status);
	result->description (p_desc);
	if (p_nesting) {
		result->nesting(p_nesting);
		p_nesting->insert<omm::Package>(result);
	}
	return result;
}

std::shared_ptr<omm::Class> Model_factory::make_class(const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const bool p_is_abstract, const omm::Lc_status p_status, const std::string& p_desc ) {
	std::shared_ptr<omm::Class> result = std::make_shared<omm::Class>(p_is_abstract);
	package_element_attributes ( result, p_name, p_package , p_status, p_desc);
	p_package->insert<omm::Class>(result);
	return result;
}

std::shared_ptr<omm::Composite_type> Model_factory::make_composite_type(const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const omm::Lc_status p_status, const std::string& p_desc ) {
	std::shared_ptr<omm::Composite_type> result = std::make_shared<omm::Composite_type>();
	package_element_attributes ( result, p_name, p_package,p_status, p_desc );
	p_package->insert<omm::Composite_type>(result);
    return result;
}

std::shared_ptr<omm::Primitive_type> Model_factory::make_primitive_type(const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const util::Native_type& p_native_type, const omm::Lc_status p_status, const std::string& p_desc ) {
    std::shared_ptr<omm::Primitive_type> result = std::make_shared<omm::Primitive_type>();
	package_element_attributes ( result, p_name, p_package,p_status, p_desc );
    result->native_type(p_native_type);
	p_package->insert<omm::Primitive_type>(result);
    return result;

}

std::shared_ptr<omm::Enumeration> Model_factory::make_enumeration(const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const omm::Lc_status p_status, const std::string& p_desc ) {
	std::shared_ptr<omm::Enumeration> result = std::make_shared<omm::Enumeration>();
	package_element_attributes ( result, p_name, p_package,p_status, p_desc);
	p_package->insert<omm::Enumeration>(result);
	return result;
}
std::shared_ptr<omm::Association> Model_factory::make_association(const std::string& p_name,const std::shared_ptr<omm::Package>& p_package, const omm::Lc_status p_status, const std::string& p_desc ) {
	std::shared_ptr<omm::Association> result = std::make_shared<omm::Association>();
	package_element_attributes ( result, p_name, p_package,p_status, p_desc);
	p_package->insert<omm::Association>(result);
	return result;
}
std::shared_ptr<omm::Property> Model_factory::make_property(const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_classifier, const std::shared_ptr<const omm::Classifier>& p_type,
			const int p_min_occurs, const int p_max_occurs, const std::string& p_computed, const std::string& p_default,  /*const bool p_part_of_id,*/ const omm::Lc_status p_status, const std::string& p_desc ) {

	std::shared_ptr<omm::Property> result = std::make_shared<omm::Property>();
	property_attributes ( result, p_name, p_classifier, p_type, p_min_occurs, p_max_occurs, p_computed, p_default, /*p_part_of_id,*/ p_status, p_desc);
    return result;
}


std::shared_ptr<omm::Property> Model_factory::make_member_end(const std::string& p_name,const std::shared_ptr<omm::Association>& p_assoc, const std::shared_ptr<const omm::Class>& p_target, const std::shared_ptr<omm::Class>& p_source,
			const int p_min, const int p_max, const bool p_is_src_class_property, const bool p_is_composition, /*const bool p_part_of_id,*/ const omm::Lc_status p_status, const std::string& p_desc ) {


	std::shared_ptr<omm::Property> result = std::make_shared<omm::Property>();
	std::shared_ptr<omm::Classifier> l_classifier =  p_is_src_class_property ? std::static_pointer_cast<omm::Classifier>(p_source) : std::static_pointer_cast<omm::Classifier>(p_assoc);
	property_attributes ( result, p_name, l_classifier, p_target, p_min, p_max, std::string(), std::string(), /* p_part_of_id,*/ p_status, p_desc);
	//result->is_navigable (true);
	result->aggregation_kind ( p_is_composition ? omm::Aggregation_kind::composite : omm::Aggregation_kind::none );
	result->association (p_assoc);
	p_assoc->member_end(result);
	return result;
}

std::shared_ptr<omm::Constraint> Model_factory::make_constraint( const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_classifier, const std::string& p_expression , const omm::Lc_status p_status, const std::string& p_desc ) {
	std::shared_ptr<omm::Constraint> result = std::make_shared<omm::Constraint>();
	result->name(p_name);
    result->lc_status(p_status);
    result->expression(p_expression);
	result->description (p_desc);
	result->classifier(p_classifier);
	p_classifier->insert_constraint(result);
	return result;
}

std::shared_ptr<omm::Generalization> Model_factory::make_generalization(const std::shared_ptr<omm::Classifier>& p_general, const std::shared_ptr<omm::Classifier>& p_specific) {
	std::shared_ptr<omm::Generalization> result = std::make_shared<omm::Generalization>();
	result->general(p_general);
	result->specific(p_specific);
    //result->lc_status(p_status);

	p_specific->insert_generalization (p_general->name(),result);
	p_general->specializations().insert(std::make_pair<const std::string&,const std::shared_ptr<omm::Generalization>&>(p_specific->name(),result));
	return result;
}

std::shared_ptr<omm::Persistent_object> Model_factory::make_persistent_object(const std::string& p_id, const std::string& p_scope_id){
	std::shared_ptr<omm::Persistent_object> result = std::make_shared<omm::Persistent_object>();
	result->id(p_id);
	result->id_scope(p_scope_id);
	return result;
}

std::shared_ptr<omm::Column_constraint> Model_factory::make_column_constraint (const std::shared_ptr<omm::Column>& p_column, const omm::Column_constraint::Type p_type) {
    std::shared_ptr<omm::Column_constraint> result;

    if (p_column) {
        result = std::make_shared<omm::Column_constraint>(p_column, p_type);
        p_column->insert_constraint(result);
    }
    return result;
}


void Model_factory::package_element_attributes (const std::shared_ptr<omm::Classifier>& p_elem, const std::string& p_name, const std::shared_ptr<omm::Package>& p_package, const omm::Lc_status p_status, const std::string& p_desc) {
	p_elem->name(p_name);
    p_elem->lc_status(p_status);
	p_elem->description (p_desc);
	p_elem->package (p_package);
	
}

void Model_factory::property_attributes (const std::shared_ptr<omm::Property>& p_prop, const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_classifier, const std::shared_ptr<const omm::Classifier>& p_type,
		const int p_min_occurs, const int p_max_occurs, const std::string& p_computed, const std::string& p_default,  /*const bool p_part_of_id,*/ const omm::Lc_status p_status, const std::string& p_desc) {
	p_prop->name(p_name);
    p_prop->lc_status(p_status);

	p_prop->description (p_desc);
	p_prop->classifier (p_classifier);
	p_prop->type(p_type);
	p_prop->min_occurs(p_min_occurs);
	p_prop->max_occurs(p_max_occurs);
	p_prop->computed_value(p_computed);
	p_prop->default_value(p_default);
	p_classifier->insert_own_property(p_prop);
}

std::shared_ptr<omm::Logical_schema> Model_factory::make_logical_schema(const std::shared_ptr<omm::Package>& p_domain, const std::string& p_name ) {
    
    std::shared_ptr<omm::Logical_schema> result = std::make_shared<omm::Logical_schema>();
    result->name(p_name);
    result->package(p_domain);
    p_domain->schema(result);
    
    return result;
}

std::shared_ptr<omm::Logical_enum> Model_factory::make_logical_enum(const std::shared_ptr<omm::Enumeration>& p_enum, const std::string& p_name ) {
    std::shared_ptr<omm::Logical_enum> result = std::make_shared<omm::Logical_enum>();
    result->name(p_name);
    result->enumeration(p_enum);
    p_enum->logical_enum(result);
    auto && schema = p_enum->package()->schema();
    result->schema(schema);
    schema->insert_element(result);
    return result;
}
    
std::shared_ptr<omm::Logical_type> Model_factory::make_logical_type(const std::shared_ptr<omm::Data_type>& p_type, const std::string& p_name ) {
    std::shared_ptr<omm::Logical_type> result = std::make_shared<omm::Logical_type>();
    result->name(p_name);
    result->data_type(p_type);
    p_type->logical_type(result);
    auto && schema = p_type->package()->schema();
    result->schema(schema);
    schema->insert_element(result);
    
    return result;
}   
    
std::shared_ptr<omm::Table> Model_factory::make_table(const std::shared_ptr<omm::Class>& p_class, const std::string& p_name ) {
    std::shared_ptr<omm::Table> result = std::make_shared<omm::Table>();
    result->name(p_name);
    result->classes().push_back(p_class);
    result->root_classifier (p_class);
    p_class->table(result);
    auto && schema = p_class->package()->schema();
    result->schema(schema);
    schema->insert_element(result);
    
    return result;
}   

std::shared_ptr<omm::Table> Model_factory::make_table(const std::shared_ptr<omm::Association>& p_assoc, const std::string& p_name ) {
    std::shared_ptr<omm::Table> result = std::make_shared<omm::Table>();
    result->name(p_name);
    result->association(p_assoc);
    result->root_classifier(p_assoc);
    p_assoc->table(result);
    auto && schema = p_assoc->package()->schema();
    result->schema(schema);
    schema->insert_element(result);
    return result;
}    

std::shared_ptr<omm::Column> Model_factory::make_column(const std::shared_ptr<omm::Property>& p_prop, const std::string& p_name, const std::shared_ptr<omm::Table>& p_table, const int p_native_type ) {
    std::shared_ptr<omm::Column> result = std::make_shared<omm::Column>();
    result->name(p_name);
    result->logical_struct(p_table);
    result->native_type(p_native_type);
    p_table->columns().insert(std::make_pair<const std::string&,const std::shared_ptr<omm::Column>&>(result->name(),result));
    if (p_prop) {
        result->property(p_prop);
        p_prop->logical_field(result);
    }
    
    return result;
}


std::shared_ptr<omm::Attribute> Model_factory::make_attribute(const std::shared_ptr<omm::Property>& p_prop, const std::string& p_name, const int p_native_type ) {
    std::shared_ptr<omm::Attribute> result = std::make_shared<omm::Attribute>();
    result->name(p_name);
    result->native_type(p_native_type);

    auto && lo= p_prop->classifier()->logical_struct();
    result->logical_struct(lo);
    auto && lo_type = std::dynamic_pointer_cast<omm::Logical_type>(lo);
    lo_type->add_attribute(result);
    if (p_prop) {
        result->property(p_prop);
        p_prop->logical_field(result);
    }
    
    return result;
}

std::shared_ptr<omm::Property_by_sub_field> Model_factory::make_property_by_sub_field(const std::shared_ptr<omm::Property>& p_result_prop, const std::shared_ptr<omm::Property>& p_filter_prop) {
    return make_property_by_sub_field (p_result_prop,p_filter_prop->column());
}

std::shared_ptr<omm::Property_by_sub_field> Model_factory::make_property_by_sub_field(const std::shared_ptr<omm::Property>& p_result_prop, const std::shared_ptr<omm::Column>& p_fk_column) {
    
    std::shared_ptr<omm::Property_by_sub_field> result = std::make_shared<omm::Property_by_sub_field>();
    
    result->filtering_column(p_fk_column);
    result->property(p_result_prop);
    p_result_prop->property_by_sub_field(result);

    return result;
}

std::shared_ptr<omm::Property_by_sub_field> Model_factory::make_property_by_sub_field(const std::shared_ptr<omm::Property>& p_result_prop, const std::shared_ptr<omm::Table>& p_filter_table, const std::string& p_filter_column) {
    auto && search = p_filter_table->columns().find(p_filter_column);
    if ( search != p_filter_table->columns().end()) {
        return  make_property_by_sub_field (p_result_prop,search->second);
    } else {
        return nullptr;
    }
}

std::shared_ptr<omm::Enum_value> Model_factory::make_enum_value (const std::string& p_value, const std::shared_ptr<omm::Enumeration>& p_enum, const omm::Lc_status p_status) {
    
    std::shared_ptr<omm::Enum_value> result = std::make_shared<omm::Enum_value>();
    
    result->name(p_value);
    result->lc_status(p_status);
    result->enumeration(p_enum);
    
    p_enum->insert_element(result);
    
    return result;
}
    
std::shared_ptr<omm::Language> Model_factory::make_language( const std::string& p_code, const std::string& p_name, const std::string& p_fts_config, const omm::Lc_status p_status, const std::string& p_desc) {
 

	std::shared_ptr<omm::Language> result = std::make_shared<omm::Language>(p_code);
    result->name(p_name);
    result->fts_config(p_fts_config);
    result->lc_status(p_status);
	result->description (p_desc);

	return result;
}

template <typename T1>  void Model_factory::update ( const std::shared_ptr<T1>& p_elem, const std::string& p_new_name,  const std::string& p_new_desc, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status ) {

     if ( !p_new_name.empty() ) {
        auto parent = p_elem->template parent_element<omm::Packageable_element>();
        if (parent) {
            parent->rename_element( p_elem, p_new_name);
        }
        else {
            throw exception::Failure("Parent element of " + p_elem->name() + " not found");
        }
        p_elem->name ( p_new_name );
     }

     if ( !p_new_desc.empty() ) {
          p_elem->description ( p_new_desc );
     }

     p_elem->lc_status ( p_status );

     specific_attributes_update ( p_elem, p_attrs );
}

template void Model_factory::update<omm::Package> ( const std::shared_ptr<omm::Package>& p_elem, const std::string& p_new_name,  const std::string& p_new_desc, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status ) ;

template void Model_factory::update<omm::Class> ( const std::shared_ptr<omm::Class>& p_elem, const std::string& p_new_name,  const std::string& p_new_desc, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status ) ;

template void Model_factory::update<omm::Association> ( const std::shared_ptr<omm::Association>& p_elem, const std::string& p_new_name,  const std::string& p_new_desc, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status ) ;

template void Model_factory::update<omm::Composite_type> ( const std::shared_ptr<omm::Composite_type>& p_elem, const std::string& p_new_name,  const std::string& p_new_desc, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status ) ;

template void Model_factory::update<omm::Primitive_type> ( const std::shared_ptr<omm::Primitive_type>& p_elem, const std::string& p_new_name,  const std::string& p_new_desc, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status ) ;

template void Model_factory::update<omm::Enumeration> ( const std::shared_ptr<omm::Enumeration>& p_elem, const std::string& p_new_name,  const std::string& p_new_desc, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status ) ;

template void Model_factory::update<omm::Property> ( const std::shared_ptr<omm::Property>& p_elem, const std::string& p_new_name,  const std::string& p_new_desc, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status ) ;

template void Model_factory::update<omm::Enum_value> ( const std::shared_ptr<omm::Enum_value>& p_elem, const std::string& p_new_name,  const std::string& p_new_desc, const omm::Element_attributes& p_attrs, const omm::Lc_status p_status ) ;

void Model_factory::specific_attributes_update ( const std::shared_ptr<omm::Class>& p_elem,  const omm::Element_attributes& p_attrs ) {
    
    
    const omm::Class_attributes& attrs =dynamic_cast<const omm::Class_attributes&> (p_attrs);
    
    if (attrs.is_abstract_ == true && !p_elem->is_abstract() ) {
        std::cerr << "Update of the class " + p_elem->name() + " : the class cannot become abstract " << std::endl;
        
    } else {
        p_elem->is_abstract(attrs.is_abstract_);
    }


}

void Model_factory::specific_attributes_update ( const std::shared_ptr<omm::Property>& p_elem,  const omm::Element_attributes& p_attrs ) {

  const omm::Property_attributes& attrs =dynamic_cast<const omm::Property_attributes&> (p_attrs);

     if ( attrs.source_member_end_ ) {
          if ( attrs.min_ != p_elem->min_occurs() ) {
               std::cerr << "Update of the property " + p_elem->name() + " : the cardinality cannot be modified" << std::endl;
          }

          if ( attrs.max_ != p_elem->max_occurs() ) {
               std::cerr << "Update of the property " + p_elem->name() + " : the cardinality cannot be modified" << std::endl;
          }
     }


     if ( attrs.source_member_end_ ) {
          // member end
          if ( attrs.is_scp_ != ( p_elem->classifier()->id() != p_elem->association()->id() ) ) {
               p_elem->classifier()->erase_element ( p_elem );
               if ( attrs.is_scp_ ) {
                    p_elem->classifier ( attrs.source_member_end_ );
                    attrs.source_member_end_->insert_own_property ( p_elem );
               } else {
                    p_elem->classifier ( p_elem->association() );
                    p_elem->association()->insert_own_property ( p_elem );
               }

          }

          if ( attrs.aggregation_ != p_elem->aggregation_kind() ) {
               p_elem->aggregation_kind ( attrs.aggregation_ );
          }


     } else {
          //simple property
          if ( attrs.min_ != p_elem->min_occurs() ) {
               p_elem->min_occurs ( attrs.min_ ) ;
          }
     }

}

void Model_factory::specific_attributes_update ( const std::shared_ptr<omm::Primitive_type>& p_elem,  const omm::Element_attributes& p_attrs ) {
    
    
    const omm::Primitive_attributes& attrs =dynamic_cast<const omm::Primitive_attributes&> (p_attrs);
    
    if (attrs.native_type_ != p_elem->native_type()) {
        std::cerr << "Update of the primitive type " + p_elem->name() + " : the native type cannot be modified. Delete the primitive and create again with another primitive type " << std::endl;
        
    } 


}

void Model_factory::specific_attributes_update ( const std::shared_ptr<omm::Packageable_element>& p_elem,  const omm::Element_attributes& p_attrs ) {

// nothing to do
}

