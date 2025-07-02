file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
    TextServerAdvanced
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

option(GRAPHITE "Enable SIL Graphite smart fonts support" ON)

add_dependencies(rex_module_${MODULE_NAME}
	rex_module_freetype
	rex_module_msdfgen
	rex_module_svg
)

if ("svg" IN_LIST ENABLED_MODULES)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/thorvg/inc/
		${CMAKE_SOURCE_DIR}/thirdparty/thorvg/common/
		${CMAKE_SOURCE_DIR}/thirdparty/thorvg/renderer/
	)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		TVG_STATIC
	)
endif()

if (BUILTIN_HARFBUZZ)
	set(THIRDPARTY_HARFBUZZ_DIR ${CMAKE_SOURCE_DIR}/thirdparty/harfbuzz/)
	add_library(harfbuzz_builtin STATIC
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-aat-layout.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-aat-map.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-blob.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-buffer-serialize.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-buffer-verify.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-buffer.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-common.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-draw.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-face-builder.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-face.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-fallback-shape.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-font.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-icu.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-map.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-number.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-cff1-table.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-cff2-table.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-color.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-face.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-font.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-layout.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-map.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-math.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-meta.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-metrics.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-name.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-arabic.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-default.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-hangul.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-hebrew.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-indic-table.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-indic.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-khmer.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-myanmar.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-syllabic.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-thai.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-use.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shaper-vowel-constraints.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shape-fallback.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shape-normalize.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-shape.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-tag.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ot-var.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-outline.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-paint-extents.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-paint.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-set.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-shape-plan.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-shape.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-shaper.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-static.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-style.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-subset-cff-common.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-subset-cff1.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-subset-cff2.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-subset-input.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-subset-instancer-iup.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-subset-instancer-solver.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-subset-plan.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-subset-serialize.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-subset.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-ucd.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/hb-unicode.cc
        ${THIRDPARTY_HARFBUZZ_DIR}src/OT/Var/VARC/VARC.cc
	)
	if ("freetype" IN_LIST ENABLED_MODULES)
		target_sources(harfbuzz_builtin PRIVATE
			${THIRDPARTY_HARFBUZZ_DIR}src/hb-ft.cc
		)
		target_compile_options(harfbuzz_builtin PRIVATE
            -DHAVE_FREETYPE
		)
		if (GRAPHITE)
			target_sources(harfbuzz_builtin PRIVATE
				${THIRDPARTY_HARFBUZZ_DIR}src/hb-graphite2.cc
			)
			target_compile_options(harfbuzz_builtin PRIVATE
				-DHAVE_GRAPHITE2
			)
		endif()
		if (BUILTIN_FREETYPE)
			target_include_directories(harfbuzz_builtin PRIVATE
				${CMAKE_SOURCE_DIR}/thirdparty/freetype/include/
			)
		endif()
		if (BUILTIN_GRAPHITE AND GRAPHITE)
			target_include_directories(harfbuzz_builtin PRIVATE
				${CMAKE_SOURCE_DIR}/thirdparty/graphite/include/
			)
			target_compile_options(harfbuzz_builtin PRIVATE
				-DGRAPHITE2_STATIC
			)
		endif()
	endif()
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_HARFBUZZ_DIR}src
	)
	target_compile_options(harfbuzz_builtin PRIVATE
		-DHAVE_ICU
	)
	if (BUILTIN_ICU4C)
		target_include_directories(harfbuzz_builtin PRIVATE
			${CMAKE_SOURCE_DIR}/thirdparty/icu4c/common/
			${CMAKE_SOURCE_DIR}/thirdparty/icu4c/i18n/
		)
		target_compile_options(harfbuzz_builtin PRIVATE
            -DU_STATIC_IMPLEMENTATION
            -DU_HAVE_LIB_SUFFIX=1
            -DU_LIB_SUFFIX_C_NAME=_godot
            -DHAVE_ICU_BUILTIN
		)
	endif()
	if (REX_PLATFORM MATCHES "android|linuxbsd|web")
		target_compile_options(harfbuzz_builtin PRIVATE
			-DHAVE_PTHREAD
		)
	endif()
	list(APPEND BUILTIN_LIBRARIES harfbuzz_builtin)

	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_HARFBUZZ_DIR}src/
	)
endif()

if (BUILTIN_GRAPHITE)
	set(THIRDPARTY_GRAPHITE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/graphite/)
	add_library(graphite_builtin STATIC
		${THIRDPARTY_GRAPHITE_DIR}src/gr_char_info.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/gr_face.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/gr_features.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/gr_font.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/gr_logging.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/gr_segment.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/gr_slot.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/CmapCache.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Code.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Collider.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Decompressor.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Face.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/FeatureMap.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Font.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/GlyphCache.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/GlyphFace.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Intervals.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Justifier.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/NameTable.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Pass.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Position.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Segment.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Silf.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Slot.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/Sparse.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/TtfUtil.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/UtfCodec.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/FileFace.cpp
		${THIRDPARTY_GRAPHITE_DIR}src/json.cpp
	)
	if (MSVC)
		target_sources(graphite_builtin PRIVATE
			${THIRDPARTY_GRAPHITE_DIR}src/call_machine.cpp
		)
	else()
		target_sources(graphite_builtin PRIVATE
			${THIRDPARTY_GRAPHITE_DIR}src/direct_machine.cpp
		)
	endif()
	target_include_directories(graphite_builtin PRIVATE
		${THIRDPARTY_GRAPHITE_DIR}src/
		${THIRDPARTY_GRAPHITE_DIR}include/
	)
	target_compile_options(graphite_builtin PRIVATE
		-DGRAPHITE2_STATIC
		-DGRAPHITE2_NTRACING
		-DGRAPHITE2_NFILEFACE
	)
	list(APPEND BUILTIN_LIBRARIES graphite_builtin)
endif()

if (BUILTIN_ICU4C)
	set(THIRDPARTY_ICU4C_DIR ${CMAKE_SOURCE_DIR}/thirdparty/icu4c/)
	add_library(icu_builtin STATIC
		${THIRDPARTY_ICU4C_DIR}common/appendable.cpp
		${THIRDPARTY_ICU4C_DIR}common/bmpset.cpp
		${THIRDPARTY_ICU4C_DIR}common/brkeng.cpp
		${THIRDPARTY_ICU4C_DIR}common/brkiter.cpp
		${THIRDPARTY_ICU4C_DIR}common/bytesinkutil.cpp
		${THIRDPARTY_ICU4C_DIR}common/bytestream.cpp
		${THIRDPARTY_ICU4C_DIR}common/bytestrie.cpp
		${THIRDPARTY_ICU4C_DIR}common/bytestriebuilder.cpp
		${THIRDPARTY_ICU4C_DIR}common/bytestrieiterator.cpp
		${THIRDPARTY_ICU4C_DIR}common/caniter.cpp
		${THIRDPARTY_ICU4C_DIR}common/characterproperties.cpp
		${THIRDPARTY_ICU4C_DIR}common/chariter.cpp
		${THIRDPARTY_ICU4C_DIR}common/charstr.cpp
		${THIRDPARTY_ICU4C_DIR}common/cmemory.cpp
		${THIRDPARTY_ICU4C_DIR}common/cstr.cpp
		${THIRDPARTY_ICU4C_DIR}common/cstring.cpp
		${THIRDPARTY_ICU4C_DIR}common/cwchar.cpp
		${THIRDPARTY_ICU4C_DIR}common/dictbe.cpp
		${THIRDPARTY_ICU4C_DIR}common/dictionarydata.cpp
		${THIRDPARTY_ICU4C_DIR}common/dtintrv.cpp
		${THIRDPARTY_ICU4C_DIR}common/edits.cpp
		${THIRDPARTY_ICU4C_DIR}common/emojiprops.cpp
		${THIRDPARTY_ICU4C_DIR}common/errorcode.cpp
		${THIRDPARTY_ICU4C_DIR}common/filteredbrk.cpp
		${THIRDPARTY_ICU4C_DIR}common/filterednormalizer2.cpp
		${THIRDPARTY_ICU4C_DIR}common/icudataver.cpp
		${THIRDPARTY_ICU4C_DIR}common/icuplug.cpp
		${THIRDPARTY_ICU4C_DIR}common/loadednormalizer2impl.cpp
		${THIRDPARTY_ICU4C_DIR}common/localebuilder.cpp
		${THIRDPARTY_ICU4C_DIR}common/localematcher.cpp
		${THIRDPARTY_ICU4C_DIR}common/localeprioritylist.cpp
		${THIRDPARTY_ICU4C_DIR}common/locavailable.cpp
		${THIRDPARTY_ICU4C_DIR}common/locbased.cpp
		${THIRDPARTY_ICU4C_DIR}common/locdispnames.cpp
		${THIRDPARTY_ICU4C_DIR}common/locdistance.cpp
		${THIRDPARTY_ICU4C_DIR}common/locdspnm.cpp
		${THIRDPARTY_ICU4C_DIR}common/locid.cpp
		${THIRDPARTY_ICU4C_DIR}common/loclikely.cpp
		${THIRDPARTY_ICU4C_DIR}common/loclikelysubtags.cpp
		${THIRDPARTY_ICU4C_DIR}common/locmap.cpp
		${THIRDPARTY_ICU4C_DIR}common/locresdata.cpp
		${THIRDPARTY_ICU4C_DIR}common/locutil.cpp
		${THIRDPARTY_ICU4C_DIR}common/lsr.cpp
		${THIRDPARTY_ICU4C_DIR}common/lstmbe.cpp
		${THIRDPARTY_ICU4C_DIR}common/messagepattern.cpp
		${THIRDPARTY_ICU4C_DIR}common/mlbe.cpp
		${THIRDPARTY_ICU4C_DIR}common/normalizer2.cpp
		${THIRDPARTY_ICU4C_DIR}common/normalizer2impl.cpp
		${THIRDPARTY_ICU4C_DIR}common/normlzr.cpp
		${THIRDPARTY_ICU4C_DIR}common/parsepos.cpp
		${THIRDPARTY_ICU4C_DIR}common/patternprops.cpp
		${THIRDPARTY_ICU4C_DIR}common/pluralmap.cpp
		${THIRDPARTY_ICU4C_DIR}common/propname.cpp
		${THIRDPARTY_ICU4C_DIR}common/propsvec.cpp
		${THIRDPARTY_ICU4C_DIR}common/punycode.cpp
		${THIRDPARTY_ICU4C_DIR}common/putil.cpp
		${THIRDPARTY_ICU4C_DIR}common/rbbi.cpp
		${THIRDPARTY_ICU4C_DIR}common/rbbi_cache.cpp
		${THIRDPARTY_ICU4C_DIR}common/rbbidata.cpp
		${THIRDPARTY_ICU4C_DIR}common/rbbinode.cpp
		${THIRDPARTY_ICU4C_DIR}common/rbbirb.cpp
		${THIRDPARTY_ICU4C_DIR}common/rbbiscan.cpp
		${THIRDPARTY_ICU4C_DIR}common/rbbisetb.cpp
		${THIRDPARTY_ICU4C_DIR}common/rbbistbl.cpp
		${THIRDPARTY_ICU4C_DIR}common/rbbitblb.cpp
		${THIRDPARTY_ICU4C_DIR}common/resbund.cpp
		${THIRDPARTY_ICU4C_DIR}common/resbund_cnv.cpp
		${THIRDPARTY_ICU4C_DIR}common/resource.cpp
		${THIRDPARTY_ICU4C_DIR}common/restrace.cpp
		${THIRDPARTY_ICU4C_DIR}common/ruleiter.cpp
		${THIRDPARTY_ICU4C_DIR}common/schriter.cpp
		${THIRDPARTY_ICU4C_DIR}common/serv.cpp
		${THIRDPARTY_ICU4C_DIR}common/servlk.cpp
		${THIRDPARTY_ICU4C_DIR}common/servlkf.cpp
		${THIRDPARTY_ICU4C_DIR}common/servls.cpp
		${THIRDPARTY_ICU4C_DIR}common/servnotf.cpp
		${THIRDPARTY_ICU4C_DIR}common/servrbf.cpp
		${THIRDPARTY_ICU4C_DIR}common/servslkf.cpp
		${THIRDPARTY_ICU4C_DIR}common/sharedobject.cpp
		${THIRDPARTY_ICU4C_DIR}common/simpleformatter.cpp
		${THIRDPARTY_ICU4C_DIR}common/static_unicode_sets.cpp
		${THIRDPARTY_ICU4C_DIR}common/stringpiece.cpp
		${THIRDPARTY_ICU4C_DIR}common/stringtriebuilder.cpp
		${THIRDPARTY_ICU4C_DIR}common/uarrsort.cpp
		${THIRDPARTY_ICU4C_DIR}common/ubidi.cpp
		${THIRDPARTY_ICU4C_DIR}common/ubidi_props.cpp
		${THIRDPARTY_ICU4C_DIR}common/ubidiln.cpp
		${THIRDPARTY_ICU4C_DIR}common/ubiditransform.cpp
		${THIRDPARTY_ICU4C_DIR}common/ubidiwrt.cpp
		${THIRDPARTY_ICU4C_DIR}common/ubrk.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucase.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucasemap.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucasemap_titlecase_brkiter.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucat.cpp
		${THIRDPARTY_ICU4C_DIR}common/uchar.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucharstrie.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucharstriebuilder.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucharstrieiterator.cpp
		${THIRDPARTY_ICU4C_DIR}common/uchriter.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucln_cmn.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucmndata.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv2022.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_bld.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_cb.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_cnv.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_ct.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_err.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_ext.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_io.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_lmb.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_set.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_u16.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_u32.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_u7.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnv_u8.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnvbocu.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnvdisp.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnvhz.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnvisci.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnvlat1.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnvmbcs.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnvscsu.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucnvsel.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucol_swp.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucptrie.cpp
		${THIRDPARTY_ICU4C_DIR}common/ucurr.cpp
		${THIRDPARTY_ICU4C_DIR}common/udata.cpp
		${THIRDPARTY_ICU4C_DIR}common/udatamem.cpp
		${THIRDPARTY_ICU4C_DIR}common/udataswp.cpp
		${THIRDPARTY_ICU4C_DIR}common/uenum.cpp
		${THIRDPARTY_ICU4C_DIR}common/uhash.cpp
		${THIRDPARTY_ICU4C_DIR}common/uhash_us.cpp
		${THIRDPARTY_ICU4C_DIR}common/uidna.cpp
		${THIRDPARTY_ICU4C_DIR}common/uinit.cpp
		${THIRDPARTY_ICU4C_DIR}common/uinvchar.cpp
		${THIRDPARTY_ICU4C_DIR}common/uiter.cpp
		${THIRDPARTY_ICU4C_DIR}common/ulist.cpp
		${THIRDPARTY_ICU4C_DIR}common/uloc.cpp
		${THIRDPARTY_ICU4C_DIR}common/uloc_keytype.cpp
		${THIRDPARTY_ICU4C_DIR}common/uloc_tag.cpp
		${THIRDPARTY_ICU4C_DIR}common/ulocale.cpp
		${THIRDPARTY_ICU4C_DIR}common/ulocbuilder.cpp
		${THIRDPARTY_ICU4C_DIR}common/umapfile.cpp
		${THIRDPARTY_ICU4C_DIR}common/umath.cpp
		${THIRDPARTY_ICU4C_DIR}common/umutablecptrie.cpp
		${THIRDPARTY_ICU4C_DIR}common/umutex.cpp
		${THIRDPARTY_ICU4C_DIR}common/unames.cpp
		${THIRDPARTY_ICU4C_DIR}common/unifiedcache.cpp
		${THIRDPARTY_ICU4C_DIR}common/unifilt.cpp
		${THIRDPARTY_ICU4C_DIR}common/unifunct.cpp
		${THIRDPARTY_ICU4C_DIR}common/uniset.cpp
		${THIRDPARTY_ICU4C_DIR}common/uniset_closure.cpp
		${THIRDPARTY_ICU4C_DIR}common/uniset_props.cpp
		${THIRDPARTY_ICU4C_DIR}common/unisetspan.cpp
		${THIRDPARTY_ICU4C_DIR}common/unistr.cpp
		${THIRDPARTY_ICU4C_DIR}common/unistr_case.cpp
		${THIRDPARTY_ICU4C_DIR}common/unistr_case_locale.cpp
		${THIRDPARTY_ICU4C_DIR}common/unistr_cnv.cpp
		${THIRDPARTY_ICU4C_DIR}common/unistr_props.cpp
		${THIRDPARTY_ICU4C_DIR}common/unistr_titlecase_brkiter.cpp
		${THIRDPARTY_ICU4C_DIR}common/unorm.cpp
		${THIRDPARTY_ICU4C_DIR}common/unormcmp.cpp
		${THIRDPARTY_ICU4C_DIR}common/uobject.cpp
		${THIRDPARTY_ICU4C_DIR}common/uprops.cpp
		${THIRDPARTY_ICU4C_DIR}common/ures_cnv.cpp
		${THIRDPARTY_ICU4C_DIR}common/uresbund.cpp
		${THIRDPARTY_ICU4C_DIR}common/uresdata.cpp
		${THIRDPARTY_ICU4C_DIR}common/usc_impl.cpp
		${THIRDPARTY_ICU4C_DIR}common/uscript.cpp
		${THIRDPARTY_ICU4C_DIR}common/uscript_props.cpp
		${THIRDPARTY_ICU4C_DIR}common/uset.cpp
		${THIRDPARTY_ICU4C_DIR}common/uset_props.cpp
		${THIRDPARTY_ICU4C_DIR}common/usetiter.cpp
		${THIRDPARTY_ICU4C_DIR}common/usprep.cpp
		${THIRDPARTY_ICU4C_DIR}common/ustack.cpp
		${THIRDPARTY_ICU4C_DIR}common/ustr_cnv.cpp
		${THIRDPARTY_ICU4C_DIR}common/ustr_titlecase_brkiter.cpp
		${THIRDPARTY_ICU4C_DIR}common/ustr_wcs.cpp
		${THIRDPARTY_ICU4C_DIR}common/ustrcase.cpp
		${THIRDPARTY_ICU4C_DIR}common/ustrcase_locale.cpp
		${THIRDPARTY_ICU4C_DIR}common/ustrenum.cpp
		${THIRDPARTY_ICU4C_DIR}common/ustrfmt.cpp
		${THIRDPARTY_ICU4C_DIR}common/ustring.cpp
		${THIRDPARTY_ICU4C_DIR}common/ustrtrns.cpp
		${THIRDPARTY_ICU4C_DIR}common/utext.cpp
		${THIRDPARTY_ICU4C_DIR}common/utf_impl.cpp
		${THIRDPARTY_ICU4C_DIR}common/util.cpp
		${THIRDPARTY_ICU4C_DIR}common/util_props.cpp
		${THIRDPARTY_ICU4C_DIR}common/utrace.cpp
		${THIRDPARTY_ICU4C_DIR}common/utrie.cpp
		${THIRDPARTY_ICU4C_DIR}common/utrie2.cpp
		${THIRDPARTY_ICU4C_DIR}common/utrie2_builder.cpp
		${THIRDPARTY_ICU4C_DIR}common/utrie_swap.cpp
		${THIRDPARTY_ICU4C_DIR}common/uts46.cpp
		${THIRDPARTY_ICU4C_DIR}common/utypes.cpp
		${THIRDPARTY_ICU4C_DIR}common/uvector.cpp
		${THIRDPARTY_ICU4C_DIR}common/uvectr32.cpp
		${THIRDPARTY_ICU4C_DIR}common/uvectr64.cpp
		${THIRDPARTY_ICU4C_DIR}common/wintz.cpp
		${THIRDPARTY_ICU4C_DIR}i18n/scriptset.cpp
		${THIRDPARTY_ICU4C_DIR}i18n/ucln_in.cpp
		${THIRDPARTY_ICU4C_DIR}i18n/uspoof.cpp
		${THIRDPARTY_ICU4C_DIR}i18n/uspoof_impl.cpp
	)
	if (EDITOR_BUILD)
		target_sources(icu_builtin PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/icu_data/icudata_stub.cpp
		)
	endif()
	target_include_directories(icu_builtin PRIVATE
		${THIRDPARTY_ICU4C_DIR}common/
		${THIRDPARTY_ICU4C_DIR}i18n/
	)
	target_compile_options(icu_builtin PRIVATE
		-DU_STATIC_IMPLEMENTATION
		-DU_COMMON_IMPLEMENTATION
		-DUCONFIG_NO_COLLATION
		-DUCONFIG_NO_CONVERSION
		-DUCONFIG_NO_FORMATTING
		-DUCONFIG_NO_SERVICE
		-DUCONFIG_NO_IDNA
		-DUCONFIG_NO_FILE_IO
		-DUCONFIG_NO_TRANSLITERATION
		-DUCONFIG_NO_REGULAR_EXPRESSIONS
		-DPKGDATA_MODE=static
		-DU_ENABLE_DYLOAD=0
		-DU_HAVE_LIB_SUFFIX=1
		-DU_LIB_SUFFIX_C_NAME=_godot
	)
	target_compile_options(rex_module_${MODULE_NAME} PRIVATE
		-DU_STATIC_IMPLEMENTATION
		-DU_HAVE_LIB_SUFFIX=1
		-DU_LIB_SUFFIX_C_NAME=_godot
	)
	if (EDITOR_BUILD)
		# TODO: Python script
		target_compile_options(rex_module_${MODULE_NAME} PRIVATE
			-DICU_STATIC_DATA
		)
		target_include_directories(rex_module_${MODULE_NAME} PRIVATE
			${THIRDPARTY_ICU4C_DIR}
		)
	endif()
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_ICU4C_DIR}common/
		${THIRDPARTY_ICU4C_DIR}i18n/
	)
	list(APPEND BUILTIN_LIBRARIES icu_builtin)
endif()

if (BUILTIN_MSDFGEN AND "msdfgen" IN_LIST ENABLED_MODULES)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		MSDFGEN_PUBLIC=
	)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/msdfgen/
	)
endif()

if (BUILTIN_FREETYPE AND "freetype" IN_LIST ENABLED_MODULES)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		FT_CONFIG_OPTION_USE_BROTLI
	)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/freetype/include/
	)
endif()

if (BUILTIN_GRAPHITE AND GRAPHITE AND "freetype" IN_LIST ENABLED_MODULES)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/graphite/include/
	)
endif()




file(GLOB TEXT_SERVER_ADV_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${TEXT_SERVER_ADV_SOURCES}
)

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/thirdparty/icu4c/icudata.gen.h
	COMMAND ${_Python3_EXECUTABLE}
	ARGS ${CMAKE_SOURCE_DIR}/generators/rex_text_server_adv_builders.py make_icu_data
	${CMAKE_SOURCE_DIR}/thirdparty/icu4c/icudata.gen.h ${CMAKE_SOURCE_DIR}/thirdparty/icu4c/icudt_godot.dat
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Generating icu4data Header..."
)
add_custom_target(icu4_data DEPENDS ${CMAKE_SOURCE_DIR}/thirdparty/icu4c/icudata.gen.h)
add_dependencies(rex_module_${MODULE_NAME} icu4_data)

set(BUILTIN_LIBRARIES ${BUILTIN_LIBRARIES} PARENT_SCOPE)
