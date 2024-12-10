--
--
-- Copyright (C) 2024 Philippe Coicadan. All rights reserved.
-- License: LGPL-3.0-or-later
--
--

 CREATE EXTENSION pgcrypto;
 
CREATE TYPE am_lc AS ENUM ('active', 'deprecated', 'deleted');

CREATE TABLE am_init_param (
    i_key varchar(64) PRIMARY KEY,
    i_value varchar(64) NOT NULL
    );
    

CREATE TABLE am_pk (
	id serial PRIMARY KEY,
	name varchar(64) NOT NULL,
	description text,
	lc am_lc DEFAULT 'active',
	-- c_date timestamp with time zone NOT NULL,
	-- m_date	timestamp with time zone NOT NULL,
	-- version integer NOT NULL CHECK(version > 0),
	nesting integer REFERENCES am_pk ON DELETE CASCADE,
	UNIQUE (name, nesting)
	);

CREATE TYPE am_elem_t AS ENUM ('type', 'class', 'association', 'enum');

CREATE TABLE am_p_elem (
	id serial PRIMARY KEY,
	name varchar(64) NOT NULL,
	description text,
	is_abstract boolean DEFAULT FALSE,
	lc am_lc DEFAULT 'active',
	-- c_date timestamp with time zone NOT NULL,
	-- m_date	timestamp with time zone NOT NULL,
	-- version integer NOT NULL CHECK(version > 0),
	elem_type am_elem_t NOT NULL, -- enum pour type, class, association ou enum
	package integer REFERENCES am_pk ON DELETE CASCADE NOT NULL,
	base integer REFERENCES am_p_elem, -- elem_types référencés doivent avoir elem_type = class ou type ou association
	UNIQUE (name, package)
	);

CREATE TABLE am_enum_val (
	id serial PRIMARY KEY,
	e_value varchar(64) NOT NULL,
    lc am_lc DEFAULT 'active',
	e_elem integer REFERENCES am_p_elem ON DELETE CASCADE NOT NULL, -- elem_type reférencé doit avoir elem_type = enum
	UNIQUE (e_value, e_elem)
	);

CREATE TABLE am_property ( 
	id serial PRIMARY KEY,
	name varchar(64) NOT NULL,
	description text,
	lc am_lc DEFAULT 'active',
	-- c_date timestamp with time zone NOT NULL,
	-- m_date	timestamp with time zone NOT NULL,
	p_type integer REFERENCES am_p_elem ON DELETE RESTRICT NOT NULL, -- targetClass for assoc
	owning_t integer REFERENCES am_p_elem ON DELETE CASCADE NOT NULL, -- type, class or assoc
	min_occurs integer DEFAULT 0 CHECK(min_occurs >=0),
	max_occurs integer DEFAULT 1,
	-- deprecated part_of_id boolean DEFAULT FALSE,
	computed varchar(64), -- only when owning_t is a type or class
	default_v varchar(64), --only when owning_t is a type or class
	is_member_end boolean DEFAULT FALSE -- true if a row in am_member_end exists,
	CHECK (  max_occurs >= min_occurs OR  max_occurs = -1),
	UNIQUE (name, owning_t)
	);
	
CREATE TABLE am_member_end (
	property integer REFERENCES am_property ON DELETE CASCADE NOT NULL,
	assoc integer REFERENCES am_p_elem ON DELETE CASCADE NOT NULL,
	source integer REFERENCES am_p_elem ON DELETE RESTRICT NOT NULL,
	c_property boolean DEFAULT TRUE, 
	is_navigable boolean DEFAULT TRUE,
	is_compo boolean DEFAULT FALSE, 
	PRIMARY KEY (property)
	);


CREATE TABLE am_primitive_type (
	primitive integer REFERENCES am_p_elem ON DELETE CASCADE NOT NULL,
	native varchar(64),
	PRIMARY KEY (primitive)
	);
	
CREATE TABLE am_csf_ident (
	i_csf integer REFERENCES am_p_elem ON DELETE CASCADE NOT NULL, -- class or association
    i_prop integer REFERENCES am_property ON DELETE CASCADE NOT NULL,
    PRIMARY KEY (i_csf, i_prop)
	);		


	
CREATE TABLE am_schema (
	l_package integer REFERENCES am_pk ON DELETE CASCADE NOT NULL,
	-- l_type am_obj_type NOT NULL,
	l_name varchar(64) NOT NULL,
	PRIMARY KEY (l_package)
	);	
	
CREATE TYPE am_struct_type AS ENUM  ('table', 'type', 'enumeration', 'null' ) ;	

CREATE TABLE am_csf_struct (
	l_elem integer REFERENCES am_p_elem ON DELETE CASCADE NOT NULL,
	l_type am_struct_type NOT NULL,
	l_name varchar(64),
	l_parent integer REFERENCES am_schema ON DELETE CASCADE, 
	CHECK ( l_type <> 'null' OR ( l_name <> NULL AND l_parent <> NULL)),
	PRIMARY KEY (l_elem)
	);		

CREATE TYPE am_field_type AS ENUM  ( 'column', 'attribute', 'sub_field' ) ;	
	
CREATE TABLE am_prop_field (
	l_property integer REFERENCES am_property ON DELETE CASCADE NOT NULL,
	l_type am_field_type NOT NULL,
	l_name varchar(64) NOT NULL,
	l_parent integer REFERENCES am_csf_struct ON DELETE CASCADE NOT NULL, 
	PRIMARY KEY (l_property)
	);	
	
CREATE TYPE am_column_constr_type AS ENUM ( 'not_null', 'unique' );

CREATE TABLE am_column_constraint (
    l_depends_on integer REFERENCES am_p_elem ON DELETE CASCADE NOT NULL,
    l_column integer REFERENCES am_prop_field ON DELETE CASCADE NOT NULL,
    l_constr_type am_column_constr_type NOT NULL,
    PRIMARY KEY (l_depends_on,l_column,l_constr_type)
    ) ;
    
CREATE TABLE am_app (
	id serial PRIMARY KEY,
	upd_count integer DEFAULT 1,
	name varchar(64) NOT NULL UNIQUE,
	description text
	);
	
CREATE TABLE am_app_domain (
    id serial PRIMARY KEY,
    a_domain integer REFERENCES am_pk ON DELETE CASCADE NOT NULL,
    a_app integer REFERENCES am_app ON DELETE CASCADE NOT NULL,
    UNIQUE
    (a_domain, a_app)
    );

CREATE TABLE am_app_prop (
    a_prop integer REFERENCES am_property ON DELETE CASCADE NOT NULL,
    a_csf integer REFERENCES am_p_elem ON DELETE CASCADE NOT NULL,
    a_app integer REFERENCES am_app ON DELETE CASCADE NOT NULL,
    PRIMARY KEY (a_prop, a_csf, a_app)
    );
    

CREATE TABLE am_language (
	id serial PRIMARY KEY,
	code varchar(64) UNIQUE NOT NULL,
	name varchar(64) NOT NULL,
	fts_config varchar(64) NOT NULL,
	description text,
	lc am_lc DEFAULT 'active'
	);
	
CREATE TABLE am_pk_tlt (
    id bigserial PRIMARY KEY,
    t_elem integer REFERENCES am_pk ON DELETE CASCADE NOT NULL,
    t_lang integer REFERENCES am_language ON DELETE CASCADE NOT NULL,
    t_name varchar(64) NOT NULL,
    UNIQUE (t_elem, t_lang)
    );
    
	
CREATE TABLE am_p_elem_tlt (
    id bigserial PRIMARY KEY,
    t_elem integer REFERENCES am_p_elem ON DELETE CASCADE NOT NULL,
    t_lang integer REFERENCES am_language ON DELETE CASCADE NOT NULL,
    t_name varchar(64) NOT NULL,
    UNIQUE (t_elem, t_lang)
    );

    	
CREATE TABLE am_property_tlt (
    id bigserial PRIMARY KEY,
    t_elem integer REFERENCES am_property ON DELETE CASCADE NOT NULL,
    t_lang integer REFERENCES am_language ON DELETE CASCADE NOT NULL,
    t_name varchar(64) NOT NULL,
    UNIQUE (t_elem, t_lang)
    );

CREATE TABLE am_enum_val_tlt (
    id bigserial PRIMARY KEY,
    t_elem integer REFERENCES am_enum_val ON DELETE CASCADE NOT NULL,
    t_lang integer REFERENCES am_language ON DELETE CASCADE NOT NULL,
    t_name varchar(64) NOT NULL,
    UNIQUE (t_elem, t_lang)
    );
    
CREATE TABLE am_trash (
	id bigserial PRIMARY KEY,
    csf_id integer REFERENCES am_p_elem ON DELETE RESTRICT UNIQUE NOT NULL -- mark classifier having at least one instance in the trash can.
	
	);

CREATE TYPE am_user_role AS ENUM ('no', 'reader', 'producer', 'admin');


CREATE TABLE am_user (
    id serial PRIMARY KEY,
    upd_count integer DEFAULT 1,
    u_identifier varchar(64) UNIQUE NOT NULL,
    -- u_first_name varchar(64),
    -- u_last_name varchar(64) NOT NULL,
    u_pwd varchar(64) NOT NULL,
    u_role am_user_role NOT NULL,
	u_lc am_lc DEFAULT 'active'
    );
    
CREATE TABLE am_app_user (
    id serial PRIMARY KEY,
    au_user integer REFERENCES am_user ON DELETE CASCADE NOT NULL,
    au_app integer REFERENCES am_app ON DELETE CASCADE NOT NULL,
    au_role am_user_role NOT NULL,
    UNIQUE (au_user, au_app)
    );

CREATE FUNCTION am_translate_enum ( integer, integer) RETURNS text AS $$
SELECT t_name FROM am_enum_val_tlt WHERE t_elem = $1 AND t_lang = $2; $$ LANGUAGE SQL;

CREATE FUNCTION am_translate_domain ( integer, integer) RETURNS text AS $$
SELECT t_name FROM am_pk_tlt WHERE t_elem = $1 AND t_lang = $2; $$ LANGUAGE SQL;

CREATE FUNCTION am_date_to_text ( date) RETURNS text AS $$ 
SELECT TO_CHAR($1, 'YYYY-MM-DD') ; $$ LANGUAGE SQL IMMUTABLE;

CREATE FUNCTION am_timestamp_to_text ( timestamp without time zone) RETURNS text AS $$ 
SELECT TO_CHAR($1, 'YYYY-MM-DD') ; $$ LANGUAGE SQL IMMUTABLE;

CREATE FUNCTION am_timestamp_tz_to_text ( timestamp with time zone) RETURNS text AS $$ 
SELECT TO_CHAR($1, 'YYYY-MM-DD') ; $$ LANGUAGE SQL IMMUTABLE;


create function immut_char (timestamp without time zone) returns text AS  $$ select to_char($1, 'YYYY-MM-DD') ; $$ language sql immutable;