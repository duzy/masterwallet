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
#include "determaddress.h"

namespace mastercoin
{
    deterministic_address_source::deterministic_address_source(const std::string & seed)
	: determ_()
	, index_(1)
    {
	determ_.set_seed(seed);
    }

    deterministic_address_source::~deterministic_address_source()
    {
    }

    bitcoin::payment_address deterministic_address_source::get_address() const
    {
	bitcoin::payment_address addr;
	bitcoin::set_public_key(addr, determ_.generate_public_key(index_));
	return addr;
    }

    bool deterministic_address_source::next() const
    {
	return true;
    }

}//namespace mastercoin
