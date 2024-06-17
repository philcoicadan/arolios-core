/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include "Named_element.h"

namespace arolios {






namespace omm {
	class Namespace : public virtual Named_element {
	public:

		Namespace () = default;
		Namespace ( const Namespace& ) = default;
		virtual ~Namespace () {};


        virtual void insert_element (const std::shared_ptr<Named_element>& p_elem) = 0;
        virtual void erase_element (const std::shared_ptr<Named_element>& p_elem) = 0;
        virtual bool has_elements (bool p_active_only = true ) const = 0;
        virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const = 0;
        template <typename T> std::shared_ptr<T> find_element_by_name (const std::string& p_name) const {
            auto elem = find_element (p_name) ;
            return std::dynamic_pointer_cast<T>( elem) ;
        }

	private:

	};

}

}

