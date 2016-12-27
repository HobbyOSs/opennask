#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10f_OS    ${root_BINARY_DIR}/projects/13_day/harib10f/os.img)
set(13_day_harib10f_SYS	  ${root_BINARY_DIR}/projects/13_day/harib10f/os.sys)
set(13_day_harib10f_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10f/ipl.bin)
set(13_day_harib10f_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10f/ipl10.nas)
set(13_day_harib10f_HEADB ${root_BINARY_DIR}/projects/13_day/harib10f/asmhead.bin)
set(13_day_harib10f_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10f/asmhead.nas)
set(13_day_harib10f_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10f/boot.bin)
set(13_day_harib10f_CCO	  ${root_BINARY_DIR}/projects/13_day/harib10f/boot.o)
set(13_day_harib10f_CCS	  ${root_SOURCE_DIR}/projects/13_day/harib10f/*.c)
set(13_day_harib10f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10f_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10f/naskfunc.nas)
set(13_day_harib10f_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10f/naskfunc.o)
set(13_day_harib10f_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10f/hankaku.txt)
set(13_day_harib10f_FONTB ${root_BINARY_DIR}/projects/13_day/harib10f/hankaku.bin)
set(13_day_harib10f_FONTO ${root_BINARY_DIR}/projects/13_day/harib10f/hankaku.o)
set(13_day_harib10f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10f_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10f/libgo.o)
set(13_day_harib10f_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10f/*.o)


add_custom_target(13_day_harib10f_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10f_OS}
  DEPENDS 13_day_harib10f_img
)
add_custom_target(13_day_harib10f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10f_img
)
add_custom_target(13_day_harib10f_clean
  COMMAND rm -f ${13_day_harib10f_OS}
  COMMAND rm -f ${13_day_harib10f_SYS}
  COMMAND rm -f ${13_day_harib10f_IPLB}
  COMMAND rm -f ${13_day_harib10f_HEADB}
  COMMAND rm -f ${13_day_harib10f_BOOTB}
  COMMAND rm -f ${13_day_harib10f_WILDOBJ}
)
add_custom_target(13_day_harib10f_ipl
  COMMAND ${NASK} ${13_day_harib10f_IPLS} ${13_day_harib10f_IPLB}
)
add_custom_target(13_day_harib10f_sys
  COMMAND ${NASK} ${13_day_harib10f_HEADS} ${13_day_harib10f_HEADB}
  COMMAND ${FONT} ${13_day_harib10f_FONTS} ${13_day_harib10f_FONTB}
  COMMAND ${B2O}  ${13_day_harib10f_FONTB} ${13_day_harib10f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10f_LIBGE} ${13_day_harib10f_LIBGC}
  COMMAND ${NASK} ${13_day_harib10f_FUNCS} ${13_day_harib10f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${13_day_harib10f_LDS} ${13_day_harib10f_CCS} ${13_day_harib10f_WILDOBJ} -o ${13_day_harib10f_BOOTB}
  COMMAND cat ${13_day_harib10f_HEADB} ${13_day_harib10f_BOOTB} > ${13_day_harib10f_SYS}
  DEPENDS 13_day_harib10f_ipl
)
add_custom_target(13_day_harib10f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10f_IPLB} -i ${13_day_harib10f_OS}
  COMMAND mcopy -i ${13_day_harib10f_OS} ${13_day_harib10f_SYS} ::
  DEPENDS 13_day_harib10f_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/asmhead_wine.img)

add_custom_target(13_day_harib10f_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(13_day_harib10f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(13_day_harib10f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10f_asmhead_od 13_day_harib10f_asmhead)
add_dependencies(13_day_harib10f_asmhead_od 13_day_harib10f_asmhead_wine)

add_dependencies(images 13_day_harib10f_asmhead)
add_dependencies(wine 13_day_harib10f_asmhead_wine)
add_dependencies(od 13_day_harib10f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/ipl10_wine.img)

add_custom_target(13_day_harib10f_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(13_day_harib10f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(13_day_harib10f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10f_ipl10_od 13_day_harib10f_ipl10)
add_dependencies(13_day_harib10f_ipl10_od 13_day_harib10f_ipl10_wine)

add_dependencies(images 13_day_harib10f_ipl10)
add_dependencies(wine 13_day_harib10f_ipl10_wine)
add_dependencies(od 13_day_harib10f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10f/naskfunc_wine.img)

add_custom_target(13_day_harib10f_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(13_day_harib10f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(13_day_harib10f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10f_naskfunc_od 13_day_harib10f_naskfunc)
add_dependencies(13_day_harib10f_naskfunc_od 13_day_harib10f_naskfunc_wine)

add_dependencies(images 13_day_harib10f_naskfunc)
add_dependencies(wine 13_day_harib10f_naskfunc_wine)
add_dependencies(od 13_day_harib10f_naskfunc_od)

#----------------------------------------------------------
