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
#ifndef __TX_BROADCAST_H___DUZY__
#define __TX_BROADCAST_H___DUZY__ 1
#include "all.h"

namespace mastercoin
{
    class transaction_broadcaster
    {
    public:
	transaction_broadcaster();

	void start();
	void stop();
	void join();

	void send(const bitcoin::transaction_type & tx);

    private:
	void run();
	void process(bitcoin::protocol & proto);
	void process_one(bitcoin::protocol & proto, const bitcoin::transaction_type & tx);

	void on_start(const std::error_code& ec);
	void on_stop(const std::error_code& ec);

    private:
	bitcoin::threadpool pool_; // using a new pool
	bitcoin::transaction_list transactions_;
	std::condition_variable has_transactions_;
	std::mutex mutex_;
	bool stopped_;
	std::thread thread_;
    };
}//namespace mastercoin

#endif//__TX_BROADCAST_H___DUZY__
