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
#ifndef __NETWORKING_H___DUZY__
#define __NETWORKING_H___DUZY__ 1
#include "all.h"
#include "transaction.h"

namespace mastercoin
{
    class networking
    {
    public:
	networking();
	virtual ~networking();

	void start();
	void stop();
	void join();

	bool stopped();

	bitcoin::threadpool & get_pool() { return pool_; }
	bitcoin::hosts & get_hosts() { return hosts_; }
	bitcoin::handshake & get_handshake() { return handshake_; }
	bitcoin::protocol & get_protocol() { return protocol_; }

    private:
	void subscribe_channel();
	void subscribe_transaction(bitcoin::channel_ptr node);

	void on_new_channel(const std::error_code& ec, bitcoin::channel_ptr node);
	void on_transaction(const std::error_code& ec, const bitcoin::transaction_type& tx, bitcoin::channel_ptr node);

	void txcast();

    private:
	bitcoin::threadpool pool_;
	bitcoin::hosts hosts_;
	bitcoin::handshake handshake_;
	bitcoin::network network_;
	bitcoin::protocol protocol_;
	std::mutex mutex_;
	bool stopped_;
	std::list<transaction> transactions_;
	std::condition_variable has_transactions_;
	std::mutex txmutex_;
	std::thread txcaster_;
    };
}

#endif//__NETWORKING_H___DUZY__
