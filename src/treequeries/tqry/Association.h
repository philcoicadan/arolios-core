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
    class Association;
    class Classifier;
}
namespace tqry {
    class Member_end;
    class Class;
class Association :  public virtual Classifier {
public:
    /**
     * Constructor
     */
    Association();

    /**
     * Destructor
     */
    virtual ~Association();
    
    void model_association (const std::shared_ptr<const omm::Association>& p_assoc);
    
    const std::shared_ptr<const omm::Association>& model_association () const ;
    virtual const std::shared_ptr<const omm::Classifier> model_classifier () const override;
    
    void insert_member_end (const std::shared_ptr<Member_end>& p_member_end) ;
    
    const std::vector<std::shared_ptr<Member_end>>& member_ends () const ;
    
    void from_class ( const std::shared_ptr<Class>& p_class) ;
    
    const std::shared_ptr<Class>& from_class () const ;
    
    virtual void accept (Query_visitor& visitor) override;
    
private:
    

    std::shared_ptr<const omm::Association> mdl_assoc_;
    std::vector<std::shared_ptr<Member_end>> ends_;
    std::shared_ptr<Class> from_class_;
};
}
}

