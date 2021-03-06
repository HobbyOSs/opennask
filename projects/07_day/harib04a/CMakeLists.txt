#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(07_day_harib04a_OS    ${root_BINARY_DIR}/projects/07_day/harib04a/os.img)
set(07_day_harib04a_SYS	  ${root_BINARY_DIR}/projects/07_day/harib04a/os.sys)
set(07_day_harib04a_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04a/ipl.bin)
set(07_day_harib04a_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04a/ipl10.nas)
set(07_day_harib04a_HEADB ${root_BINARY_DIR}/projects/07_day/harib04a/asmhead.bin)
set(07_day_harib04a_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04a/asmhead.nas)
set(07_day_harib04a_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04a/boot.bin)
set(07_day_harib04a_CCO	  ${root_BINARY_DIR}/projects/07_day/harib04a/boot.o)
set(07_day_harib04a_CCS	  ${root_SOURCE_DIR}/projects/07_day/harib04a/*.c)
set(07_day_harib04a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04a_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04a/naskfunc.nas)
set(07_day_harib04a_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04a/naskfunc.o)
set(07_day_harib04a_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04a/hankaku.txt)
set(07_day_harib04a_FONTB ${root_BINARY_DIR}/projects/07_day/harib04a/hankaku.bin)
set(07_day_harib04a_FONTO ${root_BINARY_DIR}/projects/07_day/harib04a/hankaku.o)
set(07_day_harib04a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(07_day_harib04a_LIBGC ${root_BINARY_DIR}/projects/07_day/harib04a/libgo.o)
set(07_day_harib04a_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04a/*.o)


add_custom_target(07_day_harib04a_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04a_OS}
  DEPENDS 07_day_harib04a_img
)
add_custom_target(07_day_harib04a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 07_day_harib04a_img
)
add_custom_target(07_day_harib04a_clean
  COMMAND rm -f ${07_day_harib04a_OS}
  COMMAND rm -f ${07_day_harib04a_SYS}
  COMMAND rm -f ${07_day_harib04a_IPLB}
  COMMAND rm -f ${07_day_harib04a_HEADB}
  COMMAND rm -f ${07_day_harib04a_BOOTB}
  COMMAND rm -f ${07_day_harib04a_WILDOBJ}
)
add_custom_target(07_day_harib04a_ipl
  COMMAND ${NASK} ${07_day_harib04a_IPLS} ${07_day_harib04a_IPLB}
)
add_custom_target(07_day_harib04a_sys
  COMMAND ${NASK} ${07_day_harib04a_HEADS} ${07_day_harib04a_HEADB}
  COMMAND ${FONT} ${07_day_harib04a_FONTS} ${07_day_harib04a_FONTB}
  COMMAND ${B2O}  ${07_day_harib04a_FONTB} ${07_day_harib04a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${07_day_harib04a_LIBGE} ${07_day_harib04a_LIBGC}
  COMMAND ${NASK} ${07_day_harib04a_FUNCS} ${07_day_harib04a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04a_LDS} ${07_day_harib04a_CCS} ${07_day_harib04a_WILDOBJ} -o ${07_day_harib04a_BOOTB}
  COMMAND cat ${07_day_harib04a_HEADB} ${07_day_harib04a_BOOTB} > ${07_day_harib04a_SYS}
  DEPENDS 07_day_harib04a_ipl
)
add_custom_target(07_day_harib04a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04a_IPLB} -i ${07_day_harib04a_OS}
  COMMAND mcopy -i ${07_day_harib04a_OS} ${07_day_harib04a_SYS} ::
  DEPENDS 07_day_harib04a_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/asmhead_wine.img)

add_custom_target(07_day_harib04a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(07_day_harib04a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(07_day_harib04a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04a_asmhead_od 07_day_harib04a_asmhead)
add_dependencies(07_day_harib04a_asmhead_od 07_day_harib04a_asmhead_wine)

add_dependencies(images 07_day_harib04a_asmhead)
add_dependencies(wine 07_day_harib04a_asmhead_wine)
add_dependencies(od 07_day_harib04a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/ipl10_wine.img)

add_custom_target(07_day_harib04a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(07_day_harib04a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(07_day_harib04a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04a_ipl10_od 07_day_harib04a_ipl10)
add_dependencies(07_day_harib04a_ipl10_od 07_day_harib04a_ipl10_wine)

add_dependencies(images 07_day_harib04a_ipl10)
add_dependencies(wine 07_day_harib04a_ipl10_wine)
add_dependencies(od 07_day_harib04a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04a/naskfunc_wine.img)

add_custom_target(07_day_harib04a_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(07_day_harib04a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(07_day_harib04a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04a_naskfunc_od 07_day_harib04a_naskfunc)
add_dependencies(07_day_harib04a_naskfunc_od 07_day_harib04a_naskfunc_wine)

add_dependencies(images 07_day_harib04a_naskfunc)
add_dependencies(wine 07_day_harib04a_naskfunc_wine)
add_dependencies(od 07_day_harib04a_naskfunc_od)

#----------------------------------------------------------
