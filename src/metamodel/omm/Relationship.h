/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Element.h"
#include <vector>
#include <memory>

namespace arolios {




namespace omm {

	class Relationship : public virtual Element {
	public:
		Relationship ( ): Element() {};
		virtual ~Relationship () {};
		virtual std::shared_ptr<const std::vector<std::shared_ptr<Element>>> related_elements() const = 0;

	protected:


	private:

	};

}

}

