/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Namespace.h>
#include <omm/Named_element.h>
#include <omm/Language.h>



using namespace arolios;
using namespace arolios::omm ;


char Named_element::separator() {
    
    static char sep = '.';
    return sep;
}

const std::string Named_element::qualified_name () const {

	if (own_namespace() == nullptr) {
		return name_;
	}
	else {
		return own_namespace()->qualified_name() + separator() + name_ ;
	}
}
std::shared_ptr<const std::vector<std::shared_ptr<Namespace>>> Named_element::all_namespaces() const {
	std::shared_ptr<std::vector<std::shared_ptr<Namespace>>> result = nullptr;
	if (own_namespace()) {
		std::shared_ptr<const std::vector<std::shared_ptr<Namespace>>> parents = own_namespace()->all_namespaces();
		if (! parents->empty()) {
			result->insert(result->end(), parents->begin(), parents->end());
		}
		result->push_back(own_namespace());
	}

	return result;

}

const std::string Named_element::translated_name (const std::shared_ptr<const Language>& p_lang) const {
    
    auto translation = translations_.translation(p_lang->code());
    if (translation.empty()) {
        return name_;
    } else {
        return translation;
    }
}

void Named_element::create_or_update_translation (const std::shared_ptr<const Language>& p_lang, const std::string& p_translation)  {
    translations_.create_or_update_translation(p_lang->code(), p_translation);
}

