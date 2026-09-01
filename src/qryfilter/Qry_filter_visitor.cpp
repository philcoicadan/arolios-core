/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <exception/Exceptions.h>
#include <util/String.h>
#include <qry/Util.h>
#include <Qry_filter_visitor.h>
#include <FilterParser.h>
#include <omm/Logical_field.h>
#include <omm/Property.h>



using namespace arolios;
using namespace arolios::qryfilter;

Qry_filter_visitor::Qry_filter_visitor(const std::shared_ptr<const omm::Classifier>& p_csf, const std::unordered_map<std::string,Json::Value>& p_param_values): classifier_ (p_csf) , param_values_ (p_param_values),filter_(std::string()), last_field_(nullptr) {

}

Qry_filter_visitor::~Qry_filter_visitor()
{
}


std::any Qry_filter_visitor::visitFilter(FilterParser::FilterContext *ctx) {
    
        std::vector<FilterParser::AndconditionContext *> children = ctx->andcondition();
        
        if (children.size() > 0) {
            visitAndcondition (children[0]) ;
            
            if (children.size() > 1) {
                
                for (int index = 1 ; index < children.size() ; ++index) {
                    filter_ += " OR " ;
                    visitAndcondition (children[index]) ;
                }

            }
        }
        return true;
}

std::any Qry_filter_visitor::visitAndcondition(FilterParser::AndconditionContext *ctx) {
      
        std::vector<FilterParser::ConditionContext *> children = ctx->condition();
        
        if (children.size() > 0) {
            visitCondition (children[0]) ;
            
            if (children.size() > 1) {
                
                for (int index = 1 ; index < children.size() ; ++index) {
                    filter_ +=  " AND " ;
                    visitCondition (children[index]) ;
                }

            }
        } 
        return true;

}

std::any Qry_filter_visitor::visitCondition(FilterParser::ConditionContext *ctx) {
    
    if (ctx->NotOper() ) {
        filter_ += " NOT ";
        visitFilter (ctx->filter()) ;
    } else if ( ctx->OpenPar()) {
        filter_ +=  " ( " ;
        visitFilter (ctx->filter() ) ;
        filter_ +=  " ) ";
    } else if (ctx->property()){
        visitProperty (ctx->property());
        //std::string str =  std::any_cast<std::string> (result);
        visitSimplecondition ( ctx->simplecondition()) ;
    } else {
        std::string error_msg = "error in the query filter: " + ctx->getText();
         
        throw exception::Failure (error_msg) ;
    }
    return true;

}

std::any Qry_filter_visitor::visitSimplecondition(FilterParser::SimpleconditionContext *ctx) {
    
    if (ctx->Is() ) {
        filter_ += " IS ";
        
        if (ctx->NotOper() ) {
            filter_ += " NOT ";
        }
        
        filter_ += "NULL" ;
    } else {
        if (ctx->NotOper() ) {
            filter_ += " NOT ";
        }
        if (ctx->Between()) {
            filter_ += " BETWEEN " ;
            visitTerm(ctx->term()[0]) ;
             filter_ += " AND ";
            visitTerm(ctx->term()[1]) ;
        } else if (ctx->In()) {
            filter_ += " IN ( " ;

            std::vector<FilterParser::TermContext *> children = ctx->term();

            if ( children.size() > 0) {
            visitTerm ( children[0] ) ;

                if ( children.size() > 1 ) {

                    for ( int index = 1 ; index < children.size() ; ++index ) {
                        filter_ += " , " ;
                        visitTerm ( children[index] ) ;
                    }

                }
            }

            filter_ += " ) " ;
        } else {
            visitCompare (ctx->compare());
            visitTerm (ctx->term()[0]) ;
        }
    }
    return true;

}

std::any Qry_filter_visitor::visitCompare(FilterParser::CompareContext *ctx) {
    
    //filter_ += " " + ctx->getText() + " " ;
    filter_ += " ";
    int type =  static_cast<int> (ctx->getStart()->getType());
    switch (type) {
        case FilterParser::Eq : filter_ += "="; break;
                
        case FilterParser::NotEq : filter_ += "<>"; break;
                
        case FilterParser::Gt : filter_ += ">"; break;
                
        case FilterParser::GtOrEq : filter_ += ">="; break;
                
        case FilterParser::Lt : filter_ += "<"; break;
                
        case FilterParser::LtOrEq : filter_ += "<="; break;

    }
      
    filter_ += " ";
    return true;

}

std::any Qry_filter_visitor::visitProperty(FilterParser::PropertyContext *ctx) {
    
    std::string prop_name = ctx->getText();
    auto prop_vec = util::String::standardize(util::String::split(prop_name, omm::Named_element::separator()));
    property_to_field (prop_vec);
    
    //filter_ += ctx->getText() ;
    return true;

}

std::any Qry_filter_visitor::visitBindParameter(FilterParser::BindParameterContext *ctx) {
    
;
    auto search = param_values_.find(ctx->Identifier()->getText());
    if (search != param_values_.end()) {
        if (last_field_) {
            util::Native_type native_type;
            if ( last_field_->property()->has_enum_type() ) {
                // enum value is like a char string
                native_type = util::Native_type::character_varying;
            } else {
                // prop type is a primitive
                native_type = static_cast<util::Native_type> ( last_field_->native_type() );
            }
            filter_+= qry::Util::json_to_native (native_type, search->second);
        } else {
            std::string error_msg = "error in the query filter: " + ctx->getText();
             
            throw exception::Failure (error_msg) ;
        }
    } else {
        std::string error_msg = "parameter " + ctx->getText() + " not found";
         
        throw exception::Not_found ( error_msg ) ;

    }
    
    
    //filter_ += ctx->getText() ;
    return true;

}

std::any Qry_filter_visitor::visitValue(FilterParser::ValueContext *ctx) {
    
    //filter_ += ctx->getText() ;
    
    if ( last_field_ ) {
        util::Native_type native_type;
        if ( last_field_->property()->has_enum_type() ) {
            // enum value is like a char string
            native_type = util::Native_type::character_varying;
        } else {
            // prop type is a primitive
            native_type = static_cast<util::Native_type> ( last_field_->native_type() );
        }
        std::string text = ctx->getText();
        if (text.front() == '\'' || text.front() == '"' ) {
            text.erase(0,1);
        };
        if (text.back() == '\'' || text.back() == '"' ) {
            text.erase(text.length()-1,1);
        };
        
        filter_+= qry::Util::native_data_to_string ( native_type, text );
    } else {
        std::string error_msg = "error in the query filter: " + ctx->getText();
         
        throw exception::Failure ( error_msg ) ;
    }
    return true;

}
    
const std::string & Qry_filter_visitor::filter_result () const {
    
    return filter_;
}

void Qry_filter_visitor::property_to_field (const std::vector<std::string>& p_prop_names_vec)  {
    
    std::string result;
    if (p_prop_names_vec.size() >0 ) {
            
        const auto prop = classifier_->find_element_by_name<omm::Property> (p_prop_names_vec[0]);
        if (prop) {
            const auto field = prop->logical_field() ;
            if (field) {
                
                result = "$"+ classifier_->id() + "." + field->name() ;
                last_field_ = field;
                if (p_prop_names_vec.size() >1 ) {
                    const auto attr = prop->type()->find_element_by_name<omm::Property> (p_prop_names_vec[1]);
                    if (attr) {
                        const auto field = attr->logical_field() ;
                        if ( field ) {
                            last_field_ = field;
                            result.insert(0,1,'(');
                            result += ")." + field->name() ;
                        } else {
                            std::string error_msg = "Property " + p_prop_names_vec[1] + " is not mapped to a relational object" ;
                             
                            throw exception::Not_found (error_msg) ;
                        }
                    } else {
                        std::string error_msg = "Property " + p_prop_names_vec[1] + " not found" ;
                         
                        throw exception::Not_found (error_msg) ;
                    }
                }
            } else {
                std::string error_msg = "Property " + p_prop_names_vec[0] + " is not mapped to a relational object" ;
                 
                throw exception::Not_found (error_msg) ;
            }
        } else {

            std::string error_msg = "Property " + p_prop_names_vec[0] + " not found" ;
             
            throw exception::Not_found ( error_msg ) ;
        }
    }
    filter_ += result;
}

