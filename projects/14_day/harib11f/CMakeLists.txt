#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11f_OS    ${root_BINARY_DIR}/projects/14_day/harib11f/os.img)
set(14_day_harib11f_SYS	  ${root_BINARY_DIR}/projects/14_day/harib11f/os.sys)
set(14_day_harib11f_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11f/ipl.bin)
set(14_day_harib11f_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11f/ipl10.nas)
set(14_day_harib11f_HEADB ${root_BINARY_DIR}/projects/14_day/harib11f/asmhead.bin)
set(14_day_harib11f_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11f/asmhead.nas)
set(14_day_harib11f_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11f/boot.bin)
set(14_day_harib11f_CCO	  ${root_BINARY_DIR}/projects/14_day/harib11f/boot.o)
set(14_day_harib11f_CCS	  ${root_SOURCE_DIR}/projects/14_day/harib11f/*.c)
set(14_day_harib11f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11f_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11f/naskfunc.nas)
set(14_day_harib11f_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11f/naskfunc.o)
set(14_day_harib11f_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11f/hankaku.txt)
set(14_day_harib11f_FONTB ${root_BINARY_DIR}/projects/14_day/harib11f/hankaku.bin)
set(14_day_harib11f_FONTO ${root_BINARY_DIR}/projects/14_day/harib11f/hankaku.o)
set(14_day_harib11f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11f_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11f/libgo.o)
set(14_day_harib11f_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11f/*.o)


add_custom_target(14_day_harib11f_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11f_OS}
  DEPENDS 14_day_harib11f_img
)
add_custom_target(14_day_harib11f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 14_day_harib11f_img
)
add_custom_target(14_day_harib11f_clean
  COMMAND rm -f ${14_day_harib11f_OS}
  COMMAND rm -f ${14_day_harib11f_SYS}
  COMMAND rm -f ${14_day_harib11f_IPLB}
  COMMAND rm -f ${14_day_harib11f_HEADB}
  COMMAND rm -f ${14_day_harib11f_BOOTB}
  COMMAND rm -f ${14_day_harib11f_WILDOBJ}
)
add_custom_target(14_day_harib11f_ipl
  COMMAND ${NASK} ${14_day_harib11f_IPLS} ${14_day_harib11f_IPLB}
)
add_custom_target(14_day_harib11f_sys
  COMMAND ${NASK} ${14_day_harib11f_HEADS} ${14_day_harib11f_HEADB}
  COMMAND ${FONT} ${14_day_harib11f_FONTS} ${14_day_harib11f_FONTB}
  COMMAND ${B2O}  ${14_day_harib11f_FONTB} ${14_day_harib11f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11f_LIBGE} ${14_day_harib11f_LIBGC}
  COMMAND ${NASK} ${14_day_harib11f_FUNCS} ${14_day_harib11f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${14_day_harib11f_LDS} ${14_day_harib11f_CCS} ${14_day_harib11f_WILDOBJ} -o ${14_day_harib11f_BOOTB}
  COMMAND cat ${14_day_harib11f_HEADB} ${14_day_harib11f_BOOTB} > ${14_day_harib11f_SYS}
  DEPENDS 14_day_harib11f_ipl
)
add_custom_target(14_day_harib11f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11f_IPLB} -i ${14_day_harib11f_OS}
  COMMAND mcopy -i ${14_day_harib11f_OS} ${14_day_harib11f_SYS} ::
  DEPENDS 14_day_harib11f_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11f/asmhead_wine.img)

add_custom_target(14_day_harib11f_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(14_day_harib11f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(14_day_harib11f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11f_asmhead_od 14_day_harib11f_asmhead)
add_dependencies(14_day_harib11f_asmhead_od 14_day_harib11f_asmhead_wine)

add_dependencies(images 14_day_harib11f_asmhead)
add_dependencies(wine 14_day_harib11f_asmhead_wine)
add_dependencies(od 14_day_harib11f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11f/ipl10_wine.img)

add_custom_target(14_day_harib11f_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(14_day_harib11f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(14_day_harib11f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11f_ipl10_od 14_day_harib11f_ipl10)
add_dependencies(14_day_harib11f_ipl10_od 14_day_harib11f_ipl10_wine)

add_dependencies(images 14_day_harib11f_ipl10)
add_dependencies(wine 14_day_harib11f_ipl10_wine)
add_dependencies(od 14_day_harib11f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11f/naskfunc_wine.img)

add_custom_target(14_day_harib11f_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(14_day_harib11f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(14_day_harib11f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11f_naskfunc_od 14_day_harib11f_naskfunc)
add_dependencies(14_day_harib11f_naskfunc_od 14_day_harib11f_naskfunc_wine)

add_dependencies(images 14_day_harib11f_naskfunc)
add_dependencies(wine 14_day_harib11f_naskfunc_wine)
add_dependencies(od 14_day_harib11f_naskfunc_od)

#----------------------------------------------------------
