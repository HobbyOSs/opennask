#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(05_day_harib02d_OS    ${root_BINARY_DIR}/projects/05_day/harib02d/os.img)
set(05_day_harib02d_SYS	  ${root_BINARY_DIR}/projects/05_day/harib02d/os.sys)
set(05_day_harib02d_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02d/ipl.bin)
set(05_day_harib02d_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02d/ipl10.nas)
set(05_day_harib02d_HEADB ${root_BINARY_DIR}/projects/05_day/harib02d/asmhead.bin)
set(05_day_harib02d_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02d/asmhead.nas)
set(05_day_harib02d_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02d/boot.bin)
set(05_day_harib02d_CCO	  ${root_BINARY_DIR}/projects/05_day/harib02d/boot.o)
set(05_day_harib02d_CCS	  ${root_SOURCE_DIR}/projects/05_day/harib02d/*.c)
set(05_day_harib02d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02d_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02d/naskfunc.o)
set(05_day_harib02d_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02d/naskfunc.nas)

add_custom_target(05_day_harib02d_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02d_OS}
  DEPENDS 05_day_harib02d_img
)
add_custom_target(05_day_harib02d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02d_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 05_day_harib02d_img
)
add_custom_target(05_day_harib02d_clean
  COMMAND rm -f ${05_day_harib02d_OS}
  COMMAND rm -f ${05_day_harib02d_SYS}
  COMMAND rm -f ${05_day_harib02d_IPLB}
  COMMAND rm -f ${05_day_harib02d_HEADB}
  COMMAND rm -f ${05_day_harib02d_BOOTB}
  COMMAND rm -f ${05_day_harib02d_CCO}
  COMMAND rm -f ${05_day_harib02d_FUNCO}
)
add_custom_target(05_day_harib02d_ipl
  COMMAND ${NASK} ${05_day_harib02d_IPLS} ${05_day_harib02d_IPLB}
)
add_custom_target(05_day_harib02d_sys
  COMMAND ${NASK} ${05_day_harib02d_FUNCS} ${05_day_harib02d_FUNCO}
  COMMAND ${NASK} ${05_day_harib02d_HEADS} ${05_day_harib02d_HEADB}
  COMMAND ${NASK} ${05_day_harib02d_FUNCS} ${05_day_harib02d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${05_day_harib02d_LDS} ${05_day_harib02d_CCS} ${05_day_harib02d_FUNCO} -o ${05_day_harib02d_BOOTB}
  COMMAND cat ${05_day_harib02d_HEADB} ${05_day_harib02d_BOOTB} > ${05_day_harib02d_SYS}
  DEPENDS 05_day_harib02d_ipl
)
add_custom_target(05_day_harib02d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02d_IPLB} -i ${05_day_harib02d_OS}
  COMMAND mcopy -i ${05_day_harib02d_OS} ${05_day_harib02d_SYS} ::
  DEPENDS 05_day_harib02d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/asmhead_wine.img)

add_custom_target(05_day_harib02d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(05_day_harib02d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(05_day_harib02d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(05_day_harib02d_asmhead_od 05_day_harib02d_asmhead)
add_dependencies(05_day_harib02d_asmhead_od 05_day_harib02d_asmhead_wine)

add_dependencies(images 05_day_harib02d_asmhead)
add_dependencies(wine 05_day_harib02d_asmhead_wine)
add_dependencies(od 05_day_harib02d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/ipl10_wine.img)

add_custom_target(05_day_harib02d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(05_day_harib02d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(05_day_harib02d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(05_day_harib02d_ipl10_od 05_day_harib02d_ipl10)
add_dependencies(05_day_harib02d_ipl10_od 05_day_harib02d_ipl10_wine)

add_dependencies(images 05_day_harib02d_ipl10)
add_dependencies(wine 05_day_harib02d_ipl10_wine)
add_dependencies(od 05_day_harib02d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02d/naskfunc_wine.img)

add_custom_target(05_day_harib02d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(05_day_harib02d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(05_day_harib02d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(05_day_harib02d_naskfunc_od 05_day_harib02d_naskfunc)
add_dependencies(05_day_harib02d_naskfunc_od 05_day_harib02d_naskfunc_wine)

add_dependencies(images 05_day_harib02d_naskfunc)
add_dependencies(wine 05_day_harib02d_naskfunc_wine)
add_dependencies(od 05_day_harib02d_naskfunc_od)

#----------------------------------------------------------
