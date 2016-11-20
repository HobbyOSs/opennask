#----------------------------------------------------------
message(STATUS "Entering directory projects/02_day/helloos4/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(02_day_helloos4_OS    ${root_BINARY_DIR}/projects/02_day/helloos4/os.img)
set(02_day_helloos4_SYS	  ${root_BINARY_DIR}/projects/02_day/helloos4/os.sys)
set(02_day_helloos4_IPLB  ${root_BINARY_DIR}/projects/02_day/helloos4/ipl.bin)
set(02_day_helloos4_IPLS  ${root_SOURCE_DIR}/projects/02_day/helloos4/ipl10.nas)
set(02_day_helloos4_HEADB ${root_BINARY_DIR}/projects/02_day/helloos4/asmhead.bin)
set(02_day_helloos4_HEADS ${root_SOURCE_DIR}/projects/02_day/helloos4/asmhead.nas)
set(02_day_helloos4_BOOTB ${root_BINARY_DIR}/projects/02_day/helloos4/boot.bin)
set(02_day_helloos4_CCO	  ${root_BINARY_DIR}/projects/02_day/helloos4/boot.o)
set(02_day_helloos4_CCS	  ${root_SOURCE_DIR}/projects/02_day/helloos4/*.c)
set(02_day_helloos4_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)

add_custom_target(02_day_helloos4_ipl_run
  COMMAND ${QEMU} ${QEMUOPT} ${02_day_helloos4_OS}
  DEPENDS 02_day_helloos4_img
)
add_custom_target(02_day_helloos4_ipl_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${02_day_helloos4_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 02_day_helloos4_img
)
add_custom_target(02_day_helloos4_ipl_clean
  COMMAND rm -f ${02_day_helloos4_OS}
  COMMAND rm -f ${02_day_helloos4_SYS}
  COMMAND rm -f ${02_day_helloos4_IPLB}
  COMMAND rm -f ${02_day_helloos4_HEADB}
  COMMAND rm -f ${02_day_helloos4_BOOTB}
  COMMAND rm -f ${02_day_helloos4_CCO}
)
add_custom_target(02_day_helloos4_ipl_ipl
  COMMAND ${NASK} ${02_day_helloos4_IPLS} ${02_day_helloos4_IPLB}
)
add_custom_target(02_day_helloos4_ipl_sys
  COMMAND ${NASK} ${02_day_helloos4_HEADS} ${02_day_helloos4_HEADB}
  COMMAND gcc ${BINOPT} ${02_day_helloos4_CCS} -c -o ${02_day_helloos4_CCO}
  COMMAND ld -o ${02_day_helloos4_BOOTB} -e HariMain -T ${02_day_helloos4_LDS} ${02_day_helloos4_CCO}
  COMMAND cat ${02_day_helloos4_HEADB} ${02_day_helloos4_BOOTB} > ${02_day_helloos4_SYS}
  DEPENDS 02_day_helloos4_ipl
)
add_custom_target(02_day_helloos4_ipl_img
  COMMAND mformat -f 1440 -C -B ${02_day_helloos4_IPLB} -i ${02_day_helloos4_OS}
  COMMAND mcopy -i ${02_day_helloos4_OS} ${02_day_helloos4_SYS} ::
  DEPENDS 02_day_helloos4_sys
)
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/02_day/helloos4/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos4/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos4/ipl_wine.img)

add_custom_target(02_day_helloos4_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)
add_custom_target(02_day_helloos4_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(02_day_helloos4_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(images 02_day_helloos4_ipl)
add_dependencies(wine 02_day_helloos4_ipl_wine)
add_dependencies(od 02_day_helloos4_ipl_od)

#----------------------------------------------------------
