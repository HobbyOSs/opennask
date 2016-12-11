#----------------------------------------------------------
message(STATUS "Entering directory projects/08_day/harib05b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(08_day_harib05b_OS    ${root_BINARY_DIR}/projects/08_day/harib05b/os.img)
set(08_day_harib05b_SYS	  ${root_BINARY_DIR}/projects/08_day/harib05b/os.sys)
set(08_day_harib05b_IPLB  ${root_BINARY_DIR}/projects/08_day/harib05b/ipl.bin)
set(08_day_harib05b_IPLS  ${root_SOURCE_DIR}/projects/08_day/harib05b/ipl10.nas)
set(08_day_harib05b_HEADB ${root_BINARY_DIR}/projects/08_day/harib05b/asmhead.bin)
set(08_day_harib05b_HEADS ${root_SOURCE_DIR}/projects/08_day/harib05b/asmhead.nas)
set(08_day_harib05b_BOOTB ${root_BINARY_DIR}/projects/08_day/harib05b/boot.bin)
set(08_day_harib05b_CCO	  ${root_BINARY_DIR}/projects/08_day/harib05b/boot.o)
set(08_day_harib05b_CCS	  ${root_SOURCE_DIR}/projects/08_day/harib05b/*.c)
set(08_day_harib05b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(08_day_harib05b_FUNCS ${root_SOURCE_DIR}/projects/08_day/harib05b/naskfunc.nas)
set(08_day_harib05b_FUNCO ${root_BINARY_DIR}/projects/08_day/harib05b/naskfunc.o)
set(08_day_harib05b_FONTS ${root_SOURCE_DIR}/projects/08_day/harib05b/hankaku.txt)
set(08_day_harib05b_FONTB ${root_BINARY_DIR}/projects/08_day/harib05b/hankaku.bin)
set(08_day_harib05b_FONTO ${root_BINARY_DIR}/projects/08_day/harib05b/hankaku.o)
set(08_day_harib05b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(08_day_harib05b_LIBGC ${root_BINARY_DIR}/projects/08_day/harib05b/libgo.o)
set(08_day_harib05b_WILDOBJ ${root_BINARY_DIR}/projects/08_day/harib05b/*.o)


add_custom_target(08_day_harib05b_run
  COMMAND ${QEMU} ${QEMUOPT} ${08_day_harib05b_OS}
  DEPENDS 08_day_harib05b_img
)
add_custom_target(08_day_harib05b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${08_day_harib05b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 08_day_harib05b_img
)
add_custom_target(08_day_harib05b_clean
  COMMAND rm -f ${08_day_harib05b_OS}
  COMMAND rm -f ${08_day_harib05b_SYS}
  COMMAND rm -f ${08_day_harib05b_IPLB}
  COMMAND rm -f ${08_day_harib05b_HEADB}
  COMMAND rm -f ${08_day_harib05b_BOOTB}
  COMMAND rm -f ${08_day_harib05b_WILDOBJ}
)
add_custom_target(08_day_harib05b_ipl
  COMMAND ${NASK} ${08_day_harib05b_IPLS} ${08_day_harib05b_IPLB}
)
add_custom_target(08_day_harib05b_sys
  COMMAND ${NASK} ${08_day_harib05b_HEADS} ${08_day_harib05b_HEADB}
  COMMAND ${FONT} ${08_day_harib05b_FONTS} ${08_day_harib05b_FONTB}
  COMMAND ${B2O}  ${08_day_harib05b_FONTB} ${08_day_harib05b_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${08_day_harib05b_LIBGE} ${08_day_harib05b_LIBGC}
  COMMAND ${NASK} ${08_day_harib05b_FUNCS} ${08_day_harib05b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${08_day_harib05b_LDS} ${08_day_harib05b_CCS} ${08_day_harib05b_WILDOBJ} -o ${08_day_harib05b_BOOTB}
  COMMAND cat ${08_day_harib05b_HEADB} ${08_day_harib05b_BOOTB} > ${08_day_harib05b_SYS}
  DEPENDS 08_day_harib05b_ipl
)
add_custom_target(08_day_harib05b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${08_day_harib05b_IPLB} -i ${08_day_harib05b_OS}
  COMMAND mcopy -i ${08_day_harib05b_OS} ${08_day_harib05b_SYS} ::
  DEPENDS 08_day_harib05b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/asmhead_wine.img)

add_custom_target(08_day_harib05b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(08_day_harib05b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(08_day_harib05b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(08_day_harib05b_asmhead_od 08_day_harib05b_asmhead)
add_dependencies(08_day_harib05b_asmhead_od 08_day_harib05b_asmhead_wine)

add_dependencies(images 08_day_harib05b_asmhead)
add_dependencies(wine 08_day_harib05b_asmhead_wine)
add_dependencies(od 08_day_harib05b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/ipl10_wine.img)

add_custom_target(08_day_harib05b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(08_day_harib05b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(08_day_harib05b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(08_day_harib05b_ipl10_od 08_day_harib05b_ipl10)
add_dependencies(08_day_harib05b_ipl10_od 08_day_harib05b_ipl10_wine)

add_dependencies(images 08_day_harib05b_ipl10)
add_dependencies(wine 08_day_harib05b_ipl10_wine)
add_dependencies(od 08_day_harib05b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/naskfunc_wine.img)

add_custom_target(08_day_harib05b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(08_day_harib05b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(08_day_harib05b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(08_day_harib05b_naskfunc_od 08_day_harib05b_naskfunc)
add_dependencies(08_day_harib05b_naskfunc_od 08_day_harib05b_naskfunc_wine)

add_dependencies(images 08_day_harib05b_naskfunc)
add_dependencies(wine 08_day_harib05b_naskfunc_wine)
add_dependencies(od 08_day_harib05b_naskfunc_od)

#----------------------------------------------------------
