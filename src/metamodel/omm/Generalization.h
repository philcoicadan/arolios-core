/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Relationship.h"

namespace arolios {





namespace omm {
	class Classifier;
	class Generalization : public virtual Relationship {
	public:
		Generalization ();
		Generalization ( const Generalization& ) = default;
		virtual ~Generalization () {};
		virtual std::shared_ptr<const std::vector<std::shared_ptr<Element>>> related_elements() const override;
		virtual std::shared_ptr<Classifier> general () const { return general_; }
		virtual std::shared_ptr<Classifier> specific () const { return specific_; }
		virtual void general (const std::shared_ptr<Classifier>& p_general) {  general_=p_general; }
		virtual void specific (const std::shared_ptr<Classifier>& p_specific)  {  specific_=p_specific; }
		virtual void accept (Visitor& p_visitor) const override;
	protected:


	private:
		std::shared_ptr<Classifier> general_;
		std::shared_ptr<Classifier> specific_;

	};

}


}

