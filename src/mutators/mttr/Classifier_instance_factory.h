/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <mttr/Classifier_instance_create.h>
#include <mttr/Classifier_instance_update.h>
#include <mttr/Classifier_instance_delete.h>
#include <mttr/Classifier_instance_recover.h>
#include <mttr/Classifier_instance_eliminate.h>



namespace arolios {



namespace mttr
{

class Classifier_instance_factory
{
public:
    

    static Classifier_instance_factory& instance();
   
    
    virtual std::shared_ptr<mttr::Classifier_instance_create> make_create(const std::shared_ptr<const omm::Classifier>& p_csf, const Json::Value& p_data_root, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache= {});
    virtual std::shared_ptr<mttr::Classifier_instance_update> make_update(const std::shared_ptr<const omm::Classifier>& p_csf, const Json::Value& p_data_root, const std::string& p_inst_id, const int p_rfu_count = 0, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache = {});
    virtual std::shared_ptr<mttr::Classifier_instance_delete> make_delete(const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_inst_id, const int p_rfu_count = 0);
    virtual std::shared_ptr<mttr::Classifier_instance_recover> make_recover(const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_inst_id, const int p_rfu_count = 0);
    virtual std::shared_ptr<mttr::Classifier_instance_eliminate> make_eliminate(const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_inst_id, const int p_rfu_count = 0);


private:

	Classifier_instance_factory();
	~Classifier_instance_factory();

};

}
}