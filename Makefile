#############################################################################
# File       [ Makefile ]
# Author     [ littleshamoo ]
# Synopsis   [ top makefile ]
# Date       [ 2011/11/30 ]
#############################################################################

include common.mk
include setup.mk
include macro.mk

.PHONY : build lib add clean distclean

.SUFFIXES :

.SECONDARY : $(SECS)


build : $(STA_OBJS) $(TOP_BINS)
	@echo -n ""


lib : $(LIBS)
	@echo -n ""


add :
	$(call gen_add_pkg)


clean :
	$(AT)for pkg in $(PKG); do \
		echo "    CLEAN $$pkg $(PRT_MODE)"; \
		rm -Rf $(TMP_TOP)/$(MODE)/$(TMP_DEP)/$(PKG_DIR)/$$pkg/*; \
		rm -Rf $(TMP_TOP)/$(MODE)/$(TMP_LNY)/$(PKG_DIR)/$$pkg/*; \
		rm -Rf $(TMP_TOP)/$(MODE)/$(TMP_QT)/$(PKG_DIR)/$$pkg/*; \
		rm -Rf $(TMP_TOP)/$(MODE)/$(TMP_OBJ)/$(TMP_STA)/$(PKG_DIR)/$$pkg/*; \
		rm -Rf $(TMP_TOP)/$(MODE)/$(TMP_OBJ)/$(TMP_DYN)/$(PKG_DIR)/$$pkg/*; \
		rm -Rf $(INC_TOP)/$$pkg/*; \
	done
	@echo "    RM binaries $(PRT_MODE)"
	$(AT)rm -f $(TOP_BINS)
	$(AT)rm -f $(BINS)


libclean :
	@for lib in $(STA_LIBS) $(DYN_LIBS); do \
		echo "    CLEAN $$lib $(PRT_MODE)"; \
		rm -f $(LIB_TOP)/$(MODE)/lib$${lib}.a; \
		rm -f $(LIB_TOP)/$(MODE)/lib$${lib}.so; \
	done


distclean :
	@echo "    RM build"
	$(AT)rm -Rf $(TMP_TOP)
	@echo "    RM library"
	$(AT)rm -Rf $(LIB_TOP)
	@echo "    RM include"
	$(AT)rm -Rf $(INC_TOP)
	@echo "    RM binaries"
	$(AT)rm -Rf $(BIN_TOP)
	$(AT)rm -f  $(RM_BINS)
	$(AT)for dir in $(RM_BIN_DIRS); do \
		if [ -d $$dir ]; then \
			rmdir --ignore-fail-on-non-empty $$dir; \
		fi \
	done


$(TMP_TOP)/c_rule.mk : setup.mk
	@echo "    GEN $(notdir $@)"
	$(call check_target_dir)
	$(AT)rm -f $@
	$(AT)touch $@
	$(call gen_c_rule)


$(TMP_TOP)/lny_rule.mk : setup.mk
	@echo "    GEN $(notdir $@)"
	$(call check_target_dir)
	$(AT)rm -f $@
	$(AT)touch $@
	$(call gen_lny_rule)


$(TMP_TOP)/qt_rule.mk : setup.mk
	@echo "    GEN $(notdir $@)"
	$(call check_target_dir)
	$(AT)rm -f $@
	$(AT)touch $@
	$(call gen_qt_rule)


$(TMP_TOP)/cu_rule.mk : setup.mk
	@echo "    GEN $(notdir $@)"
	$(call check_target_dir)
	$(AT)rm -f $@
	$(AT)touch $@
	$(call gen_cu_rule)


$(TMP_TOP)/ld_rule.mk : setup.mk
	@echo "    GEN $(notdir $@)"
	$(call check_target_dir)
	$(AT)rm -f $@
	$(AT)touch $@
	$(call gen_ld_rule)


$(TMP_TOP)/lib_rule.mk : setup.mk
	@echo "    GEN $(notdir $@)"
	$(call check_target_dir)
	$(AT)rm -f $@
	$(AT)touch $@
	$(call gen_lib_rule)


$(TMP_TOP)/bin_rule.mk : setup.mk
	@echo "    GEN $(notdir $@)"
	$(call check_target_dir)
	$(AT)rm -f $@
	$(AT)touch $@
	$(call gen_bin_rule)


ifeq "$(MAKECMDGOALS)" ""
    -include $(DEPS)
    -include $(TMP_TOP)/c_rule.mk
    -include $(TMP_TOP)/lny_rule.mk
    -include $(TMP_TOP)/qt_rule.mk
    -include $(TMP_TOP)/cu_rule.mk
    -include $(TMP_TOP)/ld_rule.mk
    -include $(TMP_TOP)/lib_rule.mk
    -include $(TMP_TOP)/bin_rule.mk
endif


ifneq "$(filter $(MAKECMDGOALS),build lib)" ""
    -include $(DEPS)
    -include $(TMP_TOP)/c_rule.mk
    -include $(TMP_TOP)/lny_rule.mk
    -include $(TMP_TOP)/qt_rule.mk
    -include $(TMP_TOP)/cu_rule.mk
    -include $(TMP_TOP)/ld_rule.mk
    -include $(TMP_TOP)/lib_rule.mk
    -include $(TMP_TOP)/bin_rule.mk
endif
