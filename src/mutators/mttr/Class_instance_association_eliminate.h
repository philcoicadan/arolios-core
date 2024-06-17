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
#include <mttr/Classifier_instance_eliminate.h>
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {

namespace mttr {
class Class_instance_association_eliminate : public Classifier_instance_eliminate {
public:
    /**
     * Constructor
     */
    Class_instance_association_eliminate( const std::shared_ptr<const omm::Association>& p_csf, const std::string& p_inst_id, const int p_fru_count = 0);

    /**
     * Destructor
     */
    virtual ~Class_instance_association_eliminate();
    
     virtual void execute () override ;

private:



};
}
}

