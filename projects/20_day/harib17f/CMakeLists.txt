#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17f_OS    ${root_BINARY_DIR}/projects/20_day/harib17f/os.img)
set(20_day_harib17f_SYS	  ${root_BINARY_DIR}/projects/20_day/harib17f/os.sys)
set(20_day_harib17f_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17f/ipl.bin)
set(20_day_harib17f_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17f/ipl10.nas)
set(20_day_harib17f_HEADB ${root_BINARY_DIR}/projects/20_day/harib17f/asmhead.bin)
set(20_day_harib17f_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17f/asmhead.nas)
set(20_day_harib17f_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17f/boot.bin)
set(20_day_harib17f_CCO	  ${root_BINARY_DIR}/projects/20_day/harib17f/boot.o)
set(20_day_harib17f_CCS	  ${root_SOURCE_DIR}/projects/20_day/harib17f/*.c)
set(20_day_harib17f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17f_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17f/naskfunc.nas)
set(20_day_harib17f_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17f/naskfunc.o)
set(20_day_harib17f_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17f/hankaku.txt)
set(20_day_harib17f_FONTB ${root_BINARY_DIR}/projects/20_day/harib17f/hankaku.bin)
set(20_day_harib17f_FONTO ${root_BINARY_DIR}/projects/20_day/harib17f/hankaku.o)
set(20_day_harib17f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17f_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17f/libgo.o)
set(20_day_harib17f_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17f/*.o)


add_custom_target(20_day_harib17f_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17f_OS}
  DEPENDS 20_day_harib17f_img
)
add_custom_target(20_day_harib17f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17f_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 20_day_harib17f_img
)
add_custom_target(20_day_harib17f_clean
  COMMAND rm -f ${20_day_harib17f_OS}
  COMMAND rm -f ${20_day_harib17f_SYS}
  COMMAND rm -f ${20_day_harib17f_IPLB}
  COMMAND rm -f ${20_day_harib17f_HEADB}
  COMMAND rm -f ${20_day_harib17f_BOOTB}
  COMMAND rm -f ${20_day_harib17f_WILDOBJ}
)
add_custom_target(20_day_harib17f_ipl
  COMMAND ${NASK} ${20_day_harib17f_IPLS} ${20_day_harib17f_IPLB}
)
add_custom_target(20_day_harib17f_sys
  COMMAND ${NASK} ${20_day_harib17f_HEADS} ${20_day_harib17f_HEADB}
  COMMAND ${FONT} ${20_day_harib17f_FONTS} ${20_day_harib17f_FONTB}
  COMMAND ${B2O}  ${20_day_harib17f_FONTB} ${20_day_harib17f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17f_LIBGE} ${20_day_harib17f_LIBGC}
  COMMAND ${NASK} ${20_day_harib17f_FUNCS} ${20_day_harib17f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17f_LDS} ${20_day_harib17f_CCS} ${20_day_harib17f_WILDOBJ} -o ${20_day_harib17f_BOOTB}
  COMMAND cat ${20_day_harib17f_HEADB} ${20_day_harib17f_BOOTB} > ${20_day_harib17f_SYS}
  DEPENDS 20_day_harib17f_ipl
)
add_custom_target(20_day_harib17f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17f_IPLB} -i ${20_day_harib17f_OS}
  COMMAND mcopy -i ${20_day_harib17f_OS} ${20_day_harib17f_SYS} ::
  DEPENDS 20_day_harib17f_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/asmhead_wine.img)

add_custom_target(20_day_harib17f_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(20_day_harib17f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(20_day_harib17f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17f_asmhead_od 20_day_harib17f_asmhead)
add_dependencies(20_day_harib17f_asmhead_od 20_day_harib17f_asmhead_wine)

add_dependencies(images 20_day_harib17f_asmhead)
add_dependencies(wine 20_day_harib17f_asmhead_wine)
add_dependencies(od 20_day_harib17f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17f/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/hello_wine.img)

add_custom_target(20_day_harib17f_hello
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_OUTS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/20_day/harib17f/hello.hrb)
add_custom_target(20_day_harib17f_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_HRB}
  COMMAND mcopy -i ${20_day_harib17f_OS} ${hello.img_HRB} ::hello.hrb
  DEPENDS 20_day_harib17f_img
)

add_dependencies(20_day_harib17f_run 20_day_harib17f_hello_hrb)
add_custom_target(20_day_harib17f_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(20_day_harib17f_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17f_hello_od 20_day_harib17f_hello)
add_dependencies(20_day_harib17f_hello_od 20_day_harib17f_hello_wine)

add_dependencies(images 20_day_harib17f_hello)
add_dependencies(wine 20_day_harib17f_hello_wine)
add_dependencies(od 20_day_harib17f_hello_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/ipl10_wine.img)

add_custom_target(20_day_harib17f_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(20_day_harib17f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(20_day_harib17f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17f_ipl10_od 20_day_harib17f_ipl10)
add_dependencies(20_day_harib17f_ipl10_od 20_day_harib17f_ipl10_wine)

add_dependencies(images 20_day_harib17f_ipl10)
add_dependencies(wine 20_day_harib17f_ipl10_wine)
add_dependencies(od 20_day_harib17f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/naskfunc_wine.img)

add_custom_target(20_day_harib17f_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(20_day_harib17f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(20_day_harib17f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17f_naskfunc_od 20_day_harib17f_naskfunc)
add_dependencies(20_day_harib17f_naskfunc_od 20_day_harib17f_naskfunc_wine)

add_dependencies(images 20_day_harib17f_naskfunc)
add_dependencies(wine 20_day_harib17f_naskfunc_wine)
add_dependencies(od 20_day_harib17f_naskfunc_od)

#----------------------------------------------------------
