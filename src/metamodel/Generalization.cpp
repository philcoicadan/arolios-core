/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Classifier.h>
#include <omm/Generalization.h>
#include <omm/Visitor.h>


using namespace arolios;
using namespace arolios::omm;

Generalization::Generalization ( ) {
	general_ = nullptr; specific_ = nullptr;
}

std::shared_ptr<const std::vector<std::shared_ptr<Element>>> Generalization::related_elements() const {
	std::shared_ptr<std::vector<std::shared_ptr<Element>>> result = std::make_shared<std::vector<std::shared_ptr<Element>>>();
	result->push_back(std::static_pointer_cast<Element>(std::static_pointer_cast<Namespace>(general())));
	result->push_back(std::static_pointer_cast<Element>(std::static_pointer_cast<Namespace>(specific())));
	return result;
}


void Generalization::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}
