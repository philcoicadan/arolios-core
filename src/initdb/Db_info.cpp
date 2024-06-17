/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <dbinit/Db_info.h>
#include <exception/Exceptions.h>
#include <fstream>
#include <iostream>
#include <json/json.h>
#include <string>
#include <unistd.h>
#include <termios.h>


using namespace arolios;
using namespace arolios::dbinit;

Db_info::Db_info() {}

Db_info::~Db_info() {}

void Db_info::load(const std::string &p_json_file) {
  Json::Value jroot;
  std::ifstream ifs;
  ifs.open(p_json_file);

  Json::CharReaderBuilder builder;
  builder["collectComments"] = true;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, ifs, &jroot, &errs)) {

    throw exception::Data_error(errs);
    return;
  }

  const std::string host = jroot.get("host", "127.0.0.1").asString();
  const std::string port = std::to_string(jroot.get("port", 5432).asUInt());
  const std::string dbname = jroot.get("dbname", "").asString();
  if (dbname.empty()) {
    throw exception::Not_found("Parameter dbname not found");
  }

  const std::string user = jroot.get("user", "postgres").asString();

  // password input, declaration of specific function
  const std::function<void(bool)> echo([](bool on = true) -> void {
    struct termios settings;
    tcgetattr(STDIN_FILENO, &settings);
    settings.c_lflag =
        on ? (settings.c_lflag | ECHO) : (settings.c_lflag & ~(ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &settings);
  });
  // default password is empty
  const std::string default_password;
  std::cout << "Enter user password (hit return if empty) :";
  std::string password;
  echo(false);
  std::getline(std::cin, password);
  echo(true);
  std::cout << std::endl;

  if (password.empty()) {
    password = default_password;
  }

  const std::string client_encoding =
      jroot.get("client_encoding", "").asString();

  // escape value of string, declaration of specific function
  const std::function<std::string (const std::string&)> escape_string_value ([] (const std::string& value) ->
    std::string {
        bool must_be_quoted =
            value.empty() || (value.find(' ') != std::string::npos);

        std::string result = must_be_quoted ? "'" : "";
        for (auto c : value) {
            if (c == '\'') {
                result.push_back('\\');
            }

            result.push_back(c);
        }

        if (must_be_quoted) {
            result += "'";
        }
        return result;
    } ) ;

   conn_ = "host=" + escape_string_value(host);
   conn_ += " port=" + escape_string_value(port);
   conn_ += " dbname=" + escape_string_value(dbname);
   conn_ += " user=" + escape_string_value(user);

   if (!password.empty()) {
    conn_ += " password=" + escape_string_value(password);
   }

    if (!client_encoding.empty()) {
     conn_ += " client_encoding=" + escape_string_value(client_encoding);
       
    }
}

std::string Db_info::connection() const {
    if (conn_.empty()) {
        throw exception::Failure ("Connection information not available");
    }
    return conn_;
}