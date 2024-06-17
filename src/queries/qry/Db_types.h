/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>

namespace arolios {





namespace qry {

using Db_row = drogon::orm::Row ;
using Db_result = drogon::orm::Result;
using Db_field = drogon::orm::Field ;
using Db_transaction = drogon::orm::Transaction;




}
}

