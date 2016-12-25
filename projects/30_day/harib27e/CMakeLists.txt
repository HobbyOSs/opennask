#----------------------------------------------------------
message(STATUS "Entering directory projects/30_day/harib27e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(30_day_harib27e_OS    ${root_BINARY_DIR}/projects/30_day/harib27e/os.img)
set(30_day_harib27e_SYS	  ${root_BINARY_DIR}/projects/30_day/harib27e/os.sys)
set(30_day_harib27e_IPLB  ${root_BINARY_DIR}/projects/30_day/harib27e/ipl.bin)
set(30_day_harib27e_IPLS  ${root_SOURCE_DIR}/projects/30_day/harib27e/ipl10.nas)
set(30_day_harib27e_HEADB ${root_BINARY_DIR}/projects/30_day/harib27e/asmhead.bin)
set(30_day_harib27e_HEADS ${root_SOURCE_DIR}/projects/30_day/harib27e/asmhead.nas)
set(30_day_harib27e_BOOTB ${root_BINARY_DIR}/projects/30_day/harib27e/boot.bin)
set(30_day_harib27e_CCO	  ${root_BINARY_DIR}/projects/30_day/harib27e/boot.o)
set(30_day_harib27e_CCS	  ${root_SOURCE_DIR}/projects/30_day/harib27e/*.c)
set(30_day_harib27e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(30_day_harib27e_WILDOBJ ${root_BINARY_DIR}/projects/30_day/harib27e/*.o)


add_custom_target(30_day_harib27e_ipl09_run
  COMMAND ${QEMU} ${QEMUOPT} ${30_day_harib27e_OS}
  DEPENDS 30_day_harib27e_img
)
add_custom_target(30_day_harib27e_ipl09_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${30_day_harib27e_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 30_day_harib27e_img
)
add_custom_target(30_day_harib27e_ipl09_clean
  COMMAND rm -f ${30_day_harib27e_OS}
  COMMAND rm -f ${30_day_harib27e_SYS}
  COMMAND rm -f ${30_day_harib27e_IPLB}
  COMMAND rm -f ${30_day_harib27e_HEADB}
  COMMAND rm -f ${30_day_harib27e_BOOTB}
  COMMAND rm -f ${30_day_harib27e_WILDOBJ}
)
add_custom_target(30_day_harib27e_ipl09_ipl
  COMMAND ${NASK} ${30_day_harib27e_IPLS} ${30_day_harib27e_IPLB}
)
add_custom_target(30_day_harib27e_ipl09_sys
  COMMAND ${NASK} ${30_day_harib27e_HEADS} ${30_day_harib27e_HEADB}
  COMMAND gcc ${BINOPT} -T ${30_day_harib27e_LDS} ${30_day_harib27e_CCS} -o ${30_day_harib27e_BOOTB}
  COMMAND cat ${30_day_harib27e_HEADB} ${30_day_harib27e_BOOTB} > ${30_day_harib27e_SYS}
  DEPENDS 30_day_harib27e_ipl
)
add_custom_target(30_day_harib27e_ipl09_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${30_day_harib27e_IPLB} -i ${30_day_harib27e_OS}
  COMMAND mcopy -i ${30_day_harib27e_OS} ${30_day_harib27e_SYS} ::
  DEPENDS 30_day_harib27e_sys
)
########### next target ###############
set(ipl09.img_SRCS ${root_SOURCE_DIR}/projects/30_day/harib27e/ipl09.nas)
set(ipl09.img_OUTS ${root_BINARY_DIR}/projects/30_day/harib27e/ipl09.img)
set(ipl09_wine.img_OUTS ${root_BINARY_DIR}/projects/30_day/harib27e/ipl09_wine.img)

add_custom_target(30_day_harib27e_ipl09
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl09.img_SRCS} ${ipl09.img_OUTS}
)
set(ipl09.img_HRB ${root_BINARY_DIR}/projects/30_day/harib27e/ipl09.hrb)
add_custom_target(30_day_harib27e_ipl09_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl09.img_SRCS} ${ipl09.img_HRB}
  COMMAND mcopy -i ${30_day_harib27e_OS} ${ipl09.img_HRB} ::ipl09.hrb
  DEPENDS 30_day_harib27e_img
)

add_dependencies(30_day_harib27e_run 30_day_harib27e_ipl09_hrb)
add_custom_target(30_day_harib27e_ipl09_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl09.img_SRCS} ${ipl09_wine.img_OUTS}
)
add_custom_target(30_day_harib27e_ipl09_od
  COMMAND ${OD} -t x1 ${ipl09.img_OUTS}      > ${ipl09.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl09_wine.img_OUTS} > ${ipl09_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl09.img_OUTS}_f.txt ${ipl09_wine.img_OUTS}_t.txt
)

add_dependencies(30_day_harib27e_ipl09_od 30_day_harib27e_ipl09)
add_dependencies(30_day_harib27e_ipl09_od 30_day_harib27e_ipl09_wine)

add_dependencies(images 30_day_harib27e_ipl09)
add_dependencies(wine 30_day_harib27e_ipl09_wine)
add_dependencies(od 30_day_harib27e_ipl09_od)

#----------------------------------------------------------
