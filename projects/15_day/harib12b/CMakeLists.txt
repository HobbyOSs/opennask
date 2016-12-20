#----------------------------------------------------------
message(STATUS "Entering directory projects/15_day/harib12b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(15_day_harib12b_OS    ${root_BINARY_DIR}/projects/15_day/harib12b/os.img)
set(15_day_harib12b_SYS	  ${root_BINARY_DIR}/projects/15_day/harib12b/os.sys)
set(15_day_harib12b_IPLB  ${root_BINARY_DIR}/projects/15_day/harib12b/ipl.bin)
set(15_day_harib12b_IPLS  ${root_SOURCE_DIR}/projects/15_day/harib12b/ipl10.nas)
set(15_day_harib12b_HEADB ${root_BINARY_DIR}/projects/15_day/harib12b/asmhead.bin)
set(15_day_harib12b_HEADS ${root_SOURCE_DIR}/projects/15_day/harib12b/asmhead.nas)
set(15_day_harib12b_BOOTB ${root_BINARY_DIR}/projects/15_day/harib12b/boot.bin)
set(15_day_harib12b_CCO	  ${root_BINARY_DIR}/projects/15_day/harib12b/boot.o)
set(15_day_harib12b_CCS	  ${root_SOURCE_DIR}/projects/15_day/harib12b/*.c)
set(15_day_harib12b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(15_day_harib12b_FUNCS ${root_SOURCE_DIR}/projects/15_day/harib12b/naskfunc.nas)
set(15_day_harib12b_FUNCO ${root_BINARY_DIR}/projects/15_day/harib12b/naskfunc.o)
set(15_day_harib12b_FONTS ${root_SOURCE_DIR}/projects/15_day/harib12b/hankaku.txt)
set(15_day_harib12b_FONTB ${root_BINARY_DIR}/projects/15_day/harib12b/hankaku.bin)
set(15_day_harib12b_FONTO ${root_BINARY_DIR}/projects/15_day/harib12b/hankaku.o)
set(15_day_harib12b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(15_day_harib12b_LIBGC ${root_BINARY_DIR}/projects/15_day/harib12b/libgo.o)
set(15_day_harib12b_WILDOBJ ${root_BINARY_DIR}/projects/15_day/harib12b/*.o)


add_custom_target(15_day_harib12b_run
  COMMAND ${QEMU} ${QEMUOPT} ${15_day_harib12b_OS}
  DEPENDS 15_day_harib12b_img
)
add_custom_target(15_day_harib12b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${15_day_harib12b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 15_day_harib12b_img
)
add_custom_target(15_day_harib12b_clean
  COMMAND rm -f ${15_day_harib12b_OS}
  COMMAND rm -f ${15_day_harib12b_SYS}
  COMMAND rm -f ${15_day_harib12b_IPLB}
  COMMAND rm -f ${15_day_harib12b_HEADB}
  COMMAND rm -f ${15_day_harib12b_BOOTB}
  COMMAND rm -f ${15_day_harib12b_WILDOBJ}
)
add_custom_target(15_day_harib12b_ipl
  COMMAND ${NASK} ${15_day_harib12b_IPLS} ${15_day_harib12b_IPLB}
)
add_custom_target(15_day_harib12b_sys
  COMMAND ${NASK} ${15_day_harib12b_HEADS} ${15_day_harib12b_HEADB}
  COMMAND ${FONT} ${15_day_harib12b_FONTS} ${15_day_harib12b_FONTB}
  COMMAND ${B2O}  ${15_day_harib12b_FONTB} ${15_day_harib12b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${15_day_harib12b_LIBGE} ${15_day_harib12b_LIBGC}
  COMMAND ${NASK} ${15_day_harib12b_FUNCS} ${15_day_harib12b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${15_day_harib12b_LDS} ${15_day_harib12b_CCS} ${15_day_harib12b_WILDOBJ} -o ${15_day_harib12b_BOOTB}
  COMMAND cat ${15_day_harib12b_HEADB} ${15_day_harib12b_BOOTB} > ${15_day_harib12b_SYS}
  DEPENDS 15_day_harib12b_ipl
)
add_custom_target(15_day_harib12b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${15_day_harib12b_IPLB} -i ${15_day_harib12b_OS}
  COMMAND mcopy -i ${15_day_harib12b_OS} ${15_day_harib12b_SYS} ::
  DEPENDS 15_day_harib12b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/asmhead_wine.img)

add_custom_target(15_day_harib12b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(15_day_harib12b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(15_day_harib12b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12b_asmhead_od 15_day_harib12b_asmhead)
add_dependencies(15_day_harib12b_asmhead_od 15_day_harib12b_asmhead_wine)

add_dependencies(images 15_day_harib12b_asmhead)
add_dependencies(wine 15_day_harib12b_asmhead_wine)
add_dependencies(od 15_day_harib12b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/ipl10_wine.img)

add_custom_target(15_day_harib12b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(15_day_harib12b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(15_day_harib12b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12b_ipl10_od 15_day_harib12b_ipl10)
add_dependencies(15_day_harib12b_ipl10_od 15_day_harib12b_ipl10_wine)

add_dependencies(images 15_day_harib12b_ipl10)
add_dependencies(wine 15_day_harib12b_ipl10_wine)
add_dependencies(od 15_day_harib12b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/15_day/harib12b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/15_day/harib12b/naskfunc_wine.img)

add_custom_target(15_day_harib12b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(15_day_harib12b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(15_day_harib12b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(15_day_harib12b_naskfunc_od 15_day_harib12b_naskfunc)
add_dependencies(15_day_harib12b_naskfunc_od 15_day_harib12b_naskfunc_wine)

add_dependencies(images 15_day_harib12b_naskfunc)
add_dependencies(wine 15_day_harib12b_naskfunc_wine)
add_dependencies(od 15_day_harib12b_naskfunc_od)

#----------------------------------------------------------
