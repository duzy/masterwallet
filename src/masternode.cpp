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
#ifndef __LOGGING_H___DUZY__
#define __LOGGING_H___DUZY__ 1
#include "masternode.h"
#include "logging.h"
#include <future>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace mastercoin
{
    master_node::master_node()
	: networking_()
	, masterchain_()
	, session_(networking_.get_pool(), { networking_.get_handshake(), networking_.get_protocol(), masterchain_.get_blockchain(), masterchain_.get_poller(), masterchain_.get_txpool() })
    {
    }

    master_node::~master_node()
    {
    }

    void master_node::start()
    {
	networking_.start();
	masterchain_.start();
	session_.start(std::bind(&master_node::on_session_start, this, _1));
    }

    void master_node::stop()
    {
	std::promise<std::error_code> promise_session_stop;
	session_.stop([&promise_session_stop](const std::error_code& ec) { promise_session_stop.set_value(ec); });
	std::error_code ec = promise_session_stop.get_future().get();
	if (ec) {
	    bitcoin::log_error() << "Error stopping session: " << ec.message();
	}

	networking_.stop();
	masterchain_.stop();
    }

    void master_node::join()
    {
	networking_.join();
	masterchain_.join();
    }

    void master_node::on_session_start(const std::error_code& ec)
    {
	if (ec) {
	    bitcoin::log_error() << "Error starting session: " << ec.message();
	}
    }

}

#endif//__LOGGING_H___DUZY__
