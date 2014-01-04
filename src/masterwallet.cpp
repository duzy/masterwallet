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
	: pool_(thread_pool_size)
	, addresses_()
	, addbook_()
    {
    }

    master_wallet::~master_wallet()
    {
    }

    money master_wallet::get_balance(currency cc) const
    {
	uint64_t balance = 0;
	for (const auto & addr : addresses_) {
	    balance += addr.balance.get_amount(cc);
	}
	return money(cc, balance);
    }

    void master_wallet::add_address(const bitcoin::payment_address & address, const money & m)
    {
	address_details a;
	a.address = address;
	a.balance.set_amount(m);
	addresses_.push_back(a);
    }

    void master_wallet::start()
    {
    }

    void master_wallet::stop()
    {
	pool_.stop();
	{
	    std::lock_guard<std::mutex> lock(txmutex_);
	    stopped_ = true;
	    has_transactions_.notify_one();
	}
    }

    void master_wallet::join()
    {
	if (txcaster_.joinable()) txcaster_.join();
	txcaster_ = std::thread();

	pool_.join();
    }

    void master_wallet::txcast()
    {
	while (!stopped_) {
	    transaction tx;
	    {
		std::unique_lock<std::mutex> lock(txmutex_);
		has_transactions_.wait(lock, [this]{ return !transactions_.empty() || stopped_; });
		if (stopped_) break;
		tx = transactions_.back();
		transactions_.pop_back();
	    }
	    get_protocol()
	    process_tx(tx);
	}
    }

    void master_wallet::process_tx(const transaction & tx)
    {
	//bitcoin::log_debug() << "txcast: " << bitcoin::pretty(tx);
    }
    
}//namespace mastercoin
