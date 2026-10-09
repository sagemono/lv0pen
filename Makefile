include mk/toolchains.mk

BUILD_DIR := build

.PHONY: all check clean

all: lv0ldr metldr

check: check-lv0ldr check-metldr

clean: clean-lv0ldr clean-metldr

include lv0ldr/lv0ldr.mk
include metldr/metldr.mk
