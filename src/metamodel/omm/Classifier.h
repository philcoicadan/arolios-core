/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Namespace.h"
#include "Packageable_element.h"
#include <unordered_set>

namespace arolios {






namespace omm {
	class Property;
	class Generalization;
	class Constraint;
    class Logical_struct;
    class Table;
    class Class;
    
	class Classifier : public virtual Namespace, public virtual Packageable_element {
	public:

		Classifier (const bool p_is_abstract = false) ;
		Classifier ( const Classifier& ) = default;
		virtual ~Classifier () {};
		virtual const std::unordered_map<std::string,std::shared_ptr<Property>>& own_properties () const { return properties_ ;};
        virtual void insert_own_property( const std::shared_ptr<Property>& p_prop) ;
        
		virtual const std::unordered_map<std::string,std::shared_ptr<Generalization>>& generalizations () const { return generalizations_ ;};
        virtual void insert_generalization (const std::string& p_name, const std::shared_ptr<Generalization>& p_general);
        
		virtual std::unordered_map<std::string,std::shared_ptr<Generalization>>& specializations () { return specializations_ ;};
		virtual bool has_properties () const ;
		virtual bool has_generalizations () const ;
		virtual bool has_specializations () const ;
        virtual bool has_elements (bool p_active_only = true) const override;
        virtual bool is_abstract() const { return is_abstract_; }
        virtual void is_abstract( const bool p_is_abstract) { is_abstract_ = p_is_abstract; }
		virtual std::shared_ptr<Package> package() const override { return package_;}
		virtual void package (const std::shared_ptr<Package>& p_package) { package_ = p_package;}

        virtual std::shared_ptr<Logical_struct> logical_struct() const =0;
        virtual std::shared_ptr<Table> table() const = 0 ;
        virtual std::shared_ptr<Table> managing_table() const = 0 ;
        virtual void insert_element (const std::shared_ptr<Named_element>& p_elem) override;
        virtual void erase_element (const std::shared_ptr<Named_element>& p_elem) override;
        virtual std::string read_select_template(const bool p_force = false ) =0;
        virtual const std::unordered_map<std::string,std::shared_ptr<Property>>& all_elements_as_property () const ;
        virtual void make_all_elements_as_property (const bool p_force = true );
        const std::unordered_set<std::shared_ptr<Classifier>>& more_general_classifiers() ;
        virtual std::shared_ptr<Property> find_element_in_all_properties (const std::string& p_name) const ;
        
        
        virtual std::unordered_map<std::string,std::shared_ptr<Property>> all_id_properties () const  ;
        
        virtual const std::string classifier_id_with_specializations (const std::string& p_table_prefix=std::string()) const ;
        virtual void make_more_specific_ids ( const bool p_force =true ) = 0;

        virtual bool is_kind_of (const std::shared_ptr<Classifier>& p_csf) ;
        
        virtual void recursive_lc_status (const Lc_status p_status) override;
        
        virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override ;
                
		virtual std::shared_ptr<Namespace> own_namespace() const override ;

        virtual void own_namespace ( const std::shared_ptr<Namespace>& p_parent) override;
        
        virtual void reset_all_elements_as_property ();
        virtual void reset_more_general_classifiers();
              
        virtual const std::string& meta_type() const = 0;

        virtual const std::unordered_map<std::string,std::shared_ptr<Property>>& identifier () const;
        virtual void identifier (const std::unordered_map<std::string,std::shared_ptr<Property>>& p_ident_names_map);
        
        virtual bool has_identifier (const std::shared_ptr<const Property>& p_prop) const ;
        
        virtual void insert_identifier (const std::shared_ptr<Property>& p_prop) ;
        virtual void erase_identifier (const std::shared_ptr<Property>& p_prop) ;
        
        virtual bool has_required_property (const std::shared_ptr<const Property>& p_prop) const ;

        virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) override;

        virtual const std::unordered_map<std::string,std::shared_ptr<Constraint>>& constraints () const { return constraints_; }
        virtual void insert_constraint (const std::shared_ptr<Constraint>& p_ctt);
        virtual void erase_constraint (const std::shared_ptr<Constraint>& p_ctt);

    protected:

        std::unordered_map<std::string, std::shared_ptr<Property>> all_props_;
        bool all_props_done_;
        std::string ids_w_all_spec_;
        virtual void collect_all_elements_as_property (std::unordered_map<std::string, std::shared_ptr<Property>>& p_map);


	private:
		bool is_abstract_;
        std::shared_ptr<Package> package_;
		std::unordered_map<std::string,std::shared_ptr<Property>> properties_;
		std::unordered_map<std::string,std::shared_ptr<Generalization>> generalizations_ ;
		std::unordered_map<std::string,std::shared_ptr<Generalization>> specializations_ ;

        std::unordered_set<std::shared_ptr<Classifier>> more_general_classifiers_;
        bool more_general_classifiers_done_; 
                
        std::unordered_map<std::string,std::shared_ptr<Property>> ident_map_;

        std::unordered_map<std::string,std::shared_ptr<Constraint>> constraints_;

	};
}


}

