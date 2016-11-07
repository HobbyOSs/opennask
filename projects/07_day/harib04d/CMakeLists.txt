#----------------------------------------------------------
message(STATUS "Entering directory projects/07_day/harib04d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(07_day_harib04d_OS    ${root_BINARY_DIR}/projects/07_day/harib04d/os.img)
set(07_day_harib04d_SYS	  ${root_BINARY_DIR}/projects/07_day/harib04d/os.sys)
set(07_day_harib04d_IPLB  ${root_BINARY_DIR}/projects/07_day/harib04d/ipl.bin)
set(07_day_harib04d_IPLS  ${root_SOURCE_DIR}/projects/07_day/harib04d/ipl10.nas)
set(07_day_harib04d_HEADB ${root_BINARY_DIR}/projects/07_day/harib04d/asmhead.bin)
set(07_day_harib04d_HEADS ${root_SOURCE_DIR}/projects/07_day/harib04d/asmhead.nas)
set(07_day_harib04d_BOOTB ${root_BINARY_DIR}/projects/07_day/harib04d/boot.bin)
set(07_day_harib04d_CCO	  ${root_BINARY_DIR}/projects/07_day/harib04d/boot.o)
set(07_day_harib04d_CCS	  ${root_SOURCE_DIR}/projects/07_day/harib04d/*.c)
set(07_day_harib04d_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)

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
  COMMAND rm -f ${07_day_harib04d_CCO}
)
add_custom_target(07_day_harib04d_ipl
  COMMAND ${NASK} ${07_day_harib04d_IPLS} ${07_day_harib04d_IPLB}
)
add_custom_target(07_day_harib04d_sys
  COMMAND ${NASK} ${07_day_harib04d_HEADS} ${07_day_harib04d_HEADB}
  COMMAND gcc ${07_day_harib04d_CCS}  -c -o ${07_day_harib04d_CCO}
  COMMAND ld -o ${07_day_harib04d_BOOTB} -e HariMain -T ${07_day_harib04d_LDS} ${07_day_harib04d_CCO} ${07_day_harib04d_FUNCO}
  COMMAND cat ${07_day_harib04d_HEADB} ${07_day_harib04d_BOOTB} > ${07_day_harib04d_SYS}
  DEPENDS 07_day_harib04d_ipl
)
add_custom_target(07_day_harib04d_img
  COMMAND mformat -f 1440 -C -B ${07_day_harib04d_IPLB} -i ${07_day_harib04d_OS}
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
)

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
)

add_dependencies(images 07_day_harib04d_ipl10)
add_dependencies(wine 07_day_harib04d_ipl10_wine)
add_dependencies(od 07_day_harib04d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/07_day/harib04d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/07_day/harib04d_wine.imgkfunc_wine.img)

add_custom_target(07_day_harib04dkfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(07_day_harib04dkfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(07_day_harib04dkfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 07_day_harib04dkfunc)
add_dependencies(wine 07_day_harib04dkfunc_wine)
add_dependencies(od 07_day_harib04dkfunc_od)

#----------------------------------------------------------
