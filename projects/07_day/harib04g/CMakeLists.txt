#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04g/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(07_day_harib04g_OS    ${root_BINARY_DIR}/projects/07_day/harib04g/os.img)
set(07_day_harib04g_SYS	  ${root_BINARY_DIR}/projects/07_day/harib04g/os.sys)
set(07_day_harib04g_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04g/ipl.bin)
set(07_day_harib04g_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04g/ipl10.nas)
set(07_day_harib04g_HEADB ${root_BINARY_DIR}/projects/07_day/harib04g/asmhead.bin)
set(07_day_harib04g_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04g/asmhead.nas)
set(07_day_harib04g_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04g/boot.bin)
set(07_day_harib04g_CCO	  ${root_BINARY_DIR}/projects/07_day/harib04g/boot.o)
set(07_day_harib04g_CCS	  ${root_SOURCE_DIR}/projects/07_day/harib04g/*.c)
set(07_day_harib04g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04g_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04g/naskfunc.nas)
set(07_day_harib04g_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04g/naskfunc.o)
set(07_day_harib04g_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04g/hankaku.txt)
set(07_day_harib04g_FONTB ${root_BINARY_DIR}/projects/07_day/harib04g/hankaku.bin)
set(07_day_harib04g_FONTO ${root_BINARY_DIR}/projects/07_day/harib04g/hankaku.o)
set(07_day_harib04g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(07_day_harib04g_LIBGC ${root_BINARY_DIR}/projects/07_day/harib04g/libgo.o)
set(07_day_harib04g_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04g/*.o)


add_custom_target(07_day_harib04g_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04g_OS}
  DEPENDS 07_day_harib04g_img
)
add_custom_target(07_day_harib04g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 07_day_harib04g_img
)
add_custom_target(07_day_harib04g_clean
  COMMAND rm -f ${07_day_harib04g_OS}
  COMMAND rm -f ${07_day_harib04g_SYS}
  COMMAND rm -f ${07_day_harib04g_IPLB}
  COMMAND rm -f ${07_day_harib04g_HEADB}
  COMMAND rm -f ${07_day_harib04g_BOOTB}
  COMMAND rm -f ${07_day_harib04g_WILDOBJ}
)
add_custom_target(07_day_harib04g_ipl
  COMMAND ${NASK} ${07_day_harib04g_IPLS} ${07_day_harib04g_IPLB}
)
add_custom_target(07_day_harib04g_sys
  COMMAND ${NASK} ${07_day_harib04g_HEADS} ${07_day_harib04g_HEADB}
  COMMAND ${FONT} ${07_day_harib04g_FONTS} ${07_day_harib04g_FONTB}
  COMMAND ${B2O}  ${07_day_harib04g_FONTB} ${07_day_harib04g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${07_day_harib04g_LIBGE} ${07_day_harib04g_LIBGC}
  COMMAND ${NASK} ${07_day_harib04g_FUNCS} ${07_day_harib04g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04g_LDS} ${07_day_harib04g_CCS} ${07_day_harib04g_WILDOBJ} -o ${07_day_harib04g_BOOTB}
  COMMAND cat ${07_day_harib04g_HEADB} ${07_day_harib04g_BOOTB} > ${07_day_harib04g_SYS}
  DEPENDS 07_day_harib04g_ipl
)
add_custom_target(07_day_harib04g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04g_IPLB} -i ${07_day_harib04g_OS}
  COMMAND mcopy -i ${07_day_harib04g_OS} ${07_day_harib04g_SYS} ::
  DEPENDS 07_day_harib04g_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04g/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/asmhead_wine.img)

add_custom_target(07_day_harib04g_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(07_day_harib04g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(07_day_harib04g_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04g_asmhead_od 07_day_harib04g_asmhead)
add_dependencies(07_day_harib04g_asmhead_od 07_day_harib04g_asmhead_wine)

add_dependencies(images 07_day_harib04g_asmhead)
add_dependencies(wine 07_day_harib04g_asmhead_wine)
add_dependencies(od 07_day_harib04g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04g/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/ipl10_wine.img)

add_custom_target(07_day_harib04g_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(07_day_harib04g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(07_day_harib04g_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04g_ipl10_od 07_day_harib04g_ipl10)
add_dependencies(07_day_harib04g_ipl10_od 07_day_harib04g_ipl10_wine)

add_dependencies(images 07_day_harib04g_ipl10)
add_dependencies(wine 07_day_harib04g_ipl10_wine)
add_dependencies(od 07_day_harib04g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04g/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04g/naskfunc_wine.img)

add_custom_target(07_day_harib04g_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(07_day_harib04g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(07_day_harib04g_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04g_naskfunc_od 07_day_harib04g_naskfunc)
add_dependencies(07_day_harib04g_naskfunc_od 07_day_harib04g_naskfunc_wine)

add_dependencies(images 07_day_harib04g_naskfunc)
add_dependencies(wine 07_day_harib04g_naskfunc_wine)
add_dependencies(od 07_day_harib04g_naskfunc_od)

#----------------------------------------------------------
