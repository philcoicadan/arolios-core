/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <qry/Base_query.h>
#include <tqry/query.h>

namespace arolios {


namespace tqry {
    class Tree_query;
    class Class;
    class Association;
    class Classifier;
    class Property;

class Exporter: public qry::Base_query {
public:
    /**
     * Constructor
     */
    Exporter(const std::string& p_query_file);

    /**
     * Destructor
     */
    virtual ~Exporter();

    void param_file ( const std::string& p_param_file);
    
    virtual void execute () override ;

private:
    const std::string query_file_; // XML file
    std::string param_file_; // JSON file
    
    std::unordered_map<std::string,Json::Value> param_values_;
    
    std::shared_ptr<Tree_query> query_;
    
    void fill_param_values ();
    
    std::shared_ptr<Class> execute_class (const XML_query_class_t& p_xml_elem) ;
    
    std::shared_ptr<Association> execute_association (const XML_query_association_t& p_xml_elem) ;
    
    void execute_member_end (const XML_query_member_end_t& p_xml_elem, const std::shared_ptr<Association>& p_assoc) ;
    
    void execute_member_end (const XML_query_member_end_t& p_xml_elem, const std::shared_ptr<Class>& p_cls) ;
    
    template <typename T1, typename T2> std::shared_ptr<T1> execute_classifier (const T2& p_xml_elem) ;
    
    void execute_base_class (const XML_query_base_class_t& p_xml_elem, const std::shared_ptr<Class>& p_cls) ;
    
    void execute_query_base (const XML_query_base_t& p_xml_elem, const std::shared_ptr<Classifier>& p_csf) ;

    void execute_property (const std::vector<std::string>& p_prop_names_vec, const std::shared_ptr<Classifier>& p_csf) ;
    
    std::string transform_filter (const std::string& p_filter,const std::shared_ptr<Classifier>& p_csf);


};

}
}

