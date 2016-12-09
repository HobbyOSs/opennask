#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(07_day_harib04f_OS    ${root_BINARY_DIR}/projects/07_day/harib04f/os.img)
set(07_day_harib04f_SYS	  ${root_BINARY_DIR}/projects/07_day/harib04f/os.sys)
set(07_day_harib04f_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04f/ipl.bin)
set(07_day_harib04f_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04f/ipl10.nas)
set(07_day_harib04f_HEADB ${root_BINARY_DIR}/projects/07_day/harib04f/asmhead.bin)
set(07_day_harib04f_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04f/asmhead.nas)
set(07_day_harib04f_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04f/boot.bin)
set(07_day_harib04f_CCO	  ${root_BINARY_DIR}/projects/07_day/harib04f/boot.o)
set(07_day_harib04f_CCS	  ${root_SOURCE_DIR}/projects/07_day/harib04f/*.c)
set(07_day_harib04f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04f_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04f/naskfunc.nas)
set(07_day_harib04f_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04f/naskfunc.o)
set(07_day_harib04f_FONTS ${root_SOURCE_DIR}/projects/07_day/harib04f/hankaku.txt)
set(07_day_harib04f_FONTB ${root_BINARY_DIR}/projects/07_day/harib04f/hankaku.bin)
set(07_day_harib04f_FONTO ${root_BINARY_DIR}/projects/07_day/harib04f/hankaku.o)
set(07_day_harib04f_WILDOBJ ${root_BINARY_DIR}/projects/07_day/harib04f/*.o)

add_custom_target(07_day_harib04f_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04f_OS}
  DEPENDS 07_day_harib04f_img
)
add_custom_target(07_day_harib04f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04f_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 07_day_harib04f_img
)
add_custom_target(07_day_harib04f_clean
  COMMAND rm -f ${07_day_harib04f_OS}
  COMMAND rm -f ${07_day_harib04f_SYS}
  COMMAND rm -f ${07_day_harib04f_IPLB}
  COMMAND rm -f ${07_day_harib04f_HEADB}
  COMMAND rm -f ${07_day_harib04f_BOOTB}
  COMMAND rm -f ${07_day_harib04f_WILDOBJ}
)
add_custom_target(07_day_harib04f_ipl
  COMMAND ${NASK} ${07_day_harib04f_IPLS} ${07_day_harib04f_IPLB}
)
add_custom_target(07_day_harib04f_sys
  COMMAND ${NASK} ${07_day_harib04f_HEADS} ${07_day_harib04f_HEADB}
  COMMAND ${FONT} ${07_day_harib04f_FONTS} ${07_day_harib04f_FONTB}
  COMMAND ${B2O} ${07_day_harib04f_FONTB} ${07_day_harib04f_FONTO} _hankaku
  COMMAND ${NASK} ${07_day_harib04f_FUNCS} ${07_day_harib04f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04f_LDS} ${07_day_harib04f_CCS} ${07_day_harib04f_WILDOBJ} -o ${07_day_harib04f_BOOTB}
  COMMAND cat ${07_day_harib04f_HEADB} ${07_day_harib04f_BOOTB} > ${07_day_harib04f_SYS}
  DEPENDS 07_day_harib04f_ipl
)
add_custom_target(07_day_harib04f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04f_IPLB} -i ${07_day_harib04f_OS}
  COMMAND mcopy -i ${07_day_harib04f_OS} ${07_day_harib04f_SYS} ::
  DEPENDS 07_day_harib04f_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/asmhead_wine.img)

add_custom_target(07_day_harib04f_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(07_day_harib04f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(07_day_harib04f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04f_asmhead_od 07_day_harib04f_asmhead)
add_dependencies(07_day_harib04f_asmhead_od 07_day_harib04f_asmhead_wine)

add_dependencies(images 07_day_harib04f_asmhead)
add_dependencies(wine 07_day_harib04f_asmhead_wine)
add_dependencies(od 07_day_harib04f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/ipl10_wine.img)

add_custom_target(07_day_harib04f_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(07_day_harib04f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(07_day_harib04f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04f_ipl10_od 07_day_harib04f_ipl10)
add_dependencies(07_day_harib04f_ipl10_od 07_day_harib04f_ipl10_wine)

add_dependencies(images 07_day_harib04f_ipl10)
add_dependencies(wine 07_day_harib04f_ipl10_wine)
add_dependencies(od 07_day_harib04f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04f/naskfunc_wine.img)

add_custom_target(07_day_harib04f_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(07_day_harib04f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(07_day_harib04f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(07_day_harib04f_naskfunc_od 07_day_harib04f_naskfunc)
add_dependencies(07_day_harib04f_naskfunc_od 07_day_harib04f_naskfunc_wine)

add_dependencies(images 07_day_harib04f_naskfunc)
add_dependencies(wine 07_day_harib04f_naskfunc_wine)
add_dependencies(od 07_day_harib04f_naskfunc_od)

#----------------------------------------------------------
