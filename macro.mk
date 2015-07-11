#############################################################################
# File       [ macro.mk ]
# Author     [ littleshamoo ]
# Synopsis   [ ]
# Date       [ 2012/01/06 created ]
#############################################################################

# verbosity
AT_0           := @
AT_1           :=
AT             = $(AT_$(VERBOSE))


# print mode
MODE_DOT       = 0
ifeq "$(MODE)" "."
    MODE_DOT = 1
endif
ifeq "$(MODE)" "./"
    MODE_DOT = 1
endif
ifeq "$(MODE)" ""
    MODE_DOT = 1
endif
PRT_MODE_0     := ($(MODE))
PRT_MODE_1     :=
PRT_MODE       := $(PRT_MODE_$(MODE_DOT))


# package top
PKG_DOT        = 0
ifeq "$(PKG_TOP)" "."
    PKG_DOT = 1
endif
ifeq "$(PKG_TOP)" "./"
    PKG_DOT = 1
endif
ifeq "$(PKG_TOP)" ""
    PKG_DOT = 1
endif
PKG_DIR_0     = $(PKG_TOP)
PKG_DIR_1     = .
PKG_DIR       = $(PKG_DIR_$(PKG_DOT))
PKG_UP_0      = ../
PKG_UP_1      =
PKG_UP        = $(PKG_UP_$(PKG_DOT))


# tmp layout
TMP_OBJ        = obj
TMP_DEP        = dep
TMP_STA        = sta
TMP_DYN        = dyn
TMP_LNY        = lny
TMP_QT         = qt


# build packages
PKG            = $(PKGS)
BUILD_PKGS     = $(call get_build_pkgs,$(PKG))


# C/C++
C_SRCS         = $(call get_srcs,$(BUILD_PKGS),c cpp,)
C_DEPS         = $(call get_deps,$(BUILD_PKGS),c cpp,)
C_STA_OBJS     = $(call get_sta_objs,$(BUILD_PKGS),c cpp,,)
C_DYN_OBJS     = $(call get_dyn_objs,$(BUILD_PKGS),c cpp,,)
C_MAINS        = $(call get_mains,$(BUILD_PKGS),c cpp,)
C_BINS         = $(call get_bins,$(BUILD_PKGS),c cpp,)


# lex
L_SRCS         = $(call get_srcs,$(BUILD_PKGS),l,)
L_SECS         = $(call get_secs,$(BUILD_PKGS),l,,$(TMP_LNY),cpp)
L_DEPS         = $(call get_deps,$(BUILD_PKGS),l,,cpp)
L_STA_OBJS     = $(call get_sta_objs,$(BUILD_PKGS),l,,cpp)
L_DYN_OBJS     = $(call get_dyn_objs,$(BUILD_PKGS),l,,cpp)
L_MAINS        = $(call get_mains,$(BUILD_PKGS),l,)
L_BINS         = $(call get_bins,$(BUILD_PKGS),l,)


# yacc
Y_SRCS         = $(call get_srcs,$(BUILD_PKGS),y,)
Y_SECS         = $(call get_secs,$(BUILD_PKGS),y,,$(TMP_LNY),cpp)
Y_DEPS         = $(call get_deps,$(BUILD_PKGS),y,,cpp)
Y_STA_OBJS     = $(call get_sta_objs,$(BUILD_PKGS),y,,cpp)
Y_DYN_OBJS     = $(call get_dyn_objs,$(BUILD_PKGS),y,,cpp)
Y_MAINS        = $(call get_mains,$(BUILD_PKGS),y,)
Y_BINS         = $(call get_bins,$(BUILD_PKGS),y,)


# Qt
QT_SRCS        = $(call get_srcs,$(BUILD_PKGS),h,Q_OBJECT)
QT_SECS        = $(call get_secs,$(BUILD_PKGS),h,Q_OBJECT,$(TMP_QT),cpp)
QT_DEPS        = $(call get_deps,$(BUILD_PKGS),h,Q_OBJECT,cpp)
QT_STA_OBJS    = $(call get_sta_objs,$(BUILD_PKGS),h,Q_OBJECT,cpp)
QT_DYN_OBJS    = $(call get_dyn_objs,$(BUILD_PKGS),h,Q_OBJECT,cpp)

QT_LD_PATHS    = $(QT_PATH)/lib64
QT_LD_LIBS     = QtCore QtGui
QT_INCS        = $(QT_PATH)/include/QtCore $(QT_PATH)/include/QtGui \
                 $(QT_PATH)/include


# CUDA
#CU_SRCS        = $(call get_srcs,$(BUILD_PKGS),cu,)
#CU_DEPS        = $(call get_deps,$(BUILD_PKGS),cu,)
#CU_STA_OBJS    = $(call get_sta_objs,$(BUILD_PKGS),cu,,)
#CU_DYN_OBJS    = $(call get_dyn_objs,$(BUILD_PKGS),cu,,)
#CU_MAINS       = $(call get_mains,$(BUILD_PKGS),cu,)
#CU_BINS        = $(call get_bins,$(BUILD_PKGS),cu,)
#
#CU_LD_PATHS    = $(CU_TOOL_PATH)/lib64 $(CU_SDK_PATH)/C/lib \
#                 $(CU_SDK_PATH)/shared/lib $(CU_SDK_PATH)/OpenCL/common/lib
#CU_LD_LIBS     = cudart cutil_x86_64 shrutil_x86_64 oclUtil_x86_64
#CU_INCS        = $(CU_TOOL_PATH)/include $(CU_SDK_PATH)/C/common/inc \
                 $(CU_SDK_PATH)/shared/inc $(CU_SDK_PATH)/OpenCL/common/inc


# all languages
STA_OBJS       = $(C_STA_OBJS) $(L_STA_OBJS) $(Y_STA_OBJS) \
                 $(QT_STA_OBJS) $(CU_STA_OBJS)

DYN_OBJS       = $(C_DYN_OBJS) $(L_DYN_OBJS) $(Y_DYN_OBJS) \
                 $(QT_DYN_OBJS) $(CU_DYN_OBJS)

DEPS           = $(C_DEPS) $(L_DEPS) $(Y_DEPS) $(QT_DEPS) $(CU_DEPS)

SECS           = $(L_SECS) $(Y_SECS) $(QT_SECS)

MAINS          = $(C_MAINS) $(L_MAINS) $(Y_MAINS) $(CU_MAINS)

BINS           = $(C_BINS) $(L_BINS) $(Y_BINS) $(CU_BINS)

TOP_BINS       = $(addprefix $(BIN_TOP)/$(MODE)/,$(notdir $(BINS)))

# mode specific variables
MODE_INCS      = $(addprefix -I,$(PKG_SAFE) $(INC_TOP) $(MODE_$(MODE)_INCS) \
                     $(QT_INCS) $(CU_INCS) $(CMN_INCS))

MODE_LD_PATHS  = $(addprefix -L,$(LIB_TOP) $(MODE_$(MODE)_LD_PATHS) \
                     $(QT_LD_PATHS) $(CU_LD_PATHS) $(CMN_LD_PATHS))


# mode specific flags
MODE_C_FLAGS   = $(if $(MODE_$(MODE)_C_FLAGS),$(MODE_$(MODE)_C_FLAGS),\
                     $(CMN_C_FLAGS))

MODE_L_FLAGS   = $(if $(MODE_$(MODE)_L_FLAGS),$(MODE_$(MODE)_L_FLAGS),\
                     $(CMN_L_FLAGS))

MODE_Y_FLAGS   = $(if $(MODE_$(MODE)_Y_FLAGS),$(MODE_$(MODE)_Y_FLAGS),\
                    $(CMN_Y_FLAGS))

MODE_MOC_FLAGS = $(if $(MODE_$(MODE)_MOC_FLAGS),$(MODE_$(MODE)_MOC_FLAGS),\
                     $(CMN_MOC_FLAGS))

MODE_LD_FLAGS  = $(if $(MODE_$(MODE)_LD_FLAGS),$(MODE_$(MODE)_LD_FLAGS),\
                     $(CMN_LD_FLAGS))

MODE_LD_LIBS   = $(if $(MODE_$(MODE)_LD_LIBS),$(MODE_$(MODE)_LD_LIBS),\
                     $(CMN_LD_LIBS))


# libraries
LIBS           = $(addprefix $(LIB_TOP)/$(MODE)/lib,\
                     $(addsuffix .a,$(STA_LIBS)) $(addsuffix .so,$(DYN_LIBS)))


# determine binary directory removal
RM_BINS        = $(foreach pkg,$(PKGS),$(if $(MODES),$(foreach mode,$(MODES),\
                     $(addprefix $(PKG_DIR)/$(pkg)/$(BIN_DIR)/$(mode)/,\
                         $(call get_bin_bases,$(pkg),c cpp cu l y,))),\
                             $(addprefix $(PKG_DIR)/$(pkg)/$(BIN_DIR)/,\
                                 $(call get_bin_bases,$(pkg),c cpp cu l y,))))



RM_BIN_DIRS    = $(subst .,,$(foreach pkg,$(PKGS),\
                     $(addprefix $(PKG_DIR)/$(pkg)/$(BIN_DIR)/,$(MODES))) \
                 $(foreach pkg,$(PKGS),$(PKG_DIR)/$(pkg)/$(BIN_DIR)))


#############################################################################
# package functions
#############################################################################

# $(call get_build_pkgs,<packages>)
get_build_pkgs = $(filter $(PKGS),$(call get_dep_pkgs,$1))


# $(call get_dep_pkgs,<packages>)
get_dep_pkgs = $(call rm_dup,$(if $(word 1,$1),$(call get_dep_pkgs,\
                  $(PKG_$(word 1,$1)_DEPS) $(if $(word 2,$1),\
                      $(wordlist 2,$(words $1),$1),)),) $(word 1,$1))


# $(call rm_dup,<list>)
rm_dup = $(if $(firstword $1),$(firstword $1) \
             $(filter-out $(firstword $1),\
                 $(call rm_dup,$(wordlist 2,$(words $1),$1))),)


# $(call get_top,<packages>)
NULL    :=
WHT     := $(NULL) $(NULL)
get_top = $(foreach pkg,$1,\
              $(subst $(WHT),/,$(patsubst %,..,$(subst /, ,$(pkg)))))


# $(call get_srcs,<packages>,<extensions>,<grep>)
get_srcs = $(if $3,$(if $(firstword $(call get_exts,$1,$2)),\
               $(shell grep -l "\<$3\>" $(call get_exts,$1,$2)),),\
                   $(call get_exts,$1,$2))

# $(call get_exts,<packages>,<extensions>)
get_exts = $(foreach pkg,$1,$(foreach ext,$2,\
               $(wildcard $(PKG_DIR)/$(pkg)/$(SRC_DIR)/*.$(ext)) \
                   $(if $(filter $(SRC_DIR),$(INC_DIR)),,\
                       $(wildcard $(PKG_DIR)/$(pkg)/$(INC_DIR)/*.$(ext)))))


# $(call get_secs,<packages>,<extensions>,<grep>,<target dir>,
#                 <secondary extension>)
get_secs = $(addprefix $(TMP_TOP)/$(MODE)/$4/,$(addsuffix .$5,\
               $(call get_srcs,$1,$2,$3)))


# $(call get_deps,<packages>,<extensions>,<grep>,[<secondary extension>])
get_deps = $(addprefix $(TMP_TOP)/$(MODE)/$(TMP_DEP)/,$(addsuffix \
               $(if $4,.$4,).d,$(call get_srcs,$1,$2,$3)))


# $(call get_sta_objs,<packages>,<extensions>,<grep>,[<secondary extension>])
get_sta_objs = $(addprefix $(TMP_TOP)/$(MODE)/$(TMP_OBJ)/$(TMP_STA)/,\
                   $(addsuffix $(if $4,.$4,).o,$(call get_srcs,$1,$2,$3)))


# $(call get_dyn_objs,<packages>,<extensions>,<grep>,[<secondary extension>])
get_dyn_objs = $(addprefix $(TMP_TOP)/$(MODE)/$(TMP_OBJ)/$(TMP_DYN)/,\
                   $(addsuffix $(if $4,.$4,).o,$(call get_srcs,$1,$2,$3)))


# $(call get_mains,<packages>,<extensions>,<grep>)
get_mains = $(foreach pkg,$1,$(filter $(call get_srcs,$(pkg),$2,$3),\
                $(addprefix $(PKG_DIR)/$(pkg)/$(SRC_DIR)/,\
                    $(PKG_$(pkg)_MAINS))))


# $(call get_main_bases,<packages>,<extensions>,<grep>)
get_main_bases = $(notdir $(call get_mains,$1,$2,$3))


# $(call get_bins,<packages>,<extensions>,<grep>)
get_bins = $(foreach pkg,$1,$(foreach main,$(call get_mains,$(pkg),$2,$3),\
               $(addprefix $(PKG_DIR)/$(pkg)/$(BIN_DIR)/$(MODE)/,\
                   $(if $(MAIN_$(pkg)/$(notdir $(main))_BIN),\
                       $(MAIN_$(pkg)/$(notdir $(main))_BIN),\
                           $(notdir $(basename $(main)))))))


# $(call get_bin_bases,<packages>,<extensions>,<grep>)
get_bin_bases = $(notdir $(call get_bins,$1,$2,$3))


# $(call get_ar_objs,<packages>)
get_ar_objs = \
    $(filter-out $(addprefix $(TMP_TOP)/$(MODE)/$(TMP_OBJ)/$(TMP_STA)/,\
        $(addsuffix .o,$(call get_mains,$1,c cpp cu,)) \
        $(addsuffix .cpp.o,$(call get_mains,$1,l y,))),\
            $(call get_sta_objs,$1,c cpp cu,) \
            $(call get_sta_objs,$1,l y,,cpp) \
            $(call get_sta_objs,$1,h,Q_OBJECT,cpp))


# $(call get_pic_objs,<packages>)
get_pic_objs = \
    $(filter-out $(addprefix $(TMP_TOP)/$(MODE)/$(TMP_OBJ)/$(TMP_DYN)/,\
        $(addsuffix .o,$(call get_mains,$1,c cpp cu,)) \
        $(addsuffix .cpp.o,$(call get_mains,$1,l y,))),\
            $(call get_dyn_objs,$1,c cpp cu,) \
            $(call get_dyn_objs,$1,l y,,cpp) \
            $(call get_dyn_objs,$1,h,Q_OBJECT,cpp))


# $(call get_rule_bins)
get_rule_bins = \
    $(foreach pkg,$(PKGS),$(foreach main,\
        $(call get_mains,$(pkg),c cpp cu l y,),\
            $(addprefix $$\(PKG_DIR\)/$(pkg)/$$\(BIN_DIR\)/$$\(MODE\)/,\
                $(if $(MAIN_$(pkg)/$(notdir $(main))_BIN),\
                    $(MAIN_$(pkg)/$(notdir $(main))_BIN),\
                        $(notdir $(basename $(main)))))))


# $(call get_rule_bin_pkgs)
get_rule_bin_pkgs = $(foreach pkg,$(PKGS),$(foreach main,\
                        $(call get_mains,$(pkg),c cpp cu l y,),$(pkg)))


# $(call get_rule_bin_objs)
get_rule_bin_objs = \
    $(foreach pkg,$(PKGS),\
        $(addprefix $$\(TMP_TOP\)/$$\(MODE\)/$$\(TMP_OBJ\)/$$\(TMP_STA\)/$$\(PKG_DIR\)/$(pkg)/$$\(SRC_DIR\)/,\
            $(addsuffix .o,$(notdir $(call get_mains,$(pkg),c cpp cu,))) \
            $(addsuffix .cpp.o,$(notdir $(call get_mains,$(pkg),l y,)))))


#############################################################################
# macros
#############################################################################

# $(call gen_add_pkg)
define gen_add_pkg
	@pkgs=($(ADD)); \
	tops=($(call get_top,$(ADD))); \
	for (( i = 0; i < $(words $(ADD)); i++)); do \
		pkg=$${pkgs[$$i]}; \
		top=$${tops[$$i]}; \
		if [ -d $(PKG_DIR)/$${pkgs[$$i]} ]; then \
			echo "**WARN package \`$${pkgs[$$i]}' already exist"; \
		else \
			echo "    ADD $${pkgs[$$i]}"; \
			mkdir -p $(PKG_DIR)/$${pkgs[$$i]}/$(SRC_DIR); \
			mkdir -p $(PKG_DIR)/$${pkgs[$$i]}/$(INC_DIR); \
			file=$(PKG_DIR)/$${pkgs[$$i]}/Makefile; \
			touch $$file; \
			echo    "build :"                                      >> $$file; \
			echo    "	@cd $(PKG_UP)$${tops[$$i]}; \\"            >> $$file; \
			echo -n "	make build PKG=$${pkgs[$$i]} "             >> $$file; \
			echo    "--no-print-directory"                         >> $$file; \
			echo    ""                                             >> $$file; \
			echo    "clean :"                                      >> $$file; \
			echo    "	@cd $(PKG_UP)$${tops[$$i]}; \\"            >> $$file; \
			echo -n "	make clean PKG=$${pkgs[$$i]} "             >> $$file; \
			echo    "--no-print-directory"                         >> $$file; \
			echo    ""                                             >> $$file; \
		fi \
	done
endef


# $(call gen_c_rule)
define gen_c_rule
	@for pkg in $(PKGS); do \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_DEP)/\$$(PKG_DIR)/"    >> $@; \
		echo -n "$$pkg/\$$(SRC_DIR)/%.cpp.d : "                        >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cpp"                >> $@; \
		$(call gen_c_dep_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_DEP)/\$$(PKG_DIR)/"    >> $@; \
		echo -n "$$pkg/\$$(SRC_DIR)/%.c.d : "                          >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.c"                  >> $@; \
		$(call gen_c_dep_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_STA)/"    >> $@; \
		echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cpp.o : "           >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cpp"                >> $@; \
		$(call gen_c_sta_obj_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_STA)/"    >> $@; \
		echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.c.o : "             >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.c"                  >> $@; \
		$(call gen_c_sta_obj_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_DYN)/"    >> $@; \
		echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cpp.o : "           >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cpp"                >> $@; \
		$(call gen_c_dyn_obj_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_DYN)/"    >> $@; \
		echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.c.o : "             >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.c"                  >> $@; \
		$(call gen_c_dyn_obj_rule) \
	done
endef


# $(call gen_lny_rule)
define gen_lny_rule
	@for pkg in $(PKGS); do \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_LNY)/\$$(PKG_DIR)/"    >> $@; \
		echo -n "$$pkg/\$$(SRC_DIR)/%.l.cpp : "                        >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.l"                  >> $@; \
		$(call gen_lex_c_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_LNY)/\$$(PKG_DIR)/"    >> $@; \
		echo -n "$$pkg/\$$(SRC_DIR)/%.y.cpp : "                        >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.y"                  >> $@; \
		$(call gen_yacc_c_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_DEP)/\$$(PKG_DIR)/"    >> $@; \
		echo -n "$$pkg/\$$(SRC_DIR)/%.cpp.d : "                        >> $@; \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_LNY)/\$$(PKG_DIR)/"    >> $@; \
		echo    "$$pkg/\$$(SRC_DIR)/%.cpp"                             >> $@; \
		$(call gen_c_dep_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_STA)/"    >> $@; \
		echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cpp.o : "           >> $@; \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_LNY)/\$$(PKG_DIR)/"    >> $@; \
		echo    "$$pkg/\$$(SRC_DIR)/%.cpp"                             >> $@; \
		$(call gen_c_sta_obj_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_DYN)/"    >> $@; \
		echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cpp.o : "           >> $@; \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_LNY)/\$$(PKG_DIR)/"    >> $@; \
		echo    "$$pkg/\$$(SRC_DIR)/%.cpp"                             >> $@; \
		$(call gen_c_dyn_obj_rule) \
	done
endef


# $(call gen_qt_rule)
define gen_qt_rule
	@for pkg in $(PKGS); do \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_QT)/\$$(PKG_DIR)/"     >> $@; \
		echo -n "$$pkg/\$$(INC_DIR)/%.h.cpp : "                        >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(INC_DIR)/%.h"                  >> $@; \
		$(call gen_qt_c_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_DEP)/\$$(PKG_DIR)/"    >> $@; \
		echo -n "$$pkg/\$$(INC_DIR)/%.cpp.d : "                        >> $@; \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_QT)/\$$(PKG_DIR)/"     >> $@; \
		echo    "$$pkg/\$$(INC_DIR)/%.cpp"                             >> $@; \
		$(call gen_c_dep_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_STA)/"    >> $@; \
		echo -n "\$$(PKG_DIR)/$$pkg/\$$(INC_DIR)/%.cpp.o : "           >> $@; \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_QT)/\$$(PKG_DIR)/"     >> $@; \
		echo    "$$pkg/\$$(INC_DIR)/%.cpp"                             >> $@; \
		$(call gen_c_sta_obj_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_DYN)/"    >> $@; \
		echo -n "\$$(PKG_DIR)/$$pkg/\$$(INC_DIR)/%.cpp.o : "           >> $@; \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_QT)/\$$(PKG_DIR)/"     >> $@; \
		echo    "$$pkg/\$$(INC_DIR)/%.cpp"                             >> $@; \
		$(call gen_c_dyn_obj_rule) \
	done
endef


# $(call gen_cu_rule)
define gen_cu_rule
	@for pkg in $(PKGS); do \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_DEP)/\$$(PKG_DIR)/"    >> $@; \
		echo -n "$$pkg/\$$(SRC_DIR)/%.cu.d : "                         >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cu"                 >> $@; \
		$(call gen_cu_dep_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_STA)/"    >> $@; \
		echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cu.o : "            >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cu"                 >> $@; \
		$(call gen_cu_sta_obj_rule) \
		echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_DYN)/"    >> $@; \
		echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cu.o : "            >> $@; \
		echo    "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/%.cu"                 >> $@; \
		$(call gen_cu_dyn_obj_rule) \
	done
endef


# $(call gen_ld_rule)
define gen_ld_rule
	@mains=($(call get_main_bases,$(PKGS),c cpp cu l y,)); \
	bins=($(call get_rule_bins)); \
	objs=($(call get_rule_bin_objs)); \
	pkgs=($(call get_rule_bin_pkgs)); \
	for (( i = 0; i < $(words $(get_rule_bins)); ++i )); do \
		main=$${mains[$$i]}; \
		bin=$${bins[$$i]}; \
		obj=$${objs[$$i]}; \
		pkg=$${pkgs[$$i]}; \
		echo -n "$${bin} : $${obj} "                                   >> $@; \
		echo -n "\$$(call get_ar_objs,"                                >> $@; \
		echo    "\$$(call get_dep_pkgs,$$pkg))"                         >> $@; \
		$(call check_target_dir_echo) \
		echo    "	\$$(AT)rm -f \$$@"                                 >> $@; \
		echo    "	@echo \"    LD \$$(notdir \$$@) \$$(PRT_MODE)\""   >> $@; \
		echo -n "	\$$(AT)\$$(LD) "                                   >> $@; \
		$(call gen_ld_paths) \
		$(call gen_main_flags,LD_FLAGS) \
		echo -n "-o \$$@ \$$^ "                                        >> $@; \
		$(call gen_ld_libs) \
		echo    ""                                                     >> $@; \
		echo    ""                                                     >> $@; \
	done
endef


# $(call gen_c_dep_rule)
define gen_c_dep_rule
	$(call check_target_dir_echo) \
	echo    "	\$$(AT)rm -f \$$@"                                     >> $@; \
	echo    "	@echo \"    DEP \$$(notdir \$$@) \$$(PRT_MODE)\""      >> $@; \
	echo -n "	\$$(AT)\$$(CXX) -MM "                                  >> $@; \
	$(call gen_incs) \
	echo    "\$$< > \$$@.\$$\$$\$$\$$; \\"                             >> $@; \
	echo -n "	sed 's,\\(\$$*\\)\\.o[ :]*,"                           >> $@; \
	echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_STA)/"        >> $@; \
	echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/\$$(notdir \$$<).o "      >> $@; \
	echo    "\$$@ : ,g' < \$$@.\$$\$$\$$\$$ > \$$@; \\"                >> $@; \
	echo -n "	sed 's,\\(\$$*\\)\\.o[ :]*,"                           >> $@; \
	echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_DYN)/"        >> $@; \
	echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/\$$(notdir \$$<).o "      >> $@; \
	echo    "\$$@ : ,g' < \$$@.\$$\$$\$$\$$ >> \$$@; \\"               >> $@; \
	echo    "	rm \$$@.\$$\$$\$$\$$"                                  >> $@; \
	echo    ""                                                         >> $@;
endef


# $(call gen_c_sta_obj_rule)
define gen_c_sta_obj_rule
	$(call check_target_dir_echo) \
	echo    "	\$$(AT)rm -f \$$@"                                     >> $@; \
	echo    "	@echo \"    CXX \$$(notdir \$$<) \$$(PRT_MODE)\""      >> $@; \
	echo -n "	\$$(AT)\$$(CXX) "                                      >> $@; \
	$(call gen_flags,C_FLAGS) \
	$(call gen_incs) \
	echo    "-c -o \$$@ \$$<"                                          >> $@; \
	echo    ""                                                         >> $@;
endef


# $(call gen_c_dyn_obj_rule)
define gen_c_dyn_obj_rule
	$(call check_target_dir_echo) \
	echo    "	\$$(AT)rm -f \$$@"                                     >> $@; \
	echo -n "	@echo \"    CXX \$$(notdir \$$<) \$$(PRT_MODE) "       >> $@; \
	echo    "(\$$(C_DYN_FLAGS))\""                                     >> $@; \
	echo -n "	\$$(AT)\$$(CXX) \$$(C_DYN_FLAGS) "                     >> $@; \
	$(call gen_flags,C_FLAGS) \
	$(call gen_incs) \
	echo    "-c -o \$$@ \$$<"                                          >> $@; \
	echo    ""                                                         >> $@;
endef


# $(call gen_lex_c_rule)
define gen_lex_c_rule
	$(call check_target_dir_echo) \
	echo    "	\$$(AT)rm -f \$$@"                                     >> $@; \
	echo    "	@echo \"    LEX \$$(notdir \$$@) \$$(PRT_MODE)\""      >> $@; \
	echo -n "	\$$(AT)\$$(LEX) "                                      >> $@; \
	$(call gen_flags,L_FLAGS) \
	echo    "-o\$$@ -P\$$(patsubst %.l.cpp,%,\$$(notdir \$$@)) \$$<"   >> $@; \
	echo    ""                                                         >> $@;
endef


# $(call gen_yacc_c_rule)
define gen_yacc_c_rule
	$(call check_target_dir_echo) \
	echo    "	\$$(AT)rm -f \$$@"                                     >> $@; \
	echo    "	@echo \"    YACC \$$(notdir \$$@) \$$(PRT_MODE)\""     >> $@; \
	echo -n "	\$$(AT)\$$(YACC) "                                     >> $@; \
	$(call gen_flags,Y_FLAGS) \
	echo    "-p \$$(patsubst %.y.cpp,%,\$$(notdir \$$@)) -o \$$@ \$$<" >> $@; \
	echo    ""                                                         >> $@;
endef


# $(call gen_qt_c_rule)
define gen_qt_c_rule
	$(call check_target_dir_echo) \
	echo    "	\$$(AT)rm -f \$$@"                                     >> $@; \
	echo    "	@echo \"    MOC \$$(notdir \$$@) \$$(PRT_MODE)\""      >> $@; \
	echo -n "	\$$(AT)\$$(MOC) "                                      >> $@; \
	$(call gen_flags,MOC_FLAGS) \
	echo    "-p $$pkg/\$$(INC_DIR) -o \$$@ \$$<"                       >> $@; \
	echo    ""                                                         >> $@;
endef


# $(call gen_cu_dep_rule)
define gen_cu_dep_rule
	$(call check_target_dir_echo) \
	echo    "	\$$(AT)rm -f \$$@"                                     >> $@; \
	echo    "	@echo \"    DEP \$$(notdir \$$@) \$$(PRT_MODE)\""      >> $@; \
	echo -n "	@\$$(NVCC) -M "                                        >> $@; \
	$(call gen_incs) \
	echo    "\$$< > \$$@.\$$\$$\$$\$$; \\"                             >> $@; \
	echo -n "	sed 's,\\(\$$*\\)\\.o[ :]*,"                           >> $@; \
	echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_STA)/"        >> $@; \
	echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/\$$(notdir \$$<).o "      >> $@; \
	echo    "\$$@ : ,g' < \$$@.\$$\$$\$$\$$ > \$$@; \\"                >> $@; \
	echo -n "	sed 's,\\(\$$*\\)\\.o[ :]*,"                           >> $@; \
	echo -n "\$$(TMP_TOP)/\$$(MODE)/\$$(TMP_OBJ)/\$$(TMP_DYN)/$$pkg/"  >> $@; \
	echo -n "\$$(PKG_DIR)/$$pkg/\$$(SRC_DIR)/\$$(notdir \$$<).o "      >> $@; \
	echo    "\$$@ : ,g' < \$$@.\$$\$$\$$\$$ >> \$$@; \\"               >> $@; \
	echo    "	rm \$$@.\$$\$$\$$\$$"                                  >> $@; \
	echo    ""                                                         >> $@;
endef


# $(call gen_cu_sta_obj_rule)
define gen_cu_sta_obj_rule
	$(call check_target_dir_echo) \
	echo    "	\$$(AT)rm -f \$$@"                                     >> $@; \
	echo    "	@echo \"    NVCC \$$(notdir \$$<) \$$(PRT_MODE)\""     >> $@; \
	echo -n "	\$$(AT)\$$(NVCC) "                                     >> $@; \
	$(call gen_flags,NV_FLAGS) \
	$(call gen_incs) \
	echo    "-c -o \$$@ \$$<"                                          >> $@; \
	echo    ""                                                         >> $@;
endef


# $(call gen_cu_dyn_obj_rule)
define gen_cu_dyn_obj_rule
	$(call check_target_dir_echo) \
	echo    "	\$$(AT)rm -f \$$@"                                     >> $@; \
	echo -n "	@echo \"    NVCC \$$(notdir \$$<) \$$(PRT_MODE) "      >> $@; \
	echo    "(\$$(NV_DYN_FLAGS))\""                                    >> $@; \
	echo -n "	\$$(AT)\$$(NVCC) \$$(NV_DYN_FLAGS) "                   >> $@; \
	$(call gen_flags,NV_FLAGS) \
	$(call gen_incs) \
	echo    "-c -o \$$@ \$$<"                                          >> $@; \
	echo    ""                                                         >> $@;
endef


# $(call gen_incs)
define gen_incs
	echo -n "\$$(addprefix -I,\$$(PKG_DIR)) "                          >> $@; \
	echo -n "\$$(addprefix -I,\$$(PKG_DIR)/$$pkg/\$$(INC_DIR)) "       >> $@; \
	echo -n "\$$(addprefix -I,\$$(PKG_$${pkg}_MODE_\$$(MODE)_INCS)) "  >> $@; \
	echo -n "\$$(addprefix -I,\$$(PKG_$${pkg}_INCS)) "                 >> $@; \
	echo -n "\$$(MODE_INCS) "                                          >> $@;
endef


# $(call gen_ld_paths)
define gen_ld_paths
	echo -n "\$$(addprefix -L,"                                        >> $@; \
	echo -n "\$$(MAIN_$${pkg}/$${main}_MODE_\$$(MODE)_LD_PATHS)) "     >> $@; \
	echo -n "\$$(addprefix -L,\$$(MAIN_$${pkg}/$${main}_LD_PATHS)) "   >> $@; \
	echo -n "\$$(addprefix -L,"                                        >> $@; \
	echo -n "\$$(PKG_$${pkg}_MODE_\$$(MODE)_LD_PATHS)) "               >> $@; \
	echo -n "\$$(addprefix -L,\$$(PKG_$${pkg}_LD_PATHS)) "             >> $@; \
	echo -n "\$$(MODE_LD_PATHS) "                                      >> $@;
endef


# $(call gen_main_flags,<flag>)
define gen_main_flags
	echo -n "\$$(if "                                                  >> $@; \
	echo -n "\$$(MAIN_$${pkg}/$${main}_MODE_\$$(MODE)_$1),"            >> $@; \
	echo -n "\$$(MAIN_$${pkg}/$${main}_MODE_\$$(MODE)_$1),"            >> $@; \
	echo -n "\$$(if \$$(MAIN_$${pkg}/$${main}_$1),"                    >> $@; \
	echo -n "\$$(MAIN_$${pkg}/$${main}_$1),"                           >> $@; \
	$(call gen_flags,$1) \
	echo -n ")) "                                                      >> $@;
endef


# $(call gen_ld_libs)
define gen_ld_libs
	echo -n "\$$(addprefix -l,"                                        >> $@; \
	$(call gen_main_flags,LD_LIBS) \
	echo -n ") "                                                       >> $@;
endef


# $(call gen_flags,<flag>)
define gen_flags
	echo -n "\$$(if \$$(PKG_$${pkg}_MODE_\$$(MODE)_$1),"               >> $@; \
	echo -n "\$$(PKG_$${pkg}_MODE_\$$(MODE)_$1),"                      >> $@; \
	echo -n "\$$(if \$$(PKG_$${pkg}_$1),\$$(PKG_$${pkg}_$1),"          >> $@; \
	echo -n "\$$(if \$$(MODE_\$$(MODE)_$1),\$$(MODE_\$$(MODE)_$1),"    >> $@; \
	echo -n "\$$(CMN_$1)))) "                                          >> $@;
endef


# $(call gen_lib_rule)
define gen_lib_rule
	@for lib in $(STA_LIBS); do \
		echo -n "\$$(LIB_TOP)/\$$(MODE)/lib$${lib}.a : "               >> $@; \
		echo -n "\$$(call get_ar_objs,"                                >> $@; \
		echo    "\$$(call get_dep_pkgs,\$$(LIB_$${lib}_PKGS)))"        >> $@; \
		$(call check_target_dir_echo) \
		echo    "	\$$(AT)rm -f \$$@"                                 >> $@; \
		echo    "	@echo \"    AR \$$(notdir \$$@) \$$(PRT_MODE)\""   >> $@; \
		echo    "	\$$(AT)\$$(AR) \$$(AR_FLAGS) \$$@ \$$^"            >> $@; \
		echo    "	\$$(AT)\$$(RANLIB) \$$@"                           >> $@; \
		echo    ""                                                     >> $@; \
	done
	@for lib in $(DYN_LIBS); do \
		echo -n "\$$(LIB_TOP)/\$$(MODE)/lib$${lib}.so : "              >> $@; \
		echo -n "\$$(call get_pic_objs,"                               >> $@; \
		echo    "\$$(call get_dep_pkgs,\$$(LIB_$${lib}_PKGS)))"        >> $@; \
		$(call check_target_dir_echo) \
		echo    "	\$$(AT)rm -f \$$@"                                 >> $@; \
		echo -n "	@echo \"    LD \$$(notdir \$$@) \$$(PRT_MODE) "    >> $@; \
		echo    "(-shared)\""                                          >> $@; \
		echo -n "	\$$(AT)\$$(LD) -shared -Wl,-soname,\$$@ "          >> $@; \
		echo    "-o \$$@ \$$^"                                         >> $@; \
		echo    ""                                                     >> $@; \
	done
endef


# $(call gen_bin_rule)
define gen_bin_rule
	@tops=($(call get_bin_bases,$(PKGS),c cpp cu l y,)); \
	bins=($(call get_rule_bins)); \
	pkgs=($(call get_rule_bin_pkgs)); \
	for (( i = 0; i < $(words $(get_rule_bins)); ++i )); do \
		top=$${tops[$$i]}; \
		bin=$${bins[$$i]}; \
		pkg=$${pkgs[$$i]}; \
		echo    "\$$(BIN_TOP)/\$$(MODE)/$$top : $$bin"                 >> $@; \
		$(call check_target_dir_echo) \
		echo    "	\$$(AT)rm -f \$$@"                                 >> $@; \
		echo    "	@echo \"    CP \$$(notdir \$$@) \$$(PRT_MODE)\""   >> $@; \
		echo    "	\$$(AT)cp -f \$$< \$$@"                            >> $@; \
		echo    ""                                                     >> $@; \
	done
endef


# $(call check_target_dir)
define check_target_dir
    @if [ ! -d $(dir $@) ]; then \
        mkdir -p $(dir $@); \
    fi
endef


# $(call check_target_dir_echo)
define check_target_dir_echo
	echo    "	@if [ ! -d \$$(dir \$$@) ]; then \\"                   >> $@; \
	echo    "		mkdir -p \$$(dir \$$@); \\"                        >> $@; \
	echo    "	fi"                                                    >> $@;
endef


