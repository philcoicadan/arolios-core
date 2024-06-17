/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Relationship.h"
#include "Classifier.h"
#include "Package.h"
#include <memory>

namespace arolios {







namespace omm {
	class Property;
    class Table;
    class Column;
    
	class Association : public virtual Relationship, public virtual Classifier {
	public:

		Association () {member_end_[0]= nullptr; member_end_[1]= nullptr;};

		virtual ~Association () {};
		virtual std::shared_ptr<const std::vector<std::shared_ptr<Element>>> related_elements() const override;
		virtual std::shared_ptr<const std::vector<std::shared_ptr<Property>>> member_ends() const;
		virtual void member_end ( const std::shared_ptr<Property>&  p_member_end);
		virtual void accept (Visitor& p_visitor) const override;
        virtual std::shared_ptr<Logical_struct> logical_struct() const override;

        virtual std::shared_ptr<Table> table() const override ;
        virtual std::shared_ptr<Table> managing_table() const override ;
        
        virtual std::shared_ptr<const Class> managing_table_class () const;
        virtual void managing_table_class ( const std::shared_ptr<const Class>& p_class) ;
        
        virtual std::shared_ptr<Column> managing_table_column () const;

        virtual void table (const std::shared_ptr<Table>& p_table) ;
        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;

        virtual std::string read_select_template( const bool p_force = false ) override;
        virtual std::shared_ptr<Property> find_element_in_all_properties (const std::string& p_name) const override;

        virtual void make_all_elements_as_property (const bool p_force = true ) override;
        virtual void make_more_specific_ids (const bool p_force = true ) override;



       virtual std::unordered_map<std::string,std::shared_ptr<Property>> all_id_properties () const override;
               
        virtual std::shared_ptr<Property> opposite_member_end (const std::shared_ptr<const Property>& p_prop) const;
        
        virtual std::shared_ptr<Property> find_member_end (const std::string& p_name) const;

        virtual bool has_member (const std::shared_ptr<const Property>& p_prop) const;
        

        virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override ;

        virtual bool has_elements (bool p_active_only = true) const override;
        virtual void insert_element (const std::shared_ptr<Named_element>& p_elem) override;
        virtual void erase_element (const std::shared_ptr<Named_element>& p_elem) override;

        virtual const std::string& meta_type() const override;

        virtual bool has_required_property(const std::shared_ptr<const Property>& p_prop) const override;

	private:
        virtual std::string build_select_template () ;

		std::shared_ptr<Property> member_end_[2];
        std::shared_ptr<Table> table_;
        std::shared_ptr<const Class> managing_table_class_;
        std::string select_tpt_;

	};
}

}

