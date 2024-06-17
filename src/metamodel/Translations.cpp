/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Translations.h>

using namespace arolios;
using namespace arolios::omm;


Translations::Translations() {

}

Translations::~Translations() {

}


std::string Translations::translation (const std::string& p_lang_code) const {

    auto search = translations_.find(p_lang_code);
    if ( search == translations_.end()) {
        return std::string ();
    }
    return search->second;
}
void Translations::create_or_update_translation (const std::string& p_lang_code, const std::string& p_translation) {
    translations_.insert_or_assign (p_lang_code, p_translation);
}

void Translations::delete_translation (const std::string& p_lang_code) {
    translations_.erase (p_lang_code);
}
     
