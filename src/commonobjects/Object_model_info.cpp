/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <common/Object_model_info.h>
#include <omm/Model_factory.h>
#include <qry/Model_retriever.h>
#include <qry/Init_retriever.h>
#include <qry/Util.h>
#include <util/String.h>
#include <exception/Exceptions.h>





using namespace arolios;
using namespace arolios::common;

Object_model_info::Object_model_info () {
    root_pk_ = nullptr;
    std_pk_ = nullptr;
}

Object_model_info::~Object_model_info () { 
    
}



std::shared_ptr<omm::Data_type> Object_model_info::find_standard_type_by_name ( const std::string& p_type_name , const bool p_only_active) const  {
    if (std_pk_) {
        auto && search = std_pk_->data_types().find ( p_type_name ) ;
        if ( search == std_pk_->data_types().end() ) {
            return nullptr;
        }
        
        return (!p_only_active || (search->second->lc_status() == omm::Lc_status::active) ) ? search->second : nullptr;
    } else {
        return nullptr;
    }
}


std::shared_ptr<omm::Data_type> Object_model_info::find_primitive_type_by_name ( const std::string& p_type_name, const bool p_only_active ) const  {
    // a primitive type belongs to the standard package AND in the prim_native_types map
    auto result =  find_standard_type_by_name(p_type_name, p_only_active);
    if (!result) {
        return nullptr;
    } else  if (prim_native_types_map_.find(p_type_name) == prim_native_types_map_.end() ) {
        return nullptr;
    }
    return result;
}


std::shared_ptr<omm::Package> Object_model_info::find_domain_by_name ( const std::string& p_pk_name, const bool p_only_active ) const {
     auto && search = pk_name_map_.find ( p_pk_name );
    if ( search == pk_name_map_.end() ) {
        return nullptr;
    }

    return (!p_only_active || (search->second->lc_status() == omm::Lc_status::active) ) ? search->second : nullptr;
}

std::shared_ptr<omm::Package> Object_model_info::find_domain_by_id ( const std::string& p_pk_id, const bool p_only_active ) const {

    auto && search = pk_id_map_.find ( p_pk_id );
    if ( search == pk_id_map_.end() ) {
        return nullptr;
    }
    return (!p_only_active || (search->second->lc_status() == omm::Lc_status::active) ) ? search->second : nullptr;
}



std::shared_ptr<omm::Property> Object_model_info::find_property_by_id ( const std::string& p_id , const bool p_only_active) const {
    auto && search = prop_id_map_.find ( p_id );
    if ( search == prop_id_map_.end() ) {
        return nullptr;
    }
    return (!p_only_active || (search->second->lc_status() == omm::Lc_status::active) ) ? search->second : nullptr;
}

std::shared_ptr<omm::Enum_value> Object_model_info::find_enum_value_by_id ( const std::string& p_id , const bool p_only_active) const {
    auto && search = enum_val_id_map_.find ( p_id );
    if ( search == enum_val_id_map_.end() ) {
        return nullptr;
    }
    return (!p_only_active || (search->second->lc_status() == omm::Lc_status::active) ) ? search->second : nullptr;
}

template <typename T> void Object_model_info::insert (const std::shared_ptr<T>& p_elt) {
    insert_classifier (p_elt) ;
}

template void Object_model_info::insert<omm::Class> (const std::shared_ptr<omm::Class>& p_elt) ;
template void Object_model_info::insert<omm::Data_type> (const std::shared_ptr<omm::Data_type>& p_elt) ;
template void Object_model_info::insert<omm::Enumeration> (const std::shared_ptr<omm::Enumeration>& p_elt) ;
template void Object_model_info::insert<omm::Association> (const std::shared_ptr<omm::Association>& p_elt) ;


template <> void Object_model_info::insert (const std::shared_ptr<omm::Package>& p_domain) {

    pk_id_map_.insert(std::make_pair<const std::string& ,const std::shared_ptr<omm::Package>&> (p_domain->id(), p_domain));
   pk_name_map_.insert(std::make_pair<const std::string&,const std::shared_ptr<omm::Package>&> ( p_domain->name(), p_domain ) );
}

template <> void Object_model_info::insert (const std::shared_ptr<omm::Property>& p_prop) {
    insert_property (p_prop) ;
}

template <> void Object_model_info::insert (const std::shared_ptr<omm::Enum_value>& p_enum_val) {
    insert_enum_value(p_enum_val);
}

template <typename T> void Object_model_info::rename ( const std::shared_ptr<T>& p_elt, const std::string& p_new_name ) {
// nothing to do
}

template void Object_model_info::rename<omm::Class> (const std::shared_ptr<omm::Class>& p_elt, const std::string& p_new_name) ;
template void Object_model_info::rename<omm::Data_type> (const std::shared_ptr<omm::Data_type>& p_elt, const std::string& p_new_name) ;
template void Object_model_info::rename<omm::Enumeration> (const std::shared_ptr<omm::Enumeration>& p_elt, const std::string& p_new_name) ;
template void Object_model_info::rename<omm::Association> (const std::shared_ptr<omm::Association>& p_elt, const std::string& p_new_name) ;
template void Object_model_info::rename<omm::Property> (const std::shared_ptr<omm::Property>& p_elt, const std::string& p_new_name) ;
template void Object_model_info::rename<omm::Enum_value> (const std::shared_ptr<omm::Enum_value>& p_elt, const std::string& p_new_name) ;

template <> void Object_model_info::rename ( const std::shared_ptr<omm::Package>& p_domain, const std::string& p_new_name ) {
    if ( p_new_name != p_domain->name() ) {
        pk_name_map_.insert ( {p_new_name, p_domain } );
        pk_name_map_.erase ( p_domain->name() );
    }
}

template <typename T>  void Object_model_info::eliminate (const std::shared_ptr<T>& p_elt) {

     csf_id_map_.erase (p_elt->id());
}

template void Object_model_info::eliminate<omm::Class> (const std::shared_ptr<omm::Class>& p_elt) ;
template void Object_model_info::eliminate<omm::Data_type> (const std::shared_ptr<omm::Data_type>& p_elt) ;
template void Object_model_info::eliminate<omm::Enumeration> (const std::shared_ptr<omm::Enumeration>& p_elt) ;
template void Object_model_info::eliminate<omm::Association> (const std::shared_ptr<omm::Association>& p_elt) ;

template <>  void Object_model_info::eliminate (const std::shared_ptr<omm::Package>& p_domain) {
        pk_id_map_.erase (p_domain->id());

     pk_name_map_.erase (p_domain->name());
     
}

template <>  void Object_model_info::eliminate (const std::shared_ptr<omm::Property>& p_prop) {

     prop_id_map_.erase (p_prop->id());
}


template <>  void Object_model_info::eliminate (const std::shared_ptr<omm::Enum_value>& p_prop) {

    // nothing to do
}


void Object_model_info::insert_classifier (const std::shared_ptr<omm::Classifier>& p_csf) {
    csf_id_map_.insert_or_assign( p_csf->id(), p_csf  );
}

void Object_model_info::insert_property ( const std::shared_ptr<omm::Property>& p_prop ) {
    prop_id_map_.insert_or_assign (  p_prop->id(), p_prop  ) ;
}

void Object_model_info::insert_enum_value ( const std::shared_ptr<omm::Enum_value>& p_enum_val ) {
    enum_val_id_map_.insert_or_assign (  p_enum_val->id(), p_enum_val  ) ;
}


void Object_model_info::map_native_primitive_type (const std::string& p_prim, const std::string& p_native) {
     qry::Native_type native_type = qry::Util::get_native_type(p_native);
     prim_native_types_map_.insert( std::make_pair<const std::string&, const qry::Native_type&> ( p_prim,native_type) ) ;
}

std::shared_ptr<omm::Classifier> Object_model_info::find_classifier_by_id ( const std::string& p_id, const bool p_only_active ) const {

        auto && search = csf_id_map_.find ( p_id );
        if ( search == csf_id_map_.end() ) {
            return nullptr;
        }
        return (!p_only_active || (search->second->lc_status() == omm::Lc_status::active) ) ? search->second : nullptr;

}
