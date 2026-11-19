/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Data_type.h"
#include <util/Native_type.h>

namespace arolios {


namespace omm {
	class Primitive_type : public virtual Data_type {
	public:

		virtual ~Primitive_type () {};
		Primitive_type () = default;
		Primitive_type ( const Primitive_type& ) = default;

		virtual bool has_properties () const override;
		virtual bool has_generalizations () const override;
		virtual void accept (Visitor& p_visitor) const override;

        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;

        //virtual void insert_element ( const std::shared_ptr<Named_element>& p_elem) override;
        //virtual void erase_element ( const std::shared_ptr<Named_element>& p_elem) override;

        //virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override ;

        virtual const std::string& meta_type() const override;

     	//virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) override;

        virtual void native_type (const util::Native_type p_type) ;
        virtual const util::Native_type native_type () const ;

	private:

        util::Native_type native_type_ ;

	};
}


}
