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
#include "window.h"
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
    window::window(int h, int w, int y, int x)
	: base(newwin(h, w, y, x))
	, form(NULL), fields({ NULL })
    {
    }

    window::window(window * parent, int h, int w, int y, int x)
	: base(subwin(parent->base, h, w, y, x))
	, form(NULL), fields({ NULL })
    {
    }

    window::window(window * parent, double wr, double hr)
	: base(NULL), form(NULL), fields({ NULL })
    {
	int row, col;
	getmaxyx(stdscr, row, col);
	
	int w = col * wr, h = row * hr;
	base = subwin(parent->base, h, w, (row-h) * 0.5, (col-w) * 0.5);
    }

    window::window(double wr, double hr)
	: base(NULL), form(NULL), fields({ NULL })
    {
	int row, col;
	getmaxyx(stdscr, row, col);
	
	int w = col * wr, h = row * hr;
	base = newwin(h, w, (row-h) * 0.5, (col-w) * 0.5);
    }

    window::~window()
    {
	if (form) {
	    unpost_form(form);
	    free_form(form);
	}
	    
	for (auto f : fields) free_field(f);

	wborder(base, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	werase(base); //wclear(base);
	wrefresh(base);
	delwin(base);
	endwin();
    }

    int window::attrs() const { return getattrs(base); }
    int window::attron(int a) { return wattron(base, a); }
    int window::attroff(int a) { return wattroff(base, a); }
    int window::attrset(int a) { return wattrset(base, a); }
    int window::cols() const { return getmaxx(base); }
    int window::rows() const { return getmaxy(base); }

    int window::move(int y, int x) { return wmove(base, y, x); }

    int window::hline(int r, int c, int ch, int n) { return mvwhline(base, r, c, ch, n); }
    int window::vline(int r, int c, int ch, int n) { return mvwvline(base, r, c, ch, n); }
    int window::hline(int ch, int n) { return whline(base, ch, n); }
    int window::vline(int ch, int n) { return wvline(base, ch, n); }

    int window::refresh() { return wrefresh(base); }
    
    int window::addstr(int r, int c, const std::string & s) {
	return mvwaddnstr(base, r, c, s.c_str(), s.size());
    }

    int window::border(int ls, int rs, int ts, int bs, int tl, int tr, int bl, int br) {
	return ::wborder(base, ls, rs, ts, bs, tl, tr, bl, br);
    }
    
    FIELD *window::add_label(const std::string & s, int r, int c, int offscreen, int nbuffers) {
	int w = 1, h = 1, n = 0;
	for (auto c : s) {
	    if (c == '\n') {
		if (w < n) w = n;
		h += 1;
	    } else {
		n += 1;
	    }
	}
	if (w < n) w = n;

	FIELD *field = new_field(h, w, r, c, offscreen, nbuffers);
	if (field) {
	    int opts = (int) ((unsigned) field_opts(field) & ~O_ACTIVE);
	    set_field_buffer(field, 0, s.c_str());
	    set_field_opts(field, opts);
	    field_opts_off(field, O_AUTOSKIP);
	}
	fields[fields.size()-1] = field;
	fields.push_back(NULL);
	return field;
    }

    FIELD *window::add_field(int h, int w, int r, int c, int offscreen, int nbuffers) {
	FIELD *field = new_field(h, w, r, c, offscreen, nbuffers);
	set_field_type(field, TYPE_ALNUM, 1);
	if (has_colors()) {
	    //set_field_fore(field, /*A_COLOR|*/COLOR_PAIR(1));
	    set_field_fore(field, A_BOLD);
	    set_field_back(field, A_DIM|A_UNDERLINE);
	} else {
	    set_field_fore(field, A_BOLD);
	}
	fields[fields.size()-1] = field;
	fields.push_back(NULL);
	return field;
    }

    bool window::show_form() {
	const int row = getmaxy(base);
	const int col = getmaxx(base);
	if (fields.empty()) return false;
	form = new_form(&fields[0]);
	set_form_win(form, base);
	set_form_sub(form, derwin(base, row, col, 0, 0));
	keypad(base, TRUE);
	return post_form(form) == E_OK;
    }

    int window::set_focus(FIELD *field) {
	return ::set_current_field(form, field);
    }

    bool window::do_input(int c) {
	if (on_input && on_input(this, c)) return true;
	switch (c) {
	case KEY_UP:		form_driver(form, REQ_UP_FIELD); break;
	case KEY_DOWN:		form_driver(form, REQ_DOWN_FIELD); break;
	case '':		/* FALLTHROUGH */
	case KEY_PREVIOUS:	form_driver(form, REQ_PREV_FIELD); break;
	case '	':		/* FALLTHROUGH */
	case '':		/* FALLTHROUGH */
	case KEY_NEXT:		form_driver(form, REQ_NEXT_FIELD); break;
	case '':		/* FALLTHROUGH */
	case KEY_LEFT:		form_driver(form, REQ_PREV_CHAR); break;
	case '':		/* FALLTHROUGH */
	case KEY_RIGHT:		form_driver(form, REQ_NEXT_CHAR); break;
	case '':		/* FALLTHROUGH */
	case KEY_DL:		form_driver(form, REQ_DEL_LINE); break;
	case '':		/* FALLTHROUGH */
	case KEY_DC:		form_driver(form, REQ_DEL_CHAR); break;
	case KEY_BACKSPACE:	form_driver(form, REQ_DEL_PREV); break;
	case '':		/* FALLTHROUGH */
	case KEY_HOME:		form_driver(form, REQ_BEG_FIELD); break;
	case '':		/* FALLTHROUGH */
	case KEY_END:		form_driver(form, REQ_END_FIELD); break;
	case KEY_EXIT: break;
	    //case '':		// FALLTHROUGH
	case '\n':		// FALLTHROUGH
	case KEY_ENTER:		commit_field(); break;
	default: form_driver(form, c); break;
	}
	return false;
    }

    void window::commit_field()
    {
	if (on_field_commit) {
	    FIELD *field = current_field(form);
	    if (field) on_field_commit(this, field);
	}
    }
}//namespace mastercoin
