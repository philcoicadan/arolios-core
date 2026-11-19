/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Data_type.h"

namespace arolios {



namespace omm {
    class Logical_enum;
    class Enum_value;
    
	class Enumeration : public virtual Data_type {
	public:

		const std::unordered_set<std::string> values () const;
		virtual ~Enumeration () {};
		Enumeration () = default;
		Enumeration ( const Enumeration& ) = default;

		virtual bool has_properties () const override;
		virtual bool has_generalizations () const override;
        virtual bool has_elements (bool p_active_only = true) const override;
		virtual void accept (Visitor& p_visitor) const override;
        
        virtual void logical_enum (const std::shared_ptr<Logical_enum>& p_enum) ; 

        virtual const std::shared_ptr<Logical_enum>& logical_enum () const;
        
        virtual std::shared_ptr<Logical_struct> logical_struct() const override;
        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;

        
        virtual void insert_element ( const std::shared_ptr<Named_element>& p_elem) override;
        virtual void erase_element ( const std::shared_ptr<Named_element>& p_elem) override;
              
		virtual const std::unordered_map<std::string,std::shared_ptr<Enum_value>>& enum_values () const { return values_ ;};
        
        virtual void recursive_lc_status(const omm::Lc_status p_status) override;

        virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const  override;

        virtual const std::string& meta_type() const override;

    	virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) override;

	private:

        std::unordered_map<std::string,std::shared_ptr<Enum_value>> values_;
        std::shared_ptr<Logical_enum> logical_enum_;
        
	};
}

}

