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
#include "../masternode.h"
#include "../masterwallet.h"
#include "../txbroadcast.h"
#include "../logging.h"
#include "display.h"

int main(int argc, char**argv)
{
    bool interactive = true;

    std::ofstream outfile("debug.log"), errfile("error.log");
    mastercoin::logger::setup(outfile, errfile);

    bitcoin::threadpool pool(mastercoin::thread_pool_size);
    //mastercoin::master_node node(pool);
    mastercoin::master_wallet wallet(pool);

    //node.start();
    wallet.start();

    if (interactive) {
	mastercoin::display display(wallet);
	display.run();
    }

    wallet.stop();
    wallet.join();

    pool.stop();
    pool.join();
    return 0;
}
