#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01g/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(04_day_harib01g_OS    ${root_BINARY_DIR}/projects/04_day/harib01g/os.img)
set(04_day_harib01g_SYS	  ${root_BINARY_DIR}/projects/04_day/harib01g/os.sys)
set(04_day_harib01g_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01g/ipl.bin)
set(04_day_harib01g_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01g/ipl10.nas)
set(04_day_harib01g_HEADB ${root_BINARY_DIR}/projects/04_day/harib01g/asmhead.bin)
set(04_day_harib01g_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01g/asmhead.nas)
set(04_day_harib01g_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01g/boot.bin)
set(04_day_harib01g_CCO	  ${root_BINARY_DIR}/projects/04_day/harib01g/boot.o)
set(04_day_harib01g_CCS	  ${root_SOURCE_DIR}/projects/04_day/harib01g/*.c)
set(04_day_harib01g_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)

add_custom_target(04_day_harib01g_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01g_OS}
  DEPENDS 04_day_harib01g_img
)
add_custom_target(04_day_harib01g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01g_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 04_day_harib01g_img
)
add_custom_target(04_day_harib01g_clean
  COMMAND rm -f ${04_day_harib01g_OS}
  COMMAND rm -f ${04_day_harib01g_SYS}
  COMMAND rm -f ${04_day_harib01g_IPLB}
  COMMAND rm -f ${04_day_harib01g_HEADB}
  COMMAND rm -f ${04_day_harib01g_BOOTB}
  COMMAND rm -f ${04_day_harib01g_CCO}
)
add_custom_target(04_day_harib01g_ipl
  COMMAND ${NASK} ${04_day_harib01g_IPLS} ${04_day_harib01g_IPLB}
)
add_custom_target(04_day_harib01g_sys
  COMMAND ${NASK} ${04_day_harib01g_HEADS} ${04_day_harib01g_HEADB}
  COMMAND gcc ${04_day_harib01g_CCS}  -c -o ${04_day_harib01g_CCO}
  COMMAND ld -o ${04_day_harib01g_BOOTB} -e HariMain -T ${04_day_harib01g_LDS} ${04_day_harib01g_CCO} ${04_day_harib01g_FUNCO}
  COMMAND cat ${04_day_harib01g_HEADB} ${04_day_harib01g_BOOTB} > ${04_day_harib01g_SYS}
  DEPENDS 04_day_harib01g_ipl
)
add_custom_target(04_day_harib01g_img
  COMMAND mformat -f 1440 -C -B ${04_day_harib01g_IPLB} -i ${04_day_harib01g_OS}
  COMMAND mcopy -i ${04_day_harib01g_OS} ${04_day_harib01g_SYS} ::
  DEPENDS 04_day_harib01g_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01g/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g/asmhead_wine.img)

add_custom_target(04_day_harib01g_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(04_day_harib01g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(04_day_harib01g_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 04_day_harib01g_asmhead)
add_dependencies(wine 04_day_harib01g_asmhead_wine)
add_dependencies(od 04_day_harib01g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01g/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g/ipl10_wine.img)

add_custom_target(04_day_harib01g_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(04_day_harib01g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(04_day_harib01g_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 04_day_harib01g_ipl10)
add_dependencies(wine 04_day_harib01g_ipl10_wine)
add_dependencies(od 04_day_harib01g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01g/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01g_wine.imgkfunc_wine.img)

add_custom_target(04_day_harib01gkfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(04_day_harib01gkfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(04_day_harib01gkfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 04_day_harib01gkfunc)
add_dependencies(wine 04_day_harib01gkfunc_wine)
add_dependencies(od 04_day_harib01gkfunc_od)

#----------------------------------------------------------
