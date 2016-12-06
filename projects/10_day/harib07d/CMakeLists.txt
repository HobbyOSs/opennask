#----------------------------------------------------------
message(STATUS "Entering directory projects/10_day/harib07d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(10_day_harib07d_OS    ${root_BINARY_DIR}/projects/10_day/harib07d/os.img)
set(10_day_harib07d_SYS	  ${root_BINARY_DIR}/projects/10_day/harib07d/os.sys)
set(10_day_harib07d_IPLB  ${root_BINARY_DIR}/projects/10_day/harib07d/ipl.bin)
set(10_day_harib07d_IPLS  ${root_SOURCE_DIR}/projects/10_day/harib07d/ipl10.nas)
set(10_day_harib07d_HEADB ${root_BINARY_DIR}/projects/10_day/harib07d/asmhead.bin)
set(10_day_harib07d_HEADS ${root_SOURCE_DIR}/projects/10_day/harib07d/asmhead.nas)
set(10_day_harib07d_BOOTB ${root_BINARY_DIR}/projects/10_day/harib07d/boot.bin)
set(10_day_harib07d_CCO	  ${root_BINARY_DIR}/projects/10_day/harib07d/boot.o)
set(10_day_harib07d_CCS	  ${root_SOURCE_DIR}/projects/10_day/harib07d/*.c)
set(10_day_harib07d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(10_day_harib07d_FUNCO ${root_BINARY_DIR}/projects/10_day/harib07d/naskfunc.o)
set(10_day_harib07d_FUNCS ${root_SOURCE_DIR}/projects/10_day/harib07d/naskfunc.nas)

add_custom_target(10_day_harib07d_run
  COMMAND ${QEMU} ${QEMUOPT} ${10_day_harib07d_OS}
  DEPENDS 10_day_harib07d_img
)
add_custom_target(10_day_harib07d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${10_day_harib07d_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 10_day_harib07d_img
)
add_custom_target(10_day_harib07d_clean
  COMMAND rm -f ${10_day_harib07d_OS}
  COMMAND rm -f ${10_day_harib07d_SYS}
  COMMAND rm -f ${10_day_harib07d_IPLB}
  COMMAND rm -f ${10_day_harib07d_HEADB}
  COMMAND rm -f ${10_day_harib07d_BOOTB}
  COMMAND rm -f ${10_day_harib07d_CCO}
  COMMAND rm -f ${10_day_harib07d_FUNCO}
)
add_custom_target(10_day_harib07d_ipl
  COMMAND ${NASK} ${10_day_harib07d_IPLS} ${10_day_harib07d_IPLB}
)
add_custom_target(10_day_harib07d_sys
  COMMAND ${NASK} ${10_day_harib07d_FUNCS} ${10_day_harib07d_FUNCO}
  COMMAND ${NASK} ${10_day_harib07d_HEADS} ${10_day_harib07d_HEADB}
  COMMAND ${NASK} ${10_day_harib07d_FUNCS} ${10_day_harib07d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${10_day_harib07d_LDS} ${10_day_harib07d_CCS} ${10_day_harib07d_FUNCO} -o ${10_day_harib07d_BOOTB}
  COMMAND cat ${10_day_harib07d_HEADB} ${10_day_harib07d_BOOTB} > ${10_day_harib07d_SYS}
  DEPENDS 10_day_harib07d_ipl
)
add_custom_target(10_day_harib07d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${10_day_harib07d_IPLB} -i ${10_day_harib07d_OS}
  COMMAND mcopy -i ${10_day_harib07d_OS} ${10_day_harib07d_SYS} ::
  DEPENDS 10_day_harib07d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07d/asmhead_wine.img)

add_custom_target(10_day_harib07d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(10_day_harib07d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(10_day_harib07d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(10_day_harib07d_asmhead_od 10_day_harib07d_asmhead)
add_dependencies(10_day_harib07d_asmhead_od 10_day_harib07d_asmhead_wine)

add_dependencies(images 10_day_harib07d_asmhead)
add_dependencies(wine 10_day_harib07d_asmhead_wine)
add_dependencies(od 10_day_harib07d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07d/ipl10_wine.img)

add_custom_target(10_day_harib07d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(10_day_harib07d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(10_day_harib07d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(10_day_harib07d_ipl10_od 10_day_harib07d_ipl10)
add_dependencies(10_day_harib07d_ipl10_od 10_day_harib07d_ipl10_wine)

add_dependencies(images 10_day_harib07d_ipl10)
add_dependencies(wine 10_day_harib07d_ipl10_wine)
add_dependencies(od 10_day_harib07d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07d/naskfunc_wine.img)

add_custom_target(10_day_harib07d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(10_day_harib07d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(10_day_harib07d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(10_day_harib07d_naskfunc_od 10_day_harib07d_naskfunc)
add_dependencies(10_day_harib07d_naskfunc_od 10_day_harib07d_naskfunc_wine)

add_dependencies(images 10_day_harib07d_naskfunc)
add_dependencies(wine 10_day_harib07d_naskfunc_wine)
add_dependencies(od 10_day_harib07d_naskfunc_od)

#----------------------------------------------------------
