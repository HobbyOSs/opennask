#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17h/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17h_OS    ${root_BINARY_DIR}/projects/20_day/harib17h/os.img)
set(20_day_harib17h_SYS	  ${root_BINARY_DIR}/projects/20_day/harib17h/os.sys)
set(20_day_harib17h_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17h/ipl.bin)
set(20_day_harib17h_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17h/ipl10.nas)
set(20_day_harib17h_HEADB ${root_BINARY_DIR}/projects/20_day/harib17h/asmhead.bin)
set(20_day_harib17h_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17h/asmhead.nas)
set(20_day_harib17h_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17h/boot.bin)
set(20_day_harib17h_CCO	  ${root_BINARY_DIR}/projects/20_day/harib17h/boot.o)
set(20_day_harib17h_CCS	  ${root_SOURCE_DIR}/projects/20_day/harib17h/*.c)
set(20_day_harib17h_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17h_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17h/naskfunc.nas)
set(20_day_harib17h_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17h/naskfunc.o)
set(20_day_harib17h_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17h/hankaku.txt)
set(20_day_harib17h_FONTB ${root_BINARY_DIR}/projects/20_day/harib17h/hankaku.bin)
set(20_day_harib17h_FONTO ${root_BINARY_DIR}/projects/20_day/harib17h/hankaku.o)
set(20_day_harib17h_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17h_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17h/libgo.o)
set(20_day_harib17h_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17h/*.o)


add_custom_target(20_day_harib17h_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17h_OS}
  DEPENDS 20_day_harib17h_img
)
add_custom_target(20_day_harib17h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17h_img
)
add_custom_target(20_day_harib17h_clean
  COMMAND rm -f ${20_day_harib17h_OS}
  COMMAND rm -f ${20_day_harib17h_SYS}
  COMMAND rm -f ${20_day_harib17h_IPLB}
  COMMAND rm -f ${20_day_harib17h_HEADB}
  COMMAND rm -f ${20_day_harib17h_BOOTB}
  COMMAND rm -f ${20_day_harib17h_WILDOBJ}
)
add_custom_target(20_day_harib17h_ipl
  COMMAND ${NASK} ${20_day_harib17h_IPLS} ${20_day_harib17h_IPLB}
)
add_custom_target(20_day_harib17h_sys
  COMMAND ${NASK} ${20_day_harib17h_HEADS} ${20_day_harib17h_HEADB}
  COMMAND ${FONT} ${20_day_harib17h_FONTS} ${20_day_harib17h_FONTB}
  COMMAND ${B2O}  ${20_day_harib17h_FONTB} ${20_day_harib17h_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17h_LIBGE} ${20_day_harib17h_LIBGC}
  COMMAND ${NASK} ${20_day_harib17h_FUNCS} ${20_day_harib17h_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17h_LDS} ${20_day_harib17h_CCS} ${20_day_harib17h_WILDOBJ} -o ${20_day_harib17h_BOOTB}
  COMMAND cat ${20_day_harib17h_HEADB} ${20_day_harib17h_BOOTB} > ${20_day_harib17h_SYS}
  DEPENDS 20_day_harib17h_ipl
)
add_custom_target(20_day_harib17h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17h_IPLB} -i ${20_day_harib17h_OS}
  COMMAND mcopy -i ${20_day_harib17h_OS} ${20_day_harib17h_SYS} ::
  DEPENDS 20_day_harib17h_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17h/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/asmhead_wine.img)

add_custom_target(20_day_harib17h_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(20_day_harib17h_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(20_day_harib17h_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17h_asmhead_od 20_day_harib17h_asmhead)
add_dependencies(20_day_harib17h_asmhead_od 20_day_harib17h_asmhead_wine)

add_dependencies(images 20_day_harib17h_asmhead)
add_dependencies(wine 20_day_harib17h_asmhead_wine)
add_dependencies(od 20_day_harib17h_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17h/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/hello_wine.img)

add_custom_target(20_day_harib17h_hello
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_OUTS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/20_day/harib17h/hello.hrb)
add_custom_target(20_day_harib17h_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_HRB}
  COMMAND mcopy -i ${20_day_harib17h_OS} ${hello.img_HRB} ::hello.hrb
  DEPENDS 20_day_harib17h_img
)

add_dependencies(20_day_harib17h_run 20_day_harib17h_hello_hrb)
add_custom_target(20_day_harib17h_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(20_day_harib17h_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17h_hello_od 20_day_harib17h_hello)
add_dependencies(20_day_harib17h_hello_od 20_day_harib17h_hello_wine)

add_dependencies(images 20_day_harib17h_hello)
add_dependencies(wine 20_day_harib17h_hello_wine)
add_dependencies(od 20_day_harib17h_hello_od)

#----------------------------------------------------------
########### next target ###############
set(hello2.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17h/hello2.nas)
set(hello2.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/hello2.img)
set(hello2_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/hello2_wine.img)

add_custom_target(20_day_harib17h_hello2
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_OUTS}
)
set(hello2.img_HRB ${root_BINARY_DIR}/projects/20_day/harib17h/hello2.hrb)
add_custom_target(20_day_harib17h_hello2_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_HRB}
  COMMAND mcopy -i ${20_day_harib17h_OS} ${hello2.img_HRB} ::hello2.hrb
  DEPENDS 20_day_harib17h_img
)

add_dependencies(20_day_harib17h_run 20_day_harib17h_hello2_hrb)
add_custom_target(20_day_harib17h_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello2.img_SRCS} ${hello2_wine.img_OUTS}
)
add_custom_target(20_day_harib17h_hello2_od
  COMMAND ${OD} -t x1 ${hello2.img_OUTS}      > ${hello2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello2_wine.img_OUTS} > ${hello2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello2.img_OUTS}_f.txt ${hello2_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17h_hello2_od 20_day_harib17h_hello2)
add_dependencies(20_day_harib17h_hello2_od 20_day_harib17h_hello2_wine)

add_dependencies(images 20_day_harib17h_hello2)
add_dependencies(wine 20_day_harib17h_hello2_wine)
add_dependencies(od 20_day_harib17h_hello2_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17h/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/ipl10_wine.img)

add_custom_target(20_day_harib17h_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(20_day_harib17h_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(20_day_harib17h_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17h_ipl10_od 20_day_harib17h_ipl10)
add_dependencies(20_day_harib17h_ipl10_od 20_day_harib17h_ipl10_wine)

add_dependencies(images 20_day_harib17h_ipl10)
add_dependencies(wine 20_day_harib17h_ipl10_wine)
add_dependencies(od 20_day_harib17h_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17h/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17h/naskfunc_wine.img)

add_custom_target(20_day_harib17h_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(20_day_harib17h_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(20_day_harib17h_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17h_naskfunc_od 20_day_harib17h_naskfunc)
add_dependencies(20_day_harib17h_naskfunc_od 20_day_harib17h_naskfunc_wine)

add_dependencies(images 20_day_harib17h_naskfunc)
add_dependencies(wine 20_day_harib17h_naskfunc_wine)
add_dependencies(od 20_day_harib17h_naskfunc_od)

#----------------------------------------------------------
