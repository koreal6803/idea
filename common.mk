#############################################################################
# File       [ common.mk ]
# Author     [ littleshamoo ]
# Synopsis   [ project layout and common flags ]
# Date       [ 2011/11/30 ]
#############################################################################

# shell
SHELL         = /bin/bash


# verbosity
VERBOSE       = 0


# TOP layout
PKG_TOP       = pkg
LIB_TOP       = lib
INC_TOP       = include
BIN_TOP       = bin


# packge layout
INC_DIR       = src
SRC_DIR       = src
BIN_DIR       = bin


# tmp layout
TMP_TOP       = .tmp


# C/C++
CXX           = g++
CC            = gcc
CMN_C_FLAGS   = -Wall
C_DYN_FLAGS   = -fPIC


# lex
LEX           = flex
CMN_L_FLAGS   =


# yacc
YACC          = bison
CMN_Y_FLAGS   = -d


# Qt
MOC           = moc-qt4
CMN_MOC_FLAGS =
QT_PATH       = /usr


# CUDA
NVCC          = nvcc
CMN_NV_FLAGS  =
NV_DYN_FLAGS  = -Xcompiler -fPIC
CU_TOOL_PATH  = /usr/local/cuda
CU_SDK_PATH   = /usr/local/cuda_sdk_4.2


# link
LD            = g++
CMN_INCS      =
CMN_LD_PATHS  =
CMN_LD_LIBS   =
CMN_LD_FLAGS  =


# archive
AR            = ar
AR_FLAGS      = cr
RANLIB        = ranlib


