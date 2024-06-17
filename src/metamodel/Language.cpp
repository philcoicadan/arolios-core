/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Language.h>
#include <omm/Visitor.h>

using namespace arolios;
using namespace arolios::omm;

Language::Language(const std::string& p_code) : code_ (p_code), lc_status_(Lc_status::active) {

}

Language::~Language() {

}

const std::string& Language::code() const {
    return code_;
}

    
Lc_status Language::lc_status() const  {
    return lc_status_;
}


void Language::lc_status ( const Lc_status p_status ) {
    lc_status_ = p_status;
}

void Language::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

std::shared_ptr<Namespace> Language::own_namespace() const {
    return nullptr;
}

const std::string& Language::fts_config() const {
    return fts_config_;
}

void Language::fts_config ( const std::string& p_fts_config) {
    fts_config_ = p_fts_config;
}

void Language::own_namespace( const std::shared_ptr<Namespace>& p_parent) {
    
    // nothing to do , no namespace

}
