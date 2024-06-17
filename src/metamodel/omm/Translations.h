/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <unordered_map>
#include<string>

namespace arolios {

namespace omm {

    class Translation;
    
class Translations {
public:
    /**
     * Constructor
     */
    Translations();

    /**
     * Destructor
     */
    virtual ~Translations();
    
     virtual std::string translation (const std::string& p_lang_code) const ;
     virtual void create_or_update_translation (const std::string& p_lang_code, const std::string& p_translation) ;
     virtual void delete_translation (const std::string& p_lang_code);
     
private :
    
    std::unordered_map<std::string,std::string> translations_;
};
}
}

