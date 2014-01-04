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
#ifndef __MASTER_UI_H___DUZY__
#define __MASTER_UI_H___DUZY__ 1
#include "masterwallet.h"
#include "masternode.h"

namespace mastercoin
{

    class master_ui
    {
    public:
	void run();

    protected:
	master_ui(int argc, char**argv);
	virtual void interact() = 0;

	master_wallet & wallet() { return wallet_; }
	master_node & node() { return node_; }

    private:
	master_wallet wallet_;
	master_node node_;
    };

}//namespace mastercoin

#endif//__MASTER_UI_H___DUZY__


