/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Data_type.h"

namespace arolios {


namespace omm {
	class Restriction_type : public virtual Data_type {
	public:

		virtual ~Restriction_type () {};
		Restriction_type () = default;
		Restriction_type ( const Restriction_type& ) = default;

		virtual bool has_properties () const override;
		virtual bool has_generalizations () const override;
        virtual bool has_elements (bool p_active_only = true) const override;
		virtual void accept (Visitor& p_visitor) const override;
        
       // virtual std::shared_ptr<Logical_struct> logical_struct() const override;
        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;

        virtual void insert_element ( const std::shared_ptr<Named_element>& p_elem) override;
        virtual void erase_element ( const std::shared_ptr<Named_element>& p_elem) override;
              
        virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override ;

        virtual const std::string& meta_type() const override;

    	virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) override;

        virtual void referenced_type(const std::shared_ptr<Data_type>& p_type);
        virtual const std::shared_ptr<Data_type>& referenced_type() const;


	private:

        std::shared_ptr<Data_type> referenced_type_;

        
	};
}
 

}

