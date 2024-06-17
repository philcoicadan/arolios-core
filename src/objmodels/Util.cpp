/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Util.h>
#include <omm/Package.h>
#include <omm/Property.h>
#include <omm/Class.h>
#include <omm/Association.h>
#include <omm/Generalization.h>
#include <omm/Logical_struct.h>
#include <omm/Logical_field.h>
#include <exception/Exceptions.h>
#include <common/Singleton.h>
#include <common/Object_model_info.h>
#include <util/String.h>
#include <qry/Util.h>



using namespace arolios;
using namespace arolios::model;

std::string Util::make_logical_struct_full_name ( const std::shared_ptr<const omm::Classifier>& p_csf)  {
     //TODO name separator to be configured if depending on the RDBMS: SQL standard to be checked

     char separator = '.';

     std::string qualified_name = p_csf->package()->schema()->name() + separator + make_logical_struct_name( p_csf);
     if ( qualified_name.length() > 63 ) { //TODO limit to be configured
          throw exception::Model_error ( "RDB table or type " + qualified_name + " too long" ); 
     }

 

     return qualified_name;
}

std::string Util::make_logical_struct_name ( const std::shared_ptr<const omm::Classifier>& p_csf)  
{
    return std::string ("r" + p_csf->id());
}

std::string Util::make_schema_name ( const std::shared_ptr<const omm::Package>& p_pk)  
{
    return std::string ("s" + p_pk->id());
}



std::string Util::make_column_type_full_name (const std::shared_ptr<const omm::Classifier>& p_type) {

    std::string result ;

    const auto std_pk = 
        common::Singleton<common::Object_model_info>::instance().object()->standard_domain();

    if (!std_pk ) {
       // standard_domain is loading
        return search_in_standard_domain(p_type);

    } else if ( p_type->package () == std_pk) {
        // standard domain already stored
        return search_in_standard_domain(p_type);
    }
    else {
        result = make_logical_struct_full_name(p_type);
    }
    return result;
}

std::string Util::make_logical_field_name (const std::shared_ptr<const omm::Property>& p_prop, const bool p_is_root) {

    
   return std::string ("f" + p_prop->id());
}


std::string Util::make_column_name_with_classifier (const std::shared_ptr<const omm::Property>& p_prop, const std::shared_ptr<const omm::Classifier>& p_csf) {

    return std::string ( "f" + p_csf->id() + "_" + p_prop->id());
}

std::string Util::make_column_name_for_association (const std::shared_ptr<const omm::Property>& p_prop, const std::shared_ptr<const omm::Classifier>& p_class, const std::shared_ptr<const omm::Association>& p_assoc) {

    // compute a column name. It must be unique in the table. This table is associated to the classifier given by the root of this classifier.


    if ( p_class->id() == p_assoc->id()) {
        // assoc has its own table and assoc is root
        return make_logical_field_name ( p_prop, true);
    } else {
        return make_column_name_with_classifier (p_prop, p_assoc);
        // assoc is implemented in the table of another class, take the assoc id as prefix
    }

  

}



void Util::add_identifier ( const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_class, std::unordered_map<std::string,std::shared_ptr<omm::Property>>& ident_map ) {

    const auto prop_name = util::String::standardize ( p_name );

    const auto prop = find_property_in_inheritance ( prop_name, p_class );
    if ( prop  && prop->max_occurs() == 1) {
        ident_map.insert ( {prop->name(),prop} );
    } else {
        std::string error_msg = "Property " + p_name + " cannot be in the identifier because it is not found or not valid";
         
        throw exception::Failure ( error_msg );
    }
}


std::shared_ptr<omm::Property> Util::find_property_in_inheritance ( const std::string& p_name, const std::shared_ptr<omm::Classifier>& p_csf ) {

    std::shared_ptr<omm::Property> result =nullptr;

    auto props_map = p_csf->all_elements_as_property();

    if (props_map.empty()) {
        p_csf->make_all_elements_as_property(false);
        props_map = p_csf->all_elements_as_property();
    }

    const auto search = props_map.find ( p_name ) ;

    if ( search != props_map.end() ) {
            result = search->second;

    }

    return result;
}

bool Util::check_inheritance (const std::shared_ptr<omm::Class>& p_derived, const std::shared_ptr<omm::Class>& p_base) {

    auto base_classes = p_derived->more_general_classifiers();
    return ( base_classes.find(p_base) != base_classes.end()) ;
}

std::string Util::make_fts_fragment_from_field( const std::shared_ptr<const omm::Logical_field>& p_field, const std::string& p_path) {


    const std::string column_name = p_path.empty() ? p_field->name() : p_path + "." + p_field->name();

    std::string value = column_name;

    switch (p_field->native_type()) {
        case 0: // integer
        case 1: // bigint
        case 2: // smallint
        case 3: // boolean
        case 4: // numeric
        case 5: // real
        case 6: // double precision
        case 10: // time
        case 12: // time with time zone
            value += "::text";
            break;
        case 9:  // date
            value = "am_date_to_text (" + column_name + ")";
            break;
        case 11: // timestamp
            value = "am_timestamp_to_text (" + column_name + ")";
            break;
        case 13: // timestamp with time zone
            value = "am_timestamp_tz_to_text (" + column_name + ")";
            break;
        default: // character varying or text
            break;
    }


    std::string result = "COALESCE(" + value + ", '') " ;


    return result;
}


std::shared_ptr<const omm::Classifier> Util::find_root_classifier ( const std::shared_ptr<const omm::Classifier>& p_cls) {

    std::shared_ptr<const omm::Classifier> result = p_cls;


    for ( auto i = p_cls->generalizations().begin(); i != p_cls->generalizations().end() ; ++i ) {
        auto cls =  i->second->general();
        result = find_root_classifier ( cls );
    }
    return result;
}

std::string Util::search_in_standard_domain (const std::shared_ptr<const omm::Classifier>& p_type) {

        auto model_info = common::Singleton<common::Object_model_info>::instance().object();
        auto search = model_info->primitive_native_types_map().find (p_type->name());
        if (search == model_info->primitive_native_types_map().end() ) { // standard type is not a primitive
            return p_type->logical_struct()->full_name();
        } else {
            return qry::Util::get_native_name( search->second);
        }
}
