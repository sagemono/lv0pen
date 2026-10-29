include mk/toolchains.mk

BUILD_DIR := build

.PHONY: all check clean

all: lv0ldr lv0ldr2 metldr metldr2 isoldr lv1ldr lv2ldr appldr spp_verifier spu_token_processor spu_utoken_processor spu_pkg_rvk_verifier aim_spu_module manu_info_spu_module sb_iso_spu_module sc_iso lv0

check: check-lv0ldr check-lv0ldr2 check-metldr check-metldr2 check-isoldr check-lv1ldr check-lv2ldr check-appldr check-spp_verifier check-spu_token_processor check-spu_utoken_processor check-spu_pkg_rvk_verifier check-aim_spu_module check-manu_info_spu_module check-sb_iso_spu_module check-sc_iso check-lv0

clean: clean-lv0ldr clean-lv0ldr2 clean-metldr clean-metldr2 clean-isoldr clean-lv1ldr clean-lv2ldr clean-appldr clean-spp_verifier clean-spu_token_processor clean-spu_utoken_processor clean-spu_pkg_rvk_verifier clean-aim_spu_module clean-manu_info_spu_module clean-sb_iso_spu_module clean-sc_iso clean-lv0

include lv0ldr/lv0ldr.mk
include lv0ldr2/lv0ldr2.mk
include metldr/metldr.mk
include metldr2/metldr2.mk
include isoldr/isoldr.mk
include lv1ldr/lv1ldr.mk
include lv2ldr/lv2ldr.mk
include appldr/appldr.mk
include spp_verifier/spp_verifier.mk
include spu_token_processor/spu_token_processor.mk
include spu_utoken_processor/spu_utoken_processor.mk
include spu_pkg_rvk_verifier/spu_pkg_rvk_verifier.mk
include aim_spu_module/aim_spu_module.mk
include manu_info_spu_module/manu_info_spu_module.mk
include sb_iso_spu_module/sb_iso_spu_module.mk
include sc_iso/sc_iso.mk
include lv0/lv0.mk
