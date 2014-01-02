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
#ifndef __DETERM_ADDRESS_H___DUZY__
#define __DETERM_ADDRESS_H___DUZY__ 1
#include "all.h"
#include "addressource.h"

namespace mastercoin
{
    class deterministic_address_source : public address_source
    {
    public:
	deterministic_address_source(const std::string & seed);
	virtual ~deterministic_address_source();

	virtual bitcoin::payment_address get_address() const;
	virtual bool next() const;

    private:
	libwallet::deterministic_wallet determ_;
	std::size_t index_;
    };
}//namespace mastercoin

#endif//__DETERM_ADDRESS_H___DUZY__
