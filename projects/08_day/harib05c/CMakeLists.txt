#----------------------------------------------------------
message(STATUS "Entering directory projects/08_day/harib05c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(08_day_harib05c_OS    ${root_BINARY_DIR}/projects/08_day/harib05c/os.img)
set(08_day_harib05c_SYS	  ${root_BINARY_DIR}/projects/08_day/harib05c/os.sys)
set(08_day_harib05c_IPLB  ${root_BINARY_DIR}/projects/08_day/harib05c/ipl.bin)
set(08_day_harib05c_IPLS  ${root_SOURCE_DIR}/projects/08_day/harib05c/ipl10.nas)
set(08_day_harib05c_HEADB ${root_BINARY_DIR}/projects/08_day/harib05c/asmhead.bin)
set(08_day_harib05c_HEADS ${root_SOURCE_DIR}/projects/08_day/harib05c/asmhead.nas)
set(08_day_harib05c_BOOTB ${root_BINARY_DIR}/projects/08_day/harib05c/boot.bin)
set(08_day_harib05c_CCO	  ${root_BINARY_DIR}/projects/08_day/harib05c/boot.o)
set(08_day_harib05c_CCS	  ${root_SOURCE_DIR}/projects/08_day/harib05c/*.c)
set(08_day_harib05c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(08_day_harib05c_FUNCS ${root_SOURCE_DIR}/projects/08_day/harib05c/naskfunc.nas)
set(08_day_harib05c_FUNCO ${root_BINARY_DIR}/projects/08_day/harib05c/naskfunc.o)
set(08_day_harib05c_FONTS ${root_SOURCE_DIR}/projects/08_day/harib05c/hankaku.txt)
set(08_day_harib05c_FONTB ${root_BINARY_DIR}/projects/08_day/harib05c/hankaku.bin)
set(08_day_harib05c_FONTO ${root_BINARY_DIR}/projects/08_day/harib05c/hankaku.o)
set(08_day_harib05c_WILDOBJ ${root_BINARY_DIR}/projects/08_day/harib05c/*.o)

add_custom_target(08_day_harib05c_run
  COMMAND ${QEMU} ${QEMUOPT} ${08_day_harib05c_OS}
  DEPENDS 08_day_harib05c_img
)
add_custom_target(08_day_harib05c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${08_day_harib05c_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 08_day_harib05c_img
)
add_custom_target(08_day_harib05c_clean
  COMMAND rm -f ${08_day_harib05c_OS}
  COMMAND rm -f ${08_day_harib05c_SYS}
  COMMAND rm -f ${08_day_harib05c_IPLB}
  COMMAND rm -f ${08_day_harib05c_HEADB}
  COMMAND rm -f ${08_day_harib05c_BOOTB}
  COMMAND rm -f ${08_day_harib05c_WILDOBJ}
)
add_custom_target(08_day_harib05c_ipl
  COMMAND ${NASK} ${08_day_harib05c_IPLS} ${08_day_harib05c_IPLB}
)
add_custom_target(08_day_harib05c_sys
  COMMAND ${NASK} ${08_day_harib05c_HEADS} ${08_day_harib05c_HEADB}
  COMMAND ${FONT} ${08_day_harib05c_FONTS} ${08_day_harib05c_FONTB}
  COMMAND ${B2O} ${08_day_harib05c_FONTB} ${08_day_harib05c_FONTO} _hankaku
  COMMAND ${NASK} ${08_day_harib05c_FUNCS} ${08_day_harib05c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${08_day_harib05c_LDS} ${08_day_harib05c_CCS} ${08_day_harib05c_WILDOBJ} -o ${08_day_harib05c_BOOTB}
  COMMAND cat ${08_day_harib05c_HEADB} ${08_day_harib05c_BOOTB} > ${08_day_harib05c_SYS}
  DEPENDS 08_day_harib05c_ipl
)
add_custom_target(08_day_harib05c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${08_day_harib05c_IPLB} -i ${08_day_harib05c_OS}
  COMMAND mcopy -i ${08_day_harib05c_OS} ${08_day_harib05c_SYS} ::
  DEPENDS 08_day_harib05c_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/asmhead_wine.img)

add_custom_target(08_day_harib05c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(08_day_harib05c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(08_day_harib05c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(08_day_harib05c_asmhead_od 08_day_harib05c_asmhead)
add_dependencies(08_day_harib05c_asmhead_od 08_day_harib05c_asmhead_wine)

add_dependencies(images 08_day_harib05c_asmhead)
add_dependencies(wine 08_day_harib05c_asmhead_wine)
add_dependencies(od 08_day_harib05c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/ipl10_wine.img)

add_custom_target(08_day_harib05c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(08_day_harib05c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(08_day_harib05c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(08_day_harib05c_ipl10_od 08_day_harib05c_ipl10)
add_dependencies(08_day_harib05c_ipl10_od 08_day_harib05c_ipl10_wine)

add_dependencies(images 08_day_harib05c_ipl10)
add_dependencies(wine 08_day_harib05c_ipl10_wine)
add_dependencies(od 08_day_harib05c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05c/naskfunc_wine.img)

add_custom_target(08_day_harib05c_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(08_day_harib05c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(08_day_harib05c_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(08_day_harib05c_naskfunc_od 08_day_harib05c_naskfunc)
add_dependencies(08_day_harib05c_naskfunc_od 08_day_harib05c_naskfunc_wine)

add_dependencies(images 08_day_harib05c_naskfunc)
add_dependencies(wine 08_day_harib05c_naskfunc_wine)
add_dependencies(od 08_day_harib05c_naskfunc_od)

#----------------------------------------------------------
