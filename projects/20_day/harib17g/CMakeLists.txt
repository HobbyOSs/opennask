#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17g/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17g_OS    ${root_BINARY_DIR}/projects/20_day/harib17g/os.img)
set(20_day_harib17g_SYS	  ${root_BINARY_DIR}/projects/20_day/harib17g/os.sys)
set(20_day_harib17g_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17g/ipl.bin)
set(20_day_harib17g_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17g/ipl10.nas)
set(20_day_harib17g_HEADB ${root_BINARY_DIR}/projects/20_day/harib17g/asmhead.bin)
set(20_day_harib17g_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17g/asmhead.nas)
set(20_day_harib17g_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17g/boot.bin)
set(20_day_harib17g_CCO	  ${root_BINARY_DIR}/projects/20_day/harib17g/boot.o)
set(20_day_harib17g_CCS	  ${root_SOURCE_DIR}/projects/20_day/harib17g/*.c)
set(20_day_harib17g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17g_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17g/naskfunc.nas)
set(20_day_harib17g_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17g/naskfunc.o)
set(20_day_harib17g_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17g/hankaku.txt)
set(20_day_harib17g_FONTB ${root_BINARY_DIR}/projects/20_day/harib17g/hankaku.bin)
set(20_day_harib17g_FONTO ${root_BINARY_DIR}/projects/20_day/harib17g/hankaku.o)
set(20_day_harib17g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17g_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17g/libgo.o)
set(20_day_harib17g_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17g/*.o)


add_custom_target(20_day_harib17g_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17g_OS}
  DEPENDS 20_day_harib17g_img
)
add_custom_target(20_day_harib17g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17g_img
)
add_custom_target(20_day_harib17g_clean
  COMMAND rm -f ${20_day_harib17g_OS}
  COMMAND rm -f ${20_day_harib17g_SYS}
  COMMAND rm -f ${20_day_harib17g_IPLB}
  COMMAND rm -f ${20_day_harib17g_HEADB}
  COMMAND rm -f ${20_day_harib17g_BOOTB}
  COMMAND rm -f ${20_day_harib17g_WILDOBJ}
)
add_custom_target(20_day_harib17g_ipl
  COMMAND ${NASK} ${20_day_harib17g_IPLS} ${20_day_harib17g_IPLB}
)
add_custom_target(20_day_harib17g_sys
  COMMAND ${NASK} ${20_day_harib17g_HEADS} ${20_day_harib17g_HEADB}
  COMMAND ${FONT} ${20_day_harib17g_FONTS} ${20_day_harib17g_FONTB}
  COMMAND ${B2O}  ${20_day_harib17g_FONTB} ${20_day_harib17g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17g_LIBGE} ${20_day_harib17g_LIBGC}
  COMMAND ${NASK} ${20_day_harib17g_FUNCS} ${20_day_harib17g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17g_LDS} ${20_day_harib17g_CCS} ${20_day_harib17g_WILDOBJ} -o ${20_day_harib17g_BOOTB}
  COMMAND cat ${20_day_harib17g_HEADB} ${20_day_harib17g_BOOTB} > ${20_day_harib17g_SYS}
  DEPENDS 20_day_harib17g_ipl
)
add_custom_target(20_day_harib17g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17g_IPLB} -i ${20_day_harib17g_OS}
  COMMAND mcopy -i ${20_day_harib17g_OS} ${20_day_harib17g_SYS} ::
  DEPENDS 20_day_harib17g_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17g/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/asmhead_wine.img)

add_custom_target(20_day_harib17g_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(20_day_harib17g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(20_day_harib17g_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17g_asmhead_od 20_day_harib17g_asmhead)
add_dependencies(20_day_harib17g_asmhead_od 20_day_harib17g_asmhead_wine)

add_dependencies(images 20_day_harib17g_asmhead)
add_dependencies(wine 20_day_harib17g_asmhead_wine)
add_dependencies(od 20_day_harib17g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17g/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/hello_wine.img)

add_custom_target(20_day_harib17g_hello
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_OUTS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/20_day/harib17g/hello.hrb)
add_custom_target(20_day_harib17g_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_HRB}
  COMMAND mcopy -i ${20_day_harib17g_OS} ${hello.img_HRB} ::hello.hrb
  DEPENDS 20_day_harib17g_img
)

add_dependencies(20_day_harib17g_run 20_day_harib17g_hello_hrb)
add_custom_target(20_day_harib17g_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(20_day_harib17g_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17g_hello_od 20_day_harib17g_hello)
add_dependencies(20_day_harib17g_hello_od 20_day_harib17g_hello_wine)

add_dependencies(images 20_day_harib17g_hello)
add_dependencies(wine 20_day_harib17g_hello_wine)
add_dependencies(od 20_day_harib17g_hello_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17g/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/ipl10_wine.img)

add_custom_target(20_day_harib17g_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(20_day_harib17g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(20_day_harib17g_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17g_ipl10_od 20_day_harib17g_ipl10)
add_dependencies(20_day_harib17g_ipl10_od 20_day_harib17g_ipl10_wine)

add_dependencies(images 20_day_harib17g_ipl10)
add_dependencies(wine 20_day_harib17g_ipl10_wine)
add_dependencies(od 20_day_harib17g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17g/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/naskfunc_wine.img)

add_custom_target(20_day_harib17g_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(20_day_harib17g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(20_day_harib17g_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17g_naskfunc_od 20_day_harib17g_naskfunc)
add_dependencies(20_day_harib17g_naskfunc_od 20_day_harib17g_naskfunc_wine)

add_dependencies(images 20_day_harib17g_naskfunc)
add_dependencies(wine 20_day_harib17g_naskfunc_wine)
add_dependencies(od 20_day_harib17g_naskfunc_od)

#----------------------------------------------------------
