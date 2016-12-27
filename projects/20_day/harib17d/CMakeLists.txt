#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17d_OS    ${root_BINARY_DIR}/projects/20_day/harib17d/os.img)
set(20_day_harib17d_SYS	  ${root_BINARY_DIR}/projects/20_day/harib17d/os.sys)
set(20_day_harib17d_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17d/ipl.bin)
set(20_day_harib17d_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17d/ipl10.nas)
set(20_day_harib17d_HEADB ${root_BINARY_DIR}/projects/20_day/harib17d/asmhead.bin)
set(20_day_harib17d_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17d/asmhead.nas)
set(20_day_harib17d_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17d/boot.bin)
set(20_day_harib17d_CCO	  ${root_BINARY_DIR}/projects/20_day/harib17d/boot.o)
set(20_day_harib17d_CCS	  ${root_SOURCE_DIR}/projects/20_day/harib17d/*.c)
set(20_day_harib17d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17d_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17d/naskfunc.nas)
set(20_day_harib17d_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17d/naskfunc.o)
set(20_day_harib17d_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17d/hankaku.txt)
set(20_day_harib17d_FONTB ${root_BINARY_DIR}/projects/20_day/harib17d/hankaku.bin)
set(20_day_harib17d_FONTO ${root_BINARY_DIR}/projects/20_day/harib17d/hankaku.o)
set(20_day_harib17d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17d_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17d/libgo.o)
set(20_day_harib17d_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17d/*.o)


add_custom_target(20_day_harib17d_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17d_OS}
  DEPENDS 20_day_harib17d_img
)
add_custom_target(20_day_harib17d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17d_img
)
add_custom_target(20_day_harib17d_clean
  COMMAND rm -f ${20_day_harib17d_OS}
  COMMAND rm -f ${20_day_harib17d_SYS}
  COMMAND rm -f ${20_day_harib17d_IPLB}
  COMMAND rm -f ${20_day_harib17d_HEADB}
  COMMAND rm -f ${20_day_harib17d_BOOTB}
  COMMAND rm -f ${20_day_harib17d_WILDOBJ}
)
add_custom_target(20_day_harib17d_ipl
  COMMAND ${NASK} ${20_day_harib17d_IPLS} ${20_day_harib17d_IPLB}
)
add_custom_target(20_day_harib17d_sys
  COMMAND ${NASK} ${20_day_harib17d_HEADS} ${20_day_harib17d_HEADB}
  COMMAND ${FONT} ${20_day_harib17d_FONTS} ${20_day_harib17d_FONTB}
  COMMAND ${B2O}  ${20_day_harib17d_FONTB} ${20_day_harib17d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17d_LIBGE} ${20_day_harib17d_LIBGC}
  COMMAND ${NASK} ${20_day_harib17d_FUNCS} ${20_day_harib17d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17d_LDS} ${20_day_harib17d_CCS} ${20_day_harib17d_WILDOBJ} -o ${20_day_harib17d_BOOTB}
  COMMAND cat ${20_day_harib17d_HEADB} ${20_day_harib17d_BOOTB} > ${20_day_harib17d_SYS}
  DEPENDS 20_day_harib17d_ipl
)
add_custom_target(20_day_harib17d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17d_IPLB} -i ${20_day_harib17d_OS}
  COMMAND mcopy -i ${20_day_harib17d_OS} ${20_day_harib17d_SYS} ::
  DEPENDS 20_day_harib17d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/asmhead_wine.img)

add_custom_target(20_day_harib17d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(20_day_harib17d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(20_day_harib17d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17d_asmhead_od 20_day_harib17d_asmhead)
add_dependencies(20_day_harib17d_asmhead_od 20_day_harib17d_asmhead_wine)

add_dependencies(images 20_day_harib17d_asmhead)
add_dependencies(wine 20_day_harib17d_asmhead_wine)
add_dependencies(od 20_day_harib17d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hlt.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17d/hlt.nas)
set(hlt.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/hlt.img)
set(hlt_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/hlt_wine.img)

add_custom_target(20_day_harib17d_hlt
  COMMAND ${root_BINARY_DIR}/src/opennask ${hlt.img_SRCS} ${hlt.img_OUTS}
)
set(hlt.img_HRB ${root_BINARY_DIR}/projects/20_day/harib17d/hlt.hrb)
add_custom_target(20_day_harib17d_hlt_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hlt.img_SRCS} ${hlt.img_HRB}
  COMMAND mcopy -i ${20_day_harib17d_OS} ${hlt.img_HRB} ::hlt.hrb
  DEPENDS 20_day_harib17d_img
)

add_dependencies(20_day_harib17d_run 20_day_harib17d_hlt_hrb)
add_custom_target(20_day_harib17d_hlt_wine
  COMMAND ${WINE} ${WINE_NASK} ${hlt.img_SRCS} ${hlt_wine.img_OUTS}
)
add_custom_target(20_day_harib17d_hlt_od
  COMMAND ${OD} -t x1 ${hlt.img_OUTS}      > ${hlt.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hlt_wine.img_OUTS} > ${hlt_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hlt.img_OUTS}_f.txt ${hlt_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17d_hlt_od 20_day_harib17d_hlt)
add_dependencies(20_day_harib17d_hlt_od 20_day_harib17d_hlt_wine)

add_dependencies(images 20_day_harib17d_hlt)
add_dependencies(wine 20_day_harib17d_hlt_wine)
add_dependencies(od 20_day_harib17d_hlt_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/ipl10_wine.img)

add_custom_target(20_day_harib17d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(20_day_harib17d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(20_day_harib17d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17d_ipl10_od 20_day_harib17d_ipl10)
add_dependencies(20_day_harib17d_ipl10_od 20_day_harib17d_ipl10_wine)

add_dependencies(images 20_day_harib17d_ipl10)
add_dependencies(wine 20_day_harib17d_ipl10_wine)
add_dependencies(od 20_day_harib17d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17d/naskfunc_wine.img)

add_custom_target(20_day_harib17d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(20_day_harib17d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(20_day_harib17d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17d_naskfunc_od 20_day_harib17d_naskfunc)
add_dependencies(20_day_harib17d_naskfunc_od 20_day_harib17d_naskfunc_wine)

add_dependencies(images 20_day_harib17d_naskfunc)
add_dependencies(wine 20_day_harib17d_naskfunc_wine)
add_dependencies(od 20_day_harib17d_naskfunc_od)

#----------------------------------------------------------
