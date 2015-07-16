#############################################################################
# File       [ setup.mk ]
# Author     [ littleshamoo ]
# Synopsis   [ package and library setup and customization ]
# Date       [ 2010/11/30 ]
#############################################################################


### modes ###################################################################
# MODES                - available modes
# MODE                 - current mode
# MODE_<MODE>_INCS     - include locations for MODE
# MODE_<MODE>_LD_PATHS - link paths for MODE
# MODE_<MODE>_LD_LIBS  - link libraries for MODE
# MODE_<MODE>_<FLAG>   - compiler flags for MODE
#############################################################################

MODES = debug release debug_output
MODE = release
MODE_debug_C_FLAGS = -Wall -g 
MODE_release_FLAGS = -g -pg -O3
### packages ################################################################
# PKGS                           - packages to be compiled
# PKG_<PKG>_DEPS                 - package dependencies
# PKG_<PKG>_MAINS                - package main function files
# PKG_<PKG>_INCS                 - package include locations
# PKG_<PKG>_LD_PATHS             - package link locations
# PKG_<PKG>_LD_LIBS              - package link libraries
# PKG_<PKG>_<FLAGS>              - package compiler flags
# PKG_<PKG>_MODE_<MODE>_INCS     - package include locations for MODE
# PKG_<PKG>_MODE_<MODE>_LD_PATHS - package link locations for MODE
# PKG_<PKG>_MODE_<MODE>_LD_LIBS  - package link libraries for MODE
# PKG_<PKG>_MODE_<MODE>_<FLAGS>  - package compiler flags for MODE
#############################################################################
PKGS = interface utility core ir_interface ir_core matrix idea

#interface
PKG_interface_MAINS = vlog_test.cpp sdf_test.cpp

#utility
PKG_utility_MAINS = cell_extracter.cpp wave_view_test.cpp

#core
PKG_core_DEPS = interface ir_interface
PKG_core_MAINS = circuit_test.cpp delay_info_test.cpp pat_test.cpp circuit_simulator_test.cpp find_ppo.cpp lib_test.cpp

#ir_interface
PKG_ir_interface_MAINS = spf_test.cpp 

#matrix
PKG_matrix_MAINS = matrix_test.cpp
PKG_matrix_INCS = ext_inc/klu
PKG_matrix_LD_PATHS = ext_lib/klu
PKG_matrix_LD_LIBS = klu amd btf colamd suitesparseconfig

#ir_core
PKG_ir_core_DEPS = ir_interface matrix
PKG_ir_core_MAINS = power_grid_test.cpp power_grid_solver_test.cpp
PKG_ir_core_INCS = ext_inc/klu
PKG_ir_core_LD_PATHS = ext_lib/klu
PKG_ir_core_LD_LIBS = klu amd btf colamd suitesparseconfig

#idea
PKG_idea_DEPS = ir_core core utility matrix
PKG_idea_MAINS = idea_test.cpp  path_gene.cpp regeneration.cpp idea_main.cpp
PKG_idea_INCS = ext_inc/klu
PKG_idea_LD_PATHS = ext_lib/klu
PKG_idea_LD_LIBS = klu amd btf colamd suitesparseconfig
### main functions ##########################################################
# MAIN_<PKG>/<MAIN>_BIN                  - binary name
# MAIN_<PKG>/<MAIN>_LD_PATHS             - link paths
# MAIN_<PKG>/<MAIN>_LD_LIBS              - link libraries
# MAIN_<PKG>/<MAIN>_LD_FLAGS             - link flags
# MAIN_<PKG>/<MAIN>_MODE_<MODE>_LD_PATHS - link paths for MODE
# MAIN_<PKG>/<MAIN>_MODE_<MODE>_LD_LIBS  - link libraries for MODE
# MAIN_<PKG>/<MAIN>_MODE_<MODE>_LD_FLAGS - link flags for MODE
#############################################################################

MAIN_interface/vlog_test.cpp_BIN = vlog_test.out
MAIN_interface/sdf_test.cpp_BIN = sdf_test.out
MAIN_utility/cell_extracter.cpp_BIN = cell_extracter.out
MAIN_utility/wave_view_test.cpp_BIN = wave_view_test.out
MAIN_core/circuit_test.cpp_BIN = circuit_test.out
MAIN_core/delay_info_test.cpp_BIN = delay_info_test.out
MAIN_core/pat_test.cpp_BIN = pat_test.out
MAIN_core/circuit_simulator_test.cpp_BIN = circuit_simulator_test.out
MAIN_core/find_ppo.cpp_BIN = find_ppo.out
MAIN_core/lib_test.cpp_BIN = lib_test.out
MAIN_ir_interface/spf_test.cpp_BIN = spf_test.out
MAIN_ir_core/power_grid_test.cpp_BIN = power_grid_test.out
MAIN_ir_core/power_grid_solver_test.cpp_BIN = power_grid_solver_test.out
MAIN_idea/idea_test.cpp_BIN = idea_test.out
MAIN_idea/path_gene.cpp_BIN = path_gene.out
MAIN_idea/regeneration.cpp_BIN = regeneration.out
MAIN_idea/idea_main.cpp_BIN = idea_main.out

### libraries ###############################################################
# STA_LIBS        - static archived libraries
# DYN_LIBS        - dynamic loaded shared libraries
# LIB_$(LIB)_PKGS - library packages
#############################################################################

