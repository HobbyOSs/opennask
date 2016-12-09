#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(05_day_harib02c_OS    ${root_BINARY_DIR}/projects/05_day/harib02c/os.img)
set(05_day_harib02c_SYS	  ${root_BINARY_DIR}/projects/05_day/harib02c/os.sys)
set(05_day_harib02c_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02c/ipl.bin)
set(05_day_harib02c_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02c/ipl10.nas)
set(05_day_harib02c_HEADB ${root_BINARY_DIR}/projects/05_day/harib02c/asmhead.bin)
set(05_day_harib02c_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02c/asmhead.nas)
set(05_day_harib02c_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02c/boot.bin)
set(05_day_harib02c_CCO	  ${root_BINARY_DIR}/projects/05_day/harib02c/boot.o)
set(05_day_harib02c_CCS	  ${root_SOURCE_DIR}/projects/05_day/harib02c/*.c)
set(05_day_harib02c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(05_day_harib02c_FUNCS ${root_SOURCE_DIR}/projects/05_day/harib02c/naskfunc.nas)
set(05_day_harib02c_FUNCO ${root_BINARY_DIR}/projects/05_day/harib02c/naskfunc.o)
set(05_day_harib02c_WILDOBJ ${root_BINARY_DIR}/projects/05_day/harib02c/*.o)

add_custom_target(05_day_harib02c_run
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02c_OS}
  DEPENDS 05_day_harib02c_img
)
add_custom_target(05_day_harib02c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02c_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 05_day_harib02c_img
)
add_custom_target(05_day_harib02c_clean
  COMMAND rm -f ${05_day_harib02c_OS}
  COMMAND rm -f ${05_day_harib02c_SYS}
  COMMAND rm -f ${05_day_harib02c_IPLB}
  COMMAND rm -f ${05_day_harib02c_HEADB}
  COMMAND rm -f ${05_day_harib02c_BOOTB}
  COMMAND rm -f ${05_day_harib02c_WILDOBJ}
)
add_custom_target(05_day_harib02c_ipl
  COMMAND ${NASK} ${05_day_harib02c_IPLS} ${05_day_harib02c_IPLB}
)
add_custom_target(05_day_harib02c_sys
  COMMAND ${NASK} ${05_day_harib02c_HEADS} ${05_day_harib02c_HEADB}
  COMMAND ${NASK} ${05_day_harib02c_FUNCS} ${05_day_harib02c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${05_day_harib02c_LDS} ${05_day_harib02c_CCS} ${05_day_harib02c_WILDOBJ} -o ${05_day_harib02c_BOOTB}
  COMMAND cat ${05_day_harib02c_HEADB} ${05_day_harib02c_BOOTB} > ${05_day_harib02c_SYS}
  DEPENDS 05_day_harib02c_ipl
)
add_custom_target(05_day_harib02c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${05_day_harib02c_IPLB} -i ${05_day_harib02c_OS}
  COMMAND mcopy -i ${05_day_harib02c_OS} ${05_day_harib02c_SYS} ::
  DEPENDS 05_day_harib02c_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/asmhead_wine.img)

add_custom_target(05_day_harib02c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(05_day_harib02c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(05_day_harib02c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(05_day_harib02c_asmhead_od 05_day_harib02c_asmhead)
add_dependencies(05_day_harib02c_asmhead_od 05_day_harib02c_asmhead_wine)

add_dependencies(images 05_day_harib02c_asmhead)
add_dependencies(wine 05_day_harib02c_asmhead_wine)
add_dependencies(od 05_day_harib02c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/ipl10_wine.img)

add_custom_target(05_day_harib02c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(05_day_harib02c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(05_day_harib02c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(05_day_harib02c_ipl10_od 05_day_harib02c_ipl10)
add_dependencies(05_day_harib02c_ipl10_od 05_day_harib02c_ipl10_wine)

add_dependencies(images 05_day_harib02c_ipl10)
add_dependencies(wine 05_day_harib02c_ipl10_wine)
add_dependencies(od 05_day_harib02c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02c/naskfunc_wine.img)

add_custom_target(05_day_harib02c_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(05_day_harib02c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(05_day_harib02c_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(05_day_harib02c_naskfunc_od 05_day_harib02c_naskfunc)
add_dependencies(05_day_harib02c_naskfunc_od 05_day_harib02c_naskfunc_wine)

add_dependencies(images 05_day_harib02c_naskfunc)
add_dependencies(wine 05_day_harib02c_naskfunc_wine)
add_dependencies(od 05_day_harib02c_naskfunc_od)

#----------------------------------------------------------
