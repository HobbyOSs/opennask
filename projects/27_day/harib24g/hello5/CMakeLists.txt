#----------------------------------------------------------
message(STATUS "Entering directory projects/27_day/harib24g/hello5/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(27_day_harib24g_hello5_OS    ${root_BINARY_DIR}/projects/27_day/harib24g/hello5/os.img)
set(27_day_harib24g_hello5_SYS	  ${root_BINARY_DIR}/projects/27_day/harib24g/hello5/os.sys)
set(27_day_harib24g_hello5_IPLB  ${root_BINARY_DIR}/projects/27_day/harib24g/hello5/ipl.bin)
set(27_day_harib24g_hello5_IPLS  ${root_SOURCE_DIR}/projects/27_day/harib24g/hello5/ipl10.nas)
set(27_day_harib24g_hello5_HEADB ${root_BINARY_DIR}/projects/27_day/harib24g/hello5/asmhead.bin)
set(27_day_harib24g_hello5_HEADS ${root_SOURCE_DIR}/projects/27_day/harib24g/hello5/asmhead.nas)
set(27_day_harib24g_hello5_BOOTB ${root_BINARY_DIR}/projects/27_day/harib24g/hello5/boot.bin)
set(27_day_harib24g_hello5_CCO	  ${root_BINARY_DIR}/projects/27_day/harib24g/hello5/boot.o)
set(27_day_harib24g_hello5_CCS	  ${root_SOURCE_DIR}/projects/27_day/harib24g/hello5/*.c)
set(27_day_harib24g_hello5_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(27_day_harib24g_hello5_WILDOBJ ${root_BINARY_DIR}/projects/27_day/harib24g/hello5/*.o)


add_custom_target(27_day_harib24g_hello5_hello5_run
  COMMAND ${QEMU} ${QEMUOPT} ${27_day_harib24g_hello5_OS}
  DEPENDS 27_day_harib24g_hello5_img
)
add_custom_target(27_day_harib24g_hello5_hello5_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${27_day_harib24g_hello5_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 27_day_harib24g_hello5_img
)
add_custom_target(27_day_harib24g_hello5_hello5_clean
  COMMAND rm -f ${27_day_harib24g_hello5_OS}
  COMMAND rm -f ${27_day_harib24g_hello5_SYS}
  COMMAND rm -f ${27_day_harib24g_hello5_IPLB}
  COMMAND rm -f ${27_day_harib24g_hello5_HEADB}
  COMMAND rm -f ${27_day_harib24g_hello5_BOOTB}
  COMMAND rm -f ${27_day_harib24g_hello5_WILDOBJ}
)
add_custom_target(27_day_harib24g_hello5_hello5_ipl
  COMMAND ${NASK} ${27_day_harib24g_hello5_IPLS} ${27_day_harib24g_hello5_IPLB}
)
add_custom_target(27_day_harib24g_hello5_hello5_sys
  COMMAND ${NASK} ${27_day_harib24g_hello5_HEADS} ${27_day_harib24g_hello5_HEADB}
  COMMAND gcc ${BINOPT} -T ${27_day_harib24g_hello5_LDS} ${27_day_harib24g_hello5_CCS} -o ${27_day_harib24g_hello5_BOOTB}
  COMMAND cat ${27_day_harib24g_hello5_HEADB} ${27_day_harib24g_hello5_BOOTB} > ${27_day_harib24g_hello5_SYS}
  DEPENDS 27_day_harib24g_hello5_ipl
)
add_custom_target(27_day_harib24g_hello5_hello5_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${27_day_harib24g_hello5_IPLB} -i ${27_day_harib24g_hello5_OS}
  COMMAND mcopy -i ${27_day_harib24g_hello5_OS} ${27_day_harib24g_hello5_SYS} ::
  DEPENDS 27_day_harib24g_hello5_sys
)
########### next target ###############
set(hello5.img_SRCS ${root_SOURCE_DIR}/projects/27_day/harib24g/hello5/hello5.nas)
set(hello5.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24g/hello5/hello5.img)
set(hello5_wine.img_OUTS ${root_BINARY_DIR}/projects/27_day/harib24g/hello5/hello5_wine.img)

add_custom_target(27_day_harib24g_hello5_hello5
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_OUTS}
)
set(hello5.img_HRB ${root_BINARY_DIR}/projects/27_day/harib24g/hello5/hello5.hrb)
add_custom_target(27_day_harib24g_hello5_hello5_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_HRB}
  COMMAND mcopy -i ${27_day_harib24g_hello5_OS} ${hello5.img_HRB} ::hello5.hrb
  DEPENDS 27_day_harib24g_hello5_img
)

add_dependencies(27_day_harib24g_hello5_run 27_day_harib24g_hello5_hello5_hrb)
add_custom_target(27_day_harib24g_hello5_hello5_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello5.img_SRCS} ${hello5_wine.img_OUTS}
)
add_custom_target(27_day_harib24g_hello5_hello5_od
  COMMAND ${OD} -t x1 ${hello5.img_OUTS}      > ${hello5.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello5_wine.img_OUTS} > ${hello5_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello5.img_OUTS}_f.txt ${hello5_wine.img_OUTS}_t.txt
)

add_dependencies(27_day_harib24g_hello5_hello5_od 27_day_harib24g_hello5_hello5)
add_dependencies(27_day_harib24g_hello5_hello5_od 27_day_harib24g_hello5_hello5_wine)

add_dependencies(images 27_day_harib24g_hello5_hello5)
add_dependencies(wine 27_day_harib24g_hello5_hello5_wine)
add_dependencies(od 27_day_harib24g_hello5_hello5_od)

#----------------------------------------------------------
