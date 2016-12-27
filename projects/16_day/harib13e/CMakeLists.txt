#----------------------------------------------------------
message(STATUS "Entering directory projects/16_day/harib13e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(16_day_harib13e_OS    ${root_BINARY_DIR}/projects/16_day/harib13e/os.img)
set(16_day_harib13e_SYS	  ${root_BINARY_DIR}/projects/16_day/harib13e/os.sys)
set(16_day_harib13e_IPLB  ${root_BINARY_DIR}/projects/16_day/harib13e/ipl.bin)
set(16_day_harib13e_IPLS  ${root_SOURCE_DIR}/projects/16_day/harib13e/ipl10.nas)
set(16_day_harib13e_HEADB ${root_BINARY_DIR}/projects/16_day/harib13e/asmhead.bin)
set(16_day_harib13e_HEADS ${root_SOURCE_DIR}/projects/16_day/harib13e/asmhead.nas)
set(16_day_harib13e_BOOTB ${root_BINARY_DIR}/projects/16_day/harib13e/boot.bin)
set(16_day_harib13e_CCO	  ${root_BINARY_DIR}/projects/16_day/harib13e/boot.o)
set(16_day_harib13e_CCS	  ${root_SOURCE_DIR}/projects/16_day/harib13e/*.c)
set(16_day_harib13e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(16_day_harib13e_FUNCS ${root_SOURCE_DIR}/projects/16_day/harib13e/naskfunc.nas)
set(16_day_harib13e_FUNCO ${root_BINARY_DIR}/projects/16_day/harib13e/naskfunc.o)
set(16_day_harib13e_FONTS ${root_SOURCE_DIR}/projects/16_day/harib13e/hankaku.txt)
set(16_day_harib13e_FONTB ${root_BINARY_DIR}/projects/16_day/harib13e/hankaku.bin)
set(16_day_harib13e_FONTO ${root_BINARY_DIR}/projects/16_day/harib13e/hankaku.o)
set(16_day_harib13e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(16_day_harib13e_LIBGC ${root_BINARY_DIR}/projects/16_day/harib13e/libgo.o)
set(16_day_harib13e_WILDOBJ ${root_BINARY_DIR}/projects/16_day/harib13e/*.o)


add_custom_target(16_day_harib13e_run
  COMMAND ${QEMU} ${QEMUOPT} ${16_day_harib13e_OS}
  DEPENDS 16_day_harib13e_img
)
add_custom_target(16_day_harib13e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${16_day_harib13e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 16_day_harib13e_img
)
add_custom_target(16_day_harib13e_clean
  COMMAND rm -f ${16_day_harib13e_OS}
  COMMAND rm -f ${16_day_harib13e_SYS}
  COMMAND rm -f ${16_day_harib13e_IPLB}
  COMMAND rm -f ${16_day_harib13e_HEADB}
  COMMAND rm -f ${16_day_harib13e_BOOTB}
  COMMAND rm -f ${16_day_harib13e_WILDOBJ}
)
add_custom_target(16_day_harib13e_ipl
  COMMAND ${NASK} ${16_day_harib13e_IPLS} ${16_day_harib13e_IPLB}
)
add_custom_target(16_day_harib13e_sys
  COMMAND ${NASK} ${16_day_harib13e_HEADS} ${16_day_harib13e_HEADB}
  COMMAND ${FONT} ${16_day_harib13e_FONTS} ${16_day_harib13e_FONTB}
  COMMAND ${B2O}  ${16_day_harib13e_FONTB} ${16_day_harib13e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${16_day_harib13e_LIBGE} ${16_day_harib13e_LIBGC}
  COMMAND ${NASK} ${16_day_harib13e_FUNCS} ${16_day_harib13e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${16_day_harib13e_LDS} ${16_day_harib13e_CCS} ${16_day_harib13e_WILDOBJ} -o ${16_day_harib13e_BOOTB}
  COMMAND cat ${16_day_harib13e_HEADB} ${16_day_harib13e_BOOTB} > ${16_day_harib13e_SYS}
  DEPENDS 16_day_harib13e_ipl
)
add_custom_target(16_day_harib13e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${16_day_harib13e_IPLB} -i ${16_day_harib13e_OS}
  COMMAND mcopy -i ${16_day_harib13e_OS} ${16_day_harib13e_SYS} ::
  DEPENDS 16_day_harib13e_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/asmhead_wine.img)

add_custom_target(16_day_harib13e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(16_day_harib13e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(16_day_harib13e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(16_day_harib13e_asmhead_od 16_day_harib13e_asmhead)
add_dependencies(16_day_harib13e_asmhead_od 16_day_harib13e_asmhead_wine)

add_dependencies(images 16_day_harib13e_asmhead)
add_dependencies(wine 16_day_harib13e_asmhead_wine)
add_dependencies(od 16_day_harib13e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/ipl10_wine.img)

add_custom_target(16_day_harib13e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(16_day_harib13e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(16_day_harib13e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(16_day_harib13e_ipl10_od 16_day_harib13e_ipl10)
add_dependencies(16_day_harib13e_ipl10_od 16_day_harib13e_ipl10_wine)

add_dependencies(images 16_day_harib13e_ipl10)
add_dependencies(wine 16_day_harib13e_ipl10_wine)
add_dependencies(od 16_day_harib13e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13e/naskfunc_wine.img)

add_custom_target(16_day_harib13e_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(16_day_harib13e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(16_day_harib13e_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(16_day_harib13e_naskfunc_od 16_day_harib13e_naskfunc)
add_dependencies(16_day_harib13e_naskfunc_od 16_day_harib13e_naskfunc_wine)

add_dependencies(images 16_day_harib13e_naskfunc)
add_dependencies(wine 16_day_harib13e_naskfunc_wine)
add_dependencies(od 16_day_harib13e_naskfunc_od)

#----------------------------------------------------------
