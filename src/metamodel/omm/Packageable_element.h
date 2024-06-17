/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Named_element.h"
#include "Persistent_object.h"
#include <unordered_set>
#include "Lc_status.h"

namespace arolios {





namespace omm {
	class Package;
    class Constraint;
	class Packageable_element : public virtual Named_element, public virtual Persistent_object {
	public:

		Packageable_element () { //
        } ;
		Packageable_element ( const Packageable_element& ) = default;
		virtual ~Packageable_element() {};
		virtual std::shared_ptr<Package> package() const =0;
        
        template <typename T> std::shared_ptr<T> parent_element() const {
            return  std::dynamic_pointer_cast<T>(own_namespace());
        }

        virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) = 0;
       
        virtual std::unordered_set<std::shared_ptr<Constraint>>& constraints() ;

        virtual Lc_status lc_status() const ;

        virtual void lc_status (const Lc_status p_status);
        
        virtual void recursive_lc_status (const Lc_status p_status) ;
        
        
        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) = 0;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) = 0;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) =0;

	private:

        std::unordered_set<std::shared_ptr<Constraint>> constraints_;

        Lc_status lc_status_;


	};
}

}

