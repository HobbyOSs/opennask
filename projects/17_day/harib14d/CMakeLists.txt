#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14d_OS    ${root_BINARY_DIR}/projects/17_day/harib14d/os.img)
set(17_day_harib14d_SYS	  ${root_BINARY_DIR}/projects/17_day/harib14d/os.sys)
set(17_day_harib14d_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14d/ipl.bin)
set(17_day_harib14d_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14d/ipl10.nas)
set(17_day_harib14d_HEADB ${root_BINARY_DIR}/projects/17_day/harib14d/asmhead.bin)
set(17_day_harib14d_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14d/asmhead.nas)
set(17_day_harib14d_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14d/boot.bin)
set(17_day_harib14d_CCO	  ${root_BINARY_DIR}/projects/17_day/harib14d/boot.o)
set(17_day_harib14d_CCS	  ${root_SOURCE_DIR}/projects/17_day/harib14d/*.c)
set(17_day_harib14d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14d_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14d/naskfunc.nas)
set(17_day_harib14d_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14d/naskfunc.o)
set(17_day_harib14d_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14d/hankaku.txt)
set(17_day_harib14d_FONTB ${root_BINARY_DIR}/projects/17_day/harib14d/hankaku.bin)
set(17_day_harib14d_FONTO ${root_BINARY_DIR}/projects/17_day/harib14d/hankaku.o)
set(17_day_harib14d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14d_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14d/libgo.o)
set(17_day_harib14d_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14d/*.o)


add_custom_target(17_day_harib14d_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14d_OS}
  DEPENDS 17_day_harib14d_img
)
add_custom_target(17_day_harib14d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 17_day_harib14d_img
)
add_custom_target(17_day_harib14d_clean
  COMMAND rm -f ${17_day_harib14d_OS}
  COMMAND rm -f ${17_day_harib14d_SYS}
  COMMAND rm -f ${17_day_harib14d_IPLB}
  COMMAND rm -f ${17_day_harib14d_HEADB}
  COMMAND rm -f ${17_day_harib14d_BOOTB}
  COMMAND rm -f ${17_day_harib14d_WILDOBJ}
)
add_custom_target(17_day_harib14d_ipl
  COMMAND ${NASK} ${17_day_harib14d_IPLS} ${17_day_harib14d_IPLB}
)
add_custom_target(17_day_harib14d_sys
  COMMAND ${NASK} ${17_day_harib14d_HEADS} ${17_day_harib14d_HEADB}
  COMMAND ${FONT} ${17_day_harib14d_FONTS} ${17_day_harib14d_FONTB}
  COMMAND ${B2O}  ${17_day_harib14d_FONTB} ${17_day_harib14d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14d_LIBGE} ${17_day_harib14d_LIBGC}
  COMMAND ${NASK} ${17_day_harib14d_FUNCS} ${17_day_harib14d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${17_day_harib14d_LDS} ${17_day_harib14d_CCS} ${17_day_harib14d_WILDOBJ} -o ${17_day_harib14d_BOOTB}
  COMMAND cat ${17_day_harib14d_HEADB} ${17_day_harib14d_BOOTB} > ${17_day_harib14d_SYS}
  DEPENDS 17_day_harib14d_ipl
)
add_custom_target(17_day_harib14d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14d_IPLB} -i ${17_day_harib14d_OS}
  COMMAND mcopy -i ${17_day_harib14d_OS} ${17_day_harib14d_SYS} ::
  DEPENDS 17_day_harib14d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/asmhead_wine.img)

add_custom_target(17_day_harib14d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(17_day_harib14d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(17_day_harib14d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14d_asmhead_od 17_day_harib14d_asmhead)
add_dependencies(17_day_harib14d_asmhead_od 17_day_harib14d_asmhead_wine)

add_dependencies(images 17_day_harib14d_asmhead)
add_dependencies(wine 17_day_harib14d_asmhead_wine)
add_dependencies(od 17_day_harib14d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/ipl10_wine.img)

add_custom_target(17_day_harib14d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(17_day_harib14d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(17_day_harib14d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14d_ipl10_od 17_day_harib14d_ipl10)
add_dependencies(17_day_harib14d_ipl10_od 17_day_harib14d_ipl10_wine)

add_dependencies(images 17_day_harib14d_ipl10)
add_dependencies(wine 17_day_harib14d_ipl10_wine)
add_dependencies(od 17_day_harib14d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14d/naskfunc_wine.img)

add_custom_target(17_day_harib14d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(17_day_harib14d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(17_day_harib14d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14d_naskfunc_od 17_day_harib14d_naskfunc)
add_dependencies(17_day_harib14d_naskfunc_od 17_day_harib14d_naskfunc_wine)

add_dependencies(images 17_day_harib14d_naskfunc)
add_dependencies(wine 17_day_harib14d_naskfunc_wine)
add_dependencies(od 17_day_harib14d_naskfunc_od)

#----------------------------------------------------------
