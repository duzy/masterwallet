#
#  Copyright (c) 2013 MasterWallet developers (see AUTHORS)
# 
#  This file is part of MasterWallet <https://github.com/duzy/masterwallet>.
# 
#  libwallet is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Affero General Public License as
#  published by the Free Software Foundation, either version 3 of the
#  License, or (at your option) any later version.
# 
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Affero General Public License for more details.
# 
#  You should have received a copy of the GNU Affero General Public License
#  along with this program. If not, see <http://www.gnu.org/licenses/>.
#
SPHINXINSTALL := build/.install_sphinx
SPHINXBUILD := $(shell which sphinx-build)
ifdef SPHINXBUILD
  $(SPHINXINSTALL) : ; @touch $@
else
  $(SPHINXINSTALL) :
	@echo "Installing sphinx (sudo password required)..."
	@sudo apt-get install sphinx3
endif

SPHINXOPTS :=

.DEFAULT_GOAL :=
