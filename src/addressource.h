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
#ifndef __ADDRESS_SOURCE_H___DUZY__
#define __ADDRESS_SOURCE_H___DUZY__ 1
#include "all.h"

namespace mastercoin
{
    class address_source : public std::enable_shared_from_this<address_source>
    {
    protected:
	address_source() {}
	virtual ~address_source() {}

    public:
	typedef std::shared_ptr<address_source> pointer;

	/**
	 *  @code
	 *   address_source::pointer p(...);
	 *   do {
	 *       bitcoin::payment_address addr = p->get_address();
	 *       // ...
	 *   } while (p->next());
	 *  @endcode
	 */
	virtual bitcoin::payment_address get_address() const = 0;
	virtual bool next() const = 0;
    };
}//namespace mastercoin

#endif//__ADDRESS_SOURCE_H___DUZY__

