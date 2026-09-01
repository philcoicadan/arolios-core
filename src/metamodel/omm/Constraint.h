/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Packageable_element.h"
#include "Namespace.h"

namespace arolios {





namespace omm {

	class Classifier;
	class Constraint : public virtual Packageable_element {
	public:

		virtual ~Constraint () {}
		Constraint ();
		Constraint ( const Constraint& ) = default;
		virtual std::shared_ptr<Namespace> own_namespace() const override;
		virtual const std::string& expression () const { return expression_; }
		virtual void expression (const std::string& p_expression) { expression_ = p_expression; }
		virtual const std::shared_ptr<Classifier>& classifier () const;
		virtual void classifier (const std::shared_ptr<Classifier>& p_csf);
        virtual std::shared_ptr<Package> package() const override ;
        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;
    	virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) override;


		virtual void accept (Visitor& p_visitor) const override;
   
        virtual void own_namespace(const std::shared_ptr<Namespace>& p_namespace) override;

	private:
		std::shared_ptr<Classifier> classifier_;
		std::string expression_ ;

	} ;
}

}

