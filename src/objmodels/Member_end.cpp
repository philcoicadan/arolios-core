/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <model/Model_composite.h>
#include "Model_composite.tpp"
#include <omm/Enumeration.h>
#include <omm/Specific_type.h>
#include <omm/Property.h>
#include <omm/Association.h>
#include <omm/Package.h>
#include <omm/Property_attributes.h>
#include <model/Classifier_search.h>
#include <model/xam.h>
#include <exception/Exceptions.h>








namespace arolios::model {
    



template <> void Model_element<Member_end_t, omm::Property>::fill_children() {
 
   // no child
}

template <> void Model_element<Member_end_t, omm::Property>::execute ()  {
    
		

    Classifier_search  search (xml_obj_.class_(), parent_element_.stored_element()->package());
    auto target = search.execute<omm::Classifier> ();
    auto targetClass = std::dynamic_pointer_cast<omm::Class>(target);
    bool specificClass = omm::Specific_type::reference_is_a<omm::Class>(target);
    
     if (! targetClass && !specificClass) {
        std::string error_msg = "Member end type " + xml_obj_.class_() + " is not valid" ;
            throw exception::Model_error ( error_msg );
     }

    //maxOccurs must be greater or equal to minOccurs

        const std::string min_str = xml_obj_.minOccurs();
        const int min = (min_str == "zero") ? 0: 1;
        const std::string max_occurs_str = xml_obj_.maxOccurs();
        const int max_occurs_i = ( max_occurs_str != "many" ) ? 1 : -1;

		if (max_occurs_i != -1 && max_occurs_i < min) {
			std::string error_msg = "Association" + parent_element_.standard_name()  + ", member: " + xml_obj_.name()+ " maxOccurs must be greater or equal to minOccurs";
			throw exception::Model_error ( error_msg );
			
		}

		
	
		
		if (
              (min != 0) &&
              (max_occurs_i != 1) )  {
			std::string warn_msg = "WARNING: Association" + parent_element_.standard_name()  + ", member: " + xml_obj_.name()+ " cardinality not managed: min of occurrences is different from  zero and max of occurrences is different from one. Min of occurrences is managed as zero";
			std::cout << "WARNING: " << warn_msg << std::endl;
			
		}


    
    omm::Property_attributes attrs;
    attrs.type_ = target;
    attrs.min_ = min;
    attrs.max_ = max_occurs_i;
    attrs.aggregation_ = (xml_obj_.isComposition()) ? 
        omm::Aggregation_kind::composite : 
        omm::Aggregation_kind::none;
    attrs.is_scp_ = !xml_obj_.owned();
    attrs.is_navigable_ = xml_obj_.navigable();

   
    // take the class_ of the opposite member end as source
    const Model_element<Association_t, omm::Association>& assoc_elt = dynamic_cast<const Model_element<Association_t, omm::Association>&> (parent_element_) ;

    std::shared_ptr<omm::Class> opposite_class;
    const auto& ends = assoc_elt.xml_object().memberEnd();
    for ( std::size_t k = 0; k < ends.size(); ++k ) {
        if ( ends[k].name() != xml_obj_.name() ) {
            Classifier_search  search ( ends[k].class_(), parent_element_.stored_element()->package() );
            opposite_class = search.execute<omm::Class> ();
            break;
        }
    }
    if ( ! opposite_class ) {
        // the opposite member end is inherited from a base association
        auto stored_assoc = std::dynamic_pointer_cast<omm::Association> ( parent_element_.stored_element() );
        if ( stored_assoc ) {
            auto base = stored_assoc->first_base_association();
            if ( base ) {
                auto members = base->member_ends();
                for ( auto && me : *members ) {
                    if ( me->name() != xml_obj_.name() ) {
                        opposite_class = std::dynamic_pointer_cast<omm::Class> ( std::const_pointer_cast<omm::Classifier>( me->type() ) );
                        break;
                    }
                }
            }
        }
    }
    attrs.source_member_end_ = opposite_class;

    internal_execute ( attrs ) ;
}

template class Model_element<Member_end_t, omm::Property> ;

template class Model_composite<Member_end_t, omm::Property, Model_element>;


}

