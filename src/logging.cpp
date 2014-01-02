/**
 * Copyright (c) 2013 MasterWallet developers (see AUTHORS)
 *
 * This file is part of MasterWallet <https://github.com/duzy/masterwallet>.
 *
 * libwallet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "logging.h"

namespace mastercoin {

    logger::logger(std::ostream & debug, std::ostream & error)
	: debug_(debug), error_(error)
    {
    }

    logger::~logger()
    {
    }

    void logger::setup(std::ostream & outdebug, std::ostream & outerror)
    {
	logger log(outdebug, outerror);
	bitcoin::log_info()   .set_output_function(log);
	bitcoin::log_debug()  .set_output_function(log);
	bitcoin::log_warning().set_output_function(log);
	bitcoin::log_error()  .set_output_function(log);
	bitcoin::log_fatal()  .set_output_function(log);
    }

    void logger::operator()(bitcoin::log_level level, const std::string& domain, const std::string& body)
    {
	switch (level) {
	case bitcoin::log_level::debug:	  this->debug  (domain, body);
	case bitcoin::log_level::info:    this->info   (domain, body);
	case bitcoin::log_level::warning: this->warning(domain, body);
	case bitcoin::log_level::error:	  this->error  (domain, body);
	case bitcoin::log_level::fatal:	  this->fatal  (domain, body);
	}
    }

    void logger::info(const std::string& domain, const std::string& body)
    {
    }

    void logger::warning(const std::string& domain, const std::string& body)
    {
    }

    void logger::debug(const std::string& domain, const std::string& body)
    {
    }

    void logger::error(const std::string& domain, const std::string& body)
    {
    }

    void logger::fatal(const std::string& domain, const std::string& body)
    {
    }

}//namespace mastercoin
