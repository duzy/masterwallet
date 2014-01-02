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
#ifndef __LOGGING_H___DUZY__
#define __LOGGING_H___DUZY__ 1
#include "all.h"
#include <iosfwd>

namespace mastercoin
{
    class logger
    {
	logger(std::ostream & debug, std::ostream & error);

    public:
	virtual ~logger();

	void operator()(bitcoin::log_level level, const std::string& domain, const std::string& body);

	/**
	 *  Setup logger with the specified streams.
	 *   
	 *  @code
	 *   int main()
	 *   {
	 *       std::ofstream outdebug("debug.log");
	 *       std::ofstream outerror("error.log");
	 *       mastercoin::logger::setup(outdebug, outerror);
	 *
	 *       // ...
	 *   }
	 *  @endcode
	 */
	static void setup(std::ostream & outdebug, std::ostream & outerror);

    protected:
	virtual void info(const std::string& domain, const std::string& body);
	virtual void warning(const std::string& domain, const std::string& body);
	virtual void debug(const std::string& domain, const std::string& body);
	virtual void error(const std::string& domain, const std::string& body);
	virtual void fatal(const std::string& domain, const std::string& body);

    protected:
	std::ostream & debug_;
	std::ostream & error_;
    };
}

#endif//__LOGGING_H___DUZY__
