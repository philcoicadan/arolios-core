/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Package.h>
#include <omm/Class.h>
#include <omm/Property.h>
#include <omm/Class_attributes.h>
#include <model/xam.h>
#include <model/Model_composite.h>
#include <model/Classifier_search.h>
#include "Model_composite.tpp"
#include <model/Classifier_search.h>
#include <exception/Exceptions.h>
#include <util/String.h>
#include <model/Util.h>











namespace arolios::model {


template <> void Model_element<Class_t, omm::Class >::fill_children() {
 
    

    	// properties
    Model_composite<Class_property_t, omm::Property, Model_element> container (*this);
	for (Class_t::property_const_iterator i (this->xml_obj_.property().begin ());
			i != this->xml_obj_.property ().end ();
			++i)
	{
        
		container.insert_child (*i);

	}

}

template <> void Model_element<Class_t, omm::Class >::execute () {
    
    //check base 
    std::string base = (xml_obj_.base().present() ) ? xml_obj_.base().get() : "";
    auto && pk = std::dynamic_pointer_cast<omm::Package>(parent_element_.stored_element());
    Classifier_search search ( base, pk ) ;
    auto stored_base = search.execute<omm::Class> () ;
    
    // check acyclic inheritance
    if (this->stored_ && stored_base) {
         if ( Util::check_inheritance ( stored_base, this->stored_) ) {
             std::string error_msg = "cyclic inheritance with " + stored_base->name() + " and " + this->stored_->name();
              
             throw exception::Model_error (error_msg) ;
         }
    }
        
        
        

  
    
    omm::Class_attributes c_attrs;
    c_attrs.is_abstract_ = xml_obj_.isAbstract();
    c_attrs.base_= std::dynamic_pointer_cast<omm::Class>(stored_base);
    
    internal_execute (c_attrs) ;
    
	

	this->execute_children();
    
    // identifier
    
    if (xml_obj_.identifier().present()) {
         auto ident = xml_obj_.identifier().get();
         
         std::unordered_map<std::string,std::shared_ptr<omm::Property>> ident_map;
         auto pers_cls = this->stored_;
         
         if (pers_cls == nullptr ) {
             std::string warn_msg = "Class " + xml_obj_.name() + " is not saved, identifier cannot be taken into account";
             std::cout << "WARNING: " << warn_msg<< std::endl;
             
         } else {
    
            for (Class_identifier_t::property_const_iterator i (ident.property().begin ());
                i != ident.property ().end ();
                ++i)
            {
        
                Util::add_identifier (*i, pers_cls, ident_map);
            }
            
            for (Class_identifier_t::memberEnd_const_iterator i (ident.memberEnd().begin ());
                i != ident.memberEnd ().end ();
                ++i)
            {
            
                Util::add_identifier (*i, pers_cls, ident_map);

            }

            if ( ident_map.size() > 0 ) {
                // check one is not null
                bool has_not_null = false;
                for ( auto [name, prop] : ident_map ) {
                    if ( prop->min_occurs() > 0 ) {
                        has_not_null = true;
                        break;
                    }
                }
                if ( has_not_null ) {
                    Model_recorder mdl_rcd;
                    try {
                        mdl_rcd.update_identifier ( pers_cls, ident_map );

                    } catch ( const exception::Db_exception &e ) {
                        mdl_rcd.rollback();
                         
                        throw e;
                    } catch ( const exception::Base_exception &e ) {
                        mdl_rcd.rollback();
                         
                        throw e;
                    } catch ( std::exception const &e ) {
                        mdl_rcd.rollback();
                         
                        throw e;
                    }

                    pers_cls->identifier ( ident_map );
                    
                } else {
                    std::string error_msg = "The identifier of the class " + pers_cls->name() + " cannot be saved because there is no required property ";
                     
                    throw exception::Failure ( error_msg );
                }
            }
        }
    }
}


template class Model_element<Class_t, omm::Class > ;

template class Model_composite<Class_property_t, omm::Property, Model_element>;


}

