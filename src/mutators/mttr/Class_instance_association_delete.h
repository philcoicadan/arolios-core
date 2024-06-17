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
#include <mttr/Classifier_instance_delete.h>

namespace arolios {


namespace mttr {
class Class_instance_association_delete : public Classifier_instance_delete {
public:
    /**
     * Constructor
     */
    Class_instance_association_delete( const std::shared_ptr<const omm::Association>& p_csf, const std::string& p_inst_id, const int p_rfu_count = 0);

    /**
     * Destructor
     */
    virtual ~Class_instance_association_delete();
    
     virtual void execute () override ;

    
private:



};
}
}

