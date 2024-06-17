/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Model_element.h>
#include "Model_element.tpp"
#include <omm/Enumeration.h>
#include <omm/Property.h>
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
    auto target = search.execute<omm::Class> ();
    
    
    
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
    attrs.is_scp_ = true;  
   
    // take the class_ of the opposite as source
    const Model_element<Association_t, omm::Association>& assoc_elt = dynamic_cast<const Model_element<Association_t, omm::Association>&> (parent_element_) ;
        
    const Member_end_t& m1 = assoc_elt.xml_object().memberEnd()[0];
    if (m1.name() != xml_obj_.name()) {
        Classifier_search  search (m1.class_(), parent_element_.stored_element()->package());
        attrs.source_member_end_  =  search.execute<omm::Class> ();
    } else {
        const Member_end_t& m2 = assoc_elt.xml_object().memberEnd()[1];
         Classifier_search  search (m2.class_(), parent_element_.stored_element()->package());
        attrs.source_member_end_ =  search.execute<omm::Class> ();
    }

    internal_execute ( attrs ) ;
}

template class Model_element<Member_end_t, omm::Property> ;

}

