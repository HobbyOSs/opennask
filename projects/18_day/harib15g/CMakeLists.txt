#----------------------------------------------------------
message(STATUS "Entering directory projects/18_day/harib15g/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(18_day_harib15g_OS    ${root_BINARY_DIR}/projects/18_day/harib15g/os.img)
set(18_day_harib15g_SYS	  ${root_BINARY_DIR}/projects/18_day/harib15g/os.sys)
set(18_day_harib15g_IPLB  ${root_BINARY_DIR}/projects/18_day/harib15g/ipl.bin)
set(18_day_harib15g_IPLS  ${root_SOURCE_DIR}/projects/18_day/harib15g/ipl10.nas)
set(18_day_harib15g_HEADB ${root_BINARY_DIR}/projects/18_day/harib15g/asmhead.bin)
set(18_day_harib15g_HEADS ${root_SOURCE_DIR}/projects/18_day/harib15g/asmhead.nas)
set(18_day_harib15g_BOOTB ${root_BINARY_DIR}/projects/18_day/harib15g/boot.bin)
set(18_day_harib15g_CCO	  ${root_BINARY_DIR}/projects/18_day/harib15g/boot.o)
set(18_day_harib15g_CCS	  ${root_SOURCE_DIR}/projects/18_day/harib15g/*.c)
set(18_day_harib15g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(18_day_harib15g_FUNCS ${root_SOURCE_DIR}/projects/18_day/harib15g/naskfunc.nas)
set(18_day_harib15g_FUNCO ${root_BINARY_DIR}/projects/18_day/harib15g/naskfunc.o)
set(18_day_harib15g_FONTS ${root_SOURCE_DIR}/projects/18_day/harib15g/hankaku.txt)
set(18_day_harib15g_FONTB ${root_BINARY_DIR}/projects/18_day/harib15g/hankaku.bin)
set(18_day_harib15g_FONTO ${root_BINARY_DIR}/projects/18_day/harib15g/hankaku.o)
set(18_day_harib15g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(18_day_harib15g_LIBGC ${root_BINARY_DIR}/projects/18_day/harib15g/libgo.o)
set(18_day_harib15g_WILDOBJ ${root_BINARY_DIR}/projects/18_day/harib15g/*.o)


add_custom_target(18_day_harib15g_run
  COMMAND ${QEMU} ${QEMUOPT} ${18_day_harib15g_OS}
  DEPENDS 18_day_harib15g_img
)
add_custom_target(18_day_harib15g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${18_day_harib15g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 18_day_harib15g_img
)
add_custom_target(18_day_harib15g_clean
  COMMAND rm -f ${18_day_harib15g_OS}
  COMMAND rm -f ${18_day_harib15g_SYS}
  COMMAND rm -f ${18_day_harib15g_IPLB}
  COMMAND rm -f ${18_day_harib15g_HEADB}
  COMMAND rm -f ${18_day_harib15g_BOOTB}
  COMMAND rm -f ${18_day_harib15g_WILDOBJ}
)
add_custom_target(18_day_harib15g_ipl
  COMMAND ${NASK} ${18_day_harib15g_IPLS} ${18_day_harib15g_IPLB}
)
add_custom_target(18_day_harib15g_sys
  COMMAND ${NASK} ${18_day_harib15g_HEADS} ${18_day_harib15g_HEADB}
  COMMAND ${FONT} ${18_day_harib15g_FONTS} ${18_day_harib15g_FONTB}
  COMMAND ${B2O}  ${18_day_harib15g_FONTB} ${18_day_harib15g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${18_day_harib15g_LIBGE} ${18_day_harib15g_LIBGC}
  COMMAND ${NASK} ${18_day_harib15g_FUNCS} ${18_day_harib15g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${18_day_harib15g_LDS} ${18_day_harib15g_CCS} ${18_day_harib15g_WILDOBJ} -o ${18_day_harib15g_BOOTB}
  COMMAND cat ${18_day_harib15g_HEADB} ${18_day_harib15g_BOOTB} > ${18_day_harib15g_SYS}
  DEPENDS 18_day_harib15g_ipl
)
add_custom_target(18_day_harib15g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${18_day_harib15g_IPLB} -i ${18_day_harib15g_OS}
  COMMAND mcopy -i ${18_day_harib15g_OS} ${18_day_harib15g_SYS} ::
  DEPENDS 18_day_harib15g_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15g/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/asmhead_wine.img)

add_custom_target(18_day_harib15g_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(18_day_harib15g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(18_day_harib15g_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15g_asmhead_od 18_day_harib15g_asmhead)
add_dependencies(18_day_harib15g_asmhead_od 18_day_harib15g_asmhead_wine)

add_dependencies(images 18_day_harib15g_asmhead)
add_dependencies(wine 18_day_harib15g_asmhead_wine)
add_dependencies(od 18_day_harib15g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15g/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/ipl10_wine.img)

add_custom_target(18_day_harib15g_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(18_day_harib15g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(18_day_harib15g_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15g_ipl10_od 18_day_harib15g_ipl10)
add_dependencies(18_day_harib15g_ipl10_od 18_day_harib15g_ipl10_wine)

add_dependencies(images 18_day_harib15g_ipl10)
add_dependencies(wine 18_day_harib15g_ipl10_wine)
add_dependencies(od 18_day_harib15g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/18_day/harib15g/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/18_day/harib15g/naskfunc_wine.img)

add_custom_target(18_day_harib15g_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(18_day_harib15g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(18_day_harib15g_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(18_day_harib15g_naskfunc_od 18_day_harib15g_naskfunc)
add_dependencies(18_day_harib15g_naskfunc_od 18_day_harib15g_naskfunc_wine)

add_dependencies(images 18_day_harib15g_naskfunc)
add_dependencies(wine 18_day_harib15g_naskfunc_wine)
add_dependencies(od 18_day_harib15g_naskfunc_od)

#----------------------------------------------------------
