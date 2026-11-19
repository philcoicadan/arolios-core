/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <qry/Classifier_properties_list.h>
#include <common/Singleton.h>
#include <common/App_info.h>
#include <appcfg/App.h>
#include <appcfg/Base_classifier_config.h>
#include <exception/Exceptions.h>
#include <omm/Association.h>
#include <vector>
#include <qry/Util.h>
#include <util/String.h>
#include <qry/Util.h>










using namespace arolios;
using namespace arolios::qry;

 Classifier_properties_list::Classifier_properties_list ( const std::shared_ptr<const omm::Classifier>& p_csf, const std::string& p_offset, const std::string& p_limit, const std::string& p_sort, const std::string& p_direction, const std::string& p_context , const std::shared_ptr<const omm::Language>& p_lang
) : Base_list_query ( p_offset, p_limit, p_sort, p_direction, p_lang) , csf_ (p_csf){ 
   
    context_ = qry::Util::convert_context (p_context);
}


 Classifier_properties_list::~Classifier_properties_list() { }

 void Classifier_properties_list::execute ()  {
 
    std::shared_ptr<appcfg::App> config = app_info_ptr_->app();

    std::shared_ptr<appcfg::Base_classifier_config> csf_config = config->find_config_by_name ( csf_->package()->name(), csf_->name() );

    
    
    std::vector<std::shared_ptr<const omm::Property_view>> props_vec ;
    
    switch (context_) {
        
        case qry::Util::Context::ids:
            csf_config->all_id_properties ( props_vec);
            break;
        case qry::Util::Context::list:
            csf_config->properties_for_list (props_vec);
            break;
        case qry::Util::Context::create:
            csf_config->properties_for_create (props_vec);
            break;
        case qry::Util::Context::read:
            csf_config->properties_for_read (props_vec);
            break;
        case qry::Util::Context::update:
            csf_config->properties_for_update (props_vec);
            break;
        default:
            csf_config->all_id_properties(props_vec);
            break;
    }   
    
        
    
    sort_result (props_vec) ;
    
    
    return_json_ ["classifier"] = Json::Value (Json::objectValue) ;
    Json::Value& jcsf = return_json_ ["classifier"];

    jcsf["url"] = qry::Util::url_prefix(csf_->meta_type()) + csf_->package()->name() + "." + csf_->name() ;
    jcsf["_tid_name"] = csf_->package()->translated_name(lang_) + "." +  csf_->translated_name(lang_) ;

   
    return_json_ ["total"] = static_cast<int>(props_vec.size());
    return_json_ ["elements"] = Json::Value (Json::arrayValue);

    
    Json::Value& jrows = return_json_["elements"];
    if (limit_ == "ALL" ) {
        jrows.resize(props_vec.size()-offset_);
    } else {
        jrows.resize(std::min<int>(std::stol (limit_), (props_vec.size()-offset_)));
    }
    
    int i= 0;    
    for (auto  j = props_vec.begin() + offset_; j< props_vec.end() && i<jrows.size(); ++j) {
        jrows[i] =  Json::Value ( Json::objectValue ) ;
        Json::Value& jrow = jrows[i++];

        build_property_json ( jrow, (*j), false) ;

    }
    
    return_code_ = qry::Return_code::OK;
            
}

void Classifier_properties_list::build_property_json (Json::Value& p_jrow, const std::shared_ptr<const omm::Property_view>& p_prop_v, /*const bool p_nesting_part_of_id,*/ const bool p_nesting_is_required,  const std::vector<std::shared_ptr<const omm::Property_view>>& p_nesting_prop_view_vec) {
    

    p_jrow["type"] = Json::Value (Json::objectValue);
    Json::Value& jrow_type = p_jrow["type"];
    // the simple types don't have URL
    jrow_type["id_name"] = p_prop_v->property()->type()->package()->name() + "." + p_prop_v->property()->type()->name();
    jrow_type["_tid_name"] = p_prop_v->property()->type()->package()->translated_name(lang_) + "." + p_prop_v->property()->type()->translated_name(lang_);

    p_jrow["metatype"] = p_prop_v->metatype_to_string();
    p_jrow["description"] = p_prop_v->property()->description();
    p_jrow["required"] = csf_->has_required_property(p_prop_v->property()) || p_nesting_is_required;
    p_jrow["classifier"] = Json::Value ( Json::objectValue ) ;
    Json::Value& jrow_cls = p_jrow["classifier"];
    auto meta_type = p_prop_v->property()->classifier()->meta_type();
    if (meta_type == "class" || meta_type == "association") {
        jrow_cls["url"] = qry::Util::url_prefix(meta_type) + p_prop_v->property()->package()->name() + "." + p_prop_v->property()->classifier()->name();

    }
    else {
        jrow_cls["id_name"] =  p_prop_v->property()->package()->name() + "." + p_prop_v->property()->classifier()->name();
 
    }
    jrow_cls["_tid_name"] = p_prop_v->property()->package()->translated_name(lang_) + "." + p_prop_v->property()->classifier()->translated_name(lang_);
   if (p_prop_v->property()->association()) {
        p_jrow["memberEnd"] = Json::Value ( Json::objectValue ) ;
        Json::Value& jrow_me = p_jrow["memberEnd"];
        if ( p_prop_v->property()->min_occurs() == 0 ) {
            jrow_me["minOccurs"] = "zero" ;
        } else {
            jrow_me["minOccurs"] = "one";
        }
        if ( p_prop_v->property()->max_occurs() == -1 ) {
            jrow_me["maxOccurs"] = "many" ;
        } else {
            jrow_me["maxOccurs"] = "one";
        }

        jrow_me["association"] = Json::Value ( Json::objectValue ) ;
        Json::Value& jrow_assoc = jrow_me["association"];
        jrow_assoc["url"] = qry::Util::url_prefix("association")+ p_prop_v->property()->association()->package()->name() + "." + p_prop_v->property()->association()->name();
        jrow_assoc["_tid_name"] = p_prop_v->property()->association()->package()->translated_name(lang_) + "." + p_prop_v->property()->association()->translated_name(lang_);
       if ( !p_prop_v->associated_property_views().empty() ) {
            p_jrow["associated"] = Json::Value ( Json::arrayValue );
            Json::Value& jrow_associated = p_jrow["associated"];
            int ii = 0;
            std::vector<std::shared_ptr<const omm::Property_view>> prop_view_vec = p_nesting_prop_view_vec;
            prop_view_vec.push_back(p_prop_v);

            for ( auto k = p_prop_v->associated_property_views().begin() ; k < p_prop_v->associated_property_views().end(); ++k ) {
                jrow_associated[ii] =  Json::Value ( Json::objectValue ) ;
                Json::Value& jrow_associated_prop = jrow_associated[ii++];
                build_property_json ( jrow_associated_prop, ( *k ), false, prop_view_vec ) ;
            }
        }

    }
    
    if ( p_prop_v->property()->has_composite_type () ) {
        p_jrow["nested"] = Json::Value ( Json::arrayValue );
        Json::Value& jrow_nested = p_jrow["nested"];
        int ii = 0;
        std::vector<std::shared_ptr<const omm::Property_view>> prop_view_vec = p_nesting_prop_view_vec;
        prop_view_vec.push_back(p_prop_v);
        for (auto k = p_prop_v->nested_property_views().begin() ; k < p_prop_v->nested_property_views().end(); ++k) {
            jrow_nested[ii] =  Json::Value ( Json::objectValue ) ;
            Json::Value& jrow_nested_prop = jrow_nested[ii++];

            build_property_json (jrow_nested_prop, (*k), /*p_prop_v->property()->part_of_id(),*/ csf_->has_required_property(p_prop_v->property()), prop_view_vec) ;
        }
    } else if ( p_prop_v->property()->has_instance_type () ) {
         p_jrow["linked"] = Json::Value ( Json::arrayValue );
        Json::Value& jrow_linked = p_jrow["linked"];
        int ii = 0;
        std::vector<std::shared_ptr<const omm::Property_view>> prop_view_vec = p_nesting_prop_view_vec;
        prop_view_vec.push_back(p_prop_v);

        for (auto k = p_prop_v->linked_property_views().begin() ; k < p_prop_v->linked_property_views().end(); ++k) {
            jrow_linked[ii] =  Json::Value ( Json::objectValue ) ;
            Json::Value& jrow_linked_prop = jrow_linked[ii++];

            build_property_json (jrow_linked_prop, (*k), /*p_prop_v->property()->part_of_id(),*/ csf_->has_required_property(p_prop_v->property()),prop_view_vec) ;
        }
    }
    
    p_jrow["name"] = p_prop_v->property()->name();
    p_jrow["_tname"] = p_prop_v->property()->translated_name(lang_);

    //caution,   for potential display,  the json data contain the translation of the  absolute path name (_tpathname) in the list of properties
    std::string prop_path;
    if (!p_nesting_prop_view_vec.empty() ) {
        for (auto i=0; i < p_nesting_prop_view_vec.size(); ++i) {
            prop_path += p_nesting_prop_view_vec[i]->property()->translated_name(lang_) + ".";
        }
    } 
    p_jrow["_tpathname"] = prop_path + p_prop_v->property()->translated_name(lang_);
  
    p_jrow["id_name"] = p_prop_v->id_name();
}
    

 void Classifier_properties_list::sort_result (std::vector<std::shared_ptr<const omm::Property_view>>& p_props) {
    

    const auto order_by = compute_order_by();

    switch (std::stoi(order_by)) {
        case 0: // id
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return util::String::stoui(p1->property()->id()) < util::String::stoui(p2->property()->id()) ; });
         break;  
        case 1: // name
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [this] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return p1->property()->translated_name(lang_) < p2->property()->translated_name(lang_) ; });
         break;   
                              
        case 2: // type => by id_name
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [this] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return std::string(p1->property()->type()->package()->translated_name(lang_) + "." + p1->property()->type()->translated_name(lang_) )< std::string( p2->property()->type()->package()->translated_name(lang_) + "." +p2->property()->type()->translated_name(lang_) ); });
         break;      
        case 3: // description
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return p1->property()->description() < p2->property()->description() ; });
         break;

        case 5: //required
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [this] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return static_cast<int>(csf_->has_required_property(p1->property())) < static_cast<int>(csf_->has_required_property(p2->property())) ; });
         break;
        
       
        case 7: // classifier => by id_name
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [this] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return std::string( p1->property()->classifier()->package()->translated_name(lang_)+"." + p1->property()->classifier()->translated_name(lang_) )< std::string( p2->property()->classifier()->package()->translated_name(lang_) +"." + p2->property()->classifier()->translated_name(lang_)); });
         break;
       
        case 10: // id
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return util::String::stoui(p1->property()->id()) > util::String::stoui(p2->property()->id()) ; });
         break;
        case 11: // name
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [this] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return p1->property()->translated_name(lang_) > p2->property()->translated_name(lang_) ; });
         break;   
                              
        case 12: // type => by id_name
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [this] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return std::string(p1->property()->type()->package()->translated_name(lang_) + "." + p1->property()->type()->translated_name(lang_) )> std::string( p2->property()->type()->package()->translated_name(lang_) + "." +p2->property()->type()->translated_name(lang_) ); });
         break;   
           
        case 13: // description
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return p1->property()->description() > p2->property()->description() ; });
         break;
        
        case 15: //required
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [this] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return static_cast<int>(csf_->has_required_property(p1->property())) > static_cast<int>(csf_->has_required_property(p2->property())) ; });
         break;
        
       
        case 17: // classifier => by id_name
         std::stable_sort(p_props.begin(), p_props.end(), 
                          [this] (const std::shared_ptr<const omm::Property_view>& p1, const std::shared_ptr<const omm::Property_view>& p2 ) { return std::string( p1->property()->classifier()->package()->translated_name(lang_)+"." + p1->property()->classifier()->translated_name(lang_) ) > std::string( p2->property()->classifier()->package()->translated_name(lang_) +"." + p2->property()->classifier()->translated_name(lang_)); });
         break;
        
        
        default:
            throw exception::Failure ( "Internal error, sort not possible" );
            break;
        }

    }

 std::string Classifier_properties_list::compute_order_by () {

    static std::unordered_map<std::string, int> map { 
     {"name", 1}, {"type", 2}, {"description", 3}, /*{"partOfId", 4},*/ {"required", 5},{"classifier", 7}  };
    
    
    int order_by = 0; // default = id
    if (! sort_.empty()) {
        auto search = map.find (sort_) ;
        if (search != map.end() ) {
            order_by  = search->second;
        } else {
             throw exception::Not_found ( "Attribute " + sort_ + " not found. Sort not possible" );
        }
    }
   
 
    order_by += (direction_ == "ASC") ? 0 : 10;

    return std::to_string(order_by);
 }
