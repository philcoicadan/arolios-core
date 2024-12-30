/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <tqry/Prepare_visitor.h>
#include <tqry/Class.h>
#include <omm/Class.h>
#include <tqry/Association.h>
#include <tqry/Class.h>
#include <omm/Association.h>
#include <tqry/Property.h>
#include <omm/Property.h>
#include <omm/Property_by_sub_field.h>
#include <omm/Column.h>
#include <omm/Table.h>
#include <tqry/Member_end.h>
#include <tqry/Tree_query.h>
#include <tqry/Query_stmt.h>
#include <exception/Exceptions.h>
#include <regex>



using namespace arolios;
using namespace arolios::tqry;

Prepare_visitor::Prepare_visitor(Tree_query& p_query): query_(p_query) {
    ;

}

Prepare_visitor::~Prepare_visitor() {

}

void Prepare_visitor::visit (Association& p_elem) {
    
    
    if (p_elem.is_root()) {
        // Prepare FROM
        // new qry_stmt

       std::string error_msg = "Tree query with an association as root is not yet implemented" ;
        
       throw exception::Not_yet_implemented(error_msg);
       
    } else {
        //  association from a member end, JOIN already done
        if (p_elem.table_alias().empty()) {
            std::string error_msg = "Join table not declared for association " + p_elem.model_association()->name();
             
            throw exception::Failure (error_msg);
        }

    }
           
    
    // Prepare fields for SELECT
    for (auto prop : p_elem.properties() ) {
        prop->accept(*this) ;
    }
    

    
    // Prepare filter
    auto filter = p_elem.filter();
    if (!filter.empty()) {
        add_filter (filter, p_elem);
    }
    
    if (p_elem.is_root()) {


    }

    
}
void Prepare_visitor::visit (Class& p_elem) {
    
    if (p_elem.is_root()) {
        // root class
        // Prepare FROM
        // new qry_stmt
        auto qry_stmt = std::make_shared<Query_stmt>();
        p_elem.table_alias (qry_stmt->new_alias());
        qry_stmt->add_from(p_elem);
        p_elem.query (qry_stmt);
        qry_stmts_.push(qry_stmt);
        
    } else {
        // target of a member end of an association
        // JOIN declared in the visit of the member end
        if (p_elem.table_alias().empty()) {
            std::string error_msg = "Join table not declared for class " + p_elem.model_class()->name();
             
            throw exception::Failure (error_msg);
        }
    }
           
    // Prepare fields for SELECT
    for (auto prop : p_elem.properties() ) {
        prop->accept(*this) ;
    }
    

    
    // Prepare filter
    auto filter = p_elem.filter();
    if (!filter.empty()) {
        add_filter (filter, p_elem);
    }
    
    // Prepare joints
    for (auto me : p_elem.member_ends()) {
        me->accept (*this);
    }
    
    if (p_elem.is_root()) {
     
    }
    
}
            
void Prepare_visitor::visit ( Member_end& p_elem ) {


   

    auto me_model = p_elem.model_property();

    if ( me_model->max_occurs() == 1 ) {
        // association doesn't have own table
        // need JOIN
        const auto join_type = ( me_model->min_occurs() == 1 )  ?
                               ( ( qry_stmts_.top()->join_types().top() == Query_stmt::Join_type::left ) ?
                                 Query_stmt::Join_type::left:
                                 Query_stmt::Join_type::inner )
                               : Query_stmt::Join_type::left;

        qry_stmts_.top()->join_types().push ( join_type );

        std::string left_column;
        std::string right_column;
        std::string join_stmt;

        if ( me_model->column() ) {
            //referencing table implements the association, alias given by the classifier owning the member_end
            const auto left_table = p_elem.classifier()->table_alias() ;
            if (left_table.empty()) {
                std::string error_msg = "Table alias not declared for classifier " + p_elem.classifier()->model_classifier()->name();
                 
                throw exception::Failure ( error_msg );
            }
            left_column = left_table + "." + me_model->column()->name();

            // referenced table is the target of the member end
            //new alias required
             // add alias for the target
            if (p_elem.target()) {
              p_elem.target()->table_alias(qry_stmts_.top()->new_alias());
              const auto right_table = p_elem.target()->table_alias();
              right_column = right_table + ".id";

              join_stmt = Query_stmt::join_type_to_string(
                              qry_stmts_.top()->join_types().top()) +
                          " " +
                          p_elem.target()->model_class()->table()->full_name() +
                          " AS " + right_table + " ON " + left_column + "=" +
                          right_column;
            } else {
              throw exception::Failure("Member end has no target class");
            }
        } else {
            // opposite must have a column
            auto prop_sub_field = me_model->property_by_sub_field();
            if ( prop_sub_field ) {
                // referenced table is the table of the source class of the property = target of the opposite
                const auto left_table = p_elem.classifier()->table_alias() ;
                if (left_table.empty()) {
                    std::string error_msg = "Table alias not declared for classifier " + p_elem.classifier()->model_classifier()->name();
                     
                    throw exception::Failure ( error_msg );
                }
                left_column = left_table + ".id";

                //referencing table is the target class 
                // new alias required
                if (p_elem.target()) {

                  p_elem.target()->table_alias(qry_stmts_.top()->new_alias());
                  const auto right_table = p_elem.target()->table_alias();

                  right_column = right_table + "." +
                                 prop_sub_field->filtering_column()->name();

                  join_stmt =
                      Query_stmt::join_type_to_string(
                          qry_stmts_.top()->join_types().top()) +
                      " " +
                      p_elem.target()->model_class()->table()->full_name() +
                      " AS " + right_table + " ON " + left_column + "=" +
                      right_column;
                } else {
                  throw exception::Failure("Member end has no target class");
                }
            } else {
              std::string error_msg = "Member end " + me_model->name() +
                                      " is not mapped to a relational object";

              throw exception::Failure(error_msg);
            }
        }

        qry_stmts_.top()->add_join ( join_stmt );

        if ( p_elem.target()) {
            p_elem.target()->accept (*this);
        }
        qry_stmts_.top()->join_types().pop();

        visit_optional_association (p_elem);

    } else {
        // target is Many, need a new qry_stmt
        // initialize alias counter with the current one
        auto qry_stmt = std::make_shared<Query_stmt>( qry_stmts_.top()->alias_count());
        p_elem.query ( qry_stmt );
        qry_stmts_.push ( qry_stmt );
        // FROM is the classifier owning the member end
        qry_stmt->add_from(*(p_elem.classifier()), true);



        // the source of the member end is the FROM table
        // this source = target of the opposite
        // opposite must have a column


        const auto opposite = p_elem.model_property()->association()->opposite_member_end ( p_elem.model_property() );
        if ( opposite ) {
            
            if ( opposite->column() ) {
            
                if ( opposite->max_occurs() == 1 ) { // association One to Many

                    const auto join_type = ( opposite->min_occurs() == 1 )  ?
                                           ( ( qry_stmts_.top()->join_types().top() == Query_stmt::Join_type::left ) ?
                                             Query_stmt::Join_type::left:
                                             Query_stmt::Join_type::inner )
                                           : Query_stmt::Join_type::left;

                    qry_stmts_.top()->join_types().push ( join_type );

                    // referenced table is the property source = target of the opposite
                    const auto left_table = p_elem.classifier()->table_alias() ;
                    if (left_table.empty()) {
                        std::string error_msg = "Table alias not declared for classifier " + p_elem.classifier()->model_classifier()->name();
                         
                        throw exception::Failure ( error_msg );
                    }
                    const auto left_column = left_table + ".id";

                    //referencing table is the target

                    if (p_elem.target()) {
                      p_elem.target()->table_alias(
                          qry_stmts_.top()->new_alias());
                      const auto right_table = p_elem.target()->table_alias();

                      const auto right_column =
                          right_table + "." + opposite->column()->name();

                      const auto stmt =
                          Query_stmt::join_type_to_string(
                              qry_stmts_.top()->join_types().top()) +
                          " " +
                          p_elem.target()->model_class()->table()->full_name() +
                          " AS " + right_table + " ON " + left_column + "=" +
                          right_column;

                      qry_stmts_.top()->add_join(stmt);
                    }

                } else  { // association Many to Many
                    // association has its own table , 2 JOINs are necessary

                    {
                        // first JOIN with the table implementing the association
                        
                        const auto join_type = ( opposite->min_occurs() == 1 )  ?
                                               ( ( qry_stmts_.top()->join_types().top() == Query_stmt::Join_type::left ) ?
                                                 Query_stmt::Join_type::left:
                                                 Query_stmt::Join_type::inner )
                                               : Query_stmt::Join_type::left;

                        qry_stmts_.top()->join_types().push ( join_type );

                        // referenced table is the property source = target of the opposite
                        const auto left_table = p_elem.classifier()->table_alias() ;
                        if (left_table.empty()) {
                            std::string error_msg = "Table alias not declared for classifier " + p_elem.classifier()->model_classifier()->name();
                             
                            throw exception::Failure ( error_msg );
                        }
                        const auto left_column = left_table + ".id";

                        //referencing table is the class implementing the association
                        // new alias
                        
                        
                        p_elem.association()->table_alias ( qry_stmts_.top()->new_alias() )  ;

                        const auto right_table = p_elem.association()->table_alias();

                        const auto right_column = right_table + "." + opposite->column()->name();

                        const auto stmt = Query_stmt::join_type_to_string ( qry_stmts_.top()->join_types().top() ) + " " + p_elem.association()->model_association()->managing_table()->full_name() +
                                          " AS " + right_table +
                                          " ON " + left_column + "=" + right_column;


                        qry_stmts_.top()->add_join ( stmt );
                    }

                    {
                        // second JOIN with the target class
                        if ( me_model->column() ) {

                            const auto join_type = ( me_model->min_occurs() == 1 )  ?
                                                   ( ( qry_stmts_.top()->join_types().top() == Query_stmt::Join_type::left ) ?
                                                     Query_stmt::Join_type::left:
                                                     Query_stmt::Join_type::inner )
                                                   : Query_stmt::Join_type::left;

                            qry_stmts_.top()->join_types().push ( join_type );

                            //referencing table is the table implementing the association
                            const auto left_table =  p_elem.association()->table_alias();
                            const auto left_column = left_table + "." + me_model->column()->name();

                            // referenced table is the target of the member end
                            // add alias for target
                            if (p_elem.target()) {
                              p_elem.target()->table_alias(
                                  qry_stmts_.top()->new_alias());
                              const auto right_table =
                                  p_elem.target()->table_alias();
                              const auto right_column = right_table + ".id";
                              const std::string stmt =
                                  Query_stmt::join_type_to_string(
                                      qry_stmts_.top()->join_types().top()) +
                                  " " +
                                  p_elem.target()
                                      ->model_class()
                                      ->table()
                                      ->full_name() +
                                  " AS " + right_table + " ON " + left_column +
                                  "=" + right_column;
                              qry_stmts_.top()->add_join(stmt);
                            }
                        } else {
                            std::string error_msg = "Member end " + me_model->name()  + " is not mapped to a relational object";
                             
                            throw exception::Failure ( error_msg );
                        }


                    }

                   
                }
                if (p_elem.target()) {
                  p_elem.target()->accept(*this);
                }
                qry_stmts_.top()->join_types().pop();
                
                visit_optional_association (p_elem);

                qry_stmts_.pop();

            } else {
                std::string error_msg = "Opposite member end of " + p_elem.model_property()->name() + " is not mapped to a relational object" ;
                 
                throw exception::Failure ( error_msg );
            }

        } else {
            std::string error_msg = "Opposite member end of " + p_elem.model_property()->name() + " not found" ;
             
            throw exception::Not_found ( error_msg );
        }
        

    }
    


}

void Prepare_visitor::visit_optional_association (Member_end& p_elem) {
    if (p_elem.optional_association()) {
        if (p_elem.association()->table_alias().empty()) {
            // need new JOIN
            std::string error_msg =  "association in member end not yet implemented";
             
            throw exception::Not_yet_implemented ( error_msg );
        } else {
            // JOIN already done
        }
        p_elem.association()->accept(*this);
        
    }
}
void Prepare_visitor::visit (Property& p_elem) {
    
    qry_stmts_.top()->add_column (p_elem);
    
    
}

void Prepare_visitor::add_filter (const std::string& p_filter, const Classifier& p_elem) {
    std::regex expr ( "\\$"+ p_elem.model_classifier()->id()) ;
    std::string new_str = p_elem.table_alias();
    if (new_str.empty()) {
        std::string error_msg = "Table alias for classifier " + p_elem.model_classifier()->name() + " not found" ;
         
        throw exception::Not_found ( error_msg );
    }
    std::string filter = std::regex_replace(p_filter, expr, new_str);
    qry_stmts_.top()->add_and_condition(filter);
}


    
