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
#ifndef __ADDRESS_BOOK_H___DUZY__
#define __ADDRESS_BOOK_H___DUZY__ 1
#include "all.h"
#include "addressource.h"

namespace mastercoin
{
    class address_book
    {
    public:
	address_book();
	virtual ~address_book();

	void add_seed(const std::string & seed);
	void add_source(const address_source::pointer & source);

	std::size_t get_source_count() const { return sources_.size(); }
	address_source::pointer get_source(std::size_t n) const { return sources_[n]; }

    private:
	std::vector<address_source::pointer> sources_;
	std::mutex mutex_;
    };
}//namespace mastercoin

#endif//__ADDRESS_BOOK_H___DUZY__

