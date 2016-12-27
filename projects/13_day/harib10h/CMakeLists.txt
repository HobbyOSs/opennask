#----------------------------------------------------------
message(STATUS "Entering directory projects/13_day/harib10h/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(13_day_harib10h_OS    ${root_BINARY_DIR}/projects/13_day/harib10h/os.img)
set(13_day_harib10h_SYS	  ${root_BINARY_DIR}/projects/13_day/harib10h/os.sys)
set(13_day_harib10h_IPLB  ${root_BINARY_DIR}/projects/13_day/harib10h/ipl.bin)
set(13_day_harib10h_IPLS  ${root_SOURCE_DIR}/projects/13_day/harib10h/ipl10.nas)
set(13_day_harib10h_HEADB ${root_BINARY_DIR}/projects/13_day/harib10h/asmhead.bin)
set(13_day_harib10h_HEADS ${root_SOURCE_DIR}/projects/13_day/harib10h/asmhead.nas)
set(13_day_harib10h_BOOTB ${root_BINARY_DIR}/projects/13_day/harib10h/boot.bin)
set(13_day_harib10h_CCO	  ${root_BINARY_DIR}/projects/13_day/harib10h/boot.o)
set(13_day_harib10h_CCS	  ${root_SOURCE_DIR}/projects/13_day/harib10h/*.c)
set(13_day_harib10h_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(13_day_harib10h_FUNCS ${root_SOURCE_DIR}/projects/13_day/harib10h/naskfunc.nas)
set(13_day_harib10h_FUNCO ${root_BINARY_DIR}/projects/13_day/harib10h/naskfunc.o)
set(13_day_harib10h_FONTS ${root_SOURCE_DIR}/projects/13_day/harib10h/hankaku.txt)
set(13_day_harib10h_FONTB ${root_BINARY_DIR}/projects/13_day/harib10h/hankaku.bin)
set(13_day_harib10h_FONTO ${root_BINARY_DIR}/projects/13_day/harib10h/hankaku.o)
set(13_day_harib10h_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(13_day_harib10h_LIBGC ${root_BINARY_DIR}/projects/13_day/harib10h/libgo.o)
set(13_day_harib10h_WILDOBJ ${root_BINARY_DIR}/projects/13_day/harib10h/*.o)


add_custom_target(13_day_harib10h_run
  COMMAND ${QEMU} ${QEMUOPT} ${13_day_harib10h_OS}
  DEPENDS 13_day_harib10h_img
)
add_custom_target(13_day_harib10h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${13_day_harib10h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 13_day_harib10h_img
)
add_custom_target(13_day_harib10h_clean
  COMMAND rm -f ${13_day_harib10h_OS}
  COMMAND rm -f ${13_day_harib10h_SYS}
  COMMAND rm -f ${13_day_harib10h_IPLB}
  COMMAND rm -f ${13_day_harib10h_HEADB}
  COMMAND rm -f ${13_day_harib10h_BOOTB}
  COMMAND rm -f ${13_day_harib10h_WILDOBJ}
)
add_custom_target(13_day_harib10h_ipl
  COMMAND ${NASK} ${13_day_harib10h_IPLS} ${13_day_harib10h_IPLB}
)
add_custom_target(13_day_harib10h_sys
  COMMAND ${NASK} ${13_day_harib10h_HEADS} ${13_day_harib10h_HEADB}
  COMMAND ${FONT} ${13_day_harib10h_FONTS} ${13_day_harib10h_FONTB}
  COMMAND ${B2O}  ${13_day_harib10h_FONTB} ${13_day_harib10h_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${13_day_harib10h_LIBGE} ${13_day_harib10h_LIBGC}
  COMMAND ${NASK} ${13_day_harib10h_FUNCS} ${13_day_harib10h_FUNCO}
  COMMAND gcc ${BINOPT} -T ${13_day_harib10h_LDS} ${13_day_harib10h_CCS} ${13_day_harib10h_WILDOBJ} -o ${13_day_harib10h_BOOTB}
  COMMAND cat ${13_day_harib10h_HEADB} ${13_day_harib10h_BOOTB} > ${13_day_harib10h_SYS}
  DEPENDS 13_day_harib10h_ipl
)
add_custom_target(13_day_harib10h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${13_day_harib10h_IPLB} -i ${13_day_harib10h_OS}
  COMMAND mcopy -i ${13_day_harib10h_OS} ${13_day_harib10h_SYS} ::
  DEPENDS 13_day_harib10h_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10h/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10h/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10h/asmhead_wine.img)

add_custom_target(13_day_harib10h_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(13_day_harib10h_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(13_day_harib10h_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10h_asmhead_od 13_day_harib10h_asmhead)
add_dependencies(13_day_harib10h_asmhead_od 13_day_harib10h_asmhead_wine)

add_dependencies(images 13_day_harib10h_asmhead)
add_dependencies(wine 13_day_harib10h_asmhead_wine)
add_dependencies(od 13_day_harib10h_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10h/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10h/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10h/ipl10_wine.img)

add_custom_target(13_day_harib10h_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(13_day_harib10h_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(13_day_harib10h_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10h_ipl10_od 13_day_harib10h_ipl10)
add_dependencies(13_day_harib10h_ipl10_od 13_day_harib10h_ipl10_wine)

add_dependencies(images 13_day_harib10h_ipl10)
add_dependencies(wine 13_day_harib10h_ipl10_wine)
add_dependencies(od 13_day_harib10h_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/13_day/harib10h/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10h/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/13_day/harib10h/naskfunc_wine.img)

add_custom_target(13_day_harib10h_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(13_day_harib10h_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(13_day_harib10h_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(13_day_harib10h_naskfunc_od 13_day_harib10h_naskfunc)
add_dependencies(13_day_harib10h_naskfunc_od 13_day_harib10h_naskfunc_wine)

add_dependencies(images 13_day_harib10h_naskfunc)
add_dependencies(wine 13_day_harib10h_naskfunc_wine)
add_dependencies(od 13_day_harib10h_naskfunc_od)

#----------------------------------------------------------
