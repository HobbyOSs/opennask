#----------------------------------------------------------
message(STATUS "Entering directory projects/01_day/helloos1/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(01_day_helloos1_OS    ${root_BINARY_DIR}/projects/01_day/helloos1/os.img)
set(01_day_helloos1_SYS	  ${root_BINARY_DIR}/projects/01_day/helloos1/os.sys)
set(01_day_helloos1_IPLB  ${root_BINARY_DIR}/projects/01_day/helloos1/ipl.bin)
set(01_day_helloos1_IPLS  ${root_SOURCE_DIR}/projects/01_day/helloos1/ipl10.nas)
set(01_day_helloos1_HEADB ${root_BINARY_DIR}/projects/01_day/helloos1/asmhead.bin)
set(01_day_helloos1_HEADS ${root_SOURCE_DIR}/projects/01_day/helloos1/asmhead.nas)
set(01_day_helloos1_BOOTB ${root_BINARY_DIR}/projects/01_day/helloos1/boot.bin)
set(01_day_helloos1_CCO	  ${root_BINARY_DIR}/projects/01_day/helloos1/boot.o)
set(01_day_helloos1_CCS	  ${root_SOURCE_DIR}/projects/01_day/helloos1/*.c)
set(01_day_helloos1_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)

add_custom_target(01_day_helloos1_helloos_run
  COMMAND ${QEMU} ${QEMUOPT} ${01_day_helloos1_OS}
  DEPENDS 01_day_helloos1_img
)
add_custom_target(01_day_helloos1_helloos_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${01_day_helloos1_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 01_day_helloos1_img
)
add_custom_target(01_day_helloos1_helloos_clean
  COMMAND rm -f ${01_day_helloos1_OS}
  COMMAND rm -f ${01_day_helloos1_SYS}
  COMMAND rm -f ${01_day_helloos1_IPLB}
  COMMAND rm -f ${01_day_helloos1_HEADB}
  COMMAND rm -f ${01_day_helloos1_BOOTB}
  COMMAND rm -f ${01_day_helloos1_CCO}
)
add_custom_target(01_day_helloos1_helloos_ipl
  COMMAND ${NASK} ${01_day_helloos1_IPLS} ${01_day_helloos1_IPLB}
)
add_custom_target(01_day_helloos1_helloos_sys
  COMMAND ${NASK} ${01_day_helloos1_HEADS} ${01_day_helloos1_HEADB}
  COMMAND gcc ${BINOPT} -T ${01_day_helloos1_LDS} ${01_day_helloos1_CCS} -o ${01_day_helloos1_BOOTB}
  COMMAND cat ${01_day_helloos1_HEADB} ${01_day_helloos1_BOOTB} > ${01_day_helloos1_SYS}
  DEPENDS 01_day_helloos1_ipl
)
add_custom_target(01_day_helloos1_helloos_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${01_day_helloos1_IPLB} -i ${01_day_helloos1_OS}
  COMMAND mcopy -i ${01_day_helloos1_OS} ${01_day_helloos1_SYS} ::
  DEPENDS 01_day_helloos1_sys
)
########### next target ###############
set(helloos.img_SRCS ${root_SOURCE_DIR}/projects/01_day/helloos1/helloos.nas)
set(helloos.img_OUTS ${root_BINARY_DIR}/projects/01_day/helloos1/helloos.img)
set(helloos_wine.img_OUTS ${root_BINARY_DIR}/projects/01_day/helloos1/helloos_wine.img)

add_custom_target(01_day_helloos1_helloos
  COMMAND ${root_BINARY_DIR}/src/opennask ${helloos.img_SRCS} ${helloos.img_OUTS}
)
add_custom_target(01_day_helloos1_helloos_wine
  COMMAND ${WINE} ${WINE_NASK} ${helloos.img_SRCS} ${helloos_wine.img_OUTS}
)
add_custom_target(01_day_helloos1_helloos_od
  COMMAND ${OD} -t x1 ${helloos.img_OUTS}      > ${helloos.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${helloos_wine.img_OUTS} > ${helloos_wine.img_OUTS}_t.txt
)

add_dependencies(images 01_day_helloos1_helloos)
add_dependencies(wine 01_day_helloos1_helloos_wine)
add_dependencies(od 01_day_helloos1_helloos_od)

#----------------------------------------------------------
