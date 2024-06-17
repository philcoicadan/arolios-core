/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <model/xtlt.h>
#include <common/Db_client.h>

namespace arolios {


namespace common {
    class Object_model_info ;
}

namespace omm {
    class Package;
    class Language;
}

namespace model {
class Model_tlt_loader {
public:
    /**
     * Constructor
     */
    Model_tlt_loader();

    /**
     * Destructor
     */
    virtual ~Model_tlt_loader();
    
    void execute ( const std::string& filename) ;

private:
    
    void load_tlt_domain ( const Domain_tlt& p_domain_tlt, const std::shared_ptr<omm::Package>& p_domain, const std::shared_ptr<common::Object_model_info>& p_info,const std::shared_ptr<const omm::Language>& p_lang);
    const std::shared_ptr<common::Db_client> db_;


};
}
}

