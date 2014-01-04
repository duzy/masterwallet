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
#include "masterchain.h"
#include <future>

namespace mastercoin
{
    constexpr int BLOCKCHAIN_THREADS = 2;
    constexpr int TXPOOL_THREADS = 2;

    constexpr const char * get_db_path() { return "blockchain"; }

    master_chain::master_chain()
	: pool_disk_(BLOCKCHAIN_THREADS), pool_mem_(TXPOOL_THREADS)
	, blockchain_(new bitcoin::leveldb_blockchain(pool_disk_))
	, poller_(pool_mem_, *blockchain_), txpool_(pool_mem_, *blockchain_), txindex_(pool_mem_)
    {
    }

    master_chain::~master_chain()
    {
    }

    void master_chain::initchain()
    {
	bitcoin::leveldb_blockchain *chain = (bitcoin::leveldb_blockchain*) blockchain_.get();
	std::promise<std::error_code> promise_import;
	bitcoin::block_type first_block = bitcoin::genesis_block();
	chain->import(first_block, 0, [&promise_import](const std::error_code& ec) { promise_import.set_value(ec); });
	std::error_code ec = promise_import.get_future().get();
	if (ec) {
	    bitcoin::log_error() << "Error importing genesis block: " << ec.message();
	    return;
	}

	bitcoin::log_info()
	    << "Genesis block: "
	    << bitcoin::hash_block_header(first_block.header);
    }

    void master_chain::start()
    {
	bitcoin::leveldb_blockchain *chain = (bitcoin::leveldb_blockchain*) blockchain_.get();

	std::promise<std::error_code> blockchain_promise;
	chain->start(get_db_path(), [&blockchain_promise](const std::error_code& ec){ blockchain_promise.set_value(ec); });
	std::error_code ec = blockchain_promise.get_future().get();
	if (ec) {
	    bitcoin::log_error() << "Error starting blockchain: " << ec.message();
	    return;
	}

	initchain();

	txpool_.start();
    }

    void master_chain::stop()
    {
	pool_disk_.stop();
	pool_mem_.stop();
	//((bitcoin::leveldb_blockchain*)blockchain_.get())->stop();
    }

    void master_chain::join()
    {
	pool_disk_.join();
	pool_mem_.join();

	((bitcoin::leveldb_blockchain*)blockchain_.get())->stop();
    }
}
