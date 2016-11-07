#----------------------------------------------------------
message(STATUS "Entering directory projects/06_day/harib03b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(06_day_harib03b_OS    ${root_BINARY_DIR}/projects/06_day/harib03b/os.img)
set(06_day_harib03b_SYS	  ${root_BINARY_DIR}/projects/06_day/harib03b/os.sys)
set(06_day_harib03b_IPLB  ${root_BINARY_DIR}/projects/06_day/harib03b/ipl.bin)
set(06_day_harib03b_IPLS  ${root_SOURCE_DIR}/projects/06_day/harib03b/ipl10.nas)
set(06_day_harib03b_HEADB ${root_BINARY_DIR}/projects/06_day/harib03b/asmhead.bin)
set(06_day_harib03b_HEADS ${root_SOURCE_DIR}/projects/06_day/harib03b/asmhead.nas)
set(06_day_harib03b_BOOTB ${root_BINARY_DIR}/projects/06_day/harib03b/boot.bin)
set(06_day_harib03b_CCO	  ${root_BINARY_DIR}/projects/06_day/harib03b/boot.o)
set(06_day_harib03b_CCS	  ${root_SOURCE_DIR}/projects/06_day/harib03b/*.c)
set(06_day_harib03b_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)
set(06_day_harib03b_FUNCO ${root_BINARY_DIR}/projects/06_day/harib03b/naskfunc.o)
set(06_day_harib03b_FUNCS ${root_SOURCE_DIR}/projects/06_day/harib03b/naskfunc.nas)

add_custom_target(06_day_harib03b_run
  COMMAND ${QEMU} ${QEMUOPT} ${06_day_harib03b_OS}
  DEPENDS 06_day_harib03b_img
)
add_custom_target(06_day_harib03b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${06_day_harib03b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 06_day_harib03b_img
)
add_custom_target(06_day_harib03b_clean
  COMMAND rm -f ${06_day_harib03b_OS}
  COMMAND rm -f ${06_day_harib03b_SYS}
  COMMAND rm -f ${06_day_harib03b_IPLB}
  COMMAND rm -f ${06_day_harib03b_HEADB}
  COMMAND rm -f ${06_day_harib03b_BOOTB}
  COMMAND rm -f ${06_day_harib03b_CCO}
  COMMAND rm -f ${06_day_harib03b_FUNCO}
)
add_custom_target(06_day_harib03b_ipl
  COMMAND ${NASK} ${06_day_harib03b_IPLS} ${06_day_harib03b_IPLB}
)
add_custom_target(06_day_harib03b_sys
  COMMAND ${NASK} ${06_day_harib03b_FUNCS} ${06_day_harib03b_FUNCO}
  COMMAND ${NASK} ${06_day_harib03b_HEADS} ${06_day_harib03b_HEADB}
  COMMAND gcc ${06_day_harib03b_CCS}  -c -o ${06_day_harib03b_CCO}
  COMMAND ld -o ${06_day_harib03b_BOOTB} -e HariMain -T ${06_day_harib03b_LDS} ${06_day_harib03b_CCO} ${06_day_harib03b_FUNCO}
  COMMAND cat ${06_day_harib03b_HEADB} ${06_day_harib03b_BOOTB} > ${06_day_harib03b_SYS}
  DEPENDS 06_day_harib03b_ipl
)
add_custom_target(06_day_harib03b_img
  COMMAND mformat -f 1440 -C -B ${06_day_harib03b_IPLB} -i ${06_day_harib03b_OS}
  COMMAND mcopy -i ${06_day_harib03b_OS} ${06_day_harib03b_SYS} ::
  DEPENDS 06_day_harib03b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b/asmhead_wine.img)

add_custom_target(06_day_harib03b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(06_day_harib03b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(06_day_harib03b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 06_day_harib03b_asmhead)
add_dependencies(wine 06_day_harib03b_asmhead_wine)
add_dependencies(od 06_day_harib03b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b/ipl10_wine.img)

add_custom_target(06_day_harib03b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(06_day_harib03b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(06_day_harib03b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 06_day_harib03b_ipl10)
add_dependencies(wine 06_day_harib03b_ipl10_wine)
add_dependencies(od 06_day_harib03b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03b_wine.imgkfunc_wine.img)

add_custom_target(06_day_harib03bkfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(06_day_harib03bkfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(06_day_harib03bkfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 06_day_harib03bkfunc)
add_dependencies(wine 06_day_harib03bkfunc_wine)
add_dependencies(od 06_day_harib03bkfunc_od)

#----------------------------------------------------------
