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
#include "../masterui.h"

namespace mastercoin
{
    class gui : public master_ui
    {
    public:
	gui(int argc, char**argv);

    protected:
	virtual void interact();
    };

    gui::gui(int argc, char**argv) : master_ui(argc, argv)
    {
    }

    void gui::interact()
    {
	// TODO: implentation...
    }
}

int main(int argc, char**argv)
{
    mastercoin::gui gui(argc, argv);
    gui.run();
    return 0;
}
