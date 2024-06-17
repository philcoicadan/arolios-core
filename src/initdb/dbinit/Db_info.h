/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <string>

namespace arolios {
 #pragma once


 namespace dbinit {

    class Db_info {
        public: 

        Db_info () ;
        virtual ~Db_info ();

        virtual void load (const std::string& filename);

        virtual std::string connection () const;

        private:

        std::string conn_;
    } ;
 }}

