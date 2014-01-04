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
#include "networking.h"

namespace mastercoin
{
    constexpr int NETWORKING_THREADS = 2;

    networking::networking()
	: pool_(NETWORKING_THREADS)
	, hosts_(pool_), handshake_(pool_), network_(pool_)
	, protocol_(pool_, hosts_, handshake_, network_)
	, mutex_(), stopped_(false)
	, transactions_(), has_transactions_(), txmutex_(), txcaster_()
    {
    }

    networking::~networking()
    {
    }

    void networking::start()
    {
	/*
	if (!stopped_) stop();
	if (thread_.joinable()) join();
	*/
	if (!stopped_) return;
	subscribe_channel();

	std::lock_guard<std::mutex> lock(txmutex_);
	txcaster_ = std::thread([this](){ txcast(); });
	stopped_ = false;
    }

    void networking::stop()
    {
	std::lock_guard<std::mutex> lock(mutex_);
	stopped_ = true;
	pool_.stop();
    }

    bool networking::stopped()
    {
	std::lock_guard<std::mutex> lock(mutex_);
	return stopped_;
    }

    void networking::join()
    {
	pool_.join();
    }

    void networking::subscribe_channel()
    {
	using std::placeholders::_1;
	using std::placeholders::_2;
	if (stopped()) return;
	protocol_.subscribe_channel(std::bind(&networking::on_new_channel, this, _1, _2));
    }

    void networking::subscribe_transaction(bitcoin::channel_ptr node)
    {
	using std::placeholders::_1;
	using std::placeholders::_2;
	if (stopped() || !node || node->stopped()) return;
	node->subscribe_transaction(std::bind(&networking::on_transaction, this, _1, _2, node));
    }

    void networking::on_new_channel(const std::error_code& ec, bitcoin::channel_ptr node)
    {
	if (ec) {
	    bitcoin::log_error() << "Error starting session: " << ec.message();
	} else {
	    bitcoin::log_debug() << "new channel: " << ec.message();
	}
	subscribe_transaction(node);
	subscribe_channel();
    }

    void networking::on_transaction(const std::error_code& ec, const bitcoin::transaction_type& tx, bitcoin::channel_ptr node)
    {
	if (ec) {
	    bitcoin::log_error() << "Error starting session: " << ec.message();
	} else {
	    //bitcoin::log_debug() << "tx: " << bitcoin::hash_transaction(tx);
	    bitcoin::log_debug() << "tx: " << bitcoin::pretty(tx);
	}
	subscribe_transaction(node);
    }
}
