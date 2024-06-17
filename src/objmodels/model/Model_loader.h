/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <model/xam.h>
#include <common/Db_client.h>

namespace arolios {


namespace common {
    class Object_model_info ;
}
namespace omm {
    class Package;
}
namespace model {
class Model_loader {
public:
    /**
     * Constructor
     */
    Model_loader();

    /**
     * Destructor
     */
    ~Model_loader();
    
    void execute ( const std::string& filename) ;

private:
    
    void create_domain ( const Domain_t& p_domain, const std::string& p_name, const std::shared_ptr<common::Object_model_info>& p_info);
    void update_domain ( const Domain_t& p_domain, const std::shared_ptr<omm::Package>& p_pk, const std::shared_ptr<common::Object_model_info>& p_info);
    void delete_domain ( const Domain_t& p_domain, const std::shared_ptr<omm::Package>& p_pk, const std::shared_ptr<common::Object_model_info>& p_info);
    const std::shared_ptr<common::Db_client> db_;

    void insert_domain_in_app_model ( const std::shared_ptr<omm::Package>& p_dom ) const ;
    void update_domain_in_app_model ( const std::shared_ptr<omm::Package>& p_dom ) const ;
    void associate_domain_app(const std::shared_ptr<omm::Package>& p_dom) const ;
    template<typename T> void insert_classifiers_in_app_model ( const std::unordered_map<std::string,std::shared_ptr<T>>& p_map) const;
};
}
}

