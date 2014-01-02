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
#include "addressbook.h"
#include "determaddress.h"

namespace mastercoin
{
    address_book::address_book()
	: sources_()
	, mutex_()
    {
    }

    address_book::~address_book()
    {
    }

    void address_book::add_seed(const std::string & seed)
    {
	std::lock_guard<std::mutex> lock(mutex_);
	address_source::pointer p(new deterministic_address_source(seed));
	sources_.push_back(p);
    }

    void address_book::add_source(const address_source::pointer & source)
    {
	std::lock_guard<std::mutex> lock(mutex_);
	sources_.push_back(source);
    }
    
}//namespace mastercoin
