/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Package.h>
#include <omm/Association.h>
#include <omm/Property.h>
#include <model/xam.h>
#include <model/Model_composite.h>
#include <exception/Exceptions.h>
#include <model/Util.h>
#include "Model_composite.tpp"










namespace arolios::model {



template <> bool Model_composite<Member_end_t,omm::Property,Model_element>::check_action_compatibility( const Member_end_t& p_type) const {
    
    if ( (parent_.action() == Element_action::a_delete ) ||  ( parent_.action() == Element_action::a_create && p_type.action() != "create" ) || ( parent_.action() == Element_action::a_update && p_type.action() != "update" ) ) {
        return false ;
    } else {
        return true;
    }
}

    
 

template <> void Model_element<Association_t, omm::Association >::fill_children() {



    // properties
    Model_composite<Class_property_t, omm::Property, Model_element> container ( *this );
    for ( Association_t::property_const_iterator i ( this->xml_obj_.property().begin () );
            i != this->xml_obj_.property ().end ();
            ++i ) {

        container.insert_child ( *i );

    }

    int count = xml_obj_.memberEnd().size();

    if ( action_ != Element_action::a_delete ) {
        // create or update action , check the consistency of the members ( 2 members are required)
        if ( count != 2 ) {
            throw exception::Model_error ( std::to_string(count) + "-ary association  is not authorized" );

        }
       

        const Member_end_t* members[2] = {&xml_obj_.memberEnd() [0], &xml_obj_.memberEnd() [1]};


        // if association implemented in the table of the target class of one of its member ends, the order of insertion is important: firstly the member_end whose the target class IS NOT the managing table class

        Model_composite<Member_end_t, omm::Property, Model_element> cont_me ( *this );


        if ( this->stored()->table() ) {
            // the order is indifferent

            cont_me.insert_child ( * members[0] );
            cont_me.insert_child ( * members[1] );

        } else {

            if ( members[0]->maxOccurs() == "many" && members[1]->maxOccurs() == "one" ) {
                // one-to-many
                // assoc implemented in the target of the member[0]
                cont_me.insert_child ( * members[1] );
                cont_me.insert_child ( * members[0] );

            } else if ( members[0]->maxOccurs() == "one" && members[1]->maxOccurs() == "many" ) {
                // many-to-one
                // assoc implemented in the target of the member[1]
                cont_me.insert_child ( * members[0] );
                cont_me.insert_child ( * members[1] );


            } else if ( members[0]->maxOccurs() == "one" && members[1]->maxOccurs() == "one" ) {
                // one-to-one
                if ( members[1]->minOccurs() == "one" && members[0]->minOccurs() == "zero" ) {
                    // case #1: 1,1 <-> 0,1
                    //assoc implemented in the target of the member[0]
                    cont_me.insert_child ( * members[1] );
                    cont_me.insert_child ( * members[0] );


                } else if ( members[0]->minOccurs() == "one" && members[1]->minOccurs() == "zero" ) {
                    //case #2: 0,1 <-> 1,1
                    //assoc implemented in the target of the member[1]
                    cont_me.insert_child ( * members[0] );
                    cont_me.insert_child ( * members[1] );

                } else if ( members[0]->minOccurs() == "zero" && members[1]->minOccurs() == "zero" ) {
                    // case # 3: 0,1 <-> 0,1 => previous case without the constraint "required"
                    //assoc implemented in the target of the member[1]
                    cont_me.insert_child ( * members[0] );
                    cont_me.insert_child ( * members[1] );

                } else if ( members[0]->minOccurs() == "one" && members[1]->minOccurs() == "one" ) {
                    // case #4: 1,1 <-> 1,1 this case should not be happened. It cannot be implemented because the referential integrity. Decision to implement the case #2
                    cont_me.insert_child ( * members[0] );
                    cont_me.insert_child ( * members[1] );
                }

            }

        }


    } else {

        if ( count > 0 ) {
            throw exception::Model_error ( "Declaration of member ends is not authorized when deleting association" );
        }


    }
}

template <> void Model_element<Association_t, omm::Association >::execute () {

    int count = xml_obj_.memberEnd().size();


    if ( action_ != Element_action::a_delete ) {
        // create or update action , check the consistency of the members ( 2 members are required)
        if ( count != 2 ) {
            throw exception::Model_error ( std::to_string(count) + "-ary association  is not authorized" );

        }


        const Member_end_t* members[2] = {&xml_obj_.memberEnd() [0], &xml_obj_.memberEnd() [1]};

    // decode maxOccurs
        int max_occurs_i[2] = {-1, -1};
        std::string max_occurs_str;
        for ( int k=0; k<2; ++k ) {
            max_occurs_str= members[k]->maxOccurs();
            if ( max_occurs_str != "many" ) {
                max_occurs_i [k]= 1;
            }
        }
        // check if many-many association, no one is a composition
        for ( int k=0; k<2 && max_occurs_i[0] == -1 && max_occurs_i[1] == -1 ; ++k ) {
            if ( members[k]->isComposition() ) {
                std::string error_msg = "Association" + xml_obj_.name()  + "is many-many and member: " + members[k]->name()+ " is a composition"  ;
                throw exception::Model_error ( error_msg );
                break;
            }
        }

        for ( int k=0; k<2; ++k ) {
            //check that of one end is composite the other one is not.
            if ( members[k]->isComposition() && members[1-k]->isComposition() ) {
                std::string error_msg = "Association" + xml_obj_.name()  + " only one member can be a composition" ;
                throw exception::Model_error ( error_msg );
                break;
            }
        }

   

        std::string member_names[2] = { members[0]->name(),
                                        members[1]->name()
                                      };

        for ( int k=0; k<2; ++k ) {
            member_names [k] = util::String::standardize ( member_names [k] );
        }

        // check member names
        if ( member_names[0] == member_names[1] ) {
            std::string error_msg = "Association" + xml_obj_.name()  + ", member names are similar" ;
            throw exception::Model_error ( error_msg );
        }

      

        // if both minOccurs >0 , creation of a common object is not possible.
        const std::string min_str0 = members[0]->minOccurs();
        const int min0 = ( min_str0 == "zero" ) ? 0: 1;
        const std::string min_str1 = members[1]->minOccurs();
        const int min1 = ( min_str1 == "zero" ) ? 0: 1;
        if ( min0 >0  && min1 >0 ) {
            std::string error_msg = "Association" + xml_obj_.name()  + ", both members have at least one occurrence, creation of both objects is not possible simultaneaously";
            throw exception::Model_error ( error_msg );
        }

      } else {

        if ( count > 0 ) {
            throw exception::Model_error ( "Declaration of member ends is not authorized when deleting association" );
        }

      }

    omm::Element_attributes attrs;
    
    this->internal_execute(attrs);
    
	this->execute_children();
    
    // identifier

    if (xml_obj_.identifier().present()) {
         auto ident = xml_obj_.identifier().get();
         
         std::unordered_map<std::string,std::shared_ptr<omm::Property>> ident_map;
         auto pers_assoc = this->stored_;
         
         if (pers_assoc == nullptr ) {
             std::string warn_msg = "Association " + xml_obj_.name() + " is not saved, identifier cannot be taken into account";
             std::cout << "WARNING: " << warn_msg<< std::endl;
             
         } else {
    
            for (Classifier_identifier_t::property_const_iterator i (ident.property().begin ());
                i != ident.property ().end ();
                ++i)
            {
        
                Util::add_identifier (*i, pers_assoc, ident_map);
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
                        mdl_rcd.update_identifier ( pers_assoc, ident_map );

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

                    pers_assoc->identifier ( ident_map );
                    
                } else {
                    std::string error_msg = "The identifier of the association " + pers_assoc->name() + " cannot be saved because there is no required property ";
                     
                    throw exception::Failure ( error_msg );
                }
            }
        }
    }
}

template class Model_element<Association_t, omm::Association > ;

template class Model_composite<Member_end_t, omm::Property, Model_element>;


}

