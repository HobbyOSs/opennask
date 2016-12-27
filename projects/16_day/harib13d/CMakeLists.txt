#----------------------------------------------------------
message(STATUS "Entering directory projects/16_day/harib13d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(16_day_harib13d_OS    ${root_BINARY_DIR}/projects/16_day/harib13d/os.img)
set(16_day_harib13d_SYS	  ${root_BINARY_DIR}/projects/16_day/harib13d/os.sys)
set(16_day_harib13d_IPLB  ${root_BINARY_DIR}/projects/16_day/harib13d/ipl.bin)
set(16_day_harib13d_IPLS  ${root_SOURCE_DIR}/projects/16_day/harib13d/ipl10.nas)
set(16_day_harib13d_HEADB ${root_BINARY_DIR}/projects/16_day/harib13d/asmhead.bin)
set(16_day_harib13d_HEADS ${root_SOURCE_DIR}/projects/16_day/harib13d/asmhead.nas)
set(16_day_harib13d_BOOTB ${root_BINARY_DIR}/projects/16_day/harib13d/boot.bin)
set(16_day_harib13d_CCO	  ${root_BINARY_DIR}/projects/16_day/harib13d/boot.o)
set(16_day_harib13d_CCS	  ${root_SOURCE_DIR}/projects/16_day/harib13d/*.c)
set(16_day_harib13d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(16_day_harib13d_FUNCS ${root_SOURCE_DIR}/projects/16_day/harib13d/naskfunc.nas)
set(16_day_harib13d_FUNCO ${root_BINARY_DIR}/projects/16_day/harib13d/naskfunc.o)
set(16_day_harib13d_FONTS ${root_SOURCE_DIR}/projects/16_day/harib13d/hankaku.txt)
set(16_day_harib13d_FONTB ${root_BINARY_DIR}/projects/16_day/harib13d/hankaku.bin)
set(16_day_harib13d_FONTO ${root_BINARY_DIR}/projects/16_day/harib13d/hankaku.o)
set(16_day_harib13d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(16_day_harib13d_LIBGC ${root_BINARY_DIR}/projects/16_day/harib13d/libgo.o)
set(16_day_harib13d_WILDOBJ ${root_BINARY_DIR}/projects/16_day/harib13d/*.o)


add_custom_target(16_day_harib13d_run
  COMMAND ${QEMU} ${QEMUOPT} ${16_day_harib13d_OS}
  DEPENDS 16_day_harib13d_img
)
add_custom_target(16_day_harib13d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${16_day_harib13d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 16_day_harib13d_img
)
add_custom_target(16_day_harib13d_clean
  COMMAND rm -f ${16_day_harib13d_OS}
  COMMAND rm -f ${16_day_harib13d_SYS}
  COMMAND rm -f ${16_day_harib13d_IPLB}
  COMMAND rm -f ${16_day_harib13d_HEADB}
  COMMAND rm -f ${16_day_harib13d_BOOTB}
  COMMAND rm -f ${16_day_harib13d_WILDOBJ}
)
add_custom_target(16_day_harib13d_ipl
  COMMAND ${NASK} ${16_day_harib13d_IPLS} ${16_day_harib13d_IPLB}
)
add_custom_target(16_day_harib13d_sys
  COMMAND ${NASK} ${16_day_harib13d_HEADS} ${16_day_harib13d_HEADB}
  COMMAND ${FONT} ${16_day_harib13d_FONTS} ${16_day_harib13d_FONTB}
  COMMAND ${B2O}  ${16_day_harib13d_FONTB} ${16_day_harib13d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${16_day_harib13d_LIBGE} ${16_day_harib13d_LIBGC}
  COMMAND ${NASK} ${16_day_harib13d_FUNCS} ${16_day_harib13d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${16_day_harib13d_LDS} ${16_day_harib13d_CCS} ${16_day_harib13d_WILDOBJ} -o ${16_day_harib13d_BOOTB}
  COMMAND cat ${16_day_harib13d_HEADB} ${16_day_harib13d_BOOTB} > ${16_day_harib13d_SYS}
  DEPENDS 16_day_harib13d_ipl
)
add_custom_target(16_day_harib13d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${16_day_harib13d_IPLB} -i ${16_day_harib13d_OS}
  COMMAND mcopy -i ${16_day_harib13d_OS} ${16_day_harib13d_SYS} ::
  DEPENDS 16_day_harib13d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13d/asmhead_wine.img)

add_custom_target(16_day_harib13d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(16_day_harib13d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(16_day_harib13d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(16_day_harib13d_asmhead_od 16_day_harib13d_asmhead)
add_dependencies(16_day_harib13d_asmhead_od 16_day_harib13d_asmhead_wine)

add_dependencies(images 16_day_harib13d_asmhead)
add_dependencies(wine 16_day_harib13d_asmhead_wine)
add_dependencies(od 16_day_harib13d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13d/ipl10_wine.img)

add_custom_target(16_day_harib13d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(16_day_harib13d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(16_day_harib13d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(16_day_harib13d_ipl10_od 16_day_harib13d_ipl10)
add_dependencies(16_day_harib13d_ipl10_od 16_day_harib13d_ipl10_wine)

add_dependencies(images 16_day_harib13d_ipl10)
add_dependencies(wine 16_day_harib13d_ipl10_wine)
add_dependencies(od 16_day_harib13d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/16_day/harib13d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/16_day/harib13d/naskfunc_wine.img)

add_custom_target(16_day_harib13d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(16_day_harib13d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(16_day_harib13d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(16_day_harib13d_naskfunc_od 16_day_harib13d_naskfunc)
add_dependencies(16_day_harib13d_naskfunc_od 16_day_harib13d_naskfunc_wine)

add_dependencies(images 16_day_harib13d_naskfunc)
add_dependencies(wine 16_day_harib13d_naskfunc_wine)
add_dependencies(od 16_day_harib13d_naskfunc_od)

#----------------------------------------------------------
