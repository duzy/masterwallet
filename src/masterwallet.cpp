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
#include "masterwallet.h"

namespace mastercoin
{
    master_wallet::master_wallet()
	: addresses_(), addrmutex_()
	, addbook_()
    {
    }

    master_wallet::~master_wallet()
    {
    }

    money master_wallet::get_balance(currency cc) const
    {
	std::lock_guard<std::mutex> lock(const_cast<master_wallet*>(this)->addrmutex_);
	uint64_t balance = 0;
	for (const auto & addr : addresses_) {
	    balance += addr->balance.get_amount(cc);
	}
	return money(cc, balance);
    }

    void master_wallet::add_address(const bitcoin::payment_address & address, const money & m)
    {
	std::lock_guard<std::mutex> lock(addrmutex_);
	address_details_ptr a(new address_details);
	a->address = address;
	a->balance.set_amount(m);
	addresses_.push_back(a);
    }

    void master_wallet::start()
    {
    }

    void master_wallet::stop()
    {
    }

    void master_wallet::join()
    {
    }

}//namespace mastercoin
