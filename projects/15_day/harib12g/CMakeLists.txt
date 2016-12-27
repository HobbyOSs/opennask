#----------------------------------------------------------
message(STATUS "Entering directory projects/15_day/harib12g/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(15_day_harib12g_OS    ${root_BINARY_DIR}/projects/15_day/harib12g/os.img)
set(15_day_harib12g_SYS	  ${root_BINARY_DIR}/projects/15_day/harib12g/os.sys)
set(15_day_harib12g_IPLB  ${root_BINARY_DIR}/projects/15_day/harib12g/ipl.bin)
set(15_day_harib12g_IPLS  ${root_SOURCE_DIR}/projects/15_day/harib12g/ipl10.nas)
set(15_day_harib12g_HEADB ${root_BINARY_DIR}/projects/15_day/harib12g/asmhead.bin)
set(15_day_harib12g_HEADS ${root_SOURCE_DIR}/projects/15_day/harib12g/asmhead.nas)
set(15_day_harib12g_BOOTB ${root_BINARY_DIR}/projects/15_day/harib12g/boot.bin)
set(15_day_harib12g_CCO	  ${root_BINARY_DIR}/projects/15_day/harib12g/boot.o)
set(15_day_harib12g_CCS	  ${root_SOURCE_DIR}/projects/15_day/harib12g/*.c)
set(15_day_harib12g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(15_day_harib12g_FUNCS ${root_SOURCE_DIR}/projects/15_day/harib12g/naskfunc.nas)
set(15_day_harib12g_FUNCO ${root_BINARY_DIR}/projects/15_day/harib12g/naskfunc.o)
set(15_day_harib12g_FONTS ${root_SOURCE_DIR}/projects/15_day/harib12g/hankaku.txt)
set(15_day_harib12g_FONTB ${root_BINARY_DIR}/projects/15_day/harib12g/hankaku.bin)
set(15_day_harib12g_FONTO ${root_BINARY_DIR}/projects/15_day/harib12g/hankaku.o)
set(15_day_harib12g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(15_day_harib12g_LIBGC ${root_BINARY_DIR}/projects/15_day/harib12g/libgo.o)
set(15_day_harib12g_WILDOBJ ${root_BINARY_DIR}/projects/15_day/harib12g/*.o)


add_custom_target(15_day_harib12g_run
  COMMAND ${QEMU} ${QEMUOPT} ${15_day_harib12g_OS}
  DEPENDS 15_day_harib12g_img
)
add_custom_target(15_day_harib12g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${15_day_harib12g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 15_day_harib12g_img
)
add_custom_target(15_day_harib12g_clean
  COMMAND rm -f ${15_day_harib12g_OS}
  COMMAND rm -f ${15_day_harib12g_SYS}
  COMMAND rm -f ${15_day_harib12g_IPLB}
  COMMAND rm -f ${15_day_harib12g_HEADB}
  COMMAND rm -f ${15_day_harib12g_BOOTB}
  COMMAND rm -f ${15_day_harib12g_WILDOBJ}
)
add_custom_target(15_day_harib12g_ipl
  COMMAND ${NASK} ${15_day_harib12g_IPLS} ${15_day_harib12g_IPLB}
)
add_custom_target(15_day_harib12g_sys
  COMMAND ${NASK} ${15_day_harib12g_HEADS} ${15_day_harib12g_HEADB}
  COMMAND ${FONT} ${15_day_harib12g_FONTS} ${15_day_harib12g_FONTB}
  COMMAND ${B2O}  ${15_day_harib12g_FONTB} ${15_day_harib12g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${15_day_harib12g_LIBGE} ${15_day_harib12g_LIBGC}
  COMMAND ${NASK} ${15_day_harib12g_FUNCS} ${15_day_harib12g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${15_day_harib12g_LDS} ${15_day_harib12g_CCS} ${15_day_harib12g_WILDOBJ} -o ${15_day_harib12g_BOOTB}
  COMMAND cat ${15_day_harib12g_HEADB} ${15_day_harib12g_BOOTB} > ${15_day_harib12g_SYS}
  DEPENDS 15_day_harib12g_ipl
)
add_custom_target(15_day_harib12g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${15_day_harib12g_IPLB} -i ${15_day_harib12g_OS}
  COMMAND mcopy -i ${15_day_harib12g_OS} ${15_day_harib12g_SYS} ::
  DEPENDS 15_day_harib12g_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12g/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/asmhead_wine.img)

add_custom_target(15_day_harib12g_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(15_day_harib12g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(15_day_harib12g_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12g_asmhead_od 15_day_harib12g_asmhead)
add_dependencies(15_day_harib12g_asmhead_od 15_day_harib12g_asmhead_wine)

add_dependencies(images 15_day_harib12g_asmhead)
add_dependencies(wine 15_day_harib12g_asmhead_wine)
add_dependencies(od 15_day_harib12g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12g/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/ipl10_wine.img)

add_custom_target(15_day_harib12g_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(15_day_harib12g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(15_day_harib12g_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12g_ipl10_od 15_day_harib12g_ipl10)
add_dependencies(15_day_harib12g_ipl10_od 15_day_harib12g_ipl10_wine)

add_dependencies(images 15_day_harib12g_ipl10)
add_dependencies(wine 15_day_harib12g_ipl10_wine)
add_dependencies(od 15_day_harib12g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12g/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12g/naskfunc_wine.img)

add_custom_target(15_day_harib12g_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(15_day_harib12g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(15_day_harib12g_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12g_naskfunc_od 15_day_harib12g_naskfunc)
add_dependencies(15_day_harib12g_naskfunc_od 15_day_harib12g_naskfunc_wine)

add_dependencies(images 15_day_harib12g_naskfunc)
add_dependencies(wine 15_day_harib12g_naskfunc_wine)
add_dependencies(od 15_day_harib12g_naskfunc_od)

#----------------------------------------------------------
