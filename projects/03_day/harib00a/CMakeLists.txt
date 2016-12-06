#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(03_day_harib00a_OS    ${root_BINARY_DIR}/projects/03_day/harib00a/os.img)
set(03_day_harib00a_SYS	  ${root_BINARY_DIR}/projects/03_day/harib00a/os.sys)
set(03_day_harib00a_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00a/ipl.bin)
set(03_day_harib00a_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00a/ipl10.nas)
set(03_day_harib00a_HEADB ${root_BINARY_DIR}/projects/03_day/harib00a/asmhead.bin)
set(03_day_harib00a_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00a/asmhead.nas)
set(03_day_harib00a_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00a/boot.bin)
set(03_day_harib00a_CCO	  ${root_BINARY_DIR}/projects/03_day/harib00a/boot.o)
set(03_day_harib00a_CCS	  ${root_SOURCE_DIR}/projects/03_day/harib00a/*.c)
set(03_day_harib00a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)

add_custom_target(03_day_harib00a_ipl_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00a_OS}
  DEPENDS 03_day_harib00a_img
)
add_custom_target(03_day_harib00a_ipl_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00a_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 03_day_harib00a_img
)
add_custom_target(03_day_harib00a_ipl_clean
  COMMAND rm -f ${03_day_harib00a_OS}
  COMMAND rm -f ${03_day_harib00a_SYS}
  COMMAND rm -f ${03_day_harib00a_IPLB}
  COMMAND rm -f ${03_day_harib00a_HEADB}
  COMMAND rm -f ${03_day_harib00a_BOOTB}
  COMMAND rm -f ${03_day_harib00a_CCO}
)
add_custom_target(03_day_harib00a_ipl_ipl
  COMMAND ${NASK} ${03_day_harib00a_IPLS} ${03_day_harib00a_IPLB}
)
add_custom_target(03_day_harib00a_ipl_sys
  COMMAND ${NASK} ${03_day_harib00a_HEADS} ${03_day_harib00a_HEADB}
  COMMAND gcc ${BINOPT} -T ${03_day_harib00a_LDS} ${03_day_harib00a_CCS} -o ${03_day_harib00a_BOOTB}
  COMMAND cat ${03_day_harib00a_HEADB} ${03_day_harib00a_BOOTB} > ${03_day_harib00a_SYS}
  DEPENDS 03_day_harib00a_ipl
)
add_custom_target(03_day_harib00a_ipl_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${03_day_harib00a_IPLB} -i ${03_day_harib00a_OS}
  COMMAND mcopy -i ${03_day_harib00a_OS} ${03_day_harib00a_SYS} ::
  DEPENDS 03_day_harib00a_sys
)
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00a/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00a/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00a/ipl_wine.img)

add_custom_target(03_day_harib00a_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)
add_custom_target(03_day_harib00a_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(03_day_harib00a_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl.img_OUTS}_f.txt ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(03_day_harib00a_ipl_od 03_day_harib00a_ipl)
add_dependencies(03_day_harib00a_ipl_od 03_day_harib00a_ipl_wine)

add_dependencies(images 03_day_harib00a_ipl)
add_dependencies(wine 03_day_harib00a_ipl_wine)
add_dependencies(od 03_day_harib00a_ipl_od)

#----------------------------------------------------------
