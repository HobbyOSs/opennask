#----------------------------------------------------------
message(STATUS "Entering directory projects/09_day/harib06d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(09_day_harib06d_OS    ${root_BINARY_DIR}/projects/09_day/harib06d/os.img)
set(09_day_harib06d_SYS	  ${root_BINARY_DIR}/projects/09_day/harib06d/os.sys)
set(09_day_harib06d_IPLB  ${root_BINARY_DIR}/projects/09_day/harib06d/ipl.bin)
set(09_day_harib06d_IPLS  ${root_SOURCE_DIR}/projects/09_day/harib06d/ipl10.nas)
set(09_day_harib06d_HEADB ${root_BINARY_DIR}/projects/09_day/harib06d/asmhead.bin)
set(09_day_harib06d_HEADS ${root_SOURCE_DIR}/projects/09_day/harib06d/asmhead.nas)
set(09_day_harib06d_BOOTB ${root_BINARY_DIR}/projects/09_day/harib06d/boot.bin)
set(09_day_harib06d_CCO	  ${root_BINARY_DIR}/projects/09_day/harib06d/boot.o)
set(09_day_harib06d_CCS	  ${root_SOURCE_DIR}/projects/09_day/harib06d/*.c)
set(09_day_harib06d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(09_day_harib06d_FUNCS ${root_SOURCE_DIR}/projects/09_day/harib06d/naskfunc.nas)
set(09_day_harib06d_FUNCO ${root_BINARY_DIR}/projects/09_day/harib06d/naskfunc.o)
set(09_day_harib06d_FONTS ${root_SOURCE_DIR}/projects/09_day/harib06d/hankaku.txt)
set(09_day_harib06d_FONTB ${root_BINARY_DIR}/projects/09_day/harib06d/hankaku.bin)
set(09_day_harib06d_FONTO ${root_BINARY_DIR}/projects/09_day/harib06d/hankaku.o)
set(09_day_harib06d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(09_day_harib06d_LIBGC ${root_BINARY_DIR}/projects/09_day/harib06d/libgo.o)
set(09_day_harib06d_WILDOBJ ${root_BINARY_DIR}/projects/09_day/harib06d/*.o)


add_custom_target(09_day_harib06d_run
  COMMAND ${QEMU} ${QEMUOPT} ${09_day_harib06d_OS}
  DEPENDS 09_day_harib06d_img
)
add_custom_target(09_day_harib06d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${09_day_harib06d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 09_day_harib06d_img
)
add_custom_target(09_day_harib06d_clean
  COMMAND rm -f ${09_day_harib06d_OS}
  COMMAND rm -f ${09_day_harib06d_SYS}
  COMMAND rm -f ${09_day_harib06d_IPLB}
  COMMAND rm -f ${09_day_harib06d_HEADB}
  COMMAND rm -f ${09_day_harib06d_BOOTB}
  COMMAND rm -f ${09_day_harib06d_WILDOBJ}
)
add_custom_target(09_day_harib06d_ipl
  COMMAND ${NASK} ${09_day_harib06d_IPLS} ${09_day_harib06d_IPLB}
)
add_custom_target(09_day_harib06d_sys
  COMMAND ${NASK} ${09_day_harib06d_HEADS} ${09_day_harib06d_HEADB}
  COMMAND ${FONT} ${09_day_harib06d_FONTS} ${09_day_harib06d_FONTB}
  COMMAND ${B2O}  ${09_day_harib06d_FONTB} ${09_day_harib06d_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${09_day_harib06d_LIBGE} ${09_day_harib06d_LIBGC}
  COMMAND ${NASK} ${09_day_harib06d_FUNCS} ${09_day_harib06d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${09_day_harib06d_LDS} ${09_day_harib06d_CCS} ${09_day_harib06d_WILDOBJ} -o ${09_day_harib06d_BOOTB}
  COMMAND cat ${09_day_harib06d_HEADB} ${09_day_harib06d_BOOTB} > ${09_day_harib06d_SYS}
  DEPENDS 09_day_harib06d_ipl
)
add_custom_target(09_day_harib06d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${09_day_harib06d_IPLB} -i ${09_day_harib06d_OS}
  COMMAND mcopy -i ${09_day_harib06d_OS} ${09_day_harib06d_SYS} ::
  DEPENDS 09_day_harib06d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/asmhead_wine.img)

add_custom_target(09_day_harib06d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(09_day_harib06d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(09_day_harib06d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(09_day_harib06d_asmhead_od 09_day_harib06d_asmhead)
add_dependencies(09_day_harib06d_asmhead_od 09_day_harib06d_asmhead_wine)

add_dependencies(images 09_day_harib06d_asmhead)
add_dependencies(wine 09_day_harib06d_asmhead_wine)
add_dependencies(od 09_day_harib06d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/ipl10_wine.img)

add_custom_target(09_day_harib06d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(09_day_harib06d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(09_day_harib06d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(09_day_harib06d_ipl10_od 09_day_harib06d_ipl10)
add_dependencies(09_day_harib06d_ipl10_od 09_day_harib06d_ipl10_wine)

add_dependencies(images 09_day_harib06d_ipl10)
add_dependencies(wine 09_day_harib06d_ipl10_wine)
add_dependencies(od 09_day_harib06d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/naskfunc_wine.img)

add_custom_target(09_day_harib06d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(09_day_harib06d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(09_day_harib06d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(09_day_harib06d_naskfunc_od 09_day_harib06d_naskfunc)
add_dependencies(09_day_harib06d_naskfunc_od 09_day_harib06d_naskfunc_wine)

add_dependencies(images 09_day_harib06d_naskfunc)
add_dependencies(wine 09_day_harib06d_naskfunc_wine)
add_dependencies(od 09_day_harib06d_naskfunc_od)

#----------------------------------------------------------
