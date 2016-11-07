#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(07_day_harib04e_OS    ${root_BINARY_DIR}/projects/07_day/harib04e/os.img)
set(07_day_harib04e_SYS	  ${root_BINARY_DIR}/projects/07_day/harib04e/os.sys)
set(07_day_harib04e_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04e/ipl.bin)
set(07_day_harib04e_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04e/ipl10.nas)
set(07_day_harib04e_HEADB ${root_BINARY_DIR}/projects/07_day/harib04e/asmhead.bin)
set(07_day_harib04e_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04e/asmhead.nas)
set(07_day_harib04e_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04e/boot.bin)
set(07_day_harib04e_CCO	  ${root_BINARY_DIR}/projects/07_day/harib04e/boot.o)
set(07_day_harib04e_CCS	  ${root_SOURCE_DIR}/projects/07_day/harib04e/*.c)
set(07_day_harib04e_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)
set(07_day_harib04e_FUNCO ${root_BINARY_DIR}/projects/07_day/harib04e/naskfunc.o)
set(07_day_harib04e_FUNCS ${root_SOURCE_DIR}/projects/07_day/harib04e/naskfunc.nas)

add_custom_target(07_day_harib04e_run
  COMMAND ${QEMU} ${QEMUOPT} ${07_day_harib04e_OS}
  DEPENDS 07_day_harib04e_img
)
add_custom_target(07_day_harib04e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${07_day_harib04e_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 07_day_harib04e_img
)
add_custom_target(07_day_harib04e_clean
  COMMAND rm -f ${07_day_harib04e_OS}
  COMMAND rm -f ${07_day_harib04e_SYS}
  COMMAND rm -f ${07_day_harib04e_IPLB}
  COMMAND rm -f ${07_day_harib04e_HEADB}
  COMMAND rm -f ${07_day_harib04e_BOOTB}
  COMMAND rm -f ${07_day_harib04e_CCO}
  COMMAND rm -f ${07_day_harib04e_FUNCO}
)
add_custom_target(07_day_harib04e_ipl
  COMMAND ${NASK} ${07_day_harib04e_IPLS} ${07_day_harib04e_IPLB}
)
add_custom_target(07_day_harib04e_sys
  COMMAND ${NASK} ${07_day_harib04e_FUNCS} ${07_day_harib04e_FUNCO}
  COMMAND ${NASK} ${07_day_harib04e_HEADS} ${07_day_harib04e_HEADB}
  COMMAND gcc ${07_day_harib04e_CCS}  -c -o ${07_day_harib04e_CCO}
  COMMAND ld -o ${07_day_harib04e_BOOTB} -e HariMain -T ${07_day_harib04e_LDS} ${07_day_harib04e_CCO} ${07_day_harib04e_FUNCO}
  COMMAND cat ${07_day_harib04e_HEADB} ${07_day_harib04e_BOOTB} > ${07_day_harib04e_SYS}
  DEPENDS 07_day_harib04e_ipl
)
add_custom_target(07_day_harib04e_img
  COMMAND mformat -f 1440 -C -B ${07_day_harib04e_IPLB} -i ${07_day_harib04e_OS}
  COMMAND mcopy -i ${07_day_harib04e_OS} ${07_day_harib04e_SYS} ::
  DEPENDS 07_day_harib04e_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/asmhead_wine.img)

add_custom_target(07_day_harib04e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(07_day_harib04e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(07_day_harib04e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 07_day_harib04e_asmhead)
add_dependencies(wine 07_day_harib04e_asmhead_wine)
add_dependencies(od 07_day_harib04e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/ipl10_wine.img)

add_custom_target(07_day_harib04e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(07_day_harib04e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(07_day_harib04e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 07_day_harib04e_ipl10)
add_dependencies(wine 07_day_harib04e_ipl10_wine)
add_dependencies(od 07_day_harib04e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04e/naskfunc_wine.img)

add_custom_target(07_day_harib04e_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(07_day_harib04e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(07_day_harib04e_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 07_day_harib04e_naskfunc)
add_dependencies(wine 07_day_harib04e_naskfunc_wine)
add_dependencies(od 07_day_harib04e_naskfunc_od)

#----------------------------------------------------------
