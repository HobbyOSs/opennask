#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10e_OS    ${root_BINARY_DIR}/projects/13_day/harib10e/os.img)
set(13_day_harib10e_SYS	  ${root_BINARY_DIR}/projects/13_day/harib10e/os.sys)
set(13_day_harib10e_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10e/ipl.bin)
set(13_day_harib10e_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10e/ipl10.nas)
set(13_day_harib10e_HEADB ${root_BINARY_DIR}/projects/13_day/harib10e/asmhead.bin)
set(13_day_harib10e_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10e/asmhead.nas)
set(13_day_harib10e_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10e/boot.bin)
set(13_day_harib10e_CCO	  ${root_BINARY_DIR}/projects/13_day/harib10e/boot.o)
set(13_day_harib10e_CCS	  ${root_SOURCE_DIR}/projects/13_day/harib10e/*.c)
set(13_day_harib10e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10e_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10e/naskfunc.nas)
set(13_day_harib10e_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10e/naskfunc.o)
set(13_day_harib10e_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10e/hankaku.txt)
set(13_day_harib10e_FONTB ${root_BINARY_DIR}/projects/13_day/harib10e/hankaku.bin)
set(13_day_harib10e_FONTO ${root_BINARY_DIR}/projects/13_day/harib10e/hankaku.o)
set(13_day_harib10e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10e_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10e/libgo.o)
set(13_day_harib10e_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10e/*.o)


add_custom_target(13_day_harib10e_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10e_OS}
  DEPENDS 13_day_harib10e_img
)
add_custom_target(13_day_harib10e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10e_img
)
add_custom_target(13_day_harib10e_clean
  COMMAND rm -f ${13_day_harib10e_OS}
  COMMAND rm -f ${13_day_harib10e_SYS}
  COMMAND rm -f ${13_day_harib10e_IPLB}
  COMMAND rm -f ${13_day_harib10e_HEADB}
  COMMAND rm -f ${13_day_harib10e_BOOTB}
  COMMAND rm -f ${13_day_harib10e_WILDOBJ}
)
add_custom_target(13_day_harib10e_ipl
  COMMAND ${NASK} ${13_day_harib10e_IPLS} ${13_day_harib10e_IPLB}
)
add_custom_target(13_day_harib10e_sys
  COMMAND ${NASK} ${13_day_harib10e_HEADS} ${13_day_harib10e_HEADB}
  COMMAND ${FONT} ${13_day_harib10e_FONTS} ${13_day_harib10e_FONTB}
  COMMAND ${B2O}  ${13_day_harib10e_FONTB} ${13_day_harib10e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10e_LIBGE} ${13_day_harib10e_LIBGC}
  COMMAND ${NASK} ${13_day_harib10e_FUNCS} ${13_day_harib10e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${13_day_harib10e_LDS} ${13_day_harib10e_CCS} ${13_day_harib10e_WILDOBJ} -o ${13_day_harib10e_BOOTB}
  COMMAND cat ${13_day_harib10e_HEADB} ${13_day_harib10e_BOOTB} > ${13_day_harib10e_SYS}
  DEPENDS 13_day_harib10e_ipl
)
add_custom_target(13_day_harib10e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10e_IPLB} -i ${13_day_harib10e_OS}
  COMMAND mcopy -i ${13_day_harib10e_OS} ${13_day_harib10e_SYS} ::
  DEPENDS 13_day_harib10e_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/asmhead_wine.img)

add_custom_target(13_day_harib10e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(13_day_harib10e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(13_day_harib10e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10e_asmhead_od 13_day_harib10e_asmhead)
add_dependencies(13_day_harib10e_asmhead_od 13_day_harib10e_asmhead_wine)

add_dependencies(images 13_day_harib10e_asmhead)
add_dependencies(wine 13_day_harib10e_asmhead_wine)
add_dependencies(od 13_day_harib10e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/ipl10_wine.img)

add_custom_target(13_day_harib10e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(13_day_harib10e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(13_day_harib10e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10e_ipl10_od 13_day_harib10e_ipl10)
add_dependencies(13_day_harib10e_ipl10_od 13_day_harib10e_ipl10_wine)

add_dependencies(images 13_day_harib10e_ipl10)
add_dependencies(wine 13_day_harib10e_ipl10_wine)
add_dependencies(od 13_day_harib10e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10e/naskfunc_wine.img)

add_custom_target(13_day_harib10e_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(13_day_harib10e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(13_day_harib10e_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10e_naskfunc_od 13_day_harib10e_naskfunc)
add_dependencies(13_day_harib10e_naskfunc_od 13_day_harib10e_naskfunc_wine)

add_dependencies(images 13_day_harib10e_naskfunc)
add_dependencies(wine 13_day_harib10e_naskfunc_wine)
add_dependencies(od 13_day_harib10e_naskfunc_od)

#----------------------------------------------------------
