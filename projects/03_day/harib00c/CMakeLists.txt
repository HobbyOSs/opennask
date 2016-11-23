#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(03_day_harib00c_OS    ${root_BINARY_DIR}/projects/03_day/harib00c/os.img)
set(03_day_harib00c_SYS	  ${root_BINARY_DIR}/projects/03_day/harib00c/os.sys)
set(03_day_harib00c_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00c/ipl.bin)
set(03_day_harib00c_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00c/ipl10.nas)
set(03_day_harib00c_HEADB ${root_BINARY_DIR}/projects/03_day/harib00c/asmhead.bin)
set(03_day_harib00c_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00c/asmhead.nas)
set(03_day_harib00c_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00c/boot.bin)
set(03_day_harib00c_CCO	  ${root_BINARY_DIR}/projects/03_day/harib00c/boot.o)
set(03_day_harib00c_CCS	  ${root_SOURCE_DIR}/projects/03_day/harib00c/*.c)
set(03_day_harib00c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)

add_custom_target(03_day_harib00c_ipl_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00c_OS}
  DEPENDS 03_day_harib00c_img
)
add_custom_target(03_day_harib00c_ipl_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00c_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 03_day_harib00c_img
)
add_custom_target(03_day_harib00c_ipl_clean
  COMMAND rm -f ${03_day_harib00c_OS}
  COMMAND rm -f ${03_day_harib00c_SYS}
  COMMAND rm -f ${03_day_harib00c_IPLB}
  COMMAND rm -f ${03_day_harib00c_HEADB}
  COMMAND rm -f ${03_day_harib00c_BOOTB}
  COMMAND rm -f ${03_day_harib00c_CCO}
)
add_custom_target(03_day_harib00c_ipl_ipl
  COMMAND ${NASK} ${03_day_harib00c_IPLS} ${03_day_harib00c_IPLB}
)
add_custom_target(03_day_harib00c_ipl_sys
  COMMAND ${NASK} ${03_day_harib00c_HEADS} ${03_day_harib00c_HEADB}
  COMMAND gcc ${BINOPT} -T ${03_day_harib00c_LDS} ${03_day_harib00c_CCS} -o ${03_day_harib00c_BOOTB}
  COMMAND cat ${03_day_harib00c_HEADB} ${03_day_harib00c_BOOTB} > ${03_day_harib00c_SYS}
  DEPENDS 03_day_harib00c_ipl
)
add_custom_target(03_day_harib00c_ipl_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${03_day_harib00c_IPLB} -i ${03_day_harib00c_OS}
  COMMAND mcopy -i ${03_day_harib00c_OS} ${03_day_harib00c_SYS} ::
  DEPENDS 03_day_harib00c_sys
)
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00c/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00c/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00c/ipl_wine.img)

add_custom_target(03_day_harib00c_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)
add_custom_target(03_day_harib00c_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(03_day_harib00c_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(images 03_day_harib00c_ipl)
add_dependencies(wine 03_day_harib00c_ipl_wine)
add_dependencies(od 03_day_harib00c_ipl_od)

#----------------------------------------------------------
