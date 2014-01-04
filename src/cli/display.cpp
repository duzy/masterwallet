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
#include "../masterwallet.h"
#include "display.h"
#include <boost/algorithm/string.hpp>
#include <ncurses.h>
#include <form.h>

#undef addstr
#undef border
#undef refresh
#undef hline
#undef vline
#undef attron
#undef attroff
#undef attrset

namespace mastercoin
{
    constexpr static const char * donation_address = "13ePmoDPuA3GCowrxj9BjmbnWGVKEHH8Np";
    constexpr static const char * help_screen_text[] = {
	"Commands (shortcuts):",
	"",
	"	s	:	Send money to an address.",
	"	x	:	Exchange assets.",
	"	c	:	Execute a command.",
	"",
	"	PgUp	:	Select previous address.",
	"	PgDn	:	Select next address.",
	"",
	"	d	:	Donate some funds to the author",
	"		:	to help making a better wallet.",
	"",
	"	h	:	Show this help screen.",
	"	q	:	Quit the application.",
	"	Esc	:	Close the top window.",
	NULL
    };

    display::display(master_wallet & wallet)
	: wallet_(wallet)
	, windows_()
	, help_window_(NULL)
	, exchange_window_(NULL)
	, command_window_(NULL)
    {
    }

    display::~display()
    {
    }

    void display::run()
    {
	initscr();
	start_color();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	interact();
    }

    void display::interact()
    {
	const int row = getmaxy(stdscr);
	const int col = getmaxx(stdscr);

	wrefresh(stdscr);

	show_main_screen(row, col);

	while (!windows_.empty()) {
	    update();

	    window_ptr const w(windows_.back());
	    int const c = wgetch(w->base);

	    if (w->do_input(c)) continue;

	    switch (c) {
	    case '': /* FALLTHROUGH */
	    case 'q': windows_.pop_back(); break;
	    case 'h': toggle_help_screen(); break;
	    case 'x': toggle_exchange_screen(); break;
	    case 's': toggle_send_screen(); break;
	    case 'c': show_command_window(row, col); break;
	    case 'd': donate(); break;
	    }
	}
    }

    void display::update()
    {
	for (const auto & w : windows_) {
	    wtouchln(w->base, 0, getmaxy(w->base), 1);
	    wnoutrefresh(w->base);
	}
	doupdate();
    }

    void display::show_main_screen(int row, int col)
    {
	window_ptr const win(new window(1.0, 1.0));
	windows_.push_back(win);

	FIELD *field, *fieldBalance, *fieldAddress;
	int indent = 6;
	if (col < 50) indent = (col-34)/2;
	if (indent < 0) indent = 1;

	win->set_line(4, indent);
	field = win->lineadd_label("Balance: ");
	field = win->lineadd_field(1, 34);
	fieldBalance = field;

	win->add_line(2, indent);
	field = win->lineadd_label("Address: ");
	field = win->lineadd_field(1, 34);
	fieldAddress = field;

	win->add_line(2, indent);
	field = win->lineadd_label("Details: ");

	win->add_line(2, indent);
	field = win->lineadd_label("Currencies: ");
	
	mastercoin::money btc_balance = wallet_.get_balance(mastercoin::currency::BTC);
	mastercoin::money msc_balance = wallet_.get_balance(mastercoin::currency::MSC);
	mastercoin::money tmsc_balance = wallet_.get_balance(mastercoin::currency::TMSC);
	std::stringstream ss;

	ss //<< bitcoin::satoshi_to_btc(btc_balance.get_amount()) << " BTC, "
	   << bitcoin::satoshi_to_btc(msc_balance.get_amount()) << " MSC" ;
	set_field_buffer(fieldBalance, 0, ss.str().c_str());
	set_field_back(fieldBalance, A_BOLD);
	field_opts_off(fieldBalance, O_EDIT);

	ss.str(std::string()); // ss.seekp(0);
	ss << donation_address ;
	set_field_buffer(fieldAddress, 0, ss.str().c_str());
	set_field_back(fieldAddress, A_BOLD);
	field_opts_off(fieldAddress, O_EDIT);

	win->show_form();
	win->set_line(0, 0);
	win->lineadd("MasterWallet", A_BOLD);
	win->lineadd(" -- a mastercoin and bitcoin wallet ", A_NORMAL);
	win->lineadd("(type 'h' for help, 'x' to exchange)", A_DIM);
	win->hline(1, 0, ACS_HLINE, col);
	win->hline(row-2, 0, ACS_HLINE, col);
	win->set_focus(fieldBalance);

	win->on_char = [this, row, col](window *win, int c) -> bool {
	    switch (c) {
	    case 'q': windows_.pop_back(); break;
	    case 'h': toggle_help_screen(); break;
	    case 'x': toggle_exchange_screen(); break;
	    case 's': toggle_send_screen(); break;
	    case 'c': show_command_window(row, col); break;
	    case 'd': donate(); break;
	    default: win->edit_field(c); break;
	    }
	    return true;
	};
    }

    void display::toggle_help_screen()
    {
	if (help_window_) {
	    windows_.remove(help_window_);
	    help_window_.reset();
	    return;
	}

	window_ptr const top = windows_.back();
	window_ptr const win(new window(/* top */));

	win->set_line(2, 0);
	win->lineadd_mid("Quick Help", A_BOLD);
	win->hline_mid(win->line+1, ACS_HLINE, 16), win->add_line(2);

	const char *s = NULL;
	for (int n = 0; s = help_screen_text[n]; ++n) {
	    win->add_line(1), win->indent(2);
	    win->lineadd(s);
	}

	win->border(0);
	windows_.push_back(help_window_ = win);
    }

    void display::toggle_exchange_screen()
    {
	if (exchange_window_) {
	    windows_.remove(exchange_window_);
	    exchange_window_.reset();
	    return;
	}

	window_ptr const top = windows_.back();
	window_ptr const win(new window(/* top, */ 1.0, 1.0));
	windows_.push_back(exchange_window_ = win);

	const int row = getmaxy(win->base);
	const int col = getmaxx(win->base);

	FIELD *field, *fieldBuy, *fieldSell;

	win->set_line(row - 5);

	field = win->add_label("Buy", row - 5, (col - 3) * 0.25);
	fieldBuy = field;

	field = win->add_label("Sell", row - 5, (col - 4) * 0.75);
	fieldSell = field;

	set_field_fore(fieldBuy, A_BOLD);
	set_field_back(fieldBuy, A_DIM);
	field_opts_on(fieldBuy, O_ACTIVE);
	field_opts_off(fieldBuy, O_EDIT);

	set_field_fore(fieldSell, A_BOLD);
	set_field_back(fieldSell, A_DIM);
	field_opts_on(fieldSell, O_ACTIVE);
	field_opts_off(fieldSell, O_EDIT);

	win->show_form();

	win->set_line(0, 0);
	win->lineadd("MasterWallet Exchange", A_BOLD);
	win->lineadd(" -- distributed exchange ", A_NORMAL);
	win->lineadd("(type 'b' to buy, 's' to sell, 'q' to end)", A_DIM);
	win->hline(1, 0, ACS_HLINE, col);

	win->add_line(2, 0);
	win->lineadd_mid("MSC/BTC", A_BOLD);
	win->hline_mid(win->line+1, ACS_HLINE, 20), win->add_line(3);

	//win->indent(2);
	//win->lineadd("Order Book", A_BOLD);
	//win->hline(win->line+1, 2, ACS_HLINE, 16);

	win->add_line(1, (col * 0.5 - 4) * 0.5);
	win->lineadd("Bids", A_BOLD);
	win->hline(win->line+1, (col * 0.5 - 16) * 0.5, ACS_HLINE, 16);
	win->indent((col * 0.5 - 4) * 1.55);
	win->lineadd("Asks", A_BOLD);
	win->hline(win->line+1, (col * 0.5 - 16) * 1.75, ACS_HLINE, 16);
	win->vline(win->line+3/*(row-10) * 0.5*/, col * 0.5, ACS_VLINE, 10);

	win->on_char = [this, row, col](window *win, int c) -> bool {
	    switch (c) {
	    case 'q': windows_.pop_back(); break;
	    default: win->edit_field(c); break;
	    }
	    return true;
	};
    }

    void display::toggle_send_screen()
    {
	if (send_window_) {
	    windows_.remove(send_window_);
	    send_window_.reset();
	    //update();
	    return;
	}

	window_ptr const top = windows_.back();
	window_ptr const win(new window(0.5, 0.5));
	windows_.push_back(send_window_ = win);

	const int row = getmaxy(win->base);
	const int col = getmaxx(win->base);

	FIELD *field, *fieldCurrency, *fieldBalance, *fieldPayto, *fieldAmount, *fieldSend;

	win->set_line(6, 3);
	field = win->lineadd_label("Currency: ");
	field = win->lineadd_field(1, 34);
	fieldCurrency = field;

	win->add_line(2, 3);
	field = win->lineadd_label("Balance: ");
	field = win->lineadd_field(1, 34);
	fieldBalance = field;

	win->add_line(2, 3);
	field = win->lineadd_label("Pay to : ");
	field = win->lineadd_field(1, 34);
	fieldPayto = field;

	win->add_line(2, 3);
	field = win->lineadd_label("Amount : ");
	field = win->lineadd_field(1, 34);
	fieldAmount = field;

	win->set_line(row - 3);
	field = win->add_label("Send", row - 3, col - 8);
	fieldSend = field;

	mastercoin::money btc_balance = wallet_.get_balance(mastercoin::currency::BTC);
	mastercoin::money msc_balance = wallet_.get_balance(mastercoin::currency::MSC);
	mastercoin::money tmsc_balance = wallet_.get_balance(mastercoin::currency::TMSC);
	std::stringstream ss;

	set_field_buffer(fieldCurrency, 0, "mastercoin");
	set_field_back(fieldCurrency, A_BOLD);

	ss //<< bitcoin::satoshi_to_btc(btc_balance.get_amount()) << " BTC, "
	   << bitcoin::satoshi_to_btc(msc_balance.get_amount()) << " MSC" ;
	set_field_buffer(fieldBalance, 0, ss.str().c_str());
	set_field_back(fieldBalance, A_BOLD); // A_NORMAL
	field_opts_off(fieldBalance, O_EDIT);

	set_field_type(fieldAmount, TYPE_NUMERIC);

	set_field_fore(fieldSend, /*A_COLOR|COLOR_PAIR(2)|*/A_BOLD);
	set_field_back(fieldSend, /*A_COLOR|COLOR_PAIR(1)|*/A_DIM);
	field_opts_on(fieldSend, O_ACTIVE);
	field_opts_off(fieldSend, O_EDIT);

	win->show_form();

	win->set_line(2, 0);
	win->lineadd_mid("Quick Send", A_BOLD);
	win->hline_mid(win->line+1, ACS_HLINE, 16);
	win->set_focus(fieldPayto);

	win->border(0);
	win->on_char = [this](window *win, int c) -> bool {
	    switch (c) {
	    case 'q': windows_.pop_back(); break;
	    default: win->edit_field(c); break;
	    }
	    return true;
	};
    }

    void display::show_command_window(int mrow, int mcol)
    {
	window_ptr const top = windows_.back();
	window_ptr const win(new window(/*top.get(),*/ 0.5, 0.5));
	windows_.push_back(command_window_ = win);

	const int row = getmaxy(win->base);
	const int col = getmaxx(win->base);

	FIELD *field, *fieldCommand;

	win->set_line(2, 2);
	field = win->lineadd_label("Command: ");
	field = win->lineadd_field(1, col - win->linecol - 2);
	fieldCommand = field;

	set_field_back(fieldCommand, A_BOLD);
	field_opts_off(fieldCommand, O_AUTOSKIP);
	field_opts_on(fieldCommand, O_EDIT);

	win->show_form();

	win->hline(4, 2, '-', col-4);
	win->addstr(5, 0,
	    "  Quick Refeence:\n"
	    "    help    : Show help screen.\n"
	    "\n"
	    "    send    : Send money to an address.\n"
	    "    trade   : Exchange assets.\n"
	    "\n"
	    "    donate  : Donate some funds to the developer,\n"
	    "            : it's critical to help making a better wallet.\n"
	    "\n"
	    "    quit    : Quit the application.\n"
	    );

	win->hline(row-3, 2, '-', col-4);
	win->border(0);
	win->set_focus(fieldCommand);

	win->on_char = [this, row, col, fieldCommand](window *win, int c) -> bool {
	    if (c == '') return false;
	    win->hline(row-2, 2, ' ', col-4);
	    win->edit_field(c);
	    return true;
	};
	win->on_field_commit = [this, row, col, fieldCommand](window *win, FIELD *field) {
	    if (const char * const p = field_buffer(fieldCommand, 0)) {
		std::string cmd(p);
		boost::trim(cmd);
		if (cmd == "help") {
		    toggle_help_screen();
		} else if (cmd == "send") {
		    toggle_send_screen();
		} else if (cmd == "trade") {
		    toggle_exchange_screen();
		} else if (cmd == "donate") {
		    donate();		    
		} else if (cmd == "quit") {
		    while (!windows_.empty()) windows_.pop_back();
		} else {
		    win->addstr(row-2, 2, "Unknown command, try 'help'!");
		    win->set_focus(fieldCommand);
		}
	    }
	};
    }

    void display::donate()
    {
	if (donate_window_) return;

	window_ptr const top = windows_.back();
	window_ptr const win(new window(0.5, 0.5));
	windows_.push_back(donate_window_ = win);

	const int row = getmaxy(win->base);
	const int col = getmaxx(win->base);

	FIELD *field, *fieldCurrency, *fieldBalance, *fieldPayto, *fieldAmount, *fieldDonate;

	win->set_line(6, 3);
	field = win->lineadd_label("Currency: ");
	field = win->lineadd_field(1, 34);
	fieldCurrency = field;

	win->add_line(2, 3);
	field = win->lineadd_label("Balance: ");
	field = win->lineadd_field(1, 34);
	fieldBalance = field;

	win->add_line(2, 3);
	field = win->lineadd_label("Pay to : ");
	field = win->lineadd_field(1, 34);
	fieldPayto = field;

	win->add_line(2, 3);
	field = win->lineadd_label("Amount : ");
	field = win->lineadd_field(1, 34);
	fieldAmount = field;

	win->set_line(row - 3);
	field = win->add_label("Send", row - 3, col - 8);
	fieldDonate = field;

	mastercoin::money btc_balance = wallet_.get_balance(mastercoin::currency::BTC);
	mastercoin::money msc_balance = wallet_.get_balance(mastercoin::currency::MSC);
	mastercoin::money tmsc_balance = wallet_.get_balance(mastercoin::currency::TMSC);
	std::stringstream ss;

	set_field_buffer(fieldCurrency, 0, "mastercoin");
	set_field_back(fieldCurrency, A_BOLD);

	set_field_buffer(fieldPayto, 0, donation_address);
	set_field_back(fieldPayto, A_BOLD); // A_NORMAL
	field_opts_off(fieldPayto, O_EDIT);

	set_field_type(fieldAmount, TYPE_NUMERIC);

	ss.str(std::string());
	ss //<< bitcoin::satoshi_to_btc(btc_balance.get_amount()) << " BTC, "
	   << bitcoin::satoshi_to_btc(msc_balance.get_amount()) << " MSC" ;
	set_field_buffer(fieldBalance, 0, ss.str().c_str());
	set_field_back(fieldBalance, A_BOLD); // A_NORMAL
	field_opts_off(fieldBalance, O_EDIT);

	set_field_fore(fieldDonate, /*A_COLOR|COLOR_PAIR(2)|*/A_BOLD);
	set_field_back(fieldDonate, /*A_COLOR|COLOR_PAIR(1)|*/A_DIM);
	field_opts_on(fieldDonate, O_ACTIVE);
	field_opts_off(fieldDonate, O_EDIT);

	win->show_form();

	win->set_line(2, 0);
	win->lineadd_mid("Donate Funds", A_BOLD);
	win->hline_mid(win->line+1, ACS_HLINE, 16);
	win->set_focus(fieldAmount);

	win->border(0);
	win->on_char = [this](window *win, int c) -> bool {
	    switch (c) {
	    case 'q': windows_.pop_back(); break;
	    default: win->edit_field(c); break;
	    }
	    return true;
	};
    }
}//namespace mastercoin
