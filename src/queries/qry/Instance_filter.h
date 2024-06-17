/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <vector>
#include <memory>

namespace arolios {

namespace qry {
class Instance_filter {
public:
    
    static std::vector<std::shared_ptr<const Instance_filter>> make_fts_filters ( const std::string& p_json_text) ;
    
    static std::vector<std::shared_ptr<const Instance_filter>> make_language_filters ( const std::string& p_json_text) ;
    
    /**
     * Constructor
     */
    Instance_filter (const std::string& p_item, const std::string& p_operation, const std::string& p_value) ;
    
    /**
     * Destructor
     */
    virtual ~Instance_filter();
    
    const std::string item() const;
    const std::string operation() const;
    const std::string value() const;
    
protected:

    
private:
    

    
    
    std::string item_;
    std::string operation_;
    std::string value_;



};

}
}

