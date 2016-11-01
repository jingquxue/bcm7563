#
# Reference Software Makefile
#
# This Makefile will build 3 things
# 1) driver(s)
# 2) nexus api (shared or static)
# 3) libs
# 4) kwt
#

#####
###First thing we must include the tools
#####

#########
#### Figure out OS
#########
ifeq ($(B_REFSW_OS),)
B_REFSW_OS=linuxuser
endif

include ../../../../nexus/build/os/$(B_REFSW_OS)/os_tools.inc



#/home/mdk/work/bcm7563/refsw/BSEAV
BSEAV = $(shell cd "../../.." && ${PWD})
#/home/mdk/work/bcm7563/refsw/nexus
NEXUS_TOP = $(shell cd "../../../../nexus" && ${PWD})


ifeq ($(KWT_BIN_DIR),)
# location of final binaries
# /home/mdk/work/bcm7563/refsw/obj.97563
KWT_BIN_ROOT = $(NEXUS_TOP)/../obj.${NEXUS_PLATFORM}
#/home/mdk/work/bcm7563/refsw/obj.97563/BSEAV/bin
KWT_BIN_DIR = $(KWT_BIN_ROOT)/BSEAV/bin
endif

ifeq ($(NEXUS_MODE),)
# MODE is deprecated
NEXUS_MODE = $(MODE)
endif


#########
#### Figure out ARCH
#########
ifeq ($(B_REFSW_ARCH),)
B_REFSW_ARCH=mips-linux
export B_REFSW_ARCH
endif
ifneq ($(B_REFSW_DEBUG),n)
DEBUG_SUFFIX=debug
else
DEBUG_SUFFIX=release
endif

#########
#### Include OS LIB and NEXUS .inc
#########
include $(NEXUS_TOP)/lib/os/b_os_lib.inc
include $(NEXUS_TOP)/platforms/$(NEXUS_PLATFORM)/build/platform_app.inc
# Get system-specific CFLAGS and LDFLAGS based on SYSTEM, PLATFORM, DEBUG
include $(BSEAV)/build/refsw_inc.mak

#################
############ Staging Area setup directory. It will be destroyed later
#################
STAGING_DIR = ${KWT_BIN_DIR}/setup

##############
##### KWT_ODIR keeps binary files separate
##### BCHP_REV is not included, so there should be no #ifdef BCHP_REV in Atlas
##############
KWT_ODIR = $(NEXUS_PLATFORM).$(B_REFSW_ARCH).$(DEBUG_SUFFIX)
APP = $(KWT_ODIR)/kwt
DRIVERDIR=$(BSEAV)/${SYSTEM}/driver/build


PRERENDERED_FONTS  = ../fonts/*.bwin_font
PRERENDERED_IMAGES = ../images/*.png



.PHONY: bin_install api all $(KWT_ODIR)

# The name of the installation tar ball , Change to kwt
#
BUILDDATE = `date +%Y%m%d`
KWT_TAR_BALL = refsw-$(BUILDDATE).$(NEXUS_PLATFORM)-$(SYSTEM)
KWT_TAR_BALL := $(KWT_TAR_BALL)-kwt
KWT_TAR_BALL := $(KWT_TAR_BALL).bin.tgz

#
# Set B_REFSW_SHAREABLE based on a variety of flags
#
ifeq ($(filter y,$(STATIC_KWT)),y)
B_REFSW_SHAREABLE=n
endif

# bin_install is the default rule.
# 'all' builds everything but doesn't perform bin_install or install.
# bin_install copies only binaries into BSEAV/bin. It allows you to
#  run from that directory without having your configuration files overwritten.
# install builds an installation tarball in BSEAV/bin.
bin_install:  $(KWT_BIN_DIR) api all
	@echo [Install... binaries]
# Copy over the nexus script into the correct Directory
	$(Q_)$(CP) $(APP) $(KWT_BIN_DIR)
	$(Q_)$(CP) $(NEXUS_BIN_DIR)/logger $(KWT_BIN_DIR)
	$(Q_)$(CP) $(NEXUS_BIN_DIR)/nexus $(KWT_BIN_DIR)
	$(Q_)$(CP) start $(KWT_BIN_DIR)
	@chmod +x $(KWT_BIN_DIR)/nexus
	@chmod +x $(KWT_BIN_DIR)/start
	@echo [Install... kwt Complete]




$(KWT_ODIR):
	$(Q_)$(MKDIR) $(KWT_ODIR)

##########
##### NEXUS API Libraries
##########

##########
#####always build main.o because it contains __DATE__
##########
build_date:
	$(Q_)$(RM) -f $(KWT_ODIR)/main.o

clean-kwt:
	-@$(RM) -rf $(KWT_ODIR)

lib:
	${Q_}$(MAKE) -C $(BSEAV)/lib/bwin/build
	${Q_}$(MAKE) -C $(BSEAV)/lib/mlibs
	${Q_}$(MAKE) -C $(BWIDGETS_DIR)/build

lib-clean:
	${Q_}$(MAKE) -C $(BSEAV)/lib/bwin/build clean
	${Q_}$(MAKE) -C $(BSEAV)/lib/mlibs clean
	${Q_}$(MAKE) -C $(BWIDGETS_DIR)/build clean

print-kwt-tarball:
	@echo $(KWT_TAR_BALL)

$(KWT_BIN_DIR):
	$(Q_)${MKDIR} ${KWT_BIN_DIR}

##########
##### Rule for all and clean
##########
all:  $(KWT_BIN_DIR) api lib  $(APP)
	${Q_}$(CP) -f $(APP) $(KWT_BIN_DIR)

clean: api-clean  lib-clean clean-kwt

api:  $(KWT_BIN_DIR) driver $(KWT_ODIR)
	@echo [Build ... Nexus]
	${Q_}$(MAKE) -C $(NEXUS_TOP)/lib/os INSTALL_DIR=$(KWT_BIN_DIR)
	${Q_}$(MAKE) -C $(NEXUS_TOP)/build  INSTALL_DIR=$(KWT_BIN_DIR)
ifeq  ($(NEXUS_MODE),proxy)
	@echo [Install ... KernelMode Driver]
	${Q_}$(CP)   $(NEXUS_BIN_DIR)/nexus.ko $(KWT_BIN_DIR)
endif

api-install: driver-install
	@echo [Build ... Nexus]
	${Q_}$(MAKE) -C $(NEXUS_TOP)/lib/os NEXUS_BIN_DIR=$(STAGING_DIR)
	${Q_}$(MAKE) -C $(NEXUS_TOP)/build  NEXUS_BIN_DIR=$(STAGING_DIR)
ifeq  ($(NEXUS_MODE),proxy)
	@echo [Install ... KernelMode Driver]
	${Q_}$(CP)   $(NEXUS_BIN_DIR)/nexus.ko $(STAGING_DIR)
endif


api-clean: driver-clean
	${Q_}$(MAKE) -C $(NEXUS_TOP)/lib/os clean
	${Q_}$(MAKE) -C $(NEXUS_TOP)/build clean
ifeq  ($(NEXUS_MODE),proxy)
	${Q_}$(RM) -f  $(KWT_BIN_DIR)/nexus.ko
endif

driver:
	@echo [Build ... Driver]
ifneq  ($(NEXUS_MODE),proxy)
	${Q_}$(MAKE) INSTALL_DIR=$(KWT_BIN_DIR) -C $(DRIVERDIR) install
endif

driver-install:
	@echo [Build ... Driver]
ifneq  ($(NEXUS_MODE),proxy)
	${Q_}$(MAKE) INSTALL_DIR=$(STAGING_DIR) -C $(DRIVERDIR) install
endif

driver-clean:
	${Q_}$(MAKE) -C $(DRIVERDIR) clean
	${Q_}$(RM) -f $(KWT_BIN_DIR)/bcmdriver.ko

################
######## Source Files for compiling
###############



##################
######## Atlas objs
#################

APP_OBJECTS += \
    main.o \
    display.o \
    graphics.o \
    output.o \
    screen.o \
    remote.o \
    button.o \
    label.o \
    menu_main.o \
    menu_channelsearch.o \
    dib3000.o \
    stc.o \
    video_decode.o \
    video_window.o \
    nandflash.o \
    channellist.o \
    menu_info.o \
    menu_null.o \
    hotplug.o \
    menu_record.o \
    record.o \
    menu_fileBrowser.o \
    audio_decode.o \
    channel.o \
    playback.o \
    global.o \
    menu_circlelabel.o \
    menu_displaysettings.o \
    menu_channeledit.o \
    menu_videochannellist.o \
    menu_generalsettings.o \
    menu_systeminfo.o \
    menu_datetimeset.o \
    menu_tcpipconfiguration.o \
    menu_networkconfig.o \
    menu_descramblingconfig.o \
    menu_duplexaudio.o \
    transmitter.o \
    gpio.o \
    uart.o \
    menu_playback.o \
    menu_volume.o \
    rtc.o \
    menu_recordSettings.o \
    configtool.o \


#####
### Atlas Dirs
#####
KWT_DIRS += ../src
KWT_DIRS += ../src/views



KWT_OBJS := $(addprefix $(KWT_ODIR)/,$(APP_OBJECTS))

# For linux builds, link to the correct libraries
ifeq ($(B_REFSW_OS),linuxkernel)
LDFLAGS += ${NEXUS_LDFLAGS}
else
ifneq ($(findstring linux,$(B_REFSW_OS)),)
LDFLAGS += -lnexus -L${NEXUS_BIN_DIR} -lpthread -lm  $(EXTRA_DRM_OBJS)
endif
endif


#################
##### PSI SCAN
#################
ifneq ($(PSISCAN_SUPPORT),n)
	CFLAGS += -DPSISCAN_SUPPORT
	TRANSPORTDECODE_DIR=$(BSEAV)/lib/mpeg2_ts_parse
	TSPSI_DIR=$(BSEAV)/lib/tspsi2
	KWT_DIRS += \
		$(TRANSPORTDECODE_DIR) \
		$(TSPSI_DIR)
	# all other tspsi and mpeg2_ts_parse are compiled in with nexus
	APP_OBJECTS += tspsimgr2.o
endif

KWT_OBJS := $(addprefix $(KWT_ODIR)/,$(APP_OBJECTS))

# For linux builds, link to the correct libraries
ifeq ($(B_REFSW_OS),linuxkernel)
LDFLAGS += ${NEXUS_LDFLAGS}
else
ifneq ($(findstring linux,$(B_REFSW_OS)),)
LDFLAGS += -lnexus -L${NEXUS_BIN_DIR} -lpthread -lm  $(EXTRA_DRM_OBJS)
endif
endif





###############
###### BWIN Widgets
###############
MLIBS=$(BSEAV)/lib/mlibs
CXXFLAGS += -I$(MLIBS)
CFLAGS += -I$(MLIBS)

# DEBUG SUFFIX Defined above
MLIBS_ODIR=$(B_REFSW_ARCH).$(DEBUG_SUFFIX)

CFLAGS += -I$(MLIBS) -L$(MLIBS)/$(MLIBS_ODIR)
LDFLAGS += -L$(MLIBS)/$(MLIBS_ODIR) -lmwidgets -lmcommon -lmnet

BWIN=$(BSEAV)/lib/bwin
include $(BWIN)/include/bwin.mak
CXXFLAGS += $(BWIN_CFLAGS) -DBWIN_SUPPORT
LDFLAGS += $(BWIN_LDFLAGS)
CFLAGS += -DBWIN_SUPPORT

VPATH += $(BWIDGETS_DIR)/src
include $(BSEAV)/lib/bwidgets/include/bwidgets.mak
CFLAGS += $(BWIDGETS_CFLAGS)
LDFLAGS += $(BWIDGETS_LDFLAGS)


###############
###### ASF AVI ETC MEDIA PROBE common Library
###############

# check to see if libraries exist, they should by default
ifeq ($(MEDIA_ASF_SUPPORT),)
ASF_SUPPORT= $(shell test -e ${BSEAV}/lib/asf && echo y)
else
ASF_SUPPORT=$(MEDIA_ASF_SUPPORT)
endif

ifeq ($(MEDIA_AVI_SUPPORT),)
AVI_SUPPORT= $(shell test -e ${BSEAV}/lib/avi && echo y)
else
AVI_SUPPORT=$(MEDIA_AVI_SUPPORT)
endif

ifeq ($(ASF_SUPPORT),y)
	include ${BSEAV}/lib/asf/basf.inc
endif
ifeq ($(AVI_SUPPORT),y)
	include ${BSEAV}/lib/avi/bavi.inc
endif
	include ${BSEAV}/../magnum/commonutils/vlc/bvlc.inc
	include ${BSEAV}/lib/media/bmedia.inc
        include ${BSEAV}/lib/utils/batom.inc
        include ${BSEAV}/lib/bfile/bfile.inc
ifneq ($(MEDIA_RMFF_SUPPORT),n)
        -include ${BSEAV}/lib/media/rmff/brmff.inc
endif
ifneq ($(MEDIA_FLV_SUPPORT),n)
        -include ${BSEAV}/lib/media/flv/bflv.inc
endif
        include ${BSEAV}/lib/bfile/bfile.inc
        KWT_MODULES := BMEDIA_PROBE BATOM BFILE_STDIO BVLC
	CFLAGS += -DPVR_SUPPORT

	# Direct bcmplayer access is needed for Video::indexToTime.
	CFLAGS += -I$(BSEAV)/lib/bcmplayer/include

	B_KWT_INC := $(addprefix -I,$(sort $(foreach module, $(KWT_MODULES), $($(module)_INCLUDES))))
	B_KWT_CFLAGS := $(addprefix -D,$(sort $(foreach module, $(KWT_MODULES), $($(module)_DEFINES))))
	B_KWT_SOURCES = $(sort $(foreach module, $(KWT_MODULES), $($(module)_SOURCES)))
	B_KWT_OBJS := $(patsubst %.c,%.o,$(notdir ${B_KWT_SOURCES}))
	vpath %c $(dir ${B_KWT_SOURCES})

	APP_OBJECTS += ${B_KWT_OBJS}
	CFLAGS += ${B_KWT_CFLAGS} ${B_KWT_INC}



# This is the minimum needed to compile and link with Nexus
CFLAGS += $(NEXUS_CFLAGS) $(addprefix -I,$(NEXUS_APP_INCLUDE_PATHS)) $(addprefix -D,$(NEXUS_APP_DEFINES))

# Always build with debug
CFLAGS += -MD -g
CFLAGS += -DBDBG_DEBUG_BUILD
CFLAGS += -W -MMD -pedantic
LDFLAGS += -ldl -lpthread

KWT_INCLUDE_DIRS = $(addprefix -I,$(KWT_DIRS))
VPATH += $(KWT_DIRS)

####
## Include the B_OS abstraction layer
####
KWT_INCLUDE_DIRS += -I$(BSEAV)/../nexus/lib/os/include
KWT_INCLUDE_DIRS += -I$(BSEAV)/../nexus/lib/os/include/linuxuser




##########
#### Link nexus libraries
##########
LDFLAGS += -lnexus -lb_os
CFLAGS += $(KWT_INCLUDE_DIRS)

C_ONLY_FLAGS := -std=c99 -std=c89 -Wstrict-prototypes

CXXFLAGS += -fno-rtti -fno-exceptions

vpath %.cpp $(VPATH)
vpath %.c $(VPATH)

# Implicit rule for building local app
$(APP): api lib $(KWT_ODIR) build_date $(addprefix $(KWT_ODIR)/,$(APP_OBJECTS)) $(LIBS)
	@echo [Linking... $(notdir $@)]
	$(Q_)$(CXX) -o $@ $(addprefix $(KWT_ODIR)/,$(APP_OBJECTS)) $(filter-out -lsettop,$(LDFLAGS)) $(LIBS)

$(KWT_LIB): $(addprefix $(KWT_ODIR)/,$(APP_OBJECTS))
	@echo [Linking... $(notdir $@)]
	${Q_}$(RM) $@
	${Q_}$(AR) cr $@ $^
	${Q_}$(RANLIB) $@


%.o: %.cpp
	@echo [Compile... $(notdir $<)]
	${Q_}$(CXX) $(CXXFLAGS) $(filter-out $(C_ONLY_FLAGS),$(CFLAGS)) -c $< -o $(KWT_ODIR)/$@

%.o: %.c
	@echo [Compile... $(notdir $<)]
	${Q_}$(CC) $(CFLAGS) -c $< -o $(KWT_ODIR)/$@

$(KWT_ODIR)/%.o: %.c
	@echo [Compile... $(notdir $<)]
	$(Q_)$(CC) -c -o $@ $< $(CFLAGS) $(C_ONLY_FLAGS)

$(KWT_ODIR)/%.o: %.cpp
	@echo [Compile... $(notdir $<)]
	$(Q_)$(CXX) -c -o $@ $< $(filter-out $(C_ONLY_FLAGS),$(CFLAGS)) $(CXXFLAGS)



#Only Cleans Playback-IP library not SSL, only
min-clean:  clean-kwt api-clean
	@echo [..... Minimal Cleaning Done....]

#
# This is the main installation rule.
# It will build the binaries and utilities, and then
# perform the actual installation procedure.
#
install:  bin_install $(STAGING_DIR) install-binaries make-tarball

#
# Install drivers, libraries and apps
#
.PHONY: install-binaries
install-binaries: api-install  install-config-files

#
# STAGING_DIR is phony because we always want to remove it and
# recreate it.
#
.PHONY: $(STAGING_DIR)
$(STAGING_DIR):
	@$(RM) -rf $(STAGING_DIR)
	@${MKDIR} $(STAGING_DIR)

#
# Copy everything into temporary STAGING_DIR and tar it up.
# The final tarball is left in KWT_BIN_DIR.
#

make-tarball: install-support-files
	@echo [Build..... tarball]
	@(cd $(STAGING_DIR);tar -pzckf $(KWT_BIN_DIR)/${KWT_TAR_BALL} .)
	@$(RM) -rf $(STAGING_DIR)
	@echo ""
	@echo "*********************************************"
	@echo "* Installation tarball created in $(KWT_BIN_DIR)."
	@echo "*"
	@echo "* To complete installation perform the"
	@echo "* following operations on the settop box:"
	@echo "*"
	@echo "* gunzip -c ${KWT_TAR_BALL} | tar xvpf -"
	@echo "* start kwt"
	@echo "*********************************************"
	@echo ""

#
# Install drivers, libraries and apps
#
.PHONY: install-support-files
install-support-files:
	@echo [Install... scripts]
	$(Q_)$(CP) start $(STAGING_DIR)
	$(Q_)chmod +x $(STAGING_DIR)/start
ifeq ($(NEXUS_MODE),proxy)
	$(Q_)$(CP) $(NEXUS_BIN_DIR)/nexus.ko $(STAGING_DIR)
endif
	$(Q_)$(CP) $(NEXUS_BIN_DIR)/logger $(STAGING_DIR)
	$(Q_)$(CP) $(NEXUS_BIN_DIR)/nexus $(STAGING_DIR)
	$(Q_)chmod +x $(STAGING_DIR)/nexus


	$(Q_)$(CP) $(APP) $(STAGING_DIR)

	@echo [Install... fonts]
	@${MKDIR} $(STAGING_DIR)/fonts
	@$(CP) $(PRERENDERED_FONTS) $(STAGING_DIR)/fonts

	@echo [Install... images]
	@${MKDIR} $(STAGING_DIR)/images
	@$(CP) $(PRERENDERED_IMAGES) $(STAGING_DIR)/images


####
## install Config Files
####
install-config-files:
	@echo  [...... Installing config files to $(KWT_BIN_DIR) ......]

bin-install-config-files:
# Copy over config files so we can run without doing a make install
# you can run this manually with "make bin-install-config-files"
	@echo  [...... Installing config files to $(KWT_BIN_DIR) ......]

-include $(KWT_ODIR)/*.d
