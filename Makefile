# 3DS Engine — Multi-game Makefile

# Usage:
#   make GAME=<name>       — build a specific game (default: bomberman)
#   make clean GAME=<name> — clean a specific game's build
#   make clean-all         — clean all builds

# Available games are the folders under games/
# Outputs go to build/<game>/

.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
    $(error "Please set DEVKITARM in your environment: export DEVKITARM=<path to>devkitARM")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

GAME     ?= bomberman
TARGET   := $(GAME)
BUILD    := build/$(GAME)

SOURCES  := engine/source \
            engine/source/systems \
            engine/external/cjson \
            games/$(GAME)/source

INCLUDES := engine/include \
            engine/include/systems \
            engine/external/cjson \
            games/$(GAME)/include

ARCH     := -march=armv6k -mtune=mpcore -mfpu=vfp -mfloat-abi=hard -marm

CFLAGS   := -Wall -O2 -ffunction-sections $(ARCH) $(INCLUDE) -D__3DS__
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=c++17
ASFLAGS  := -g $(ARCH)
LDFLAGS  = -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS     := -lcitro2d -lcitro3d -lctru -lm
PORTLIBS := $(DEVKITPRO)/portlibs/3ds $(DEVKITPRO)/portlibs/armv6k
LIBDIRS  := $(CTRULIB) $(PORTLIBS)

ifneq ($(notdir $(BUILD)),$(notdir $(CURDIR)))

export OUTPUT   := $(TOPDIR)/$(BUILD)/$(TARGET)
export ROMFSDIR := $(TOPDIR)/games/$(GAME)/romfs
export DEPSDIR  := $(TOPDIR)/$(BUILD)

export VPATH    := $(foreach dir,$(SOURCES),$(TOPDIR)/$(dir))

CFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(TOPDIR)/$(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(TOPDIR)/$(dir)/*.cpp)))
SFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(TOPDIR)/$(dir)/*.s)))

ifeq ($(strip $(CPPFILES)),)
    export LD := $(CC)
else
    export LD := $(CXX)
endif

export OFILES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE  := $(foreach dir,$(INCLUDES),-I$(TOPDIR)/$(dir)) \
                   $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                   -I$(TOPDIR)/$(BUILD)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: all clean clean-all $(BUILD) info

all: $(BUILD)

$(BUILD):
	@echo "Building $(GAME) -> $(BUILD)/"
	@mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(TOPDIR)/Makefile GAME=$(GAME) $(OUTPUT).3dsx

clean:
	@echo "Cleaning $(GAME)..."
	@rm -fr $(BUILD)

clean-all:
	@echo "Cleaning all builds..."
	@rm -fr build

info:
	@echo "GAME     = $(GAME)"
	@echo "TOPDIR   = $(TOPDIR)"
	@echo "BUILD    = $(BUILD)"
	@echo "OUTPUT   = $(OUTPUT)"
	@echo "ROMFSDIR = $(ROMFSDIR)"
	@echo "SOURCES  = $(SOURCES)"
	@echo ""
	@echo "CPPFILES = $(CPPFILES)"
	@echo "CFILES   = $(CFILES)"

else

DEPENDS := $(OFILES:.o=.d)
$(info INCLUDE = $(INCLUDE))

$(OUTPUT).elf: $(OFILES)
	@echo "Linking $(notdir $@)"
	@$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

$(OUTPUT).smdh:
	@echo "Building SMDH"
	@smdhtool --create "$(GAME)" "Built with custom 3DS engine" "Thomas" $(CTRULIB)/default_icon.png $@

$(OUTPUT).3dsx: $(OUTPUT).elf $(OUTPUT).smdh
	@echo "Building 3dsx"
	@3dsxtool $< $@ --smdh=$(OUTPUT).smdh --romfs=$(ROMFSDIR)
	@echo "Done: $(BUILD)/$(notdir $@)"

-include $(DEPENDS)

endif