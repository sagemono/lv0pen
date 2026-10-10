include mk/toolchains.mk

BUILD_DIR := build

.PHONY: all check clean

all: lv0ldr metldr isoldr lv1ldr lv2ldr appldr spp_verifier spu_token_processor spu_utoken_processor spu_pkg_rvk_verifier lv0

check: check-lv0ldr check-metldr check-isoldr check-lv1ldr check-lv2ldr check-appldr check-spp_verifier check-spu_token_processor check-spu_utoken_processor check-spu_pkg_rvk_verifier check-lv0

clean: clean-lv0ldr clean-metldr clean-isoldr clean-lv1ldr clean-lv2ldr clean-appldr clean-spp_verifier clean-spu_token_processor clean-spu_utoken_processor clean-spu_pkg_rvk_verifier clean-lv0

include lv0ldr/lv0ldr.mk
include metldr/metldr.mk
include isoldr/isoldr.mk
include lv1ldr/lv1ldr.mk
include lv2ldr/lv2ldr.mk
include appldr/appldr.mk
include spp_verifier/spp_verifier.mk
include spu_token_processor/spu_token_processor.mk
include spu_utoken_processor/spu_utoken_processor.mk
include spu_pkg_rvk_verifier/spu_pkg_rvk_verifier.mk
include lv0/lv0.mk
