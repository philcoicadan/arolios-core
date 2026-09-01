/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <common/App_info.h>
#include <common/Db_client.h>
#include <common/Object_model_info.h>
#include <common/Singleton.h>
#include <common/UAC_info.h>
#include <drogon/orm/DbClient.h>
#include <iostream>
#include <qry/Return_code.h>
#include <dbinit/Loader.h>
#include <dbinit/Db_info.h>
#include <thread>



using namespace arolios;
using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
  std::cout << "AROLIOS is a trademark registered in France." << std::endl
  << "AROLIOS Core is a software library implementing the AROLIOS features." << std::endl
  << "Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved." << std::endl 
  << "License: LGPL-3.0-or-later" << std::endl
  ;
    int expected_args = 3 ;
  if (argc > expected_args) {
    try {
      const std::string db_info_file(argv[1]);
      const std::string params_file(argv[2]);
      const std::string standard_types_file(argv[3]);

      dbinit::Db_info db_info;
      db_info.load(db_info_file);

      auto clientPtr = drogon::orm::DbClient::newPgClient(
               db_info.connection(),
               1) ;

        std::this_thread::sleep_for(1s);

      common::Singleton<common::Db_client>::instance().object(clientPtr);

      std::shared_ptr<common::Object_model_info> mdl_info =
          std::make_shared<common::Object_model_info>();
      common::Singleton<common::Object_model_info>::instance().object(mdl_info);

      std::shared_ptr<common::App_info> app_info =
          std::make_shared<common::App_info>();
      common::Singleton<common::App_info>::instance().object(app_info);

      app_info->init_root();

      std::shared_ptr<common::UAC_info> uac_info =
          std::make_shared<common::UAC_info>();
      common::Singleton<common::UAC_info>::instance().object(uac_info);

      dbinit::Loader init_l(params_file, standard_types_file);

      init_l.execute();

      if (init_l.return_code() == qry::Return_code::OK) {
        std::cout << "Database initialized successfully" << std::endl;
        std::exit(EXIT_SUCCESS);
      } else {
        std::cout << "Database initialization failed" << std::endl;

      }
    } catch (std::exception const &e) {

      throw e;
    }

    
  }
  else {
    std::cerr << "Argument missing, " << expected_args << " arguments are expected" << std::endl;
  }
  std::exit(EXIT_FAILURE);
}
