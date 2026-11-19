/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Classifier.h"

namespace arolios {





namespace omm {
	class Operation;
    class Table;
	class Class : public virtual Classifier {
	public:

		Class (const bool p_is_abstract = false) ;
		Class ( const Class& ) = default;
		virtual ~Class () {};
		virtual void accept (Visitor& p_visitor) const override;
        virtual bool has_id () const  override;
        virtual std::shared_ptr<Logical_struct> logical_struct() const override;
        virtual std::shared_ptr<Table> table() const override;
        virtual std::shared_ptr<Table> managing_table() const override ;
        virtual void table (const std::shared_ptr<Table>& p_table) ;
        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;

        virtual std::string read_select_template(const bool force = false ) override;
        virtual std::shared_ptr<Property> find_property_member_end( const std::string& p_prop_key_name) const ;
        virtual void make_more_specific_ids (const bool force = true ) override;
        
        virtual std::shared_ptr<omm::Class> first_base_class () const ;
        

        virtual const std::string& meta_type() const override;
        
        //virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override ;



        
	private:
    
        virtual std::string build_select_template () ;
        
        std::shared_ptr<Table> table_;
        std::string select_tpt_;

	};
}


}

