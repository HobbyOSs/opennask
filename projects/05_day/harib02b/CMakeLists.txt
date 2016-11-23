#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(05_day_harib02b_OS    ${root_BINARY_DIR}/projects/05_day/harib02b/os.img)
set(05_day_harib02b_SYS	  ${root_BINARY_DIR}/projects/05_day/harib02b/os.sys)
set(05_day_harib02b_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02b/ipl.bin)
set(05_day_harib02b_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02b/ipl10.nas)
set(05_day_harib02b_HEADB ${root_BINARY_DIR}/projects/05_day/harib02b/asmhead.bin)
set(05_day_harib02b_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02b/asmhead.nas)
set(05_day_harib02b_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02b/boot.bin)
set(05_day_harib02b_CCO	  ${root_BINARY_DIR}/projects/05_day/harib02b/boot.o)
set(05_day_harib02b_CCS	  ${root_SOURCE_DIR}/projects/05_day/harib02b/*.c)
set(05_day_harib02b_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)
set(05_day_harib02b_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02b/naskfunc.o)
set(05_day_harib02b_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02b/naskfunc.nas)

add_custom_target(05_day_harib02b_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02b_OS}
  DEPENDS 05_day_harib02b_img
)
add_custom_target(05_day_harib02b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 05_day_harib02b_img
)
add_custom_target(05_day_harib02b_clean
  COMMAND rm -f ${05_day_harib02b_OS}
  COMMAND rm -f ${05_day_harib02b_SYS}
  COMMAND rm -f ${05_day_harib02b_IPLB}
  COMMAND rm -f ${05_day_harib02b_HEADB}
  COMMAND rm -f ${05_day_harib02b_BOOTB}
  COMMAND rm -f ${05_day_harib02b_CCO}
  COMMAND rm -f ${05_day_harib02b_FUNCO}
)
add_custom_target(05_day_harib02b_ipl
  COMMAND ${NASK} ${05_day_harib02b_IPLS} ${05_day_harib02b_IPLB}
)
add_custom_target(05_day_harib02b_sys
  COMMAND ${NASK} ${05_day_harib02b_FUNCS} ${05_day_harib02b_FUNCO}
  COMMAND ${NASK} ${05_day_harib02b_HEADS} ${05_day_harib02b_HEADB}
  COMMAND ${NASK} ${05_day_harib02b_FUNCS} ${05_day_harib02b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${05_day_harib02b_LDS} ${05_day_harib02b_CCS} ${05_day_harib02b_FUNCO} -o ${05_day_harib02b_BOOTB}
  COMMAND cat ${05_day_harib02b_HEADB} ${05_day_harib02b_BOOTB} > ${05_day_harib02b_SYS}
  DEPENDS 05_day_harib02b_ipl
)
add_custom_target(05_day_harib02b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02b_IPLB} -i ${05_day_harib02b_OS}
  COMMAND mcopy -i ${05_day_harib02b_OS} ${05_day_harib02b_SYS} ::
  DEPENDS 05_day_harib02b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/asmhead_wine.img)

add_custom_target(05_day_harib02b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(05_day_harib02b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(05_day_harib02b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02b_asmhead)
add_dependencies(wine 05_day_harib02b_asmhead_wine)
add_dependencies(od 05_day_harib02b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/ipl10_wine.img)

add_custom_target(05_day_harib02b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(05_day_harib02b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(05_day_harib02b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02b_ipl10)
add_dependencies(wine 05_day_harib02b_ipl10_wine)
add_dependencies(od 05_day_harib02b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02b/naskfunc_wine.img)

add_custom_target(05_day_harib02b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(05_day_harib02b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(05_day_harib02b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02b_naskfunc)
add_dependencies(wine 05_day_harib02b_naskfunc_wine)
add_dependencies(od 05_day_harib02b_naskfunc_od)

#----------------------------------------------------------
