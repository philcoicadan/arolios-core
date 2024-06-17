/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Packageable_element.h"
#include "Aggregation_kind.h"

namespace arolios {







namespace omm {
	class Classifier;
	class Association;
    class Logical_field;
    class Column;
    class Attribute;
    class Property_by_sub_field;
	class Property : public virtual Packageable_element {
	public:

		virtual ~Property () {}
		Property () ;
		Property ( const Property& ) = default;
		virtual std::shared_ptr<Namespace> own_namespace() const override;
		virtual const std::shared_ptr<Classifier> classifier() const { return classifier_;}
		virtual const std::shared_ptr<const Classifier> type() const { return type_;}
		virtual const std::shared_ptr<Association> association() const { return assoc_;}
		virtual Aggregation_kind aggregation_kind () const { return aggregation_; }

		virtual long min_occurs () const { return min_; }
		virtual long max_occurs () const { return max_; }
		virtual const std::string& default_value () const { return default_; }
		virtual const std::string& computed_value () const { return computed_; }

		virtual void classifier(const std::shared_ptr<Classifier>& p_classifier)  { classifier_ = p_classifier;}
		virtual void type(const std::shared_ptr<const Classifier>& p_type) ;
		virtual void association(const std::shared_ptr<Association>& p_assoc)  {  assoc_ = p_assoc;}
		virtual void aggregation_kind (Aggregation_kind p_aggregation)  { aggregation_ =p_aggregation; }
		virtual void min_occurs (long p_min)  {min_= p_min; }
		virtual void max_occurs (long p_max)  { max_ = p_max; }
		void default_value (const std::string& p_default)  {  default_ = p_default; }
		void computed_value (const std::string& p_computed)  {  computed_ = p_computed; }
		
		virtual void logical_field (const std::shared_ptr<Logical_field>& p_field) { field_ = p_field; }

        virtual const std::shared_ptr<Logical_field>& logical_field () const { return field_; }
        
		virtual void property_by_sub_field (const std::shared_ptr<Property_by_sub_field>& p_sub_field) { sub_field_ = p_sub_field; }

        virtual const std::shared_ptr<Property_by_sub_field>& property_by_sub_field () const { return sub_field_; }
        virtual const std::shared_ptr<Column> column () const;
        virtual const std::shared_ptr<Attribute > attribute() const;

        virtual std::shared_ptr<Package> package() const override ;

		virtual void accept (Visitor& p_visitor) const override;
        
        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;

        
        virtual bool has_composite_type() const ;
        
        virtual bool has_enum_type () const;   
        
        virtual bool has_referencing_type () const;
        
        virtual bool has_sub_field () const ;
        
        virtual bool is_required () const ;

		virtual bool is_class_property () const;
		virtual bool has_many_targets () const;
         
        virtual const std::vector<std::shared_ptr<omm::Property>>& nested_properties () const ;
        
        virtual std::string select_template(const std::string& p_path, const std::string& p_path_alias=std::string()) ;

    virtual void own_namespace ( const std::shared_ptr<Namespace>& p_ns) override;   

	virtual  std::shared_ptr<omm::Property> find_nested_property (const std::string& p_name) const ;   
	
    virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) override;

private:
		std::shared_ptr<Classifier> classifier_; // Data_type, Class or Association
		std::shared_ptr<const Classifier> type_; // Data_type, Enumeration or Class
		std::shared_ptr<Association> assoc_;
		std::shared_ptr<Logical_field> field_;
        std::shared_ptr<Property_by_sub_field> sub_field_;
        std::vector<std::shared_ptr<omm::Property>> nested_properties_;
        
        
		Aggregation_kind aggregation_ ;

		long min_;
		long max_; // -1 means *
		std::string default_;
		std::string computed_;





	};
}
}

