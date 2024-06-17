/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Translations.h"
#include "Element.h"

namespace arolios {





namespace omm {
	class Namespace;
    class Language;

	class Named_element : public virtual Element {
	public:
		Named_element () { } ;
		Named_element ( const Named_element& ) = default;
		virtual ~Named_element () {};

		virtual const std::string qualified_name () const ;
		virtual const std::string& name() const  { return name_;} ;
		virtual void name (const std::string& p_name)  { name_ = p_name;};
        virtual const std::string translated_name (const std::shared_ptr<const Language>& p_lang) const;

        virtual void create_or_update_translation (const std::shared_ptr<const Language>& p_lang, const std::string& p_translation) ;
		
		virtual std::shared_ptr<Namespace> own_namespace() const = 0;
		virtual std::shared_ptr<const std::vector<std::shared_ptr<Namespace>>> all_namespaces() const ;

        static char separator() ;

        virtual  void own_namespace(const std::shared_ptr<Namespace>& p_ns) = 0;

	private:

		std::string name_;
        Translations translations_;


	};

}

}

