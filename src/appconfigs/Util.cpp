/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <appcfg/Util.h>
#include <omm/Enumeration.h>
#include <omm/Property.h>
#include <omm/Logical_field.h>
#include <omm/Logical_struct.h>
#include <memory>



using namespace arolios;
using namespace arolios::appcfg;

Util::Util() {

}

Util::~Util() {

}

    
void Util::add_prop_for_select_template ( const std::shared_ptr<const omm::Property>& p_prop,
        std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_prop_map,
        std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_enum_map,
        std::unordered_map<std::string,std::string>& p_prop_column_name_map,
        std::unordered_map<std::string,std::string>& p_prop_alias_column_name_map,
        const std::string& p_id_name , const std::string& p_table_prefix) {
    if ( ! p_prop->has_composite_type() ) {
        p_prop_map.insert ( std::make_pair<const std::string&,const std::shared_ptr<const omm::Property>&> ( p_id_name, p_prop ) );
        std::string col_name = p_prop->logical_field()->name();
        std::string col_alias_name;
        if (!p_table_prefix.empty()) {
            col_alias_name = p_table_prefix + "_" + col_name;
            col_name = p_table_prefix + "." + col_name;
            p_prop_alias_column_name_map.insert ( std::make_pair<const std::string&,const std::string&> ( p_id_name, col_alias_name ) );

        }
        
        p_prop_column_name_map.insert ( std::make_pair<const std::string&,const std::string&> ( p_id_name, col_name ) );
        const auto enum_ptr = std::dynamic_pointer_cast<const omm::Enumeration>(p_prop->type());
        if (enum_ptr) {
            p_enum_map.insert( std::make_pair<const std::string&,const std::shared_ptr<const omm::Property>&> ( p_id_name, p_prop ) );
        }
        
    } else {
        add_nested_prop_for_select_template ( p_prop, p_prop_map, p_enum_map, p_prop_column_name_map, p_prop_alias_column_name_map, p_id_name,std::string(), std::string(), p_table_prefix ) ;
    }
}


void Util::add_nested_prop_for_select_template ( const std::shared_ptr<const omm::Property>& p_prop,
            std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_prop_map,
            std::unordered_map<std::string,std::shared_ptr<const omm::Property>>& p_enum_map,
            std::unordered_map<std::string,std::string>& p_prop_column_name_map,
            std::unordered_map<std::string,std::string>& p_prop_alias_column_name_map,
            const std::string& p_id_name, const std::string& p_col_path, const std::string& p_alias_col_path, const std::string& p_table_prefix ) {
  
        for ( auto nested_prop : p_prop->nested_properties() ) {
        if ( nested_prop->has_composite_type() ) {
            std::string nested_id_name =  p_id_name + "." + nested_prop->name();
            std::string col_path = "(";
            std::string alias_col_path;
            if ( p_col_path.empty() ){
                if (!p_table_prefix.empty()) {
                    col_path += p_table_prefix + ".";
                    alias_col_path = p_table_prefix + "_";
                }
                col_path += p_prop->logical_field()->name() + ").";
                alias_col_path += p_prop->logical_field()->name() + "_";
            } else {
                col_path += p_col_path + p_prop->logical_field()->name() + ").";
                alias_col_path += p_alias_col_path + p_prop->logical_field()->name() + "_";
                
            }
            
           
            add_nested_prop_for_select_template ( nested_prop, p_prop_map, p_enum_map, p_prop_column_name_map, p_prop_alias_column_name_map, nested_id_name, col_path, alias_col_path, p_table_prefix );
        } else {
            
            auto nested_id_name = p_id_name + "." + nested_prop->name();
            p_prop_map.insert ( std::make_pair<const std::string&,const std::shared_ptr<const omm::Property>&> ( nested_id_name, nested_prop ) );
            std::string col_name = "(";
            std::string col_alias_name ; 
            if ( p_col_path.empty() ){
                if (!p_table_prefix.empty()) {
                    col_name += p_table_prefix + ".";
                    col_alias_name = p_table_prefix + "_";
                }
                col_name += p_prop->logical_field()->name() + ")." + nested_prop->logical_field()->name() ;
                col_alias_name += p_prop->logical_field()->name() + "_" + nested_prop->logical_field()->name();
            } else {
                col_name += p_col_path + p_prop->logical_field()->name() + ")." + nested_prop->logical_field()->name() ;
                col_alias_name = p_alias_col_path + p_prop->logical_field()->name() + "_" + nested_prop->logical_field()->name();
              
            }
            p_prop_column_name_map.insert ( std::make_pair<const std::string&,const std::string&> ( nested_id_name, col_name ) );
            p_prop_alias_column_name_map.insert ( std::make_pair<const std::string&,const std::string&> ( nested_id_name, col_alias_name ) );
            //}
            const auto enum_ptr = std::dynamic_pointer_cast<const omm::Enumeration>(nested_prop->type());
            if (enum_ptr) {
                p_enum_map.insert( std::make_pair<const std::string&,const std::shared_ptr<const omm::Property>&> ( nested_id_name, nested_prop ) );
            }
            
        }
    }
}
    
