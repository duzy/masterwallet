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
#ifndef __WINDOW_H___DUZY__
#define __WINDOW_H___DUZY__ 1
#include "../all.h"

typedef struct _win_st WINDOW;
typedef struct formnode FORM;
typedef struct fieldnode FIELD;

namespace mastercoin
{
    struct window
    {
	WINDOW *base = NULL;
	FORM *form = NULL;
	std::vector<FIELD*> fields = { NULL };
	std::function<bool (window *, int)> on_char;
	std::function<void (window *, FIELD *)> on_field_commit;
	std::function<void (window *, FIELD *)> on_field_changed;

	int linecol = 0;
	int line = 0;

	window(WINDOW *w) : base(w), form(NULL) {}

	window(window * parent, double wr = 0.9, double hr = 0.8);
	window(window * parent, int h, int w, int y, int x);
	//window(window * parent, int h, int w);
	window(double wr = 0.9, double hr = 0.8);
	window(int h, int w, int y, int x);
	//window(int h, int w);

	virtual ~window();

	int addstr(int r, int c, const std::string & s);

	void indent(int n) { linecol = n; }
	void set_line(int n) { line = n; }
	void set_line(int n, int c) { line = n, linecol = c; }
	void add_line(int n) { line += n; }
	void add_line(int n, int c) { line += n, linecol = c; }

	int lineadd(const std::string & s, int attr = 0) {
	    const int a = attrs();
	    if (attr != 0 ) attron(attr);
	    addstr(line, linecol, s);
	    if (attr != 0 ) attrset(a);
	    return (linecol += s.size());
	}

	int lineadd_mid(const std::string & s, int attr = 0) {
	    const int col = cols();
	    linecol = (col - s.size()) * 0.5;
	    return lineadd(s, attr);
	}

	//int print(int r, int c, const char *fmt, ...) { return mvprintw(r, c, fmt, ...); }

	int border(int hv) { return border(hv, hv); }
	int border(int v, int h) { return border(v, v, h, h, 0, 0, 0, 0); }
	int border(int ls, int rs, int ts, int bs, int tl, int tr, int bl, int br);

	int hline(int r, int c, int ch, int n);
	int vline(int r, int c, int ch, int n);

	int hline(int ch, int n);
	int vline(int ch, int n);

	int hline_mid(int r, int ch, int n) {
	    return hline(r, (cols() - n) * 0.5, ch, n);
	}

	int attrs() const;
	int attron(int a);
	int attroff(int a);
	int attrset(int a);

	int rows() const;
	int cols() const;

	int move(int y, int x);
	
	int refresh();

	virtual bool do_input(int c);

	void edit_field(int c);

	FIELD *add_label(const std::string & s, int r, int c, int offscreen = 0, int nbuffers = 0);
	FIELD *add_field(int h, int w, int r, int c, int offscreen = 0, int nbuffers = 0);

	FIELD *lineadd_label(const std::string & s, int offscreen = 0, int nbuffers = 0) {
	    FIELD *field = add_label(s, line, linecol, offscreen, nbuffers);
	    linecol += s.size();
	    return field;
	}
	FIELD *lineadd_field(int h, int w, int offscreen = 0, int nbuffers = 0) {
	    FIELD *field = add_field(h, w, line, linecol, offscreen, nbuffers);
	    linecol += w;
	    return field;
	}

	int set_focus(FIELD *field);
	
	bool show_form();

    private:
	void commit_field();
    };

    typedef std::shared_ptr<window> window_ptr;
}//namespace mastercoin

#endif//__WINDOW_H___DUZY__
