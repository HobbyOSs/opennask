#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(07_day_harib04c_OS    ${root_BINARY_DIR}/projects/07_day/harib04c/os.img)
set(07_day_harib04c_SYS	  ${root_BINARY_DIR}/projects/07_day/harib04c/os.sys)
set(07_day_harib04c_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04c/ipl.bin)
set(07_day_harib04c_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04c/ipl10.nas)
set(07_day_harib04c_HEADB ${root_BINARY_DIR}/projects/07_day/harib04c/asmhead.bin)
set(07_day_harib04c_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04c/asmhead.nas)
set(07_day_harib04c_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04c/boot.bin)
set(07_day_harib04c_CCO	  ${root_BINARY_DIR}/projects/07_day/harib04c/boot.o)
set(07_day_harib04c_CCS	  ${root_SOURCE_DIR}/projects/07_day/harib04c/*.c)
set(07_day_harib04c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04c_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04c/naskfunc.nas)
set(07_day_harib04c_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04c/naskfunc.o)
set(07_day_harib04c_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04c/hankaku.txt)
set(07_day_harib04c_FONTB ${root_BINARY_DIR}/projects/07_day/harib04c/hankaku.bin)
set(07_day_harib04c_FONTO ${root_BINARY_DIR}/projects/07_day/harib04c/hankaku.o)
set(07_day_harib04c_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04c/*.o)

add_custom_target(07_day_harib04c_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04c_OS}
  DEPENDS 07_day_harib04c_img
)
add_custom_target(07_day_harib04c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04c_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 07_day_harib04c_img
)
add_custom_target(07_day_harib04c_clean
  COMMAND rm -f ${07_day_harib04c_OS}
  COMMAND rm -f ${07_day_harib04c_SYS}
  COMMAND rm -f ${07_day_harib04c_IPLB}
  COMMAND rm -f ${07_day_harib04c_HEADB}
  COMMAND rm -f ${07_day_harib04c_BOOTB}
  COMMAND rm -f ${07_day_harib04c_WILDOBJ}
)
add_custom_target(07_day_harib04c_ipl
  COMMAND ${NASK} ${07_day_harib04c_IPLS} ${07_day_harib04c_IPLB}
)
add_custom_target(07_day_harib04c_sys
  COMMAND ${NASK} ${07_day_harib04c_HEADS} ${07_day_harib04c_HEADB}
  COMMAND ${FONT} ${07_day_harib04c_FONTS} ${07_day_harib04c_FONTB}
  COMMAND ${B2O} ${07_day_harib04c_FONTB} ${07_day_harib04c_FONTO} _hankaku
  COMMAND ${NASK} ${07_day_harib04c_FUNCS} ${07_day_harib04c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04c_LDS} ${07_day_harib04c_CCS} ${07_day_harib04c_WILDOBJ} -o ${07_day_harib04c_BOOTB}
  COMMAND cat ${07_day_harib04c_HEADB} ${07_day_harib04c_BOOTB} > ${07_day_harib04c_SYS}
  DEPENDS 07_day_harib04c_ipl
)
add_custom_target(07_day_harib04c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04c_IPLB} -i ${07_day_harib04c_OS}
  COMMAND mcopy -i ${07_day_harib04c_OS} ${07_day_harib04c_SYS} ::
  DEPENDS 07_day_harib04c_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/asmhead_wine.img)

add_custom_target(07_day_harib04c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(07_day_harib04c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(07_day_harib04c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04c_asmhead_od 07_day_harib04c_asmhead)
add_dependencies(07_day_harib04c_asmhead_od 07_day_harib04c_asmhead_wine)

add_dependencies(images 07_day_harib04c_asmhead)
add_dependencies(wine 07_day_harib04c_asmhead_wine)
add_dependencies(od 07_day_harib04c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/ipl10_wine.img)

add_custom_target(07_day_harib04c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(07_day_harib04c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(07_day_harib04c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04c_ipl10_od 07_day_harib04c_ipl10)
add_dependencies(07_day_harib04c_ipl10_od 07_day_harib04c_ipl10_wine)

add_dependencies(images 07_day_harib04c_ipl10)
add_dependencies(wine 07_day_harib04c_ipl10_wine)
add_dependencies(od 07_day_harib04c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04c/naskfunc_wine.img)

add_custom_target(07_day_harib04c_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(07_day_harib04c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(07_day_harib04c_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04c_naskfunc_od 07_day_harib04c_naskfunc)
add_dependencies(07_day_harib04c_naskfunc_od 07_day_harib04c_naskfunc_wine)

add_dependencies(images 07_day_harib04c_naskfunc)
add_dependencies(wine 07_day_harib04c_naskfunc_wine)
add_dependencies(od 07_day_harib04c_naskfunc_od)

#----------------------------------------------------------
