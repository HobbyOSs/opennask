#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14c_OS    ${root_BINARY_DIR}/projects/17_day/harib14c/os.img)
set(17_day_harib14c_SYS	  ${root_BINARY_DIR}/projects/17_day/harib14c/os.sys)
set(17_day_harib14c_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14c/ipl.bin)
set(17_day_harib14c_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14c/ipl10.nas)
set(17_day_harib14c_HEADB ${root_BINARY_DIR}/projects/17_day/harib14c/asmhead.bin)
set(17_day_harib14c_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14c/asmhead.nas)
set(17_day_harib14c_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14c/boot.bin)
set(17_day_harib14c_CCO	  ${root_BINARY_DIR}/projects/17_day/harib14c/boot.o)
set(17_day_harib14c_CCS	  ${root_SOURCE_DIR}/projects/17_day/harib14c/*.c)
set(17_day_harib14c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14c_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14c/naskfunc.nas)
set(17_day_harib14c_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14c/naskfunc.o)
set(17_day_harib14c_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14c/hankaku.txt)
set(17_day_harib14c_FONTB ${root_BINARY_DIR}/projects/17_day/harib14c/hankaku.bin)
set(17_day_harib14c_FONTO ${root_BINARY_DIR}/projects/17_day/harib14c/hankaku.o)
set(17_day_harib14c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14c_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14c/libgo.o)
set(17_day_harib14c_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14c/*.o)


add_custom_target(17_day_harib14c_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14c_OS}
  DEPENDS 17_day_harib14c_img
)
add_custom_target(17_day_harib14c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 17_day_harib14c_img
)
add_custom_target(17_day_harib14c_clean
  COMMAND rm -f ${17_day_harib14c_OS}
  COMMAND rm -f ${17_day_harib14c_SYS}
  COMMAND rm -f ${17_day_harib14c_IPLB}
  COMMAND rm -f ${17_day_harib14c_HEADB}
  COMMAND rm -f ${17_day_harib14c_BOOTB}
  COMMAND rm -f ${17_day_harib14c_WILDOBJ}
)
add_custom_target(17_day_harib14c_ipl
  COMMAND ${NASK} ${17_day_harib14c_IPLS} ${17_day_harib14c_IPLB}
)
add_custom_target(17_day_harib14c_sys
  COMMAND ${NASK} ${17_day_harib14c_HEADS} ${17_day_harib14c_HEADB}
  COMMAND ${FONT} ${17_day_harib14c_FONTS} ${17_day_harib14c_FONTB}
  COMMAND ${B2O}  ${17_day_harib14c_FONTB} ${17_day_harib14c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14c_LIBGE} ${17_day_harib14c_LIBGC}
  COMMAND ${NASK} ${17_day_harib14c_FUNCS} ${17_day_harib14c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${17_day_harib14c_LDS} ${17_day_harib14c_CCS} ${17_day_harib14c_WILDOBJ} -o ${17_day_harib14c_BOOTB}
  COMMAND cat ${17_day_harib14c_HEADB} ${17_day_harib14c_BOOTB} > ${17_day_harib14c_SYS}
  DEPENDS 17_day_harib14c_ipl
)
add_custom_target(17_day_harib14c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14c_IPLB} -i ${17_day_harib14c_OS}
  COMMAND mcopy -i ${17_day_harib14c_OS} ${17_day_harib14c_SYS} ::
  DEPENDS 17_day_harib14c_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/asmhead_wine.img)

add_custom_target(17_day_harib14c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(17_day_harib14c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(17_day_harib14c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14c_asmhead_od 17_day_harib14c_asmhead)
add_dependencies(17_day_harib14c_asmhead_od 17_day_harib14c_asmhead_wine)

add_dependencies(images 17_day_harib14c_asmhead)
add_dependencies(wine 17_day_harib14c_asmhead_wine)
add_dependencies(od 17_day_harib14c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/ipl10_wine.img)

add_custom_target(17_day_harib14c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(17_day_harib14c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(17_day_harib14c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14c_ipl10_od 17_day_harib14c_ipl10)
add_dependencies(17_day_harib14c_ipl10_od 17_day_harib14c_ipl10_wine)

add_dependencies(images 17_day_harib14c_ipl10)
add_dependencies(wine 17_day_harib14c_ipl10_wine)
add_dependencies(od 17_day_harib14c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14c/naskfunc_wine.img)

add_custom_target(17_day_harib14c_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(17_day_harib14c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(17_day_harib14c_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14c_naskfunc_od 17_day_harib14c_naskfunc)
add_dependencies(17_day_harib14c_naskfunc_od 17_day_harib14c_naskfunc_wine)

add_dependencies(images 17_day_harib14c_naskfunc)
add_dependencies(wine 17_day_harib14c_naskfunc_wine)
add_dependencies(od 17_day_harib14c_naskfunc_od)

#----------------------------------------------------------
