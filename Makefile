#
# Top-level makefile to build the library and the plugins. Recursive
# Make is a bad idea, but #YOLO.
#

ifndef PLUGINS
$(warning No plugins selected, building only library)
endif

# ######################################################################
# Some housekeeping to determine if we are running on a POSIX
# platform or on Windows

MAKEPROGRAM_EXE=$(findstring exe,$(MAKE))
MAKEPROGRAM_MINGW=$(findstring mingw,$(MAKE))
GITSHELL=$(findstring Git,$(SHELL))
GITSHELL+=$(findstring git,$(SHELL))

# TODO: Remember that freebsd might use not gmake/gnu-make; must add in
# some diagnostics so that user gets a message to install gnu make.

ifneq ($(MAKEPROGRAM_EXE),)
ifeq ($(strip $(GITSHELL)),)
$(error On windows this must be executed from the Git bash shell)
endif
	HOME=$(subst \,/,$(HOMEDRIVE)$(HOMEPATH))
	PLATFORM:=Windows
	EXE_EXT:=.exe
	LIB_EXT:=.dll
	PLATFORM_LDFLAGS:=--L$(HOME)/lib lmingw32 -lws2_32 -lmsvcrt -lgcc
	PLATFORM_CFLAGS:= -D__USE_MINGW_ANSI_STDIO
	ECHO:=echo -e
endif

ifneq ($(MAKEPROGRAM_MINGW),)
ifeq ($(strip $(GITSHELL)),)
$(error On windows this must be executed from the Git bash shell)
endif
	HOME=$(subst \,/,$(HOMEDRIVE)$(HOMEPATH))
	PLATFORM:=Windows
	EXE_EXT:=.exe
	LIB_EXT:=.dll
	PLATFORM_LDFLAGS:=-L$(HOME)/lib -lmingw32 -lws2_32 -lmsvcrt -lgcc
	PLATFORM_CFLAGS:= -D__USE_MINGW_ANSI_STDIO
	ECHO:=echo -e
endif

# If neither of the above are true then we assume a working POSIX
# platform
ifeq ($(PLATFORM),)
	PLATFORM:=POSIX
	EXE_EXT:=.elf
	LIB_EXT:=.so
	PLATFORM_LDFLAGS:= -lpthread -ldl
	ECHO:=echo
	REAL_SHOW:=real-show
endif

ACTUAL_TARGETS=libezsql $(PLUGINS)

.PHONY: plugins

all:	help

help:
	@$(ECHO) ""
	@$(ECHO) "Usage: PLUGINS=<list of plugins> make <target>"
	@$(ECHO) ""
	@$(ECHO) "   The library is always built. Only the plugins that are"
	@$(ECHO) "specified are built.  The plugin name ' all' is shorthand"
	@$(ECHO) "for specifying all plugins in this repository."
	@$(ECHO) ""
	@$(ECHO) "Possible targets:"
	@$(ECHO) "help:                This message."
	@$(ECHO) "show:                Display all the variable values that will be"
	@$(ECHO) "                     used during execution. Also 'show debug' or"
	@$(ECHO) "                     'show release' works."
	@$(ECHO) "plugins:             Display list of plugins available."
	@$(ECHO) "deps:                Make the dependencies only."
	@$(ECHO) "debug:               Build debug binaries."
	@$(ECHO) "release:             Build release binaries."
	@$(ECHO) "clean-debug:         Clean a debug build (release is ignored)."
	@$(ECHO) "clean-release:       Clean a release build (debug is ignored)."
	@$(ECHO) "clean-all:           Clean everything."

plugins:
	$(ECHO) "Available plugins"
	@ls -1 plugins

show:
	@$(ECHO) "AVAILABLE PLUGINS:   "`ls plugins`
	@$(ECHO) "SELECTED PLUGINS:    $(PLUGINS)"
	@$(ECHO) "ACTUAL TARGETS:      $(ACTUAL_TARGETS)"
	@$(ECHO) ""
	@$(ECHO) "Use target 'help' to display all the options"

deps:          TARGET+=deps
deps:          $(ACTUAL_TARGETS)
debug:         TARGET+=debug
debug:         $(ACTUAL_TARGETS)
release:       TARGET+=release
release:       $(ACTUAL_TARGETS)
clean-debug:   TARGET+=clean-debug
clean-debug:   $(ACTUAL_TARGETS)
clean-release: TARGET+=clean-release
clean-release: $(ACTUAL_TARGETS)
clean-all:     TARGET+=clean-all
clean-all:     $(ACTUAL_TARGETS)

$(PLUGINS):	libezsql
	$(MAKE) $(MAKEFLAGS) -C plugins/$@ $(TARGET)

libezsql:
	$(MAKE) $(MAKEFLAGS) -C src $(TARGET)

