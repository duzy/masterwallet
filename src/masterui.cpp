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
#include "masterui.h"
#include "logging.h"

namespace mastercoin
{
    master_ui::master_ui(int argc, char**argv)
	: wallet_()
	, node_()
    {
    }

    void master_ui::run()
    {
	std::ofstream outfile("debug.log"), errfile("error.log");
	logger::setup(outfile, errfile);

	node_.start();
	wallet_.start();

	interact();

	node_.stop();
	wallet_.stop();

	wallet_.join();
	node_.join();
    }

}//namespace mastercoin
