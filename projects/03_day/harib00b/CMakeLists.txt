#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(03_day_harib00b_OS    ${root_BINARY_DIR}/projects/03_day/harib00b/os.img)
set(03_day_harib00b_SYS	  ${root_BINARY_DIR}/projects/03_day/harib00b/os.sys)
set(03_day_harib00b_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00b/ipl.bin)
set(03_day_harib00b_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00b/ipl10.nas)
set(03_day_harib00b_HEADB ${root_BINARY_DIR}/projects/03_day/harib00b/asmhead.bin)
set(03_day_harib00b_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00b/asmhead.nas)
set(03_day_harib00b_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00b/boot.bin)
set(03_day_harib00b_CCO	  ${root_BINARY_DIR}/projects/03_day/harib00b/boot.o)
set(03_day_harib00b_CCS	  ${root_SOURCE_DIR}/projects/03_day/harib00b/*.c)
set(03_day_harib00b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)

add_custom_target(03_day_harib00b_ipl_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00b_OS}
  DEPENDS 03_day_harib00b_img
)
add_custom_target(03_day_harib00b_ipl_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 03_day_harib00b_img
)
add_custom_target(03_day_harib00b_ipl_clean
  COMMAND rm -f ${03_day_harib00b_OS}
  COMMAND rm -f ${03_day_harib00b_SYS}
  COMMAND rm -f ${03_day_harib00b_IPLB}
  COMMAND rm -f ${03_day_harib00b_HEADB}
  COMMAND rm -f ${03_day_harib00b_BOOTB}
  COMMAND rm -f ${03_day_harib00b_CCO}
)
add_custom_target(03_day_harib00b_ipl_ipl
  COMMAND ${NASK} ${03_day_harib00b_IPLS} ${03_day_harib00b_IPLB}
)
add_custom_target(03_day_harib00b_ipl_sys
  COMMAND ${NASK} ${03_day_harib00b_HEADS} ${03_day_harib00b_HEADB}
  COMMAND gcc ${BINOPT} -T ${03_day_harib00b_LDS} ${03_day_harib00b_CCS} -o ${03_day_harib00b_BOOTB}
  COMMAND cat ${03_day_harib00b_HEADB} ${03_day_harib00b_BOOTB} > ${03_day_harib00b_SYS}
  DEPENDS 03_day_harib00b_ipl
)
add_custom_target(03_day_harib00b_ipl_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${03_day_harib00b_IPLB} -i ${03_day_harib00b_OS}
  COMMAND mcopy -i ${03_day_harib00b_OS} ${03_day_harib00b_SYS} ::
  DEPENDS 03_day_harib00b_sys
)
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00b/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00b/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00b/ipl_wine.img)

add_custom_target(03_day_harib00b_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)
add_custom_target(03_day_harib00b_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(03_day_harib00b_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(images 03_day_harib00b_ipl)
add_dependencies(wine 03_day_harib00b_ipl_wine)
add_dependencies(od 03_day_harib00b_ipl_od)

#----------------------------------------------------------
