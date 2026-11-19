/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Classifier.h"

namespace arolios {


namespace omm {
	class Collection_attributes;
	class Specific_type : public virtual Classifier {
	public:

		virtual ~Specific_type () {};
		Specific_type () = default;
		Specific_type ( const Specific_type& ) = default;

		virtual bool has_properties () const override;
		virtual bool has_generalizations () const override;
        virtual bool has_id () const  override;

        //virtual bool has_elements (bool p_active_only = true) const override;
		virtual void accept (Visitor& p_visitor) const override;
        
        virtual std::shared_ptr<Logical_struct> logical_struct() const override;
        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;

        //virtual void insert_element ( const std::shared_ptr<Named_element>& p_elem) override;
        //virtual void erase_element ( const std::shared_ptr<Named_element>& p_elem) override;
              

        virtual const std::string& meta_type() const override;

    	//virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) override;

        virtual void referenced_classifier(const std::shared_ptr<const Classifier>& p_type);
        virtual const std::shared_ptr<const Classifier>& referenced_classifier() const;

        virtual void collection(const std::shared_ptr<Collection_attributes>& p_collection);
        virtual const std::shared_ptr<Collection_attributes>& collection() const;


        virtual std::shared_ptr<Table> table() const override;
        virtual std::shared_ptr<Table> managing_table() const override; 
        virtual std::string read_select_template(const bool p_force = false ) override;
        virtual void make_more_specific_ids ( const bool p_force =true ) override;

        template<typename T> static bool reference_is_a( const std::shared_ptr<const Classifier>& p_cls) ;

	private: 

        std::shared_ptr<const Classifier> referenced_classifier_;
        std::shared_ptr<Collection_attributes> collection_;

        
	};
}
 

}

