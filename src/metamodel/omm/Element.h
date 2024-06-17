/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>

namespace arolios {




namespace omm {

	class Visitor;
	class Element {
	public:

		Element () {};
		virtual ~Element () {} ;

        
        virtual const std::string& description () const;
        virtual void description (const std::string& p_desc) ;

		virtual void accept (Visitor& p_visitor) const = 0;
	protected:

	private:

        std::string description_ ;

	};

}


}

