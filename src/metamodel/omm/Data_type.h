/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Classifier.h"

namespace arolios {




namespace omm {
    class Logical_type;
	class Data_type : public virtual Classifier {
	public:

		virtual ~Data_type () {};
		Data_type () = default;
		Data_type ( const Data_type& ) = default;

        virtual std::shared_ptr<Logical_struct> logical_struct() const override;
        virtual const std::shared_ptr<Logical_type>& logical_type() const ;
        virtual void logical_type (const std::shared_ptr<Logical_type>& p_type) ;
        virtual std::shared_ptr<Table> table() const override ;
        virtual std::shared_ptr<Table> managing_table() const override ;

        virtual std::string read_select_template(const bool p_force = false ) override;
        virtual std::unordered_map<std::string,std::shared_ptr<Property>> all_id_properties () const override;
        virtual void make_more_specific_ids (const bool p_force = true ) override;





	private:

        std::shared_ptr<Logical_type> type_;

	};
}


}

