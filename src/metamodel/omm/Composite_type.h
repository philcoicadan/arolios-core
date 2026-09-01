/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Data_type.h"

namespace arolios {
namespace omm {
    class Composite_type : public virtual Data_type {
    public:
        virtual ~Composite_type () {};
        Composite_type () = default;
        Composite_type ( const Composite_type& ) = default;

        virtual void accept (Visitor& p_visitor) const override;
        virtual const std::string& meta_type() const override;

        virtual void insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) override;
        virtual void rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) override;
    };
}
}
