#----------------------------------------------------------
message(STATUS "Entering directory projects/23_day/harib20a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(23_day_harib20a_OS    ${root_BINARY_DIR}/projects/23_day/harib20a/os.img)
set(23_day_harib20a_SYS	  ${root_BINARY_DIR}/projects/23_day/harib20a/os.sys)
set(23_day_harib20a_IPLB  ${root_BINARY_DIR}/projects/23_day/harib20a/ipl.bin)
set(23_day_harib20a_IPLS  ${root_SOURCE_DIR}/projects/23_day/harib20a/ipl10.nas)
set(23_day_harib20a_HEADB ${root_BINARY_DIR}/projects/23_day/harib20a/asmhead.bin)
set(23_day_harib20a_HEADS ${root_SOURCE_DIR}/projects/23_day/harib20a/asmhead.nas)
set(23_day_harib20a_BOOTB ${root_BINARY_DIR}/projects/23_day/harib20a/boot.bin)
set(23_day_harib20a_CCO	  ${root_BINARY_DIR}/projects/23_day/harib20a/boot.o)
set(23_day_harib20a_CCS	  ${root_SOURCE_DIR}/projects/23_day/harib20a/*.c)
set(23_day_harib20a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(23_day_harib20a_FUNCS ${root_SOURCE_DIR}/projects/23_day/harib20a/naskfunc.nas)
set(23_day_harib20a_FUNCO ${root_BINARY_DIR}/projects/23_day/harib20a/naskfunc.o)
set(23_day_harib20a_FONTS ${root_SOURCE_DIR}/projects/23_day/harib20a/hankaku.txt)
set(23_day_harib20a_FONTB ${root_BINARY_DIR}/projects/23_day/harib20a/hankaku.bin)
set(23_day_harib20a_FONTO ${root_BINARY_DIR}/projects/23_day/harib20a/hankaku.o)
set(23_day_harib20a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(23_day_harib20a_LIBGC ${root_BINARY_DIR}/projects/23_day/harib20a/libgo.o)
set(23_day_harib20a_WILDOBJ ${root_BINARY_DIR}/projects/23_day/harib20a/*.o)


add_custom_target(23_day_harib20a_a_nask_run
  COMMAND ${QEMU} ${QEMUOPT} ${23_day_harib20a_OS}
  DEPENDS 23_day_harib20a_img
)
add_custom_target(23_day_harib20a_a_nask_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${23_day_harib20a_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 23_day_harib20a_img
)
add_custom_target(23_day_harib20a_a_nask_clean
  COMMAND rm -f ${23_day_harib20a_OS}
  COMMAND rm -f ${23_day_harib20a_SYS}
  COMMAND rm -f ${23_day_harib20a_IPLB}
  COMMAND rm -f ${23_day_harib20a_HEADB}
  COMMAND rm -f ${23_day_harib20a_BOOTB}
  COMMAND rm -f ${23_day_harib20a_WILDOBJ}
)
add_custom_target(23_day_harib20a_a_nask_ipl
  COMMAND ${NASK} ${23_day_harib20a_IPLS} ${23_day_harib20a_IPLB}
)
add_custom_target(23_day_harib20a_a_nask_sys
  COMMAND ${NASK} ${23_day_harib20a_HEADS} ${23_day_harib20a_HEADB}
  COMMAND ${FONT} ${23_day_harib20a_FONTS} ${23_day_harib20a_FONTB}
  COMMAND ${B2O}  ${23_day_harib20a_FONTB} ${23_day_harib20a_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${23_day_harib20a_LIBGE} ${23_day_harib20a_LIBGC}
  COMMAND ${NASK} ${23_day_harib20a_FUNCS} ${23_day_harib20a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${23_day_harib20a_LDS} ${23_day_harib20a_CCS} ${23_day_harib20a_WILDOBJ} -o ${23_day_harib20a_BOOTB}
  COMMAND cat ${23_day_harib20a_HEADB} ${23_day_harib20a_BOOTB} > ${23_day_harib20a_SYS}
  DEPENDS 23_day_harib20a_ipl
)
add_custom_target(23_day_harib20a_a_nask_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${23_day_harib20a_IPLB} -i ${23_day_harib20a_OS}
  COMMAND mcopy -i ${23_day_harib20a_OS} ${23_day_harib20a_SYS} ::
  DEPENDS 23_day_harib20a_sys
)
########### next target ###############
set(a.imgk.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20a/a_nask.nas)
set(a.imgk.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/a_nask.img)
set(a_wine.imgk_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/a_nask_wine.img)

add_custom_target(23_day_harib20a_a_nask
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_OUTS}
)
set(a.imgk.img_HRB ${root_BINARY_DIR}/projects/23_day/harib20a/a_nask.hrb)
add_custom_target(23_day_harib20a_a_nask_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_HRB}
  COMMAND mcopy -i ${23_day_harib20a_OS} ${a.imgk.img_HRB} ::a.hrbk.hrb
  DEPENDS 23_day_harib20a_img
)

add_dependencies(23_day_harib20a_run 23_day_harib20a_a_nask_hrb)
add_custom_target(23_day_harib20a_a_nask_wine
  COMMAND ${WINE} ${WINE_NASK} ${a.imgk.img_SRCS} ${a_wine.imgk_wine.img_OUTS}
)
add_custom_target(23_day_harib20a_a_nask_od
  COMMAND ${OD} -t x1 ${a.imgk.img_OUTS}      > ${a.imgk.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${a_wine.imgk_wine.img_OUTS} > ${a_wine.imgk_wine.img_OUTS}_t.txt
  COMMAND diff -s ${a.imgk.img_OUTS}_f.txt ${a_wine.imgk_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20a_a_nask_od 23_day_harib20a_a_nask)
add_dependencies(23_day_harib20a_a_nask_od 23_day_harib20a_a_nask_wine)

add_dependencies(images 23_day_harib20a_a_nask)
add_dependencies(wine 23_day_harib20a_a_nask_wine)
add_dependencies(od 23_day_harib20a_a_nask_od)

#----------------------------------------------------------
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/asmhead_wine.img)

add_custom_target(23_day_harib20a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(23_day_harib20a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(23_day_harib20a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20a_asmhead_od 23_day_harib20a_asmhead)
add_dependencies(23_day_harib20a_asmhead_od 23_day_harib20a_asmhead_wine)

add_dependencies(images 23_day_harib20a_asmhead)
add_dependencies(wine 23_day_harib20a_asmhead_wine)
add_dependencies(od 23_day_harib20a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20a/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/hello_wine.img)

add_custom_target(23_day_harib20a_hello
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_OUTS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/23_day/harib20a/hello.hrb)
add_custom_target(23_day_harib20a_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_HRB}
  COMMAND mcopy -i ${23_day_harib20a_OS} ${hello.img_HRB} ::hello.hrb
  DEPENDS 23_day_harib20a_img
)

add_dependencies(23_day_harib20a_run 23_day_harib20a_hello_hrb)
add_custom_target(23_day_harib20a_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(23_day_harib20a_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20a_hello_od 23_day_harib20a_hello)
add_dependencies(23_day_harib20a_hello_od 23_day_harib20a_hello_wine)

add_dependencies(images 23_day_harib20a_hello)
add_dependencies(wine 23_day_harib20a_hello_wine)
add_dependencies(od 23_day_harib20a_hello_od)

#----------------------------------------------------------
########### next target ###############
set(hello2.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20a/hello2.nas)
set(hello2.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/hello2.img)
set(hello2_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/hello2_wine.img)

add_custom_target(23_day_harib20a_hello2
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_OUTS}
)
set(hello2.img_HRB ${root_BINARY_DIR}/projects/23_day/harib20a/hello2.hrb)
add_custom_target(23_day_harib20a_hello2_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_HRB}
  COMMAND mcopy -i ${23_day_harib20a_OS} ${hello2.img_HRB} ::hello2.hrb
  DEPENDS 23_day_harib20a_img
)

add_dependencies(23_day_harib20a_run 23_day_harib20a_hello2_hrb)
add_custom_target(23_day_harib20a_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello2.img_SRCS} ${hello2_wine.img_OUTS}
)
add_custom_target(23_day_harib20a_hello2_od
  COMMAND ${OD} -t x1 ${hello2.img_OUTS}      > ${hello2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello2_wine.img_OUTS} > ${hello2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello2.img_OUTS}_f.txt ${hello2_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20a_hello2_od 23_day_harib20a_hello2)
add_dependencies(23_day_harib20a_hello2_od 23_day_harib20a_hello2_wine)

add_dependencies(images 23_day_harib20a_hello2)
add_dependencies(wine 23_day_harib20a_hello2_wine)
add_dependencies(od 23_day_harib20a_hello2_od)

#----------------------------------------------------------
########### next target ###############
set(hello5.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20a/hello5.nas)
set(hello5.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/hello5.img)
set(hello5_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/hello5_wine.img)

add_custom_target(23_day_harib20a_hello5
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_OUTS}
)
set(hello5.img_HRB ${root_BINARY_DIR}/projects/23_day/harib20a/hello5.hrb)
add_custom_target(23_day_harib20a_hello5_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_HRB}
  COMMAND mcopy -i ${23_day_harib20a_OS} ${hello5.img_HRB} ::hello5.hrb
  DEPENDS 23_day_harib20a_img
)

add_dependencies(23_day_harib20a_run 23_day_harib20a_hello5_hrb)
add_custom_target(23_day_harib20a_hello5_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello5.img_SRCS} ${hello5_wine.img_OUTS}
)
add_custom_target(23_day_harib20a_hello5_od
  COMMAND ${OD} -t x1 ${hello5.img_OUTS}      > ${hello5.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello5_wine.img_OUTS} > ${hello5_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello5.img_OUTS}_f.txt ${hello5_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20a_hello5_od 23_day_harib20a_hello5)
add_dependencies(23_day_harib20a_hello5_od 23_day_harib20a_hello5_wine)

add_dependencies(images 23_day_harib20a_hello5)
add_dependencies(wine 23_day_harib20a_hello5_wine)
add_dependencies(od 23_day_harib20a_hello5_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/ipl10_wine.img)

add_custom_target(23_day_harib20a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(23_day_harib20a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(23_day_harib20a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20a_ipl10_od 23_day_harib20a_ipl10)
add_dependencies(23_day_harib20a_ipl10_od 23_day_harib20a_ipl10_wine)

add_dependencies(images 23_day_harib20a_ipl10)
add_dependencies(wine 23_day_harib20a_ipl10_wine)
add_dependencies(od 23_day_harib20a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/23_day/harib20a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/23_day/harib20a/naskfunc_wine.img)

add_custom_target(23_day_harib20a_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(23_day_harib20a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(23_day_harib20a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(23_day_harib20a_naskfunc_od 23_day_harib20a_naskfunc)
add_dependencies(23_day_harib20a_naskfunc_od 23_day_harib20a_naskfunc_wine)

add_dependencies(images 23_day_harib20a_naskfunc)
add_dependencies(wine 23_day_harib20a_naskfunc_wine)
add_dependencies(od 23_day_harib20a_naskfunc_od)

#----------------------------------------------------------
