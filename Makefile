# SPDX-License-Identifier: BSD-2-Clause
# 
# Copyright (c) 2022 Vincent DEFERT. All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions 
# are met:
# 
# 1. Redistributions of source code must retain the above copyright 
# notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright 
# notice, this list of conditions and the following disclaimer in the 
# documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
# POSSIBILITY OF SUCH DAMAGE.

# Prerequisites --------------------------------------------------------
#
# Besides make, his project requires: 
#
# - sdcc
# - stcgal-patched
# - minicom
# - doxygen

# Usage ----------------------------------------------------------------
#
# Build executable in release mode:
#   make
#
# Build executable in debug mode:
#   make BUILD_MODE=debug
#
# Build documentation:
#   make doc
#
# Upload executable to MCU:
#   make upload
#
# Open serial console in new window:
#   make console
#
# Clean project (remove all build files):
#   make clean

# Target MCU settings --------------------------------------------------

# Note: using a system clock around 24MHz works with all MCU
# having an internal RC oscillator.
MCU_FREQ_KHZ := 32000

STACK_SIZE := 112

# IAP15W413AS
MEMORY_SIZES := \
	--xram-loc 0 \
	--xram-size 256 \
	--stack-size $(STACK_SIZE) \
	--code-size 13312

MEMORY_MODEL := --model-medium

HAS_DUAL_DPTR := n

# Define UNISTC_DIR, HAL_DIR, DRIVER_DIR, and MAKE_DIR -----------------
include ./uni-STC/makefiles/0-directories.mk

# Project settings -----------------------------------------------------
PROJECT_NAME := ec1515s_clock

SRCS := \
	$(HAL_DIR)/delay.c \
	$(HAL_DIR)/gpio-hal.c \
	$(HAL_DIR)/timer-hal.c \
	$(HAL_DIR)/fifo-buffer.c \
	$(HAL_DIR)/uart-hal.c \
	$(HAL_DIR)/adc-comp-hal.c \
	$(HAL_DIR)/pca-hal.c \
	$(HAL_DIR)/serial-console.c \
	driver/led-seg.c \
	driver/button.c \
	driver/sensor.c \
	driver/ds1302.c \
	driver/buzzer.c \
	ui/datetime.c \
	ui/temp-light.c \
	main.c

CONSOLE_BAUDRATE := 57600
CONSOLE_PORT := COM5

ISP_PORT := COM5

PROJECT_FLAGS := -I./driver -I./ui

STCGAL_OPTIONS := -P stc15

# Boilerplate rules ----------------------------------------------------
include $(MAKE_DIR)/1-mcu-settings.mk
-include $(DEP_FILE)
include $(MAKE_DIR)/2-mcu-rules.mk

# show memory usage
showmem:
	cat ./build/$(PROJECT_NAME).mem
