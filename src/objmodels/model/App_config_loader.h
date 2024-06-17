/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <common/Db_client.h>
#include <model/appconfig.h>

namespace arolios {



namespace common {
    class App_info;
}
namespace appcfg {
    class App;
}
namespace omm {
    class Package;
}

namespace model {
class App_config_loader {
public:
    /**
     * Constructor
     */
    App_config_loader();

    /**
     * Destructor
     */
    ~App_config_loader();
    
    void execute ( const std::string& p_app_id, const std::string& filename) ;

private:
    
    const std::shared_ptr<common::Db_client> db_;
    std::shared_ptr<common::App_info> app_info_;

    std::shared_ptr<appcfg::App> retrieve_app( const std::string& p_name)  const ;
    
    void load_domain_config ( const Domain_c& p_domain_config, const std::shared_ptr<omm::Package>& p_domain, std::shared_ptr<common::App_info>& p_app_info);
    
    

};
}
}

