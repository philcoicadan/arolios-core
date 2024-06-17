/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Element_factory.h>



using namespace arolios;
using namespace arolios::model;






Element_factory::Element_factory() {

}

Element_factory::~Element_factory() {

}

Element_factory& Element_factory::instance () {
    static Element_factory inst = Element_factory();
    return inst;
}

