/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Namespace.h"
#include "Packageable_element.h"
#include <unordered_map>
#include <exception/Exceptions.h>

namespace arolios {




namespace omm {
	class Classifier;
	class Class;
	class Data_type;
	class Restriction_type;
	class Collection_type;
	class Enumeration;
	class Primitive_type;
	class Select_type;
	class Association;
    class Logical_schema;
    class Composite_type;
    
	class Package : public virtual Namespace ,public virtual Packageable_element {
	public:

		Package () = default;
		Package ( const Package& ) = default;
		virtual ~Package () {};
		virtual std::shared_ptr<Namespace> own_namespace() const override ;

		virtual const std::unordered_map<std::string,std::shared_ptr<Packageable_element>>& package_elements () const ;
        virtual const std::unordered_map<std::string, std::shared_ptr<Package> >& nested_packages() const ;
		virtual const std::unordered_map<std::string,std::shared_ptr<Enumeration>>& enumerations () const ;
		virtual const std::unordered_map<std::string,std::shared_ptr<Primitive_type>>& primitive_types () const ;
		virtual const std::unordered_map<std::string,std::shared_ptr<Select_type>>& select_types () const ;
		virtual const std::unordered_map<std::string,std::shared_ptr<Composite_type>>& composite_types () const ;
		virtual const std::unordered_map<std::string,std::shared_ptr<Collection_type>>& collection_types () const ;
		virtual const std::unordered_map<std::string,std::shared_ptr<Restriction_type>>& restriction_types () const ;
		virtual const std::unordered_map<std::string,std::shared_ptr<Class>>& classes () const ;
		virtual const std::unordered_map<std::string,std::shared_ptr<Association>>& associations () const ;
		virtual void accept (Visitor& p_visitor) const override;
        virtual void schema (const std::shared_ptr<Logical_schema>& p_schema) ; 
        virtual const std::shared_ptr<Logical_schema>& schema () const;
        virtual void nesting (const std::shared_ptr<Package>& p_nesting);
        virtual std::shared_ptr<Package> package() const override;
        virtual void insert_element (const std::shared_ptr<Named_element>& p_elem) override;
        virtual void erase_element (const std::shared_ptr<Named_element>& p_elem) override;
        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;

        
        virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override ;


        template <typename T1> void insert (const std::shared_ptr<Packageable_element>& p_elem ) ;
        
        template <typename T1> void rename (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) ;

        
        template <typename T1> void erase (const std::shared_ptr<Packageable_element>& p_elem ) ;

        
        virtual bool has_elements (bool p_active_only = true) const override;
        
 
        virtual void own_namespace ( const std::shared_ptr<Namespace>& p_parent) override;

    virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) override;



	private:
		std::unordered_map<std::string,std::shared_ptr<Package>> nested_packages_;
		std::unordered_map<std::string,std::shared_ptr<Enumeration>> enums_;
		std::unordered_map<std::string,std::shared_ptr<Primitive_type>> primitive_types_;
		std::unordered_map<std::string,std::shared_ptr<Select_type>> select_types_;
		std::unordered_map<std::string,std::shared_ptr<Composite_type>> composite_types_;
		std::unordered_map<std::string,std::shared_ptr<Restriction_type>> restriction_types_;
		std::unordered_map<std::string,std::shared_ptr<Collection_type>> collection_types_;
		std::unordered_map<std::string,std::shared_ptr<Class>> classes_;
		std::unordered_map<std::string,std::shared_ptr<Association>> assocs_;
		std::unordered_map<std::string,std::shared_ptr<Packageable_element>> pk_elems_;
        std::shared_ptr<Logical_schema> schema_;
        std::shared_ptr<Package> nesting_;

        template <typename T> std::unordered_map<std::string,std::shared_ptr<T>>& element_type_map() ;
        template <typename T> const std::unordered_map<std::string,std::shared_ptr<T>>& const_element_type_map() const ;

	};
    
      
}


}

