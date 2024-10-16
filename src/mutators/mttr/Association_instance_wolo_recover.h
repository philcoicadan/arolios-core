/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <omm/Association.h>
#include <common/Db_client.h>
#include <qry/Return_code.h>
#include <mttr/Classifier_instance_recover.h>
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {

namespace mttr {
class Association_instance_wolo_recover : public Classifier_instance_recover {
public:
    /**
     * Constructor
     */
    Association_instance_wolo_recover( const std::shared_ptr<const omm::Association>& p_csf, const std::string& p_inst_id, const int p_fru_count = 0);

    /**
     * Destructor
     */
    virtual ~Association_instance_wolo_recover();
    
     virtual void execute () override ;

private:



};
}
}

