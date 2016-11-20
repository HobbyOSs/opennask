#----------------------------------------------------------
message(STATUS "Entering directory projects/08_day/harib05d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(08_day_harib05d_OS    ${root_BINARY_DIR}/projects/08_day/harib05d/os.img)
set(08_day_harib05d_SYS	  ${root_BINARY_DIR}/projects/08_day/harib05d/os.sys)
set(08_day_harib05d_IPLB  ${root_BINARY_DIR}/projects/08_day/harib05d/ipl.bin)
set(08_day_harib05d_IPLS  ${root_SOURCE_DIR}/projects/08_day/harib05d/ipl10.nas)
set(08_day_harib05d_HEADB ${root_BINARY_DIR}/projects/08_day/harib05d/asmhead.bin)
set(08_day_harib05d_HEADS ${root_SOURCE_DIR}/projects/08_day/harib05d/asmhead.nas)
set(08_day_harib05d_BOOTB ${root_BINARY_DIR}/projects/08_day/harib05d/boot.bin)
set(08_day_harib05d_CCO	  ${root_BINARY_DIR}/projects/08_day/harib05d/boot.o)
set(08_day_harib05d_CCS	  ${root_SOURCE_DIR}/projects/08_day/harib05d/*.c)
set(08_day_harib05d_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)
set(08_day_harib05d_FUNCO ${root_BINARY_DIR}/projects/08_day/harib05d/naskfunc.o)
set(08_day_harib05d_FUNCS ${root_SOURCE_DIR}/projects/08_day/harib05d/naskfunc.nas)

add_custom_target(08_day_harib05d_run
  COMMAND ${QEMU} ${QEMUOPT} ${08_day_harib05d_OS}
  DEPENDS 08_day_harib05d_img
)
add_custom_target(08_day_harib05d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${08_day_harib05d_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 08_day_harib05d_img
)
add_custom_target(08_day_harib05d_clean
  COMMAND rm -f ${08_day_harib05d_OS}
  COMMAND rm -f ${08_day_harib05d_SYS}
  COMMAND rm -f ${08_day_harib05d_IPLB}
  COMMAND rm -f ${08_day_harib05d_HEADB}
  COMMAND rm -f ${08_day_harib05d_BOOTB}
  COMMAND rm -f ${08_day_harib05d_CCO}
  COMMAND rm -f ${08_day_harib05d_FUNCO}
)
add_custom_target(08_day_harib05d_ipl
  COMMAND ${NASK} ${08_day_harib05d_IPLS} ${08_day_harib05d_IPLB}
)
add_custom_target(08_day_harib05d_sys
  COMMAND ${NASK} ${08_day_harib05d_FUNCS} ${08_day_harib05d_FUNCO}
  COMMAND ${NASK} ${08_day_harib05d_HEADS} ${08_day_harib05d_HEADB}
  COMMAND gcc ${BINOPT} ${08_day_harib05d_CCS} -c -o ${08_day_harib05d_CCO}
  COMMAND ld -o ${08_day_harib05d_BOOTB} -e HariMain -T ${08_day_harib05d_LDS} ${08_day_harib05d_CCO} ${08_day_harib05d_FUNCO}
  COMMAND cat ${08_day_harib05d_HEADB} ${08_day_harib05d_BOOTB} > ${08_day_harib05d_SYS}
  DEPENDS 08_day_harib05d_ipl
)
add_custom_target(08_day_harib05d_img
  COMMAND mformat -f 1440 -C -B ${08_day_harib05d_IPLB} -i ${08_day_harib05d_OS}
  COMMAND mcopy -i ${08_day_harib05d_OS} ${08_day_harib05d_SYS} ::
  DEPENDS 08_day_harib05d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/asmhead_wine.img)

add_custom_target(08_day_harib05d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(08_day_harib05d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(08_day_harib05d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05d_asmhead)
add_dependencies(wine 08_day_harib05d_asmhead_wine)
add_dependencies(od 08_day_harib05d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/ipl10_wine.img)

add_custom_target(08_day_harib05d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(08_day_harib05d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(08_day_harib05d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05d_ipl10)
add_dependencies(wine 08_day_harib05d_ipl10_wine)
add_dependencies(od 08_day_harib05d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05d/naskfunc_wine.img)

add_custom_target(08_day_harib05d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(08_day_harib05d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(08_day_harib05d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05d_naskfunc)
add_dependencies(wine 08_day_harib05d_naskfunc_wine)
add_dependencies(od 08_day_harib05d_naskfunc_od)

#----------------------------------------------------------
