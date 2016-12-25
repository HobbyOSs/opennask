#----------------------------------------------------------
message(STATUS "Entering directory projects/30_day/harib27f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(30_day_harib27f_OS    ${root_BINARY_DIR}/projects/30_day/harib27f/os.img)
set(30_day_harib27f_SYS	  ${root_BINARY_DIR}/projects/30_day/harib27f/os.sys)
set(30_day_harib27f_IPLB  ${root_BINARY_DIR}/projects/30_day/harib27f/ipl.bin)
set(30_day_harib27f_IPLS  ${root_SOURCE_DIR}/projects/30_day/harib27f/ipl10.nas)
set(30_day_harib27f_HEADB ${root_BINARY_DIR}/projects/30_day/harib27f/asmhead.bin)
set(30_day_harib27f_HEADS ${root_SOURCE_DIR}/projects/30_day/harib27f/asmhead.nas)
set(30_day_harib27f_BOOTB ${root_BINARY_DIR}/projects/30_day/harib27f/boot.bin)
set(30_day_harib27f_CCO	  ${root_BINARY_DIR}/projects/30_day/harib27f/boot.o)
set(30_day_harib27f_CCS	  ${root_SOURCE_DIR}/projects/30_day/harib27f/*.c)
set(30_day_harib27f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(30_day_harib27f_WILDOBJ ${root_BINARY_DIR}/projects/30_day/harib27f/*.o)


add_custom_target(30_day_harib27f_ipl09_run
  COMMAND ${QEMU} ${QEMUOPT} ${30_day_harib27f_OS}
  DEPENDS 30_day_harib27f_img
)
add_custom_target(30_day_harib27f_ipl09_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${30_day_harib27f_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 30_day_harib27f_img
)
add_custom_target(30_day_harib27f_ipl09_clean
  COMMAND rm -f ${30_day_harib27f_OS}
  COMMAND rm -f ${30_day_harib27f_SYS}
  COMMAND rm -f ${30_day_harib27f_IPLB}
  COMMAND rm -f ${30_day_harib27f_HEADB}
  COMMAND rm -f ${30_day_harib27f_BOOTB}
  COMMAND rm -f ${30_day_harib27f_WILDOBJ}
)
add_custom_target(30_day_harib27f_ipl09_ipl
  COMMAND ${NASK} ${30_day_harib27f_IPLS} ${30_day_harib27f_IPLB}
)
add_custom_target(30_day_harib27f_ipl09_sys
  COMMAND ${NASK} ${30_day_harib27f_HEADS} ${30_day_harib27f_HEADB}
  COMMAND gcc ${BINOPT} -T ${30_day_harib27f_LDS} ${30_day_harib27f_CCS} -o ${30_day_harib27f_BOOTB}
  COMMAND cat ${30_day_harib27f_HEADB} ${30_day_harib27f_BOOTB} > ${30_day_harib27f_SYS}
  DEPENDS 30_day_harib27f_ipl
)
add_custom_target(30_day_harib27f_ipl09_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${30_day_harib27f_IPLB} -i ${30_day_harib27f_OS}
  COMMAND mcopy -i ${30_day_harib27f_OS} ${30_day_harib27f_SYS} ::
  DEPENDS 30_day_harib27f_sys
)
########### next target ###############
set(ipl09.img_SRCS ${root_SOURCE_DIR}/projects/30_day/harib27f/ipl09.nas)
set(ipl09.img_OUTS ${root_BINARY_DIR}/projects/30_day/harib27f/ipl09.img)
set(ipl09_wine.img_OUTS ${root_BINARY_DIR}/projects/30_day/harib27f/ipl09_wine.img)

add_custom_target(30_day_harib27f_ipl09
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl09.img_SRCS} ${ipl09.img_OUTS}
)
set(ipl09.img_HRB ${root_BINARY_DIR}/projects/30_day/harib27f/ipl09.hrb)
add_custom_target(30_day_harib27f_ipl09_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl09.img_SRCS} ${ipl09.img_HRB}
  COMMAND mcopy -i ${30_day_harib27f_OS} ${ipl09.img_HRB} ::ipl09.hrb
  DEPENDS 30_day_harib27f_img
)

add_dependencies(30_day_harib27f_run 30_day_harib27f_ipl09_hrb)
add_custom_target(30_day_harib27f_ipl09_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl09.img_SRCS} ${ipl09_wine.img_OUTS}
)
add_custom_target(30_day_harib27f_ipl09_od
  COMMAND ${OD} -t x1 ${ipl09.img_OUTS}      > ${ipl09.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl09_wine.img_OUTS} > ${ipl09_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl09.img_OUTS}_f.txt ${ipl09_wine.img_OUTS}_t.txt
)

add_dependencies(30_day_harib27f_ipl09_od 30_day_harib27f_ipl09)
add_dependencies(30_day_harib27f_ipl09_od 30_day_harib27f_ipl09_wine)

add_dependencies(images 30_day_harib27f_ipl09)
add_dependencies(wine 30_day_harib27f_ipl09_wine)
add_dependencies(od 30_day_harib27f_ipl09_od)

#----------------------------------------------------------
