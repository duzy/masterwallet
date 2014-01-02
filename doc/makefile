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
include scripts/build/sphinx.mk

BUILDDIR := build/doc

ALLSPHINXOPTS = -b $@ \
  -d $(BUILDDIR)/doctrees $(PAPEROPT_$(PAPER)) $(SPHINXOPTS) doc \
  $(BUILDDIR)/$@

.PHONY: help clean html

help:
	@echo "Please use \`make <type>' where <type> is one of"
	@echo "  html       to make standalone HTML files"
	@echo "  man        to make manual pages"
	@echo "  texinfo    to make Texinfo files"
	@echo "  info       to make Texinfo files and run them through makeinfo"

html:	$(SPHINXINSTALL) ; $(SPHINXBUILD) $(ALLSPHINXOPTS)
man:	$(SPHINXINSTALL) ; $(SPHINXBUILD) $(ALLSPHINXOPTS)
texinfo:$(SPHINXINSTALL) ; $(SPHINXBUILD) $(ALLSPHINXOPTS)

info: texinfo
	@test -d $(BUILDDIR)/texinfo && make -C $(BUILDDIR)/texinfo info

