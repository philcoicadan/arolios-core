/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Association_instance_row.h>
#include <mttr/Field_maker.h>
#include <util/String.h>
#include <drogon/orm/Exception.h>
#include <exception/Exceptions.h>
#include <qry/Util.h>




using namespace arolios;
using namespace arolios::mttr;

Association_instance_row::Association_instance_row(const std::shared_ptr<const omm::Logical_struct>& p_lo, const std::shared_ptr<const omm::Classifier>& p_prop_csf, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache): Classifier_instance_row(p_lo, p_prop_csf, p_import_cache) {
    class_instance_row_id_ = 0;

}

Association_instance_row::~Association_instance_row() {

}

void Association_instance_row::build_field (  mttr::Field_maker& p_f_maker, const std::shared_ptr<const Json::Value>& p_data, const std::shared_ptr<const omm::Property_view>& p_prop_view, const std::string& p_path_name ) {


    const auto & type = p_prop_view->property()->type();
    const auto & lf = p_prop_view->property()->logical_field();
    if ( lf ) {
        p_f_maker.data ( p_data );
        p_f_maker.logical_field ( lf ) ;
        p_f_maker.type ( type ) ;
        p_f_maker.property_view ( p_prop_view );
        type->accept ( p_f_maker ) ;

        // the value of this property is a target instance of an association  (cardinality O,N or 0,1) implemented in the table of the target class. This value is the instance to be updated.
    }  else if ( p_prop_view->property()->has_sub_field() ) {
        if ( p_data->isObject() ) {

                const Json::Value& jtarget = *p_data ;
                if ( jtarget.isMember ( "id" ) ) {
                    std::string string_value;
                    std::string target = jtarget["id"].asString();

                    if ( target[0] == '$' ) {
                        target.erase ( 0,1 );
                        target = util::String::standardize ( target );
                        std::transform ( target.begin(), target.end(), target.begin(),
                                         [] ( unsigned char c ) -> unsigned char {return std::tolower ( c ); } );

                        auto search = import_cache().find ( target );
                        if ( search != import_cache().end() ) {
                            const Json::Value& jinst = * ( search->second );
                            string_value = jinst.asString();
                        } else {
                            const std::string error ( "instance value not found in the imported instances" );
                             
                            throw exception::Not_found ( error );
                        }
                    } else {

                        string_value =  jtarget["id"].asString();
                    }
                    
                    //check format of instance id
                    const auto [csf_id, row_id] = qry::Util::split_instance_id(string_value);
                    
    
                    class_instance_row_id_ = row_id;
                    

                } else {
                    std::string error_msg = "JSON data for property " + p_prop_view->id_name() + " bad declared";
                     
                    throw exception::Not_found ( error_msg );
                }


        } else {
            std::string error_msg = "JSON data for property" + p_prop_view->id_name()+ " bad declared";
             
            throw exception::Not_found ( error_msg );
        }

    } else {
        std::string warn_msg = "for the classifier " + classifier()->name() + " property " + p_prop_view->id_name() + " cannot accept a value ";
        std::cout << "WARNING: " << warn_msg << std::endl;
    }
}

const unsigned long mttr::Association_instance_row::class_instance_row_id() const
{
    return class_instance_row_id_;
}
