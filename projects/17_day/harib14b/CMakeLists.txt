#----------------------------------------------------------
message(STATUS "Entering directory projects/17_day/harib14b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(17_day_harib14b_OS    ${root_BINARY_DIR}/projects/17_day/harib14b/os.img)
set(17_day_harib14b_SYS	  ${root_BINARY_DIR}/projects/17_day/harib14b/os.sys)
set(17_day_harib14b_IPLB  ${root_BINARY_DIR}/projects/17_day/harib14b/ipl.bin)
set(17_day_harib14b_IPLS  ${root_SOURCE_DIR}/projects/17_day/harib14b/ipl10.nas)
set(17_day_harib14b_HEADB ${root_BINARY_DIR}/projects/17_day/harib14b/asmhead.bin)
set(17_day_harib14b_HEADS ${root_SOURCE_DIR}/projects/17_day/harib14b/asmhead.nas)
set(17_day_harib14b_BOOTB ${root_BINARY_DIR}/projects/17_day/harib14b/boot.bin)
set(17_day_harib14b_CCO	  ${root_BINARY_DIR}/projects/17_day/harib14b/boot.o)
set(17_day_harib14b_CCS	  ${root_SOURCE_DIR}/projects/17_day/harib14b/*.c)
set(17_day_harib14b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(17_day_harib14b_FUNCS ${root_SOURCE_DIR}/projects/17_day/harib14b/naskfunc.nas)
set(17_day_harib14b_FUNCO ${root_BINARY_DIR}/projects/17_day/harib14b/naskfunc.o)
set(17_day_harib14b_FONTS ${root_SOURCE_DIR}/projects/17_day/harib14b/hankaku.txt)
set(17_day_harib14b_FONTB ${root_BINARY_DIR}/projects/17_day/harib14b/hankaku.bin)
set(17_day_harib14b_FONTO ${root_BINARY_DIR}/projects/17_day/harib14b/hankaku.o)
set(17_day_harib14b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(17_day_harib14b_LIBGC ${root_BINARY_DIR}/projects/17_day/harib14b/libgo.o)
set(17_day_harib14b_WILDOBJ ${root_BINARY_DIR}/projects/17_day/harib14b/*.o)


add_custom_target(17_day_harib14b_run
  COMMAND ${QEMU} ${QEMUOPT} ${17_day_harib14b_OS}
  DEPENDS 17_day_harib14b_img
)
add_custom_target(17_day_harib14b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${17_day_harib14b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 17_day_harib14b_img
)
add_custom_target(17_day_harib14b_clean
  COMMAND rm -f ${17_day_harib14b_OS}
  COMMAND rm -f ${17_day_harib14b_SYS}
  COMMAND rm -f ${17_day_harib14b_IPLB}
  COMMAND rm -f ${17_day_harib14b_HEADB}
  COMMAND rm -f ${17_day_harib14b_BOOTB}
  COMMAND rm -f ${17_day_harib14b_WILDOBJ}
)
add_custom_target(17_day_harib14b_ipl
  COMMAND ${NASK} ${17_day_harib14b_IPLS} ${17_day_harib14b_IPLB}
)
add_custom_target(17_day_harib14b_sys
  COMMAND ${NASK} ${17_day_harib14b_HEADS} ${17_day_harib14b_HEADB}
  COMMAND ${FONT} ${17_day_harib14b_FONTS} ${17_day_harib14b_FONTB}
  COMMAND ${B2O}  ${17_day_harib14b_FONTB} ${17_day_harib14b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${17_day_harib14b_LIBGE} ${17_day_harib14b_LIBGC}
  COMMAND ${NASK} ${17_day_harib14b_FUNCS} ${17_day_harib14b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${17_day_harib14b_LDS} ${17_day_harib14b_CCS} ${17_day_harib14b_WILDOBJ} -o ${17_day_harib14b_BOOTB}
  COMMAND cat ${17_day_harib14b_HEADB} ${17_day_harib14b_BOOTB} > ${17_day_harib14b_SYS}
  DEPENDS 17_day_harib14b_ipl
)
add_custom_target(17_day_harib14b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${17_day_harib14b_IPLB} -i ${17_day_harib14b_OS}
  COMMAND mcopy -i ${17_day_harib14b_OS} ${17_day_harib14b_SYS} ::
  DEPENDS 17_day_harib14b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/asmhead_wine.img)

add_custom_target(17_day_harib14b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(17_day_harib14b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(17_day_harib14b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14b_asmhead_od 17_day_harib14b_asmhead)
add_dependencies(17_day_harib14b_asmhead_od 17_day_harib14b_asmhead_wine)

add_dependencies(images 17_day_harib14b_asmhead)
add_dependencies(wine 17_day_harib14b_asmhead_wine)
add_dependencies(od 17_day_harib14b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/ipl10_wine.img)

add_custom_target(17_day_harib14b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(17_day_harib14b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(17_day_harib14b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14b_ipl10_od 17_day_harib14b_ipl10)
add_dependencies(17_day_harib14b_ipl10_od 17_day_harib14b_ipl10_wine)

add_dependencies(images 17_day_harib14b_ipl10)
add_dependencies(wine 17_day_harib14b_ipl10_wine)
add_dependencies(od 17_day_harib14b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/17_day/harib14b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/17_day/harib14b/naskfunc_wine.img)

add_custom_target(17_day_harib14b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(17_day_harib14b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(17_day_harib14b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(17_day_harib14b_naskfunc_od 17_day_harib14b_naskfunc)
add_dependencies(17_day_harib14b_naskfunc_od 17_day_harib14b_naskfunc_wine)

add_dependencies(images 17_day_harib14b_naskfunc)
add_dependencies(wine 17_day_harib14b_naskfunc_wine)
add_dependencies(od 17_day_harib14b_naskfunc_od)

#----------------------------------------------------------
