#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(03_day_harib00e_OS    ${root_BINARY_DIR}/projects/03_day/harib00e/os.img)
set(03_day_harib00e_SYS	  ${root_BINARY_DIR}/projects/03_day/harib00e/os.sys)
set(03_day_harib00e_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00e/ipl.bin)
set(03_day_harib00e_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00e/ipl10.nas)
set(03_day_harib00e_HEADB ${root_BINARY_DIR}/projects/03_day/harib00e/asmhead.bin)
set(03_day_harib00e_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00e/asmhead.nas)
set(03_day_harib00e_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00e/boot.bin)
set(03_day_harib00e_CCO	  ${root_BINARY_DIR}/projects/03_day/harib00e/boot.o)
set(03_day_harib00e_CCS	  ${root_SOURCE_DIR}/projects/03_day/harib00e/*.c)
set(03_day_harib00e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)

add_custom_target(03_day_harib00e_ipl_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00e_OS}
  DEPENDS 03_day_harib00e_img
)
add_custom_target(03_day_harib00e_ipl_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00e_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 03_day_harib00e_img
)
add_custom_target(03_day_harib00e_ipl_clean
  COMMAND rm -f ${03_day_harib00e_OS}
  COMMAND rm -f ${03_day_harib00e_SYS}
  COMMAND rm -f ${03_day_harib00e_IPLB}
  COMMAND rm -f ${03_day_harib00e_HEADB}
  COMMAND rm -f ${03_day_harib00e_BOOTB}
  COMMAND rm -f ${03_day_harib00e_CCO}
)
add_custom_target(03_day_harib00e_ipl_ipl
  COMMAND ${NASK} ${03_day_harib00e_IPLS} ${03_day_harib00e_IPLB}
)
add_custom_target(03_day_harib00e_ipl_sys
  COMMAND ${NASK} ${03_day_harib00e_HEADS} ${03_day_harib00e_HEADB}
  COMMAND gcc ${BINOPT} -T ${03_day_harib00e_LDS} ${03_day_harib00e_CCS} -o ${03_day_harib00e_BOOTB}
  COMMAND cat ${03_day_harib00e_HEADB} ${03_day_harib00e_BOOTB} > ${03_day_harib00e_SYS}
  DEPENDS 03_day_harib00e_ipl
)
add_custom_target(03_day_harib00e_ipl_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${03_day_harib00e_IPLB} -i ${03_day_harib00e_OS}
  COMMAND mcopy -i ${03_day_harib00e_OS} ${03_day_harib00e_SYS} ::
  DEPENDS 03_day_harib00e_sys
)
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00e/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00e/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00e/ipl_wine.img)

add_custom_target(03_day_harib00e_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)
add_custom_target(03_day_harib00e_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(03_day_harib00e_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl.img_OUTS}_f.txt ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(03_day_harib00e_ipl_od 03_day_harib00e_ipl)
add_dependencies(03_day_harib00e_ipl_od 03_day_harib00e_ipl_wine)

add_dependencies(images 03_day_harib00e_ipl)
add_dependencies(wine 03_day_harib00e_ipl_wine)
add_dependencies(od 03_day_harib00e_ipl_od)

#----------------------------------------------------------
