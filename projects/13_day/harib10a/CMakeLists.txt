#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10a_OS    ${root_BINARY_DIR}/projects/13_day/harib10a/os.img)
set(13_day_harib10a_SYS	  ${root_BINARY_DIR}/projects/13_day/harib10a/os.sys)
set(13_day_harib10a_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10a/ipl.bin)
set(13_day_harib10a_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10a/ipl10.nas)
set(13_day_harib10a_HEADB ${root_BINARY_DIR}/projects/13_day/harib10a/asmhead.bin)
set(13_day_harib10a_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10a/asmhead.nas)
set(13_day_harib10a_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10a/boot.bin)
set(13_day_harib10a_CCO	  ${root_BINARY_DIR}/projects/13_day/harib10a/boot.o)
set(13_day_harib10a_CCS	  ${root_SOURCE_DIR}/projects/13_day/harib10a/*.c)
set(13_day_harib10a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10a_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10a/naskfunc.nas)
set(13_day_harib10a_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10a/naskfunc.o)
set(13_day_harib10a_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10a/hankaku.txt)
set(13_day_harib10a_FONTB ${root_BINARY_DIR}/projects/13_day/harib10a/hankaku.bin)
set(13_day_harib10a_FONTO ${root_BINARY_DIR}/projects/13_day/harib10a/hankaku.o)
set(13_day_harib10a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10a_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10a/libgo.o)
set(13_day_harib10a_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10a/*.o)


add_custom_target(13_day_harib10a_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10a_OS}
  DEPENDS 13_day_harib10a_img
)
add_custom_target(13_day_harib10a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10a_img
)
add_custom_target(13_day_harib10a_clean
  COMMAND rm -f ${13_day_harib10a_OS}
  COMMAND rm -f ${13_day_harib10a_SYS}
  COMMAND rm -f ${13_day_harib10a_IPLB}
  COMMAND rm -f ${13_day_harib10a_HEADB}
  COMMAND rm -f ${13_day_harib10a_BOOTB}
  COMMAND rm -f ${13_day_harib10a_WILDOBJ}
)
add_custom_target(13_day_harib10a_ipl
  COMMAND ${NASK} ${13_day_harib10a_IPLS} ${13_day_harib10a_IPLB}
)
add_custom_target(13_day_harib10a_sys
  COMMAND ${NASK} ${13_day_harib10a_HEADS} ${13_day_harib10a_HEADB}
  COMMAND ${FONT} ${13_day_harib10a_FONTS} ${13_day_harib10a_FONTB}
  COMMAND ${B2O}  ${13_day_harib10a_FONTB} ${13_day_harib10a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10a_LIBGE} ${13_day_harib10a_LIBGC}
  COMMAND ${NASK} ${13_day_harib10a_FUNCS} ${13_day_harib10a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${13_day_harib10a_LDS} ${13_day_harib10a_CCS} ${13_day_harib10a_WILDOBJ} -o ${13_day_harib10a_BOOTB}
  COMMAND cat ${13_day_harib10a_HEADB} ${13_day_harib10a_BOOTB} > ${13_day_harib10a_SYS}
  DEPENDS 13_day_harib10a_ipl
)
add_custom_target(13_day_harib10a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10a_IPLB} -i ${13_day_harib10a_OS}
  COMMAND mcopy -i ${13_day_harib10a_OS} ${13_day_harib10a_SYS} ::
  DEPENDS 13_day_harib10a_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/asmhead_wine.img)

add_custom_target(13_day_harib10a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(13_day_harib10a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(13_day_harib10a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10a_asmhead_od 13_day_harib10a_asmhead)
add_dependencies(13_day_harib10a_asmhead_od 13_day_harib10a_asmhead_wine)

add_dependencies(images 13_day_harib10a_asmhead)
add_dependencies(wine 13_day_harib10a_asmhead_wine)
add_dependencies(od 13_day_harib10a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/ipl10_wine.img)

add_custom_target(13_day_harib10a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(13_day_harib10a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(13_day_harib10a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10a_ipl10_od 13_day_harib10a_ipl10)
add_dependencies(13_day_harib10a_ipl10_od 13_day_harib10a_ipl10_wine)

add_dependencies(images 13_day_harib10a_ipl10)
add_dependencies(wine 13_day_harib10a_ipl10_wine)
add_dependencies(od 13_day_harib10a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10a/naskfunc_wine.img)

add_custom_target(13_day_harib10a_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(13_day_harib10a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(13_day_harib10a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10a_naskfunc_od 13_day_harib10a_naskfunc)
add_dependencies(13_day_harib10a_naskfunc_od 13_day_harib10a_naskfunc_wine)

add_dependencies(images 13_day_harib10a_naskfunc)
add_dependencies(wine 13_day_harib10a_naskfunc_wine)
add_dependencies(od 13_day_harib10a_naskfunc_od)

#----------------------------------------------------------
