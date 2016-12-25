#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(03_day_harib00d_OS    ${root_BINARY_DIR}/projects/03_day/harib00d/os.img)
set(03_day_harib00d_SYS	  ${root_BINARY_DIR}/projects/03_day/harib00d/os.sys)
set(03_day_harib00d_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00d/ipl.bin)
set(03_day_harib00d_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00d/ipl10.nas)
set(03_day_harib00d_HEADB ${root_BINARY_DIR}/projects/03_day/harib00d/asmhead.bin)
set(03_day_harib00d_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00d/asmhead.nas)
set(03_day_harib00d_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00d/boot.bin)
set(03_day_harib00d_CCO	  ${root_BINARY_DIR}/projects/03_day/harib00d/boot.o)
set(03_day_harib00d_CCS	  ${root_SOURCE_DIR}/projects/03_day/harib00d/*.c)
set(03_day_harib00d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(03_day_harib00d_WILDOBJ ${root_BINARY_DIR}/projects/03_day/harib00d/*.o)


add_custom_target(03_day_harib00d_ipl_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00d_OS}
  DEPENDS 03_day_harib00d_img
)
add_custom_target(03_day_harib00d_ipl_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00d_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 03_day_harib00d_img
)
add_custom_target(03_day_harib00d_ipl_clean
  COMMAND rm -f ${03_day_harib00d_OS}
  COMMAND rm -f ${03_day_harib00d_SYS}
  COMMAND rm -f ${03_day_harib00d_IPLB}
  COMMAND rm -f ${03_day_harib00d_HEADB}
  COMMAND rm -f ${03_day_harib00d_BOOTB}
  COMMAND rm -f ${03_day_harib00d_WILDOBJ}
)
add_custom_target(03_day_harib00d_ipl_ipl
  COMMAND ${NASK} ${03_day_harib00d_IPLS} ${03_day_harib00d_IPLB}
)
add_custom_target(03_day_harib00d_ipl_sys
  COMMAND ${NASK} ${03_day_harib00d_HEADS} ${03_day_harib00d_HEADB}
  COMMAND gcc ${BINOPT} -T ${03_day_harib00d_LDS} ${03_day_harib00d_CCS} -o ${03_day_harib00d_BOOTB}
  COMMAND cat ${03_day_harib00d_HEADB} ${03_day_harib00d_BOOTB} > ${03_day_harib00d_SYS}
  DEPENDS 03_day_harib00d_ipl
)
add_custom_target(03_day_harib00d_ipl_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${03_day_harib00d_IPLB} -i ${03_day_harib00d_OS}
  COMMAND mcopy -i ${03_day_harib00d_OS} ${03_day_harib00d_SYS} ::
  DEPENDS 03_day_harib00d_sys
)
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00d/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00d/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00d/ipl_wine.img)

add_custom_target(03_day_harib00d_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)
set(ipl.img_HRB ${root_BINARY_DIR}/projects/03_day/harib00d/ipl.hrb)
add_custom_target(03_day_harib00d_ipl_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_HRB}
  COMMAND mcopy -i ${03_day_harib00d_OS} ${ipl.img_HRB} ::ipl.hrb
  DEPENDS 03_day_harib00d_img
)

add_custom_target(03_day_harib00d_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(03_day_harib00d_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl.img_OUTS}_f.txt ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(03_day_harib00d_ipl_od 03_day_harib00d_ipl)
add_dependencies(03_day_harib00d_ipl_od 03_day_harib00d_ipl_wine)

add_dependencies(images 03_day_harib00d_ipl)
add_dependencies(wine 03_day_harib00d_ipl_wine)
add_dependencies(od 03_day_harib00d_ipl_od)

#----------------------------------------------------------
