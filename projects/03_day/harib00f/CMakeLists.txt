#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(03_day_harib00f_OS    ${root_BINARY_DIR}/projects/03_day/harib00f/os.img)
set(03_day_harib00f_SYS	  ${root_BINARY_DIR}/projects/03_day/harib00f/os.sys)
set(03_day_harib00f_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00f/ipl.bin)
set(03_day_harib00f_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00f/ipl10.nas)
set(03_day_harib00f_HEADB ${root_BINARY_DIR}/projects/03_day/harib00f/asmhead.bin)
set(03_day_harib00f_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00f/asmhead.nas)
set(03_day_harib00f_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00f/boot.bin)
set(03_day_harib00f_CCO	  ${root_BINARY_DIR}/projects/03_day/harib00f/boot.o)
set(03_day_harib00f_CCS	  ${root_SOURCE_DIR}/projects/03_day/harib00f/*.c)
set(03_day_harib00f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(03_day_harib00f_WILDOBJ ${root_BINARY_DIR}/projects/03_day/harib00f/*.o)


add_custom_target(03_day_harib00f_haribote_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00f_OS}
  DEPENDS 03_day_harib00f_img
)
add_custom_target(03_day_harib00f_haribote_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00f_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 03_day_harib00f_img
)
add_custom_target(03_day_harib00f_haribote_clean
  COMMAND rm -f ${03_day_harib00f_OS}
  COMMAND rm -f ${03_day_harib00f_SYS}
  COMMAND rm -f ${03_day_harib00f_IPLB}
  COMMAND rm -f ${03_day_harib00f_HEADB}
  COMMAND rm -f ${03_day_harib00f_BOOTB}
  COMMAND rm -f ${03_day_harib00f_WILDOBJ}
)
add_custom_target(03_day_harib00f_haribote_ipl
  COMMAND ${NASK} ${03_day_harib00f_IPLS} ${03_day_harib00f_IPLB}
)
add_custom_target(03_day_harib00f_haribote_sys
  COMMAND ${NASK} ${03_day_harib00f_HEADS} ${03_day_harib00f_HEADB}
  COMMAND gcc ${BINOPT} -T ${03_day_harib00f_LDS} ${03_day_harib00f_CCS} -o ${03_day_harib00f_BOOTB}
  COMMAND cat ${03_day_harib00f_HEADB} ${03_day_harib00f_BOOTB} > ${03_day_harib00f_SYS}
  DEPENDS 03_day_harib00f_ipl
)
add_custom_target(03_day_harib00f_haribote_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${03_day_harib00f_IPLB} -i ${03_day_harib00f_OS}
  COMMAND mcopy -i ${03_day_harib00f_OS} ${03_day_harib00f_SYS} ::
  DEPENDS 03_day_harib00f_sys
)
########### next target ###############
set(haribote.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00f/haribote.nas)
set(haribote.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00f/haribote.img)
set(haribote_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00f/haribote_wine.img)

add_custom_target(03_day_harib00f_haribote
  COMMAND ${root_BINARY_DIR}/src/opennask ${haribote.img_SRCS} ${haribote.img_OUTS}
)
add_custom_target(03_day_harib00f_haribote_wine
  COMMAND ${WINE} ${WINE_NASK} ${haribote.img_SRCS} ${haribote_wine.img_OUTS}
)
add_custom_target(03_day_harib00f_haribote_od
  COMMAND ${OD} -t x1 ${haribote.img_OUTS}      > ${haribote.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${haribote_wine.img_OUTS} > ${haribote_wine.img_OUTS}_t.txt
  COMMAND diff -s ${haribote.img_OUTS}_f.txt ${haribote_wine.img_OUTS}_t.txt
)

add_dependencies(03_day_harib00f_haribote_od 03_day_harib00f_haribote)
add_dependencies(03_day_harib00f_haribote_od 03_day_harib00f_haribote_wine)

add_dependencies(images 03_day_harib00f_haribote)
add_dependencies(wine 03_day_harib00f_haribote_wine)
add_dependencies(od 03_day_harib00f_haribote_od)

#----------------------------------------------------------
########### next target ###############
set(ipl.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00f/ipl.nas)
set(ipl.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00f/ipl.img)
set(ipl_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00f/ipl_wine.img)

add_custom_target(03_day_harib00f_ipl
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl.img_SRCS} ${ipl.img_OUTS}
)
add_custom_target(03_day_harib00f_ipl_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl.img_SRCS} ${ipl_wine.img_OUTS}
)
add_custom_target(03_day_harib00f_ipl_od
  COMMAND ${OD} -t x1 ${ipl.img_OUTS}      > ${ipl.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl_wine.img_OUTS} > ${ipl_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl.img_OUTS}_f.txt ${ipl_wine.img_OUTS}_t.txt
)

add_dependencies(03_day_harib00f_ipl_od 03_day_harib00f_ipl)
add_dependencies(03_day_harib00f_ipl_od 03_day_harib00f_ipl_wine)

add_dependencies(images 03_day_harib00f_ipl)
add_dependencies(wine 03_day_harib00f_ipl_wine)
add_dependencies(od 03_day_harib00f_ipl_od)

#----------------------------------------------------------
