#----------------------------------------------------------
message(STATUS "Entering directory projects/09_day/harib06c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(09_day_harib06c_OS    ${root_BINARY_DIR}/projects/09_day/harib06c/os.img)
set(09_day_harib06c_SYS	  ${root_BINARY_DIR}/projects/09_day/harib06c/os.sys)
set(09_day_harib06c_IPLB  ${root_BINARY_DIR}/projects/09_day/harib06c/ipl.bin)
set(09_day_harib06c_IPLS  ${root_SOURCE_DIR}/projects/09_day/harib06c/ipl10.nas)
set(09_day_harib06c_HEADB ${root_BINARY_DIR}/projects/09_day/harib06c/asmhead.bin)
set(09_day_harib06c_HEADS ${root_SOURCE_DIR}/projects/09_day/harib06c/asmhead.nas)
set(09_day_harib06c_BOOTB ${root_BINARY_DIR}/projects/09_day/harib06c/boot.bin)
set(09_day_harib06c_CCO	  ${root_BINARY_DIR}/projects/09_day/harib06c/boot.o)
set(09_day_harib06c_CCS	  ${root_SOURCE_DIR}/projects/09_day/harib06c/*.c)
set(09_day_harib06c_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)

add_custom_target(09_day_harib06c_run
  COMMAND ${QEMU} ${QEMUOPT} ${09_day_harib06c_OS}
  DEPENDS 09_day_harib06c_img
)
add_custom_target(09_day_harib06c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${09_day_harib06c_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 09_day_harib06c_img
)
add_custom_target(09_day_harib06c_clean
  COMMAND rm -f ${09_day_harib06c_OS}
  COMMAND rm -f ${09_day_harib06c_SYS}
  COMMAND rm -f ${09_day_harib06c_IPLB}
  COMMAND rm -f ${09_day_harib06c_HEADB}
  COMMAND rm -f ${09_day_harib06c_BOOTB}
  COMMAND rm -f ${09_day_harib06c_CCO}
)
add_custom_target(09_day_harib06c_ipl
  COMMAND ${NASK} ${09_day_harib06c_IPLS} ${09_day_harib06c_IPLB}
)
add_custom_target(09_day_harib06c_sys
  COMMAND ${NASK} ${09_day_harib06c_HEADS} ${09_day_harib06c_HEADB}
  COMMAND gcc ${09_day_harib06c_CCS}  -c -o ${09_day_harib06c_CCO}
  COMMAND ld -o ${09_day_harib06c_BOOTB} -e HariMain -T ${09_day_harib06c_LDS} ${09_day_harib06c_CCO} ${09_day_harib06c_FUNCO}
  COMMAND cat ${09_day_harib06c_HEADB} ${09_day_harib06c_BOOTB} > ${09_day_harib06c_SYS}
  DEPENDS 09_day_harib06c_ipl
)
add_custom_target(09_day_harib06c_img
  COMMAND mformat -f 1440 -C -B ${09_day_harib06c_IPLB} -i ${09_day_harib06c_OS}
  COMMAND mcopy -i ${09_day_harib06c_OS} ${09_day_harib06c_SYS} ::
  DEPENDS 09_day_harib06c_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c/asmhead_wine.img)

add_custom_target(09_day_harib06c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(09_day_harib06c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(09_day_harib06c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 09_day_harib06c_asmhead)
add_dependencies(wine 09_day_harib06c_asmhead_wine)
add_dependencies(od 09_day_harib06c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c/ipl10_wine.img)

add_custom_target(09_day_harib06c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(09_day_harib06c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(09_day_harib06c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 09_day_harib06c_ipl10)
add_dependencies(wine 09_day_harib06c_ipl10_wine)
add_dependencies(od 09_day_harib06c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06c_wine.imgkfunc_wine.img)

add_custom_target(09_day_harib06ckfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(09_day_harib06ckfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(09_day_harib06ckfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 09_day_harib06ckfunc)
add_dependencies(wine 09_day_harib06ckfunc_wine)
add_dependencies(od 09_day_harib06ckfunc_od)

#----------------------------------------------------------
