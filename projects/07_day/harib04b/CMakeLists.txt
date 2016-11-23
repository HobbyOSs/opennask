#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(07_day_harib04b_OS    ${root_BINARY_DIR}/projects/07_day/harib04b/os.img)
set(07_day_harib04b_SYS	  ${root_BINARY_DIR}/projects/07_day/harib04b/os.sys)
set(07_day_harib04b_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04b/ipl.bin)
set(07_day_harib04b_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04b/ipl10.nas)
set(07_day_harib04b_HEADB ${root_BINARY_DIR}/projects/07_day/harib04b/asmhead.bin)
set(07_day_harib04b_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04b/asmhead.nas)
set(07_day_harib04b_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04b/boot.bin)
set(07_day_harib04b_CCO	  ${root_BINARY_DIR}/projects/07_day/harib04b/boot.o)
set(07_day_harib04b_CCS	  ${root_SOURCE_DIR}/projects/07_day/harib04b/*.c)
set(07_day_harib04b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(07_day_harib04b_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04b/naskfunc.o)
set(07_day_harib04b_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04b/naskfunc.nas)

add_custom_target(07_day_harib04b_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04b_OS}
  DEPENDS 07_day_harib04b_img
)
add_custom_target(07_day_harib04b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 07_day_harib04b_img
)
add_custom_target(07_day_harib04b_clean
  COMMAND rm -f ${07_day_harib04b_OS}
  COMMAND rm -f ${07_day_harib04b_SYS}
  COMMAND rm -f ${07_day_harib04b_IPLB}
  COMMAND rm -f ${07_day_harib04b_HEADB}
  COMMAND rm -f ${07_day_harib04b_BOOTB}
  COMMAND rm -f ${07_day_harib04b_CCO}
  COMMAND rm -f ${07_day_harib04b_FUNCO}
)
add_custom_target(07_day_harib04b_ipl
  COMMAND ${NASK} ${07_day_harib04b_IPLS} ${07_day_harib04b_IPLB}
)
add_custom_target(07_day_harib04b_sys
  COMMAND ${NASK} ${07_day_harib04b_FUNCS} ${07_day_harib04b_FUNCO}
  COMMAND ${NASK} ${07_day_harib04b_HEADS} ${07_day_harib04b_HEADB}
  COMMAND ${NASK} ${07_day_harib04b_FUNCS} ${07_day_harib04b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${07_day_harib04b_LDS} ${07_day_harib04b_CCS} ${07_day_harib04b_FUNCO} -o ${07_day_harib04b_BOOTB}
  COMMAND cat ${07_day_harib04b_HEADB} ${07_day_harib04b_BOOTB} > ${07_day_harib04b_SYS}
  DEPENDS 07_day_harib04b_ipl
)
add_custom_target(07_day_harib04b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${07_day_harib04b_IPLB} -i ${07_day_harib04b_OS}
  COMMAND mcopy -i ${07_day_harib04b_OS} ${07_day_harib04b_SYS} ::
  DEPENDS 07_day_harib04b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/asmhead_wine.img)

add_custom_target(07_day_harib04b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(07_day_harib04b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(07_day_harib04b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 07_day_harib04b_asmhead)
add_dependencies(wine 07_day_harib04b_asmhead_wine)
add_dependencies(od 07_day_harib04b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/ipl10_wine.img)

add_custom_target(07_day_harib04b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(07_day_harib04b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(07_day_harib04b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 07_day_harib04b_ipl10)
add_dependencies(wine 07_day_harib04b_ipl10_wine)
add_dependencies(od 07_day_harib04b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04b/naskfunc_wine.img)

add_custom_target(07_day_harib04b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(07_day_harib04b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(07_day_harib04b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 07_day_harib04b_naskfunc)
add_dependencies(wine 07_day_harib04b_naskfunc_wine)
add_dependencies(od 07_day_harib04b_naskfunc_od)

#----------------------------------------------------------
