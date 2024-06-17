/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <tqry/Classifier.h>

namespace arolios {

namespace omm {
    class Class;
    class Classifier;

}

namespace tqry {
        class Association;
class Class :  public virtual Classifier {
public:
    /**
     * Constructor
     */
    Class();

    /**
     * Destructor
     */
    virtual ~Class();
    
    void model_class (const std::shared_ptr<const omm::Class>& p_class);
    
    const std::shared_ptr<const omm::Class>& model_class () const ;
    virtual const std::shared_ptr<const omm::Classifier> model_classifier () const override;
    
    virtual void accept (Query_visitor& visitor) override;
    
    void insert_association (const std::shared_ptr<Association>& p_assoc) ;

    const std::vector<std::shared_ptr<Association>>& associations() const ;
    
private:
    
    std::shared_ptr<const omm::Class> mdl_class_;
    std::unordered_map<std::string, std::shared_ptr<Association>> assoc_map_;
    std::vector<std::shared_ptr<Association>> assoc_vec_;
};
}
}

