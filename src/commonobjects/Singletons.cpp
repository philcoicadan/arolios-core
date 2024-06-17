/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
 #include <common/UAC_info.h>
 #include <common/App_info.h>
 #include <common/Object_model_info.h>
 #include <common/Db_client.h>

 #include "Singleton.tpp"
namespace arolios::common 
{
 template class Singleton<UAC_info>;
 template class Singleton<App_info>;
 template class Singleton<Object_model_info> ;
 template class Singleton<Db_client>;
}