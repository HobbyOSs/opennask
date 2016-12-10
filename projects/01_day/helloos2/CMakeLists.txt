#----------------------------------------------------------
message(STATUS "Entering directory projects/01_day/helloos2/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(01_day_helloos2_OS    ${root_BINARY_DIR}/projects/01_day/helloos2/os.img)
set(01_day_helloos2_SYS	  ${root_BINARY_DIR}/projects/01_day/helloos2/os.sys)
set(01_day_helloos2_IPLB  ${root_BINARY_DIR}/projects/01_day/helloos2/ipl.bin)
set(01_day_helloos2_IPLS  ${root_SOURCE_DIR}/projects/01_day/helloos2/ipl10.nas)
set(01_day_helloos2_HEADB ${root_BINARY_DIR}/projects/01_day/helloos2/asmhead.bin)
set(01_day_helloos2_HEADS ${root_SOURCE_DIR}/projects/01_day/helloos2/asmhead.nas)
set(01_day_helloos2_BOOTB ${root_BINARY_DIR}/projects/01_day/helloos2/boot.bin)
set(01_day_helloos2_CCO	  ${root_BINARY_DIR}/projects/01_day/helloos2/boot.o)
set(01_day_helloos2_CCS	  ${root_SOURCE_DIR}/projects/01_day/helloos2/*.c)
set(01_day_helloos2_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(01_day_helloos2_WILDOBJ ${root_BINARY_DIR}/projects/01_day/helloos2/*.o)


add_custom_target(01_day_helloos2_helloos_run
  COMMAND ${QEMU} ${QEMUOPT} ${01_day_helloos2_OS}
  DEPENDS 01_day_helloos2_img
)
add_custom_target(01_day_helloos2_helloos_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${01_day_helloos2_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 01_day_helloos2_img
)
add_custom_target(01_day_helloos2_helloos_clean
  COMMAND rm -f ${01_day_helloos2_OS}
  COMMAND rm -f ${01_day_helloos2_SYS}
  COMMAND rm -f ${01_day_helloos2_IPLB}
  COMMAND rm -f ${01_day_helloos2_HEADB}
  COMMAND rm -f ${01_day_helloos2_BOOTB}
  COMMAND rm -f ${01_day_helloos2_WILDOBJ}
)
add_custom_target(01_day_helloos2_helloos_ipl
  COMMAND ${NASK} ${01_day_helloos2_IPLS} ${01_day_helloos2_IPLB}
)
add_custom_target(01_day_helloos2_helloos_sys
  COMMAND ${NASK} ${01_day_helloos2_HEADS} ${01_day_helloos2_HEADB}
  COMMAND gcc ${BINOPT} -T ${01_day_helloos2_LDS} ${01_day_helloos2_CCS} -o ${01_day_helloos2_BOOTB}
  COMMAND cat ${01_day_helloos2_HEADB} ${01_day_helloos2_BOOTB} > ${01_day_helloos2_SYS}
  DEPENDS 01_day_helloos2_ipl
)
add_custom_target(01_day_helloos2_helloos_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${01_day_helloos2_IPLB} -i ${01_day_helloos2_OS}
  COMMAND mcopy -i ${01_day_helloos2_OS} ${01_day_helloos2_SYS} ::
  DEPENDS 01_day_helloos2_sys
)
########### next target ###############
set(helloos.img_SRCS ${root_SOURCE_DIR}/projects/01_day/helloos2/helloos.nas)
set(helloos.img_OUTS ${root_BINARY_DIR}/projects/01_day/helloos2/helloos.img)
set(helloos_wine.img_OUTS ${root_BINARY_DIR}/projects/01_day/helloos2/helloos_wine.img)

add_custom_target(01_day_helloos2_helloos
  COMMAND ${root_BINARY_DIR}/src/opennask ${helloos.img_SRCS} ${helloos.img_OUTS}
)
add_custom_target(01_day_helloos2_helloos_wine
  COMMAND ${WINE} ${WINE_NASK} ${helloos.img_SRCS} ${helloos_wine.img_OUTS}
)
add_custom_target(01_day_helloos2_helloos_od
  COMMAND ${OD} -t x1 ${helloos.img_OUTS}      > ${helloos.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${helloos_wine.img_OUTS} > ${helloos_wine.img_OUTS}_t.txt
  COMMAND diff -s ${helloos.img_OUTS}_f.txt ${helloos_wine.img_OUTS}_t.txt
)

add_dependencies(01_day_helloos2_helloos_od 01_day_helloos2_helloos)
add_dependencies(01_day_helloos2_helloos_od 01_day_helloos2_helloos_wine)

add_dependencies(images 01_day_helloos2_helloos)
add_dependencies(wine 01_day_helloos2_helloos_wine)
add_dependencies(od 01_day_helloos2_helloos_od)

#----------------------------------------------------------
