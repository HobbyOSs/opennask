#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00h/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(03_day_harib00h_OS    ${root_BINARY_DIR}/projects/03_day/harib00h/os.img)
set(03_day_harib00h_SYS	  ${root_BINARY_DIR}/projects/03_day/harib00h/os.sys)
set(03_day_harib00h_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00h/ipl.bin)
set(03_day_harib00h_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00h/ipl10.nas)
set(03_day_harib00h_HEADB ${root_BINARY_DIR}/projects/03_day/harib00h/asmhead.bin)
set(03_day_harib00h_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00h/asmhead.nas)
set(03_day_harib00h_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00h/boot.bin)
set(03_day_harib00h_CCO	  ${root_BINARY_DIR}/projects/03_day/harib00h/boot.o)
set(03_day_harib00h_CCS	  ${root_SOURCE_DIR}/projects/03_day/harib00h/*.c)
set(03_day_harib00h_LDS	  ${root_SOURCE_DIR}/projects/os.lds)

add_custom_target(03_day_harib00h_haribote_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00h_OS}
  DEPENDS 03_day_harib00h_img
)
add_custom_target(03_day_harib00h_haribote_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00h_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 03_day_harib00h_img
)
add_custom_target(03_day_harib00h_haribote_clean
  COMMAND rm -f ${03_day_harib00h_OS}
  COMMAND rm -f ${03_day_harib00h_SYS}
  COMMAND rm -f ${03_day_harib00h_IPLB}
  COMMAND rm -f ${03_day_harib00h_HEADB}
  COMMAND rm -f ${03_day_harib00h_BOOTB}
  COMMAND rm -f ${03_day_harib00h_CCO}
)
add_custom_target(03_day_harib00h_haribote_ipl
  COMMAND ${NASK} ${03_day_harib00h_IPLS} ${03_day_harib00h_IPLB}
)
add_custom_target(03_day_harib00h_haribote_sys
  COMMAND ${NASK} ${03_day_harib00h_HEADS} ${03_day_harib00h_HEADB}
  COMMAND gcc ${BINOPT} -T ${03_day_harib00h_LDS} ${03_day_harib00h_CCS} -o ${03_day_harib00h_BOOTB}
  COMMAND cat ${03_day_harib00h_HEADB} ${03_day_harib00h_BOOTB} > ${03_day_harib00h_SYS}
  DEPENDS 03_day_harib00h_ipl
)
add_custom_target(03_day_harib00h_haribote_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${03_day_harib00h_IPLB} -i ${03_day_harib00h_OS}
  COMMAND mcopy -i ${03_day_harib00h_OS} ${03_day_harib00h_SYS} ::
  DEPENDS 03_day_harib00h_sys
)
########### next target ###############
set(haribote.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00h/haribote.nas)
set(haribote.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00h/haribote.img)
set(haribote_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00h/haribote_wine.img)

add_custom_target(03_day_harib00h_haribote
  COMMAND ${root_BINARY_DIR}/src/opennask ${haribote.img_SRCS} ${haribote.img_OUTS}
)
add_custom_target(03_day_harib00h_haribote_wine
  COMMAND ${WINE} ${WINE_NASK} ${haribote.img_SRCS} ${haribote_wine.img_OUTS}
)
add_custom_target(03_day_harib00h_haribote_od
  COMMAND ${OD} -t x1 ${haribote.img_OUTS}      > ${haribote.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${haribote_wine.img_OUTS} > ${haribote_wine.img_OUTS}_t.txt
)

add_dependencies(images 03_day_harib00h_haribote)
add_dependencies(wine 03_day_harib00h_haribote_wine)
add_dependencies(od 03_day_harib00h_haribote_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00h/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00h/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00h/ipl10_wine.img)

add_custom_target(03_day_harib00h_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(03_day_harib00h_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(03_day_harib00h_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 03_day_harib00h_ipl10)
add_dependencies(wine 03_day_harib00h_ipl10_wine)
add_dependencies(od 03_day_harib00h_ipl10_od)

#----------------------------------------------------------
