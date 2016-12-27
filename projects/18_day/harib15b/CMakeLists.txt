#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15b_OS    ${root_BINARY_DIR}/projects/18_day/harib15b/os.img)
set(18_day_harib15b_SYS	  ${root_BINARY_DIR}/projects/18_day/harib15b/os.sys)
set(18_day_harib15b_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15b/ipl.bin)
set(18_day_harib15b_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15b/ipl10.nas)
set(18_day_harib15b_HEADB ${root_BINARY_DIR}/projects/18_day/harib15b/asmhead.bin)
set(18_day_harib15b_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15b/asmhead.nas)
set(18_day_harib15b_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15b/boot.bin)
set(18_day_harib15b_CCO	  ${root_BINARY_DIR}/projects/18_day/harib15b/boot.o)
set(18_day_harib15b_CCS	  ${root_SOURCE_DIR}/projects/18_day/harib15b/*.c)
set(18_day_harib15b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15b_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15b/naskfunc.nas)
set(18_day_harib15b_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15b/naskfunc.o)
set(18_day_harib15b_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15b/hankaku.txt)
set(18_day_harib15b_FONTB ${root_BINARY_DIR}/projects/18_day/harib15b/hankaku.bin)
set(18_day_harib15b_FONTO ${root_BINARY_DIR}/projects/18_day/harib15b/hankaku.o)
set(18_day_harib15b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15b_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15b/libgo.o)
set(18_day_harib15b_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15b/*.o)


add_custom_target(18_day_harib15b_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15b_OS}
  DEPENDS 18_day_harib15b_img
)
add_custom_target(18_day_harib15b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 18_day_harib15b_img
)
add_custom_target(18_day_harib15b_clean
  COMMAND rm -f ${18_day_harib15b_OS}
  COMMAND rm -f ${18_day_harib15b_SYS}
  COMMAND rm -f ${18_day_harib15b_IPLB}
  COMMAND rm -f ${18_day_harib15b_HEADB}
  COMMAND rm -f ${18_day_harib15b_BOOTB}
  COMMAND rm -f ${18_day_harib15b_WILDOBJ}
)
add_custom_target(18_day_harib15b_ipl
  COMMAND ${NASK} ${18_day_harib15b_IPLS} ${18_day_harib15b_IPLB}
)
add_custom_target(18_day_harib15b_sys
  COMMAND ${NASK} ${18_day_harib15b_HEADS} ${18_day_harib15b_HEADB}
  COMMAND ${FONT} ${18_day_harib15b_FONTS} ${18_day_harib15b_FONTB}
  COMMAND ${B2O}  ${18_day_harib15b_FONTB} ${18_day_harib15b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15b_LIBGE} ${18_day_harib15b_LIBGC}
  COMMAND ${NASK} ${18_day_harib15b_FUNCS} ${18_day_harib15b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${18_day_harib15b_LDS} ${18_day_harib15b_CCS} ${18_day_harib15b_WILDOBJ} -o ${18_day_harib15b_BOOTB}
  COMMAND cat ${18_day_harib15b_HEADB} ${18_day_harib15b_BOOTB} > ${18_day_harib15b_SYS}
  DEPENDS 18_day_harib15b_ipl
)
add_custom_target(18_day_harib15b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15b_IPLB} -i ${18_day_harib15b_OS}
  COMMAND mcopy -i ${18_day_harib15b_OS} ${18_day_harib15b_SYS} ::
  DEPENDS 18_day_harib15b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/asmhead_wine.img)

add_custom_target(18_day_harib15b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(18_day_harib15b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(18_day_harib15b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15b_asmhead_od 18_day_harib15b_asmhead)
add_dependencies(18_day_harib15b_asmhead_od 18_day_harib15b_asmhead_wine)

add_dependencies(images 18_day_harib15b_asmhead)
add_dependencies(wine 18_day_harib15b_asmhead_wine)
add_dependencies(od 18_day_harib15b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/ipl10_wine.img)

add_custom_target(18_day_harib15b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(18_day_harib15b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(18_day_harib15b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15b_ipl10_od 18_day_harib15b_ipl10)
add_dependencies(18_day_harib15b_ipl10_od 18_day_harib15b_ipl10_wine)

add_dependencies(images 18_day_harib15b_ipl10)
add_dependencies(wine 18_day_harib15b_ipl10_wine)
add_dependencies(od 18_day_harib15b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15b/naskfunc_wine.img)

add_custom_target(18_day_harib15b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(18_day_harib15b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(18_day_harib15b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15b_naskfunc_od 18_day_harib15b_naskfunc)
add_dependencies(18_day_harib15b_naskfunc_od 18_day_harib15b_naskfunc_wine)

add_dependencies(images 18_day_harib15b_naskfunc)
add_dependencies(wine 18_day_harib15b_naskfunc_wine)
add_dependencies(od 18_day_harib15b_naskfunc_od)

#----------------------------------------------------------
