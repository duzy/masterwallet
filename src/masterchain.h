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
#ifndef __MASTER_CHAIN_H___DUZY__
#define __MASTER_CHAIN_H___DUZY__ 1
#include "all.h"

namespace mastercoin
{
    class master_chain
    {
    public:
	master_chain();
	virtual ~master_chain();

	void start();
	void stop();
	void join();

	bitcoin::blockchain & get_blockchain() { return *blockchain_; }
	bitcoin::poller & get_poller() { return poller_; }
	bitcoin::transaction_pool & get_txpool() { return txpool_; }
	bitcoin::transaction_indexer & get_txindex() { return txindex_; }

    private:
	void initchain();

    private:
	bitcoin::threadpool pool_disk_, pool_mem_;
	std::shared_ptr<bitcoin::blockchain> blockchain_;
	bitcoin::poller poller_;
	bitcoin::transaction_pool txpool_;
	bitcoin::transaction_indexer txindex_;
    };
}

#endif//__MASTER_CHAIN_H___DUZY__
