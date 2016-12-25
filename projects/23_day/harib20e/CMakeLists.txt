#----------------------------------------------------------
message(STATUS "Entering directory projects/23_day/harib20e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(23_day_harib20e_OS    ${root_BINARY_DIR}/projects/23_day/harib20e/os.img)
set(23_day_harib20e_SYS	  ${root_BINARY_DIR}/projects/23_day/harib20e/os.sys)
set(23_day_harib20e_IPLB  ${root_BINARY_DIR}/projects/23_day/harib20e/ipl.bin)
set(23_day_harib20e_IPLS  ${root_SOURCE_DIR}/projects/23_day/harib20e/ipl10.nas)
set(23_day_harib20e_HEADB ${root_BINARY_DIR}/projects/23_day/harib20e/asmhead.bin)
set(23_day_harib20e_HEADS ${root_SOURCE_DIR}/projects/23_day/harib20e/asmhead.nas)
set(23_day_harib20e_BOOTB ${root_BINARY_DIR}/projects/23_day/harib20e/boot.bin)
set(23_day_harib20e_CCO	  ${root_BINARY_DIR}/projects/23_day/harib20e/boot.o)
set(23_day_harib20e_CCS	  ${root_SOURCE_DIR}/projects/23_day/harib20e/*.c)
set(23_day_harib20e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(23_day_harib20e_FUNCS ${root_SOURCE_DIR}/projects/23_day/harib20e/naskfunc.nas)
set(23_day_harib20e_FUNCO ${root_BINARY_DIR}/projects/23_day/harib20e/naskfunc.o)
set(23_day_harib20e_FONTS ${root_SOURCE_DIR}/projects/23_day/harib20e/hankaku.txt)
set(23_day_harib20e_FONTB ${root_BINARY_DIR}/projects/23_day/harib20e/hankaku.bin)
set(23_day_harib20e_FONTO ${root_BINARY_DIR}/projects/23_day/harib20e/hankaku.o)
set(23_day_harib20e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(23_day_harib20e_LIBGC ${root_BINARY_DIR}/projects/23_day/harib20e/libgo.o)
set(23_day_harib20e_WILDOBJ ${root_BINARY_DIR}/projects/23_day/harib20e/*.o)


add_custom_target(23_day_harib20e_a_nask_run
  COMMAND ${QEMU} ${QEMUOPT} ${23_day_harib20e_OS}
  DEPENDS 23_day_harib20e_img
)
add_custom_target(23_day_harib20e_a_nask_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${23_day_harib20e_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 23_day_harib20e_img
)
add_custom_target(23_day_harib20e_a_nask_clean
  COMMAND rm -f ${23_day_harib20e_OS}
  COMMAND rm -f ${23_day_harib20e_SYS}
  COMMAND rm -f ${23_day_harib20e_IPLB}
  COMMAND rm -f ${23_day_harib20e_HEADB}
  COMMAND rm -f ${23_day_harib20e_BOOTB}
  COMMAND rm -f ${23_day_harib20e_WILDOBJ}
)
add_custom_target(23_day_harib20e_a_nask_ipl
  COMMAND ${NASK} ${23_day_harib20e_IPLS} ${23_day_harib20e_IPLB}
)
add_custom_target(23_day_harib20e_a_nask_sys
  COMMAND ${NASK} ${23_day_harib20e_HEADS} ${23_day_harib20e_HEADB}
  COMMAND ${FONT} ${23_day_harib20e_FONTS} ${23_day_harib20e_FONTB}
  COMMAND ${B2O}  ${23_day_harib20e_FONTB} ${23_day_harib20e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${23_day_harib20e_LIBGE} ${23_day_harib20e_LIBGC}
  COMMAND ${NASK} ${23_day_harib20e_FUNCS} ${23_day_harib20e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_LDS} ${23_day_harib20e_CCS} ${23_day_harib20e_WILDOBJ} -o ${23_day_harib20e_BOOTB}
  COMMAND cat ${23_day_harib20e_HEADB} ${23_day_harib20e_BOOTB} > ${23_day_harib20e_SYS}
  DEPENDS 23_day_harib20e_ipl
)
add_custom_target(23_day_harib20e_a_nask_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${23_day_harib20e_IPLB} -i ${23_day_harib20e_OS}
  COMMAND mcopy -i ${23_day_harib20e_OS} ${23_day_harib20e_SYS} ::
  DEPENDS 23_day_harib20e_sys
)
########### next target ###############
set(a.imgk.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20e/a_nask.nas)
set(a.imgk.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/a_nask.img)
set(a_wine.imgk_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/a_nask_wine.img)

add_custom_target(23_day_harib20e_a_nask
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_OUTS}
)
set(a.imgk.img_HRB ${root_BINARY_DIR}/projects/23_day/harib20e/a_nask.hrb)
add_custom_target(23_day_harib20e_a_nask_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_HRB}
  COMMAND mcopy -i ${23_day_harib20e_OS} ${a.imgk.img_HRB} ::a.hrbk.hrb
  DEPENDS 23_day_harib20e_img
)

add_dependencies(23_day_harib20e_run 23_day_harib20e_a_nask_hrb)
add_custom_target(23_day_harib20e_a_nask_wine
  COMMAND ${WINE} ${WINE_NASK} ${a.imgk.img_SRCS} ${a_wine.imgk_wine.img_OUTS}
)
add_custom_target(23_day_harib20e_a_nask_od
  COMMAND ${OD} -t x1 ${a.imgk.img_OUTS}      > ${a.imgk.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${a_wine.imgk_wine.img_OUTS} > ${a_wine.imgk_wine.img_OUTS}_t.txt
  COMMAND diff -s ${a.imgk.img_OUTS}_f.txt ${a_wine.imgk_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20e_a_nask_od 23_day_harib20e_a_nask)
add_dependencies(23_day_harib20e_a_nask_od 23_day_harib20e_a_nask_wine)

add_dependencies(images 23_day_harib20e_a_nask)
add_dependencies(wine 23_day_harib20e_a_nask_wine)
add_dependencies(od 23_day_harib20e_a_nask_od)

#----------------------------------------------------------
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/asmhead_wine.img)

add_custom_target(23_day_harib20e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(23_day_harib20e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(23_day_harib20e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20e_asmhead_od 23_day_harib20e_asmhead)
add_dependencies(23_day_harib20e_asmhead_od 23_day_harib20e_asmhead_wine)

add_dependencies(images 23_day_harib20e_asmhead)
add_dependencies(wine 23_day_harib20e_asmhead_wine)
add_dependencies(od 23_day_harib20e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20e/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/hello_wine.img)

add_custom_target(23_day_harib20e_hello
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_OUTS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/23_day/harib20e/hello.hrb)
add_custom_target(23_day_harib20e_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_HRB}
  COMMAND mcopy -i ${23_day_harib20e_OS} ${hello.img_HRB} ::hello.hrb
  DEPENDS 23_day_harib20e_img
)

add_dependencies(23_day_harib20e_run 23_day_harib20e_hello_hrb)
add_custom_target(23_day_harib20e_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(23_day_harib20e_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20e_hello_od 23_day_harib20e_hello)
add_dependencies(23_day_harib20e_hello_od 23_day_harib20e_hello_wine)

add_dependencies(images 23_day_harib20e_hello)
add_dependencies(wine 23_day_harib20e_hello_wine)
add_dependencies(od 23_day_harib20e_hello_od)

#----------------------------------------------------------
########### next target ###############
set(hello2.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20e/hello2.nas)
set(hello2.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/hello2.img)
set(hello2_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/hello2_wine.img)

add_custom_target(23_day_harib20e_hello2
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_OUTS}
)
set(hello2.img_HRB ${root_BINARY_DIR}/projects/23_day/harib20e/hello2.hrb)
add_custom_target(23_day_harib20e_hello2_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_HRB}
  COMMAND mcopy -i ${23_day_harib20e_OS} ${hello2.img_HRB} ::hello2.hrb
  DEPENDS 23_day_harib20e_img
)

add_dependencies(23_day_harib20e_run 23_day_harib20e_hello2_hrb)
add_custom_target(23_day_harib20e_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello2.img_SRCS} ${hello2_wine.img_OUTS}
)
add_custom_target(23_day_harib20e_hello2_od
  COMMAND ${OD} -t x1 ${hello2.img_OUTS}      > ${hello2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello2_wine.img_OUTS} > ${hello2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello2.img_OUTS}_f.txt ${hello2_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20e_hello2_od 23_day_harib20e_hello2)
add_dependencies(23_day_harib20e_hello2_od 23_day_harib20e_hello2_wine)

add_dependencies(images 23_day_harib20e_hello2)
add_dependencies(wine 23_day_harib20e_hello2_wine)
add_dependencies(od 23_day_harib20e_hello2_od)

#----------------------------------------------------------
########### next target ###############
set(hello5.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20e/hello5.nas)
set(hello5.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/hello5.img)
set(hello5_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/hello5_wine.img)

add_custom_target(23_day_harib20e_hello5
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_OUTS}
)
set(hello5.img_HRB ${root_BINARY_DIR}/projects/23_day/harib20e/hello5.hrb)
add_custom_target(23_day_harib20e_hello5_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_HRB}
  COMMAND mcopy -i ${23_day_harib20e_OS} ${hello5.img_HRB} ::hello5.hrb
  DEPENDS 23_day_harib20e_img
)

add_dependencies(23_day_harib20e_run 23_day_harib20e_hello5_hrb)
add_custom_target(23_day_harib20e_hello5_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello5.img_SRCS} ${hello5_wine.img_OUTS}
)
add_custom_target(23_day_harib20e_hello5_od
  COMMAND ${OD} -t x1 ${hello5.img_OUTS}      > ${hello5.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello5_wine.img_OUTS} > ${hello5_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello5.img_OUTS}_f.txt ${hello5_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20e_hello5_od 23_day_harib20e_hello5)
add_dependencies(23_day_harib20e_hello5_od 23_day_harib20e_hello5_wine)

add_dependencies(images 23_day_harib20e_hello5)
add_dependencies(wine 23_day_harib20e_hello5_wine)
add_dependencies(od 23_day_harib20e_hello5_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/ipl10_wine.img)

add_custom_target(23_day_harib20e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(23_day_harib20e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(23_day_harib20e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20e_ipl10_od 23_day_harib20e_ipl10)
add_dependencies(23_day_harib20e_ipl10_od 23_day_harib20e_ipl10_wine)

add_dependencies(images 23_day_harib20e_ipl10)
add_dependencies(wine 23_day_harib20e_ipl10_wine)
add_dependencies(od 23_day_harib20e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20e/naskfunc_wine.img)

add_custom_target(23_day_harib20e_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(23_day_harib20e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(23_day_harib20e_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20e_naskfunc_od 23_day_harib20e_naskfunc)
add_dependencies(23_day_harib20e_naskfunc_od 23_day_harib20e_naskfunc_wine)

add_dependencies(images 23_day_harib20e_naskfunc)
add_dependencies(wine 23_day_harib20e_naskfunc_wine)
add_dependencies(od 23_day_harib20e_naskfunc_od)

#----------------------------------------------------------
