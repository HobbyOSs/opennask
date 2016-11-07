#----------------------------------------------------------
message(STATUS "Entering directory projects/09_day/harib06d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(09_day_harib06d_OS    ${root_BINARY_DIR}/projects/09_day/harib06d/os.img)
set(09_day_harib06d_SYS	  ${root_BINARY_DIR}/projects/09_day/harib06d/os.sys)
set(09_day_harib06d_IPLB  ${root_BINARY_DIR}/projects/09_day/harib06d/ipl.bin)
set(09_day_harib06d_IPLS  ${root_SOURCE_DIR}/projects/09_day/harib06d/ipl10.nas)
set(09_day_harib06d_HEADB ${root_BINARY_DIR}/projects/09_day/harib06d/asmhead.bin)
set(09_day_harib06d_HEADS ${root_SOURCE_DIR}/projects/09_day/harib06d/asmhead.nas)
set(09_day_harib06d_BOOTB ${root_BINARY_DIR}/projects/09_day/harib06d/boot.bin)
set(09_day_harib06d_CCO	  ${root_BINARY_DIR}/projects/09_day/harib06d/boot.o)
set(09_day_harib06d_CCS	  ${root_SOURCE_DIR}/projects/09_day/harib06d/*.c)
set(09_day_harib06d_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)

add_custom_target(09_day_harib06d_run
  COMMAND ${QEMU} ${QEMUOPT} ${09_day_harib06d_OS}
  DEPENDS 09_day_harib06d_img
)
add_custom_target(09_day_harib06d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${09_day_harib06d_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 09_day_harib06d_img
)
add_custom_target(09_day_harib06d_clean
  COMMAND rm -f ${09_day_harib06d_OS}
  COMMAND rm -f ${09_day_harib06d_SYS}
  COMMAND rm -f ${09_day_harib06d_IPLB}
  COMMAND rm -f ${09_day_harib06d_HEADB}
  COMMAND rm -f ${09_day_harib06d_BOOTB}
  COMMAND rm -f ${09_day_harib06d_CCO}
)
add_custom_target(09_day_harib06d_ipl
  COMMAND ${NASK} ${09_day_harib06d_IPLS} ${09_day_harib06d_IPLB}
)
add_custom_target(09_day_harib06d_sys
  COMMAND ${NASK} ${09_day_harib06d_HEADS} ${09_day_harib06d_HEADB}
  COMMAND gcc ${09_day_harib06d_CCS}  -c -o ${09_day_harib06d_CCO}
  COMMAND ld -o ${09_day_harib06d_BOOTB} -e HariMain -T ${09_day_harib06d_LDS} ${09_day_harib06d_CCO} ${09_day_harib06d_FUNCO}
  COMMAND cat ${09_day_harib06d_HEADB} ${09_day_harib06d_BOOTB} > ${09_day_harib06d_SYS}
  DEPENDS 09_day_harib06d_ipl
)
add_custom_target(09_day_harib06d_img
  COMMAND mformat -f 1440 -C -B ${09_day_harib06d_IPLB} -i ${09_day_harib06d_OS}
  COMMAND mcopy -i ${09_day_harib06d_OS} ${09_day_harib06d_SYS} ::
  DEPENDS 09_day_harib06d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/asmhead_wine.img)

add_custom_target(09_day_harib06d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(09_day_harib06d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(09_day_harib06d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 09_day_harib06d_asmhead)
add_dependencies(wine 09_day_harib06d_asmhead_wine)
add_dependencies(od 09_day_harib06d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d/ipl10_wine.img)

add_custom_target(09_day_harib06d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(09_day_harib06d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(09_day_harib06d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 09_day_harib06d_ipl10)
add_dependencies(wine 09_day_harib06d_ipl10_wine)
add_dependencies(od 09_day_harib06d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06d_wine.imgkfunc_wine.img)

add_custom_target(09_day_harib06dkfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(09_day_harib06dkfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(09_day_harib06dkfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 09_day_harib06dkfunc)
add_dependencies(wine 09_day_harib06dkfunc_wine)
add_dependencies(od 09_day_harib06dkfunc_od)

#----------------------------------------------------------
