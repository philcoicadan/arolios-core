/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Property_attributes.h>


using namespace arolios;
using namespace arolios::omm;

Property_attributes::Property_attributes() : min_ (0) , max_ (1), aggregation_ (Aggregation_kind::none), type_ (nullptr) , source_member_end_ (nullptr) , collection_ (nullptr) , is_scp_ (true), is_member_end_ (false), opposite_name_ (""), is_navigable_ (true) {

}

Property_attributes::~Property_attributes() {

}
