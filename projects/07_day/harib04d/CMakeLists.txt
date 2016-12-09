#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(07_day_harib04d_OS    ${root_BINARY_DIR}/projects/07_day/harib04d/os.img)
set(07_day_harib04d_SYS	  ${root_BINARY_DIR}/projects/07_day/harib04d/os.sys)
set(07_day_harib04d_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04d/ipl.bin)
set(07_day_harib04d_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04d/ipl10.nas)
set(07_day_harib04d_HEADB ${root_BINARY_DIR}/projects/07_day/harib04d/asmhead.bin)
set(07_day_harib04d_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04d/asmhead.nas)
set(07_day_harib04d_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04d/boot.bin)
set(07_day_harib04d_CCO	  ${root_BINARY_DIR}/projects/07_day/harib04d/boot.o)
set(07_day_harib04d_CCS	  ${root_SOURCE_DIR}/projects/07_day/harib04d/*.c)
set(07_day_harib04d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04d_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04d/naskfunc.nas)
set(07_day_harib04d_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04d/naskfunc.o)
set(07_day_harib04d_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04d/hankaku.txt)
set(07_day_harib04d_FONTB ${root_BINARY_DIR}/projects/07_day/harib04d/hankaku.bin)
set(07_day_harib04d_FONTO ${root_BINARY_DIR}/projects/07_day/harib04d/hankaku.o)
set(07_day_harib04d_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04d/*.o)

add_custom_target(07_day_harib04d_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04d_OS}
  DEPENDS 07_day_harib04d_img
)
add_custom_target(07_day_harib04d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04d_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 07_day_harib04d_img
)
add_custom_target(07_day_harib04d_clean
  COMMAND rm -f ${07_day_harib04d_OS}
  COMMAND rm -f ${07_day_harib04d_SYS}
  COMMAND rm -f ${07_day_harib04d_IPLB}
  COMMAND rm -f ${07_day_harib04d_HEADB}
  COMMAND rm -f ${07_day_harib04d_BOOTB}
  COMMAND rm -f ${07_day_harib04d_WILDOBJ}
)
add_custom_target(07_day_harib04d_ipl
  COMMAND ${NASK} ${07_day_harib04d_IPLS} ${07_day_harib04d_IPLB}
)
add_custom_target(07_day_harib04d_sys
  COMMAND ${NASK} ${07_day_harib04d_HEADS} ${07_day_harib04d_HEADB}
  COMMAND ${FONT} ${07_day_harib04d_FONTS} ${07_day_harib04d_FONTB}
  COMMAND ${B2O} ${07_day_harib04d_FONTB} ${07_day_harib04d_FONTO} _hankaku
  COMMAND ${NASK} ${07_day_harib04d_FUNCS} ${07_day_harib04d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04d_LDS} ${07_day_harib04d_CCS} ${07_day_harib04d_WILDOBJ} -o ${07_day_harib04d_BOOTB}
  COMMAND cat ${07_day_harib04d_HEADB} ${07_day_harib04d_BOOTB} > ${07_day_harib04d_SYS}
  DEPENDS 07_day_harib04d_ipl
)
add_custom_target(07_day_harib04d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04d_IPLB} -i ${07_day_harib04d_OS}
  COMMAND mcopy -i ${07_day_harib04d_OS} ${07_day_harib04d_SYS} ::
  DEPENDS 07_day_harib04d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/asmhead_wine.img)

add_custom_target(07_day_harib04d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(07_day_harib04d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(07_day_harib04d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04d_asmhead_od 07_day_harib04d_asmhead)
add_dependencies(07_day_harib04d_asmhead_od 07_day_harib04d_asmhead_wine)

add_dependencies(images 07_day_harib04d_asmhead)
add_dependencies(wine 07_day_harib04d_asmhead_wine)
add_dependencies(od 07_day_harib04d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/ipl10_wine.img)

add_custom_target(07_day_harib04d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(07_day_harib04d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(07_day_harib04d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04d_ipl10_od 07_day_harib04d_ipl10)
add_dependencies(07_day_harib04d_ipl10_od 07_day_harib04d_ipl10_wine)

add_dependencies(images 07_day_harib04d_ipl10)
add_dependencies(wine 07_day_harib04d_ipl10_wine)
add_dependencies(od 07_day_harib04d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d/naskfunc_wine.img)

add_custom_target(07_day_harib04d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(07_day_harib04d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(07_day_harib04d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04d_naskfunc_od 07_day_harib04d_naskfunc)
add_dependencies(07_day_harib04d_naskfunc_od 07_day_harib04d_naskfunc_wine)

add_dependencies(images 07_day_harib04d_naskfunc)
add_dependencies(wine 07_day_harib04d_naskfunc_wine)
add_dependencies(od 07_day_harib04d_naskfunc_od)

#----------------------------------------------------------
