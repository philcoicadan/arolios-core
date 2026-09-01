/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <model/Visitor.h>
#include <string>
#include <omm/Property.h>
#include <omm/Class.h>
#include <omm/Column_constraint.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>

namespace arolios {


namespace model {
    class Composite_element;
    class Model_recorder;

class Model_implementor : public model::Visitor {
public:
    /**
     * Constructor
     */
    Model_implementor( const Model_recorder& p_mdl_rcd);

    /**
     * Destructor
     */
   virtual ~Model_implementor();


    virtual void visit (const Model_element_create<Enumeration_t, omm::Enumeration >& p_obj) override ;
    virtual void visit (const Model_element_delete<Enumeration_t, omm::Enumeration >& p_obj) override ;
    virtual void visit (const Model_element_update<Enumeration_t, omm::Enumeration >& p_obj) override ;
    virtual void visit (const Model_element_create<Element_t, omm::Enum_value >& p_obj) override ;
    virtual void visit (const Model_element_delete<Element_t, omm::Enum_value >& p_obj) override ;
    virtual void visit (const Model_element_update<Element_t, omm::Enum_value >& p_obj) override ; 
    virtual void visit (const Model_element_create<Type_t, omm::Composite_type >& p_obj) override;
    virtual void visit (const Model_element_delete<Type_t, omm::Composite_type >& p_obj) override;
    virtual void visit (const Model_element_update<Type_t, omm::Composite_type >& p_obj) override; 
    virtual void visit (const Model_element_create<Primitive_t, omm::Primitive_type >& p_obj) override;
    virtual void visit (const Model_element_delete<Primitive_t, omm::Primitive_type >& p_obj) override;
    virtual void visit (const Model_element_update<Primitive_t, omm::Primitive_type >& p_obj) override; 
    virtual void visit (const Model_element_create<Class_t, omm::Class >& p_obj)  override;
    virtual void visit (const Model_element_delete<Class_t, omm::Class >& p_obj)  override;
    virtual void visit (const Model_element_update<Class_t, omm::Class >& p_obj)  override;
    virtual void visit (const Model_element_create<Association_t, omm::Association >& p_obj)  override;
    virtual void visit (const Model_element_delete<Association_t, omm::Association >& p_obj)  override;
    virtual void visit (const Model_element_update<Association_t, omm::Association >& p_obj)  override;
    virtual void visit (const Model_element_create<Type_property_t, omm::Property >& p_obj)  override;
    virtual void visit (const Model_element_delete<Type_property_t, omm::Property >& p_obj)  override;
    virtual void visit (const Model_element_update<Type_property_t, omm::Property >& p_obj)  override;
    virtual void visit (const Model_element_create<Class_property_t, omm::Property >& p_obj)  override;
    virtual void visit (const Model_element_delete<Class_property_t, omm::Property >& p_obj)  override;
    virtual void visit (const Model_element_update<Class_property_t, omm::Property >& p_obj)  override;
    virtual void visit (const Model_element_create<Member_end_t, omm::Property >& p_obj)  override;
    virtual void visit (const Model_element_delete<Member_end_t, omm::Property >& p_obj)  override;
    virtual void visit (const Model_element_update<Member_end_t, omm::Property >& p_obj)  override; 
    virtual void visit (const Model_element_create<Domain_t, omm::Package >& p_obj) override;
    virtual void visit (const Model_element_delete<Domain_t, omm::Package >& p_obj) override ;
    virtual void visit (const Model_element_update<Domain_t, omm::Package >& p_obj) override;   


    virtual void visit (const Root_loader& p_obj) override ;



protected:

private:

    const Model_recorder& mdl_rcd_;
    
    
    void update_classifier (const std::shared_ptr<omm::Classifier>& p_elem, const std::string& p_new_name, const std::string& p_elem_type) const;
    
    void delete_classifier (const std::shared_ptr<omm::Classifier>& p_elem, const std::string& p_elem_type) const ;
    
    void update_property_field ( const std::shared_ptr<omm::Property>& p_elem, const std::string& p_name ) const ;
    
    void execute_insert_for_classifier ( const std::shared_ptr<const omm::Classifier>& p_elem ) const;
    void execute_insert_for_classifier_with_no_logical ( const std::shared_ptr<const omm::Classifier>& p_elem ) const ;
    
    void execute_insert_for_property ( const std::shared_ptr<const omm::Property>& p_prop, const std::shared_ptr<const omm::Classifier>& p_csf ) const ;
    
    void execute_insert_for_property_by_sub_field ( const std::shared_ptr<const omm::Property>& p_prop
    ) const ;
    
    void execute_insert_column_constraint (const std::shared_ptr<const omm::Column_constraint>& p_cctt)  const ;

    void execute_delete_column_constraint (const std::shared_ptr<const omm::Column>& p_column, const omm::Column_constraint::Type p_type)  const ;
    
    void prepare_declaration_for_column (std::string& p_stmt, const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_column_name, bool p_required = false)  const ;
    void post_prepare_declaration_for_enum_column (std::string& p_stmt, const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_column_name)  const ;
    void post_prepare_declaration_for_enum_attribute (std::string& p_stmt, const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_attr_name)  const ;
    

    void prepare_declaration_for_reference_column (std::string& p_stmt, const std::shared_ptr<const omm::Property>& p_prop, const std::string& p_column_name, const std::string& p_table_name, const bool is_root, const bool p_delete_cascade, const bool p_not_null, const bool p_unique, const std::shared_ptr<const omm::Classifier>& p_class) const ;

    void add_complementary_table (const std::shared_ptr<omm::Association>& p_assoc)  const ;
    
  
    

   
};

};
}

