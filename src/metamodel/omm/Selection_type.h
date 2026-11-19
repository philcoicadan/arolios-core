/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Classifier.h"
#include <vector>

namespace arolios {


namespace omm {
	class Selection_type : public virtual Classifier {
	public:

		virtual ~Selection_type () {};
		Selection_type () = default;
		Selection_type ( const Selection_type& ) = default;

		virtual bool has_properties () const override;
		virtual bool has_generalizations () const override;
        virtual bool has_id () const  override;

        virtual bool has_elements (bool p_active_only = true) const override;
		virtual void accept (Visitor& p_visitor) const override;
        
        virtual std::shared_ptr<Logical_struct> logical_struct() const override;
        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;

        virtual void insert_element ( const std::shared_ptr<Named_element>& p_elem) override;
        virtual void erase_element ( const std::shared_ptr<Named_element>& p_elem) override;
              
        virtual std::shared_ptr<Named_element> find_element ( const std::string& p_name) const override ;

        virtual const std::string& meta_type() const override;

     	virtual void rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) override;

        virtual void options(const std::vector<std::shared_ptr<Classifier>>& p_classifiers);
        virtual const std::vector<std::shared_ptr<Classifier>>& options() const;

	private:

        std::vector<std::shared_ptr<Classifier>> options_;
        
	};
}
  
  
}
