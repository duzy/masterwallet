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
#include "txbroadcast.h"

namespace mastercoin
{
    constexpr static std::uint32_t TX_THREADS = 2;
    constexpr static const char * domain = "transaction_broadcaster";

    transaction_broadcaster::transaction_broadcaster()
	: pool_(TX_THREADS)
	, transactions_()
	, has_transactions_()
	, mutex_()
	, stopped_(false)
	, thread_()
    {
    }

    void transaction_broadcaster::run()
    {
	bitcoin::hosts hosts(pool_);
	bitcoin::handshake handshake(pool_);
	bitcoin::network network(pool_);
	bitcoin::protocol proto(pool_, hosts, handshake, network);
	proto.set_max_outbound(TX_THREADS);
	proto.start([this](const std::error_code& ec){ on_start(ec); });
	this->process(proto);
	proto.stop([this](const std::error_code& ec){ on_stop(ec); });
	pool_.stop();
	pool_.join();
    }

    void transaction_broadcaster::send(const bitcoin::transaction_type & tx)
    {
	std::lock_guard<std::mutex> lock(mutex_);
	transactions_.push_back(tx);
	has_transactions_.notify_one();
    }

    void transaction_broadcaster::process(bitcoin::protocol & proto)
    {
	while (!stopped_) {
	    bitcoin::transaction_type tx;
	    {
		std::unique_lock<std::mutex> lock(mutex_);
		has_transactions_.wait(lock, [this]{ return !transactions_.empty() || stopped_; });
		if (stopped_) break;
		tx = transactions_.back();
		transactions_.pop_back();
	    }
	    process_one(proto, tx);
	}
    }

    void transaction_broadcaster::process_one(bitcoin::protocol & proto, const bitcoin::transaction_type & tx)
    {
	// TODO: ...
    }

    void transaction_broadcaster::start()
    {
	if (!stopped_) stop();
	if (thread_.joinable()) join();
	std::lock_guard<std::mutex> lock(mutex_);
	thread_ = std::thread([this](){ this->run(); });
	stopped_ = false;
    }

    void transaction_broadcaster::stop()
    {
	if (stopped_) return;
	std::lock_guard<std::mutex> lock(mutex_);
	stopped_ = true;
	has_transactions_.notify_one();
    }

    void transaction_broadcaster::join()
    {
	if (thread_.joinable()) thread_.join();
    }

    void transaction_broadcaster::on_start(const std::error_code& ec)
    {
	bitcoin::log_debug(domain) << ec ;
    }

    void transaction_broadcaster::on_stop(const std::error_code& ec)
    {
	bitcoin::log_debug(domain) << ec ;
    }

}//namespace mastercoin
