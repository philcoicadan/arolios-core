/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <model/Model_recorder.h>
#include <model/Model_element.h>
#include <model/xam.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <string>
#include <memory>
#include <vector>
#include <omm/Column_constraint.h>

namespace arolios {



namespace omm {
    class Package;
    class Classifier;
    class Logical_field;
}
namespace model {
    
class Domain_post_implementor : public Model_recorder {
public:
    /**
     * Constructor
     */
    Domain_post_implementor();

    /**
     * Destructor
     */
    virtual ~Domain_post_implementor();
    
    void add_fts_column (const Model_element<Domain_t,omm::Package>& p_store) const;
    
    void prepare_declaration_for_fts_column (const std::shared_ptr<const omm::Logical_field>& p_field, const std::string& p_code, std::string& p_fts_fragments_str, const std::string& p_fragment_sep,  const std::string& p_path=std::string()) const;
    
    void process_required_properties (const Model_element<Domain_t,omm::Package>& p_store) const;
    
    void process_id_properties (const Model_element<Domain_t,omm::Package>& p_store) const;
  
    void fill_column_constraints_with_inheritance(  const std::shared_ptr<const omm::Classifier>& p_csf, std::vector<std::shared_ptr<omm::Column_constraint>>& p_columns, omm::Column_constraint::Type p_type, bool p_with_root ) const ;
    
    void fill_column_constraints_for_many_to_many_association(  const std::shared_ptr<const omm::Classifier>& p_csf, std::vector<std::shared_ptr<omm::Column_constraint>>& p_columns ) const ;

    void fill_column_constraints_for_uniqueness ( const std::shared_ptr<const omm::Classifier>& p_csf, std::vector<std::shared_ptr<omm::Column_constraint>>& p_columns )  const;
};
}
}

