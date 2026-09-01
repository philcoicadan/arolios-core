/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_recorder.h>
#include <omm/Persistent_object.h>
#include <omm/Model_factory.h>
#include <appcfg/App.h>
#include <appcfg/App_domain.h>
#include <appcfg/Base_classifier_config.h>
#include <model/Model_element.h>
#include <model/Root_loader.h>
#include <omm/Class_attributes.h>
#include <omm/Property_attributes.h>
#include <omm/Primitive_attributes.h>
#include <omm/Primitive_type.h>
#include <qry/Util.h>
#include <exception/Exceptions.h>
#include <common/Db_client.h>
#include <common/Singleton.h>
#include <util/String.h>
#include <util/String.h>




using namespace arolios;
using namespace arolios::model;



Model_recorder::Model_recorder ( )
{
	const std::shared_ptr<common::Db_client> db = common::Singleton<common::Db_client>::instance().object();
    //start transaction
    transPtr_ = db->newTransaction();

	
}

Model_recorder::~Model_recorder () {
    //when destroyed , commit automatically
	transPtr_ =nullptr;
}


void Model_recorder::rollback( ) {
	 transPtr_->rollback() ;
}


template <> std::shared_ptr<omm::Persistent_object> Model_recorder::create<omm::Package > ( const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const std::string& p_desc, const omm::Element_attributes& p_attrs) const {

    std::shared_ptr<omm::Persistent_object> result ;
    std::shared_ptr<omm::Package> nesting = std::dynamic_pointer_cast<omm::Package>( p_parent);
    try {
        unsigned int id = 0;
        int nb_r = 0;
        if ( nesting == nullptr ) {
            auto r = transPtr_->execSqlSync ( "INSERT INTO am_pk (name) VALUES ($1) RETURNING id",p_name );
            nb_r = r.size();
            if ( nb_r >0 ) {
                auto row = r[0];
                id = row["id"].as<unsigned int>();
            }
        } else {
            auto r = transPtr_->execSqlSync ( "INSERT INTO am_pk (name, nesting) VALUES ($1,$2) RETURNING id",p_name, util::String::stoui(nesting->id()) );
            nb_r = r.size();
            if ( nb_r >0 ) {
                auto row = r[0];
                id = row["id"].as<unsigned int>();
            }
        }
        
        if (nb_r >0 && p_desc.length()>0 ) {
            auto r = transPtr_->execSqlSync ("UPDATE am_pk SET description = $1 WHERE id = $2 RETURNING id",p_desc, id);
            nb_r = r.size();
        }
        
        if ( nb_r >0 ) {
            result = omm::Model_factory::instance().make_persistent_object ( std::to_string(id), "am_pk" );
        } else {
            std::string error_msg = "Error in recording domain " + p_name;
            throw exception::Sql_error ( error_msg );
        }
    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

    return result;
}

template<> void Model_recorder::update<omm::Package> ( const std::shared_ptr<omm::Package>& p_pk, const std::string& p_new_name, const std::string& p_new_desc, const omm::Element_attributes& p_attrs) const {

    try {

        if ( ! p_new_name.empty()) {
            const std::string new_name = util::String::standardize(p_new_name);
            auto r = transPtr_->execSqlSync ( "UPDATE am_pk SET name = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_pk->id()), new_name );
            if ( r.size() == 0 ) {
                std::string error_msg = "Error in updating domain " + p_pk->name();
                throw exception::Sql_error ( error_msg );
            }
        } 
        
        if (!p_new_desc.empty() ) {
            auto r = transPtr_->execSqlSync ("UPDATE am_pk SET description = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_pk->id()), p_new_desc);
            if ( r.size() == 0 ) {
                std::string error_msg = "Error in updating domain " + p_pk->name();
                throw exception::Sql_error ( error_msg );
            }
        }
        

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

}

template<> void Model_recorder::deprecate<omm::Package> ( const std::shared_ptr<omm::Package>& p_pk) const {

    try {

        auto r = transPtr_->execSqlSync ( "UPDATE am_pk SET lc = 'deprecated' WHERE id = $1 RETURNING id",util::String::stoui ( p_pk->id() ) );
        if ( r.size() == 0 ) {
            std::string error_msg = "Error in updating domain " + p_pk->name();
            throw exception::Sql_error ( error_msg );
        }

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

}
template<> bool Model_recorder::eliminate<omm::Package> ( const std::shared_ptr<omm::Package>& p_pk ) const {

    try {


        auto r = transPtr_->execSqlSync ( "DELETE from am_pk WHERE id = $1 RETURNING id",util::String::stoui ( p_pk->id() ) );
        if ( r.size() == 0 ) {
            std::string warn_msg = "Deletion of domain " + p_pk->name() + " failed";
            std::cout << "WARNING: " << warn_msg<< std::endl;
            return false;
        }

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
    return true;

}

template <> std::shared_ptr<omm::Persistent_object> Model_recorder::create<omm::Class > ( const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const std::string& p_desc, const omm::Element_attributes& p_attrs) const {
    
    const omm::Class_attributes& attrs = dynamic_cast<const omm::Class_attributes&> (p_attrs);
    if (attrs.base_) {
        return store_class (p_name, "class", util::String::stoui(p_parent->id()), util::String::stoui(attrs.base_->id()),attrs.is_abstract_, p_desc);
    } else {
        return store_class (p_name, "class", util::String::stoui(p_parent->id()), attrs.is_abstract_, p_desc);
    }
    
}



std::shared_ptr<omm::Persistent_object> Model_recorder::store_class ( const std::string& p_name, const std::string& p_elem_type, const unsigned int p_pk_id, const unsigned int p_base_id , const bool p_is_abstract, const std::string& p_desc) const {

    std::shared_ptr<omm::Persistent_object> result ;
    try {
        const std::string name = util::String::standardize(p_name);
        auto r = transPtr_->execSqlSync ( "INSERT INTO am_p_elem (name, elem_type, package, base, is_abstract) VALUES ($1,$2,$3,$4,$5) RETURNING id", name, p_elem_type, p_pk_id, p_base_id, p_is_abstract );
        if ( r.size() >0 ) {

            auto row = r[0];
            result = omm::Model_factory::instance().make_persistent_object ( std::to_string(row["id"].as<unsigned int>()), "am_p_elem" );
              
            store_optional_description (p_desc, result);
        }
        else {
            std::string error_msg = "Error in recording class " + p_name;
            throw exception::Sql_error ( error_msg );
        }
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

    return result;
    
}

std::shared_ptr<omm::Persistent_object> Model_recorder::store_class ( const std::string& p_name, const std::string& p_elem_type, const unsigned int p_pk_id , const bool p_is_abstract, const std::string& p_desc) const {

    std::shared_ptr<omm::Persistent_object> result ;
    try {
        const std::string name = util::String::standardize(p_name);
        auto r = transPtr_->execSqlSync ( "INSERT INTO am_p_elem (name, elem_type, package, is_abstract) VALUES ($1,$2,$3,$4) RETURNING id", name, p_elem_type, p_pk_id, p_is_abstract );
        if ( r.size() >0 ) {
            auto row = r[0];
            result = omm::Model_factory::instance().make_persistent_object ( std::to_string(row["id"].as<unsigned int>()), "am_p_elem" );
            
            store_optional_description (p_desc, result);
        }
        else {
            std::string error_msg = "Error in recording class " + p_name;
            throw exception::Sql_error ( error_msg );
        }
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

    return result;

}

template <> std::shared_ptr<omm::Persistent_object> Model_recorder::create<omm::Enumeration >( const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_pk, const std::string& p_desc, const omm::Element_attributes& p_attrs) const {
    
    return store_classifier (p_name, "enum", util::String::stoui(p_pk->id()), p_desc) ;
}

template <> std::shared_ptr<omm::Persistent_object> Model_recorder::create<omm::Composite_type >( const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_pk, const std::string& p_desc, const omm::Element_attributes& p_attrs) const {
    
    return store_classifier (p_name, "type", util::String::stoui(p_pk->id()), p_desc) ;
}

template <> std::shared_ptr<omm::Persistent_object> Model_recorder::create<omm::Primitive_type >( const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_pk, const std::string& p_desc, const omm::Element_attributes& p_attrs) const {
    
    const omm::Primitive_attributes& attrs = dynamic_cast<const omm::Primitive_attributes&> (p_attrs);
    return store_primitive (p_name, util::String::stoui(p_pk->id()), p_desc, qry::Util::get_native_name(attrs.native_type_)) ;
}

template <> std::shared_ptr<omm::Persistent_object> Model_recorder::create<omm::Association >( const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_pk, const std::string& p_desc, const omm::Element_attributes& p_attrs) const {
    
    return store_classifier (p_name, "association", util::String::stoui(p_pk->id()), p_desc) ;
}

template <> std::shared_ptr<omm::Persistent_object> Model_recorder::create<omm::Enum_value >( const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const std::string& p_desc, const omm::Element_attributes& p_attrs) const {
    
    return store_enumeration_value (p_name, util::String::stoui(p_parent->id())) ;
}

template <> std::shared_ptr<omm::Persistent_object> Model_recorder::create<omm::Property >( const std::string& p_name, const std::shared_ptr<omm::Packageable_element>& p_parent, const std::string& p_desc, const omm::Element_attributes& p_attrs) const {
    
    const omm::Property_attributes& attrs = dynamic_cast<const omm::Property_attributes&> (p_attrs);
    if (attrs.source_member_end_) {
        return store_member_end (p_name, util::String::stoui(attrs.type_->id()), util::String::stoui(attrs.source_member_end_->id()),util::String::stoui(p_parent->id()), attrs.min_, attrs.max_,  attrs.is_scp_, (attrs.aggregation_ == omm::Aggregation_kind::composite) ,  /*attrs.part_of_id_,*/ p_desc);
    } else {
        
        return store_property (p_name, util::String::stoui(p_parent->id()), util::String::stoui(attrs.type_->id()), attrs.min_, attrs.max_, std::string(), std::string(), /*attrs.part_of_id_,*/ p_desc);
        
    }
    
}

std::shared_ptr<omm::Persistent_object> Model_recorder::store_classifier ( const std::string& p_name, const std::string& p_elem_type, const unsigned int p_pk_id, const std::string& p_desc) const {

    std::shared_ptr<omm::Persistent_object> result ;
    try {
        const std::string name = util::String::standardize(p_name);
        auto r = transPtr_->execSqlSync ( "INSERT INTO am_p_elem (name, elem_type, package) VALUES ($1,$2,$3) RETURNING id", name, p_elem_type, p_pk_id );
        if ( r.size() >0 ) {
            auto row = r[0];
            result = omm::Model_factory::instance().make_persistent_object ( std::to_string(row["id"].as<unsigned int>()), "am_p_elem" );
            
            store_optional_description (p_desc, result);
        }
        else {
            std::string error_msg = "Error in recording classifier " + p_name;
            throw exception::Sql_error ( error_msg );
        }
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

    return result;

}

std::shared_ptr<omm::Persistent_object> Model_recorder::store_primitive ( const std::string& p_name, const unsigned int p_pk_id, const std::string& p_desc, const std::string& p_native_name ) const {

    std::shared_ptr<omm::Persistent_object> result ;
    try {
        const std::string name = util::String::standardize(p_name);
        auto r = transPtr_->execSqlSync ( "INSERT INTO am_p_elem (name, elem_type, package) VALUES ($1,'primitive',$2) RETURNING id", name, p_pk_id );
        if ( r.size() >0 ) {
            auto row = r[0];
            result = omm::Model_factory::instance().make_persistent_object ( std::to_string(row["id"].as<unsigned int>()), "am_p_elem" );

            auto r2 = transPtr_->execSqlSync ( "INSERT INTO am_primitive_type (primitive, native) VALUES ($1,$2) RETURNING primitive", row["id"].as<unsigned int>(), p_native_name );
            if ( r2.size() == 0 ) {
                std::string error_msg = "Error in recording primitive native type for " + p_name;
                throw exception::Sql_error ( error_msg );
            }

            store_optional_description (p_desc, result);
        }
        else {
            std::string error_msg = "Error in recording primitive " + p_name;
            throw exception::Sql_error ( error_msg );
        }
    }
    catch ( const exception::Db_exception &e ) {
          
        throw e;
    }

    return result;

}

template<> void Model_recorder::update<omm::Class> ( const std::shared_ptr<omm::Class>& p_elt, const std::string& p_new_name, const std::string& p_new_desc, const omm::Element_attributes& p_attrs ) const {
    
    update_classifier (p_elt, p_new_name, p_new_desc) ;
    
    const omm::Class_attributes& attrs = dynamic_cast<const omm::Class_attributes&> (p_attrs);
    
    if ( attrs.base_ ) {
        if ( attrs.base_->id() != p_elt->first_base_class()->id() ) {
            std::cout << "WARNING: Update of the class " + p_elt->name() + " : the base class cannot be modified" << std::endl;
        }
    }
    bool update_abstract = attrs.is_abstract_;
    if (attrs.is_abstract_ == true && !p_elt->is_abstract() ) {
        std::cout << "WARNING: Update of the class " + p_elt->name() + " : the class cannot become abstract " << std::endl;
        update_abstract = false;
    }
    
    try {
        if ( update_abstract !=  p_elt->is_abstract() ) {
            auto r = transPtr_->execSqlSync ("UPDATE am_p_elem SET is_abstract = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_elt->id()), update_abstract);
            if ( r.size() == 0 ) {
                std::string error_msg = "Error in updating model_element " + p_elt->name();
                throw exception::Sql_error ( error_msg );
            }
        }
        

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

    
}

template<> void Model_recorder::update<omm::Association> ( const std::shared_ptr<omm::Association>& p_elt, const std::string& p_new_name, const std::string& p_new_desc, const omm::Element_attributes& p_attrs ) const {
    
    update_classifier (p_elt, p_new_name, p_new_desc) ;
}
template<> void Model_recorder::update<omm::Composite_type> ( const std::shared_ptr<omm::Composite_type>& p_elt, const std::string& p_new_name, const std::string& p_new_desc, const omm::Element_attributes& p_attrs ) const {
    
    update_classifier (p_elt, p_new_name, p_new_desc) ;
}
template<> void Model_recorder::update<omm::Primitive_type> ( const std::shared_ptr<omm::Primitive_type>& p_elt, const std::string& p_new_name, const std::string& p_new_desc, const omm::Element_attributes& p_attrs ) const {
    
    update_classifier (p_elt, p_new_name, p_new_desc) ;
}
template<> void Model_recorder::update<omm::Enumeration> ( const std::shared_ptr<omm::Enumeration>& p_elt, const std::string& p_new_name, const std::string& p_new_desc, const omm::Element_attributes& p_attrs ) const {
    
    update_classifier (p_elt, p_new_name, p_new_desc) ;
}

template<> void Model_recorder::update<omm::Enum_value> ( const std::shared_ptr<omm::Enum_value>& p_elt, const std::string& p_new_name, const std::string& p_new_desc, const omm::Element_attributes& p_attrs ) const {
    
    try {

        if ( ! p_new_name.empty()) {
            const std::string new_name = util::String::standardize(p_new_name);
            auto r = transPtr_->execSqlSync ( "UPDATE am_enum_val SET e_value = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_elt->id()), new_name );
            if ( r.size() == 0 ) {
                std::string error_msg = "Error in updating model element " + p_elt->name();
                throw exception::Sql_error ( error_msg );
            }
        } 
        
       
        

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
}

template<> void Model_recorder::update<omm::Property> ( const std::shared_ptr<omm::Property>& p_elt, const std::string& p_new_name, const std::string& p_new_desc, const omm::Element_attributes& p_attrs ) const {
    
   const omm::Property_attributes& attrs = dynamic_cast<const omm::Property_attributes&> (p_attrs);
    
    if (attrs.source_member_end_) {
        if (attrs.min_ != p_elt->min_occurs() ) {
            std::cout << ": Update of the property " + p_elt->name() + " : the cardinality cannot be modified"<< std::endl ;
        }

        if (attrs.max_ != p_elt->max_occurs()) {
            std::cout << "WARNING: Update of the property " + p_elt->name() + " : the cardinality cannot be modified" << std::endl;
        }
    }
    
        
    try {
        if ( ! p_new_name.empty()) {
            const std::string new_name = util::String::standardize(p_new_name);
            auto r = transPtr_->execSqlSync ( "UPDATE am_property SET name = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_elt->id()), new_name );
            if ( r.size() == 0 ) {
                std::string error_msg = "Error in updating model element " + p_elt->name();
                throw exception::Sql_error ( error_msg );
            }
        } 
        
        if (!p_new_desc.empty() ) {
            auto r = transPtr_->execSqlSync ("UPDATE am_property SET description = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_elt->id()), p_new_desc);
            if ( r.size() == 0 ) {
                std::string error_msg = "Error in updating model_element " + p_elt->name();
                throw exception::Sql_error ( error_msg );
            }
        }
        
      
        
        if (attrs.source_member_end_) {
            // member end
            if (attrs.is_scp_ != (p_elt->classifier()->id() != p_elt->association()->id())) {
                auto r = transPtr_->execSqlSync ("UPDATE am_member_end SET c_property = $2 WHERE property = $1 RETURNING property",util::String::stoui(p_elt->id()), attrs.is_scp_ );
                if ( r.size() == 0 ) {
                    std::string error_msg = "Error in updating model_element " + p_elt->name();
                    throw exception::Sql_error ( error_msg );
                }
                
                auto r2 = attrs.is_scp_ ?
                    transPtr_->execSqlSync ("UPDATE am_property SET owning_t = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_elt->id()), util::String::stoui(attrs.source_member_end_->id()) ) :
                    transPtr_->execSqlSync ("UPDATE am_property SET owning_t = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_elt->id()), util::String::stoui(p_elt->association()->id()) ) ;
                    if ( r2.size() == 0 ) {
                        std::string error_msg = "Error in updating model_element " + p_elt->name();
                        throw exception::Sql_error ( error_msg );
                    } ;
            }
            
            if (attrs.aggregation_ != p_elt->aggregation_kind()) {
                auto r = transPtr_->execSqlSync ("UPDATE am_member_end SET is_compo = $2 WHERE property = $1 RETURNING property",util::String::stoui(p_elt->id()), (attrs.aggregation_ == omm::Aggregation_kind::composite));
                if ( r.size() == 0 ) {
                    std::string error_msg = "Error in updating model_element " + p_elt->name();
                    throw exception::Sql_error ( error_msg );
                }
            }
        } else {
            //simple property , parameter "required" can be updated
            if (attrs.min_ != p_elt->min_occurs()) {
                 auto r = transPtr_->execSqlSync ("UPDATE am_property SET min_occurs = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_elt->id()), static_cast<int>(attrs.min_) );
                if ( r.size() == 0 ) {
                    std::string error_msg = "Error in updating model_element " + p_elt->name();
                    throw exception::Sql_error ( error_msg );
                }
            }
        }
        

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

    
}

void Model_recorder::update_classifier ( const std::shared_ptr<omm::Classifier>& p_elt, const std::string& p_new_name, const std::string& p_new_desc ) const {

    try {

        if ( ! p_new_name.empty()) {
            const std::string new_name = util::String::standardize(p_new_name);
            auto r = transPtr_->execSqlSync ( "UPDATE am_p_elem SET name = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_elt->id()), new_name );
            if ( r.size() == 0 ) {
                std::string error_msg = "Error in updating model element " + p_elt->name();
                throw exception::Sql_error ( error_msg );
            }
        } 
        
        if (!p_new_desc.empty() ) {
            auto r = transPtr_->execSqlSync ("UPDATE am_p_elem SET description = $2 WHERE id = $1 RETURNING id",util::String::stoui(p_elt->id()), p_new_desc);
            if ( r.size() == 0 ) {
                std::string error_msg = "Error in updating model_element " + p_elt->name();
                throw exception::Sql_error ( error_msg );
            }
        }
        

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

}


template<> void Model_recorder::deprecate<omm::Class> ( const std::shared_ptr<omm::Class>& p_elt ) const {
    
    deprecate_classifier (p_elt);
}

template<> void Model_recorder::deprecate<omm::Association> ( const std::shared_ptr<omm::Association>& p_elt ) const {
    
    deprecate_classifier (p_elt);
}

template<> void Model_recorder::deprecate<omm::Composite_type> ( const std::shared_ptr<omm::Composite_type>& p_elt ) const {
    
    deprecate_classifier (p_elt);
}

template<> void Model_recorder::deprecate<omm::Primitive_type> ( const std::shared_ptr<omm::Primitive_type>& p_elt ) const {
    
    deprecate_classifier (p_elt);
}

template<> void Model_recorder::deprecate<omm::Enumeration> ( const std::shared_ptr<omm::Enumeration>& p_elt ) const {
    
    deprecate_classifier (p_elt);
}


template<> void Model_recorder::deprecate<omm::Enum_value> ( const std::shared_ptr<omm::Enum_value>& p_elt ) const {
 try {

        auto r = transPtr_->execSqlSync ( "UPDATE am_enum_val SET lc = 'deprecated' WHERE id = $1 RETURNING id",util::String::stoui ( p_elt->id() ) );
        if ( r.size() == 0 ) {
            std::string error_msg = "Error in updating model element" + p_elt->name();
            throw exception::Sql_error ( error_msg );
        }

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

}

template<> void Model_recorder::deprecate<omm::Property> ( const std::shared_ptr<omm::Property>& p_elt ) const {
 try {

        auto r = transPtr_->execSqlSync ( "UPDATE am_property SET lc = 'deprecated' WHERE id = $1 RETURNING id",util::String::stoui ( p_elt->id() ) );
        if ( r.size() == 0 ) {
            std::string error_msg = "Error in updating model element" + p_elt->name();
            throw exception::Sql_error ( error_msg );
        }

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

}
    
    
void Model_recorder::deprecate_classifier ( const std::shared_ptr<omm::Classifier>& p_elt) const {

    try {

        auto r = transPtr_->execSqlSync ( "UPDATE am_p_elem SET lc = 'deprecated' WHERE id = $1 RETURNING id",util::String::stoui ( p_elt->id() ) );
        if ( r.size() == 0 ) {
            std::string error_msg = "Error in updating model element" + p_elt->name();
            throw exception::Sql_error ( error_msg );
        }

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

}

template<> bool Model_recorder::eliminate<omm::Class> ( const std::shared_ptr<omm::Class>& p_elt ) const {
    
    return eliminate_classifier (p_elt);
}

template<> bool Model_recorder::eliminate<omm::Association> ( const std::shared_ptr<omm::Association>& p_elt ) const {
    
    return eliminate_classifier (p_elt);
}

template<> bool Model_recorder::eliminate<omm::Composite_type> ( const std::shared_ptr<omm::Composite_type>& p_elt ) const {
    
    return eliminate_classifier (p_elt);
}

template<> bool Model_recorder::eliminate<omm::Primitive_type> ( const std::shared_ptr<omm::Primitive_type>& p_elt ) const {
    
    return eliminate_classifier (p_elt);
}

template<> bool Model_recorder::eliminate<omm::Enumeration> ( const std::shared_ptr<omm::Enumeration>& p_elt ) const {
    
    return eliminate_classifier (p_elt);
}

template<> bool Model_recorder::eliminate<omm::Enum_value> ( const std::shared_ptr<omm::Enum_value>& p_elt ) const {
    
   try {


        auto r = transPtr_->execSqlSync ( "DELETE from am_enum_val WHERE id = $1 RETURNING id",util::String::stoui ( p_elt->id() ) );
        if ( r.size() == 0 ) {
            std::string warn_msg = "Deletion of model element " + p_elt->name() + " failed";
            std::cout << "WARNING: " << warn_msg<< std::endl;
            return false;
        }

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
    return true;

}

template<> bool Model_recorder::eliminate<omm::Property> ( const std::shared_ptr<omm::Property>& p_elt ) const {
    
   try {


        auto r = transPtr_->execSqlSync ( "DELETE from am_property WHERE id = $1 RETURNING id",util::String::stoui ( p_elt->id() ) );
        if ( r.size() == 0 ) {
            std::string warn_msg = "Deletion of model element " + p_elt->name() + " failed";
            std::cout << "WARNING: " << warn_msg<< std::endl;
            return false;
        }

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
    return true;
}

bool Model_recorder::eliminate_classifier ( const std::shared_ptr<omm::Classifier>& p_elt ) const {

    try {


        auto r = transPtr_->execSqlSync ( "DELETE from am_p_elem WHERE id = $1 RETURNING id",util::String::stoui ( p_elt->id() ) );
        if ( r.size() == 0 ) {
            std::string warn_msg = "Deletion of model element " + p_elt->name() + " failed";
            std::cout << "WARNING: " << warn_msg<< std::endl;
            return false;
        }

    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
    return true;

}



std::shared_ptr<omm::Persistent_object> Model_recorder::store_enumeration_value ( const std::string& p_token, const unsigned int enum_id ) const {

    std::shared_ptr<omm::Persistent_object> result ;

    try {
        const std::string token = util::String::standardize(p_token);

        auto r = transPtr_->execSqlSync ( "INSERT INTO am_enum_val (e_value, e_elem) VALUES ($1,$2) RETURNING id", token, enum_id);
        if ( r.size() >0 ) {
            auto row = r[0];
            result = omm::Model_factory::instance().make_persistent_object ( std::to_string(row["id"].as<unsigned int>()), "am_enum_val" );
        } else {
            std::string error_msg = "Error in recording enum value " + p_token;
            throw exception::Sql_error ( error_msg );
        }
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

    return result;

}

std::shared_ptr<omm::Persistent_object> Model_recorder::store_property ( const std::string& p_name, const unsigned int p_classifier_id, const unsigned int p_type_id,
        const int p_min_occurs, const int p_max_occurs, const std::string& p_computed, const std::string& p_default, /*const bool p_part_of_id,*/ const std::string& p_desc ) const {

    std::shared_ptr<omm::Persistent_object> result ;
    try {
        int choice = 1;
       
        const std::string name = util::String::standardize(p_name);
        qry::Db_result r (nullptr);
        switch ( choice ) {
        case 1:
            r = transPtr_->execSqlSync ( "INSERT INTO am_property (name, p_type, owning_t, min_occurs, max_occurs) VALUES ($1,$2,$3,$4,$5) RETURNING id",  name, p_type_id, p_classifier_id, p_min_occurs, p_max_occurs /*, p_part_of_id */);
            break;
        case 2:
            r = transPtr_->execSqlSync ( "INSERT INTO am_property (name, p_type, owning_t, min_occurs, max_occurs, computed) VALUES ($1,$2,$3,$4,$5,$6) RETURNING id",  name, p_type_id, p_classifier_id, p_min_occurs, p_max_occurs, /*p_part_of_id,*/ p_computed );
            break;
        case 11:
            r = transPtr_->execSqlSync ( "INSERT INTO am_property (name, p_type, owning_t, min_occurs, max_occurs, default_v) VALUES ($1,$2,$3,$4,$5,$6) RETURNING id",  name, p_type_id, p_classifier_id, p_min_occurs, p_max_occurs,/* p_part_of_id,*/ p_default );
            break;
        case 12:
            r = transPtr_->execSqlSync ( "INSERT INTO am_property (name, p_type, owning_t, min_occurs, max_occurs, computed, default_v) VALUES ($1,$2,$3,$4,$5,$6,$7) RETURNING id",  name, p_type_id, p_classifier_id, p_min_occurs, p_max_occurs, /*p_part_of_id,*/ p_computed, p_default );
            break;

        }

        if ( r.size() >0 ) {
            auto row = r[0];
            result = omm::Model_factory::instance().make_persistent_object ( std::to_string( row["id"].as<unsigned int>()), "am_property" );
            
            store_optional_description (p_desc, result);
        }
        else {
            std::string error_msg = "Error in recording property " + name;
            throw exception::Sql_error ( error_msg );
        }
    } 
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

    return result;

    
}

std::shared_ptr<omm::Persistent_object> Model_recorder::store_member_end ( const std::string& p_name, const unsigned int p_pk_elem_target_id,
        const unsigned int p_pk_elem_source_id, const unsigned int p_pk_assoc_id,
        const int p_min_occurs, const int p_max_occurs, const bool p_is_source_class_property, const bool p_is_composition, /*const bool p_part_of_id,*/ const std::string& p_desc ) const {

    const std::string name = util::String::standardize(p_name);
    std::shared_ptr<omm::Persistent_object> result ;
    try {
        auto r = p_is_source_class_property ?
			transPtr_->execSqlSync ( "INSERT INTO am_property (name, p_type, owning_t, min_occurs, max_occurs, is_member_end) VALUES ($1,$2,$3,$4,$5,TRUE) RETURNING id", name, p_pk_elem_target_id, p_pk_elem_source_id, p_min_occurs, p_max_occurs):
			// memberEnd is a property of the association, therefore it is an id property
            transPtr_->execSqlSync ( "INSERT INTO am_property (name, p_type, owning_t, min_occurs, max_occurs, is_member_end) VALUES ($1,$2,$3,$4,$5,TRUE) RETURNING id", name, p_pk_elem_target_id, p_pk_assoc_id,
					p_min_occurs, p_max_occurs);
       
        if ( r.size() >0 ) {
            auto row = r[0];
            result = omm::Model_factory::instance().make_persistent_object ( std::to_string(row["id"].as<unsigned int>()), "am_property" );
            
            store_optional_description (p_desc, result);
            
            r = transPtr_->execSqlSync ("INSERT INTO am_member_end (property, assoc, source, c_property, is_compo) VALUES ($1,$2,$3,$4,$5) RETURNING property",row["id"].as<unsigned int>(), p_pk_assoc_id, p_pk_elem_source_id, p_is_source_class_property, p_is_composition);
            
            if ( r.size() == 0 ) {
                std::string error_msg = "Error in recording attributes of association member " + p_name;
                throw exception::Sql_error ( error_msg );
            }
            
        } 
        else {
            std::string error_msg = "Error in recording association member " + p_name;
            throw exception::Sql_error ( error_msg );
        }
    } 
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }

    return result;

}

void Model_recorder::store_app_domain ( const std::shared_ptr<const omm::Package>& p_pk, const std::shared_ptr<const appcfg::App>& p_app) const {
    
    try {
        auto r = transPtr_->execSqlSync ( "INSERT INTO am_app_domain (a_domain, a_app) VALUES ($1,$2) ",util::String::stoui(p_pk->id()), util::String::stoui(p_app->id()));
    }
    catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
}

void Model_recorder::store_optional_description ( const std::string& p_desc, const std::shared_ptr<const omm::Persistent_object>& p_elem ) const {
    if ( p_desc.length() >0 ) {
        try {
            auto r = transPtr_->execSqlSync ( "UPDATE " + p_elem->id_scope() + " SET description = $1 WHERE id = $2 RETURNING id",p_desc, util::String::stoui(p_elem->id()) );
            if (!(r.size() > 0)) {
                std::string error_msg = "Error in recording the description:" + p_desc ;
                throw exception::Sql_error ( error_msg );
            }
        } catch ( const exception::Db_exception &e ) {
             
            throw e;
        }
    }
}

void Model_recorder::update_translation (const std::shared_ptr<omm::Persistent_object>& p_elem, const std::string& p_lang_id, const std::string& p_translation) const {
    
     try {
        const std::string table = p_elem->id_scope() + "_tlt";
        const std::string stmt = "INSERT INTO " + table + " (t_elem,t_lang, t_name) VALUES ($1,$2,$3) ON CONFLICT (t_elem,t_lang) DO UPDATE SET t_name = $3 RETURNING id";
        auto r = transPtr_->execSqlSync ( stmt,util::String::stoui(p_elem->id()), util::String::stoui(p_lang_id), p_translation );

        if ( r.size() == 0) {

            std::string error_msg = "Error in recording translation of element " + p_elem->id() + " for language " + p_lang_id;
            throw exception::Sql_error ( error_msg );
        }
    } catch ( const exception::Db_exception &e ) {
         
        throw e;
    }
}

void Model_recorder::update_identifier ( const std::shared_ptr<omm::Classifier>& p_csf, const std::unordered_map<std::string,std::shared_ptr<omm::Property>>& p_ident_map) const {
    
    //clear identifier
    reset_identifier(p_csf);
    
    //save new identifier
    int csf_id = util::String::stoui(p_csf->id());

    std::string stmt = "INSERT INTO am_csf_ident (i_csf, i_prop) VALUES ($1, $2) RETURNING i_csf";
    
    for (auto [name, prop] : p_ident_map) {
        auto r = transPtr_->execSqlSync(stmt,csf_id, util::String::stoui(prop->id()));
        int nb_r = r.size();
        if ( nb_r != 1 ) {
            std::string error_msg = "Error in recording the property " + name + " for identifier of class " + p_csf->name();
             
            throw exception::Sql_error ( error_msg );
        }
    }
}

void Model_recorder::reset_identifier ( const std::shared_ptr<omm::Classifier>& p_csf) const {
    
    int csf_id = util::String::stoui(p_csf->id());
    //clear identifier
    std::string stmt = "DELETE FROM am_csf_ident WHERE i_csf = $1 RETURNING i_csf";
    auto r = transPtr_->execSqlSync(stmt, csf_id);
    

}
void Model_recorder::clear_config_collection_props (const std::shared_ptr<omm::Package>& p_domain) {
    const std::string stmt = "DELETE FROM am_app_prop WHERE a_csf IN  (SELECT id FROM am_p_elem WHERE package = $1) RETURNING a_prop";
    auto r = transPtr_->execSqlSync(stmt, p_domain->id());
    
}
    
    
void Model_recorder::update_config_collection_props ( const std::shared_ptr<appcfg::App_domain>& p_app_dom ) {
    
    clear_config_collection_props (p_app_dom->domain());
    
    const std::string stmt = "INSERT INTO am_app_prop ( a_prop, a_csf, a_app) VALUES ( $1, $2, " + p_app_dom->app()->id() + ") RETURNING a_prop" ;
    
    for ( auto [name, app_csf] : p_app_dom->app_classifiers() ) {
        const std::string csf_id = app_csf->classifier()->id();
        for ( auto [name, prop ] : app_csf->collection_props () ) {
            auto r = transPtr_->execSqlSync ( stmt, prop->id(), csf_id );
            int nb_r = r.size();
            if ( nb_r != 1 ) {
                std::string error_msg = "Error in recording the property " + name + " for configuration of classifier " + app_csf->classifier()->name();
                 
                throw exception::Sql_error ( error_msg );
            }
        }
    }
}



