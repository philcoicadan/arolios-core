/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once

namespace arolios {



// Design pattern Visitor: visit method for concrete classes


    

namespace omm {


	class Association;
	class Class;
	class Enumeration;
	class Data_type;
	class Package;
	class Property;
	class Constraint;
	class Generalization;
    class Logical_field;
    class Logical_struct;
    class Logical_schema;
    class Table;
    class Logical_type;
    class Logical_enum;
    class Column;
    class Attribute;
    class Property_by_sub_field;
	class Enum_value;
	class Language;
	class Restriction_type;
	class Collection_type;
	class Select_type;
	class Primitive_type;
	class Composite_type;
	
	
	class Visitor {
	public:
		virtual ~Visitor () {};

		virtual void visit (const Package& p_elem) ;
		virtual void visit (const Association& p_elem) ;
		virtual void visit (const Class& p_elem) ;
		virtual void visit (const Enumeration& p_elem) ;
		virtual void visit (const Data_type& p_elem) ;
		virtual void visit (const Property& p_elem) ;
		virtual void visit (const Constraint& p_elem) ;
		virtual void visit (const Generalization& p_elem) ;
        virtual void visit (const Logical_schema& p_elem) ;
        virtual void visit (const Table& p_elem) ;
        virtual void visit (const Logical_type& p_elem) ;
        virtual void visit (const Logical_enum& p_elem) ;
        virtual void visit (const Column& p_elem) ;
        virtual void visit (const Attribute& p_elem) ;
        virtual void visit (const Property_by_sub_field& p_elem);
        virtual void visit (const Enum_value& p_elem);
        virtual void visit (const Language& p_elem);
        virtual void visit (const Restriction_type& p_elem);
        virtual void visit (const Collection_type& p_elem);
        virtual void visit (const Select_type& p_elem);
        virtual void visit (const Primitive_type& p_elem);
        virtual void visit (const Composite_type& p_elem);

	protected:
		Visitor () {};
	};
}


}

