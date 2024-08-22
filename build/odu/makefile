################################################################################
#   Copyright (c) [2017-2019] [Radisys]                                        #
#                                                                              #
#   Licensed under the Apache License, Version 2.0 (the "License");            #
#   you may not use this file except in compliance with the License.           #
#   You may obtain a copy of the License at                                    #
#                                                                              #
#       http://www.apache.org/licenses/LICENSE-2.0                             #
#                                                                              #
#   Unless required by applicable law or agreed to in writing, software        #
#   distributed under the License is distributed on an "AS IS" BASIS,          #
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
#   See the License for the specific language governing permissions and        #
#   limitations under the License.                                             #
################################################################################

# Master makefile

# Identify the location our software which is used by rest
# of the build scripts

include ../common/rsys_fancy.mak
include ../common/env.mak

RULE=$(COLOR_RED)
OPTS=$(COLOR_GREEN)
NORM=$(REVERT_COLOR)

# For improved speed
CPUINFO=`cat /proc/cpuinfo | grep processor| wc -l`
ifeq ($(CPUINFO), 1)
   NUM_PARALLEL=1
else
      NUM_PARALLEL=$(shell echo $(CPUINFO) - 1 |bc)
endif

ifeq ($(FAST), 1)
      NUM_PARALLEL=$(shell echo $(CPUINFO) + 2 |bc)
endif

PARALLEL_COMP=-j $(NUM_PARALLEL)

# Different options to CPUH and CPUL builds
# # Other apsects of tool chain set here
# # These should be made available appropriately to the builds
ifeq ($(MACHINE),BIT64)
ifeq ($(COVERAGE),ENABLE)
CC          =gcc -m64 --coverage
else
CC          =gcc -m64
endif
CC_STRIP    =strip --strip-all
CC_LINKER   =gcc -m64
CCPP        =g++ -m64
CCPP_LINKER =g++ -m64
else
CC          =gcc -m32
CC_STRIP    =strip --strip-all
CC_LINKER   =gcc -m32
CCPP        =g++ -m32
CCPP_LINKER =g++ -m32
endif

CC1= $(CC)
CCPP1= $(CCPP)

ifeq ($(MODE),TDD)
CNM_ENABLE = YES
else
CNM_ENABLE=NO
endif

#-----------------------------------------------------------------------
# macro for output file name and makefile name
#

PLTFRM_FLAGS=-UMSPD -DODU -DINTEL_FAPI -UODU_MEMORY_DEBUG_LOG -DDEBUG_ASN_PRINT -UDEBUG_PRINT -DERROR_PRINT -USTART_DL_UL_DATA -UNR_DRX -UCALL_FLOW_DEBUG_LOG -UODU_SLOT_IND_DEBUG_LOG -UNFAPI_ENABLED -UTHREAD_AFFINITY -UMEM_SIZE_CHECK 

ifeq ($(MODE),TDD)
   PLTFRM_FLAGS += -DNR_TDD
endif
ifeq ($(CA_ENABLE),YES)
   PLTFRM_FLAGS += -DCA_ENABLE=YES
endif

ifeq ($(NODE),TEST_STUB)
	PLTFRM_FLAGS+=-DODU_TEST_STUB
endif

ifeq ($(PHY), INTEL_L1)
	PLTFRM_FLAGS+=-DSS_USE_WLS_MEM -DINTEL_WLS_MEM -DDEBUG_MODE -DINTEL_L1_V20_03_ONWARDS 
ifeq ($(PHY_MODE),TIMER)
	PLTFRM_FLAGS+=-DINTEL_TIMER_MODE
endif
endif

ifeq ($(O1_ENABLE),YES)
	PLTFRM_FLAGS+=-DO1_ENABLE
endif

# The include options get merged with the options needed by
# # the called makefiles and hence we export these to make them
# # available to them.
BUILD=i686-linux

# The called makefiles depend on these macros and these need to be exported
export PLTFRM
export PLTFRM_FLAGS
export BUILD
export I_OPTS

# Add to the linker options the platform specific components
L_OPTS+=-lnsl -lrt -lm -lpthread -lsctp -lxml2

ifeq ($(PHY), INTEL_L1)
	L_OPTS+=-L/root/Intel-L1-20.11.1/phy/wls_lib/ -lwls                         \
        -lhugetlbfs -lnuma -ldl -L/root/Intel-L1-20.11.1/dpdk-20.11.1/build/lib                        \
        -lrte_gso -lrte_acl -lrte_hash -lrte_bbdev -lrte_ip_frag -lrte_bitratestats -lrte_ipsec        \
        -lrte_bpf -lrte_jobstats -lrte_telemetry -lrte_kni -lrte_kvargs -lrte_latencystats -lrte_port  \
        -lrte_lpm -lrte_power -lrte_mbuf -lrte_rawdev -lrte_member -lrte_cfgfile -lrte_mempool         \
		  -lrte_cmdline -lrte_rcu -lrte_compressdev -lrte_reorder -lrte_cryptodev -lrte_rib              \
		  -lrte_distributor -lrte_meter  -lrte_ring -lrte_eal -lrte_metrics -lrte_sched -lrte_efd        \
		  -lrte_net -lrte_security -lrte_ethdev -lrte_pci -lrte_stack -lrte_eventdev -lrte_pdump         \
		  -lrte_table -lrte_fib -lrte_pipeline -lrte_timer -lrte_flow_classify -lrte_vhost               \
        -lrte_gro
endif

ifeq ($(O1_ENABLE),YES)
  L_OPTS+=-lsysrepo -lyang
  L_OPTS+=-lsysrepo-cpp -lyang-cpp
L_OPTS+= -lnetconf2 -lcjson -lcurl
  L_OPTS+=-lstdc++
endif

# Export some of the flags expected from the command line.
# # These macros are made available for the makefiles called from this makefile
export BOARD
export MACHINE
export MODE
export OAM_ENABLE
export CNM_ENABLE
export TEXT_LOG
export CA_ENABLE
export O1_ENABLE

prepare_dirs:
	$(Q)echo -e "Preparing directories for build..."
	$(Q)mkdir -p $(BUILD_DIR)/obj/odu
	$(Q)mkdir -p $(LIB_ROOT)/odu
	$(Q)mkdir -p $(ROOT_DIR)/libs/
	$(Q)mkdir -p $(BIN_DIR)/odu
	$(Q)mkdir -p $(ROOT_DIR)/bin
	$(Q)echo -e "Directories are successfully prepared"

du:
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/du_app.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'
		
ifeq ($(O1_ENABLE),YES)
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/o1.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CCPP1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/ves.mak OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CCPP1)'
endif

link_du: du
		   $(Q)$(CC1) -g -o $(OBJ_ROOT)/odu/odu -Wl,-R../lib/:. $(OBJ_ROOT)/odu/*.o\
			$(L_OPTS) -L$(LIB_ROOT)/odu -L$(ROOT_DIR)/libs/odu 

clean_odu: 
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/du_app.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CC1)'

ifeq ($(O1_ENABLE),YES)
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/o1.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CCPP1)'
		$(Q)$(MAKE) -j -f $(COM_BUILD_DIR)/ves.mak clean OBJ_DIR=$(OBJ_ROOT)/odu LIB_DIR=$(LIB_ROOT)/odu LOG_DIR=$(LOG_ROOT)/odu CC='$(CCPP1)'
endif
		$(Q)rm -rf $(OBJ_ROOT)/odu/*
		$(Q)rm -rf $(LIB_ROOT)/odu/*
		$(Q)rm -rf $(BIN_DIR)/odu/*
	   $(Q)rm -rf $(ROOT_DIR)/bin/odu
	   $(Q)echo -e "***** ODU CLEAN COMPLETE *****"

clean_all: clean_odu
			  $(Q)rm -rf $(OBJ_ROOT)
			  $(Q)rm -rf $(LIB_ROOT)
			  $(Q)rm -rf $(LOG_ROOT)
			  $(Q)rm -rf $(BIN_DIR)
			  $(Q)rm -rf $(ROOT_DIR)/bin
			  $(Q)rm -rf $(ROOT_DIR)/libs


copy_build: link_du
			$(Q)cp -f ./obj/odu/odu ./bin/odu
			$(Q)cp -rf ./bin/odu $(ROOT_DIR)/bin/
			$(Q)cp -f ../scripts/start_du_logging.sh $(ROOT_DIR)/bin/odu
			$(Q)cp -rf ../config/ $(ROOT_DIR)/bin/odu
			$(Q)cp -f ./lib/odu/*.a $(ROOT_DIR)/libs/
		   $(Q)echo -e "***** BUILD COMPLETE *****"

odu: prepare_dirs copy_build

#**********************************************************************
#         End of file
#**********************************************************************
