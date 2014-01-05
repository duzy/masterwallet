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

SOURCES := \
  logging.cpp \
  addressbook.cpp \
  addressource.cpp \
  determaddress.cpp \
  networking.cpp \
  masterchain.cpp \
  masternode.cpp \
  masterwallet.cpp \
  masterui.cpp \

SOURCES.CLI := \
  cli/main.cpp \
  cli/window.cpp \
  cli/display.cpp \

SOURCES.GUI := \
  gui/main.cpp \

DEPENDS := $(patsubst %.cpp,%.d,$(SOURCES) $(SOURCES.CLI) $(SOURCES.GUI))

CXXFLAGS := $(shell pkg-config --cflags libbitcoin libwallet) -g -ggdb
LDLIBS := $(shell pkg-config --libs libbitcoin libwallet)

OBJECTS := $(SOURCES:%.cpp=%.o)
OBJECTS.CLI := $(SOURCES.CLI:%.cpp=%.o)
OBJECTS.GUI := $(SOURCES.GUI:%.cpp=%.o)

TARGETS := masterwallet masterwallet-ui

BUILD_TARGET = $(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

all: $(TARGETS)

all.h.gch: all.h ; $(COMPILE.cpp) $(OUTPUT_OPTION) -g $<

$(DEPENDS): %.d: %.cpp ; $(COMPILE.cpp) -MM -MF $@ -MQ $(@:%.d=%.o) $<

masterwallet: $(OBJECTS) $(OBJECTS.CLI)
	$(call BUILD_TARGET) -lform -lncurses

masterwallet-ui: $(OBJECTS) $(OBJECTS.GUI)
	$(call BUILD_TARGET)

include $(DEPENDS)
