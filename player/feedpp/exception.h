/* Copyright (C) 2016 Behrooz Aliabadi
 * Licensed under the MIT/X Consortium License.
 * See file LICENSE for more information.
 */

#pragma once

#include <exception>
#include <string>

class exception : public std::exception {

 public:
  exception(const std::string& errmsg = "", const int errcode = 0);
  ~exception() noexcept;
  virtual const char* what() const noexcept;

 private:
  std::string emsg;
  int         ecode;

};

