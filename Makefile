include mk/toolchains.mk

BUILD_DIR := build

.PHONY: all check clean

all: lv0ldr metldr isoldr lv1ldr lv2ldr appldr lv0

check: check-lv0ldr check-metldr check-isoldr check-lv1ldr check-lv2ldr check-appldr check-lv0

clean: clean-lv0ldr clean-metldr clean-isoldr clean-lv1ldr clean-lv2ldr clean-appldr clean-lv0

include lv0ldr/lv0ldr.mk
include metldr/metldr.mk
include isoldr/isoldr.mk
include lv1ldr/lv1ldr.mk
include lv2ldr/lv2ldr.mk
include appldr/appldr.mk
include lv0/lv0.mk
