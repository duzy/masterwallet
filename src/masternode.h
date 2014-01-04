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
#ifndef __MASTER_NODE_H___DUZY__
#define __MASTER_NODE_H___DUZY__ 1
#include "all.h"
#include "networking.h"
#include "masterchain.h"

namespace mastercoin
{
    class master_node
    {
    public:
	master_node();
	virtual ~master_node();

	void start();
	void stop();
	void join();

    private:
	void on_session_start(const std::error_code& ec);

    private:
	networking networking_;
	master_chain masterchain_;
	bitcoin::session session_;
    };
}

#endif//__MASTER_NODE_H___DUZY__
