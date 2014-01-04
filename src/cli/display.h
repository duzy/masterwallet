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
#ifndef __DISPLAY_H___DUZY__
#define __DISPLAY_H___DUZY__ 1
#include <list>
#include "window.h"

namespace mastercoin
{
    class master_wallet;

    class display
    {
    public:
	display(master_wallet & wallet);
	virtual ~display();

	void run();

    private:
	void interact();
	void update();
	void show_main_screen(int row, int col);
	void toggle_help_screen();
	void toggle_exchange_screen();
	void toggle_send_screen();
	void show_command_window(int row, int col);
	void donate();

    private:
	master_wallet & wallet_;
	std::list<window_ptr> windows_;
	window_ptr help_window_;
	window_ptr exchange_window_;
	window_ptr send_window_;
	window_ptr command_window_;
	window_ptr donate_window_;
    };
}//namespace mastercoin

#endif//__DISPLAY_H___DUZY__
