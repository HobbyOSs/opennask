#----------------------------------------------------------
message(STATUS "Entering directory projects/22_day/harib19g/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(22_day_harib19g_OS    ${root_BINARY_DIR}/projects/22_day/harib19g/os.img)
set(22_day_harib19g_SYS	  ${root_BINARY_DIR}/projects/22_day/harib19g/os.sys)
set(22_day_harib19g_IPLB  ${root_BINARY_DIR}/projects/22_day/harib19g/ipl.bin)
set(22_day_harib19g_IPLS  ${root_SOURCE_DIR}/projects/22_day/harib19g/ipl10.nas)
set(22_day_harib19g_HEADB ${root_BINARY_DIR}/projects/22_day/harib19g/asmhead.bin)
set(22_day_harib19g_HEADS ${root_SOURCE_DIR}/projects/22_day/harib19g/asmhead.nas)
set(22_day_harib19g_BOOTB ${root_BINARY_DIR}/projects/22_day/harib19g/boot.bin)
set(22_day_harib19g_CCO	  ${root_BINARY_DIR}/projects/22_day/harib19g/boot.o)
set(22_day_harib19g_CCS	  ${root_SOURCE_DIR}/projects/22_day/harib19g/*.c)
set(22_day_harib19g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(22_day_harib19g_FUNCS ${root_SOURCE_DIR}/projects/22_day/harib19g/naskfunc.nas)
set(22_day_harib19g_FUNCO ${root_BINARY_DIR}/projects/22_day/harib19g/naskfunc.o)
set(22_day_harib19g_FONTS ${root_SOURCE_DIR}/projects/22_day/harib19g/hankaku.txt)
set(22_day_harib19g_FONTB ${root_BINARY_DIR}/projects/22_day/harib19g/hankaku.bin)
set(22_day_harib19g_FONTO ${root_BINARY_DIR}/projects/22_day/harib19g/hankaku.o)
set(22_day_harib19g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(22_day_harib19g_LIBGC ${root_BINARY_DIR}/projects/22_day/harib19g/libgo.o)
set(22_day_harib19g_WILDOBJ ${root_BINARY_DIR}/projects/22_day/harib19g/*.o)


add_custom_target(22_day_harib19g_a_nask_run
  COMMAND ${QEMU} ${QEMUOPT} ${22_day_harib19g_OS}
  DEPENDS 22_day_harib19g_img
)
add_custom_target(22_day_harib19g_a_nask_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${22_day_harib19g_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 22_day_harib19g_img
)
add_custom_target(22_day_harib19g_a_nask_clean
  COMMAND rm -f ${22_day_harib19g_OS}
  COMMAND rm -f ${22_day_harib19g_SYS}
  COMMAND rm -f ${22_day_harib19g_IPLB}
  COMMAND rm -f ${22_day_harib19g_HEADB}
  COMMAND rm -f ${22_day_harib19g_BOOTB}
  COMMAND rm -f ${22_day_harib19g_WILDOBJ}
)
add_custom_target(22_day_harib19g_a_nask_ipl
  COMMAND ${NASK} ${22_day_harib19g_IPLS} ${22_day_harib19g_IPLB}
)
add_custom_target(22_day_harib19g_a_nask_sys
  COMMAND ${NASK} ${22_day_harib19g_HEADS} ${22_day_harib19g_HEADB}
  COMMAND ${FONT} ${22_day_harib19g_FONTS} ${22_day_harib19g_FONTB}
  COMMAND ${B2O}  ${22_day_harib19g_FONTB} ${22_day_harib19g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${22_day_harib19g_LIBGE} ${22_day_harib19g_LIBGC}
  COMMAND ${NASK} ${22_day_harib19g_FUNCS} ${22_day_harib19g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${22_day_harib19g_LDS} ${22_day_harib19g_CCS} ${22_day_harib19g_WILDOBJ} -o ${22_day_harib19g_BOOTB}
  COMMAND cat ${22_day_harib19g_HEADB} ${22_day_harib19g_BOOTB} > ${22_day_harib19g_SYS}
  DEPENDS 22_day_harib19g_ipl
)
add_custom_target(22_day_harib19g_a_nask_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${22_day_harib19g_IPLB} -i ${22_day_harib19g_OS}
  COMMAND mcopy -i ${22_day_harib19g_OS} ${22_day_harib19g_SYS} ::
  DEPENDS 22_day_harib19g_sys
)
########### next target ###############
set(a.imgk.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19g/a_nask.nas)
set(a.imgk.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/a_nask.img)
set(a_wine.imgk_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/a_nask_wine.img)

add_custom_target(22_day_harib19g_a_nask
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_OUTS}
)
set(a.imgk.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19g/a_nask.hrb)
add_custom_target(22_day_harib19g_a_nask_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_HRB}
  COMMAND mcopy -i ${22_day_harib19g_OS} ${a.imgk.img_HRB} ::a.hrbk.hrb
  DEPENDS 22_day_harib19g_img
)

add_dependencies(22_day_harib19g_run 22_day_harib19g_a_nask_hrb)
add_custom_target(22_day_harib19g_a_nask_wine
  COMMAND ${WINE} ${WINE_NASK} ${a.imgk.img_SRCS} ${a_wine.imgk_wine.img_OUTS}
)
add_custom_target(22_day_harib19g_a_nask_od
  COMMAND ${OD} -t x1 ${a.imgk.img_OUTS}      > ${a.imgk.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${a_wine.imgk_wine.img_OUTS} > ${a_wine.imgk_wine.img_OUTS}_t.txt
  COMMAND diff -s ${a.imgk.img_OUTS}_f.txt ${a_wine.imgk_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19g_a_nask_od 22_day_harib19g_a_nask)
add_dependencies(22_day_harib19g_a_nask_od 22_day_harib19g_a_nask_wine)

add_dependencies(images 22_day_harib19g_a_nask)
add_dependencies(wine 22_day_harib19g_a_nask_wine)
add_dependencies(od 22_day_harib19g_a_nask_od)

#----------------------------------------------------------
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19g/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/asmhead_wine.img)

add_custom_target(22_day_harib19g_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(22_day_harib19g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(22_day_harib19g_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19g_asmhead_od 22_day_harib19g_asmhead)
add_dependencies(22_day_harib19g_asmhead_od 22_day_harib19g_asmhead_wine)

add_dependencies(images 22_day_harib19g_asmhead)
add_dependencies(wine 22_day_harib19g_asmhead_wine)
add_dependencies(od 22_day_harib19g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19g/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/hello_wine.img)

add_custom_target(22_day_harib19g_hello
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_OUTS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19g/hello.hrb)
add_custom_target(22_day_harib19g_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_HRB}
  COMMAND mcopy -i ${22_day_harib19g_OS} ${hello.img_HRB} ::hello.hrb
  DEPENDS 22_day_harib19g_img
)

add_dependencies(22_day_harib19g_run 22_day_harib19g_hello_hrb)
add_custom_target(22_day_harib19g_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(22_day_harib19g_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19g_hello_od 22_day_harib19g_hello)
add_dependencies(22_day_harib19g_hello_od 22_day_harib19g_hello_wine)

add_dependencies(images 22_day_harib19g_hello)
add_dependencies(wine 22_day_harib19g_hello_wine)
add_dependencies(od 22_day_harib19g_hello_od)

#----------------------------------------------------------
########### next target ###############
set(hello2.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19g/hello2.nas)
set(hello2.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/hello2.img)
set(hello2_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/hello2_wine.img)

add_custom_target(22_day_harib19g_hello2
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_OUTS}
)
set(hello2.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19g/hello2.hrb)
add_custom_target(22_day_harib19g_hello2_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_HRB}
  COMMAND mcopy -i ${22_day_harib19g_OS} ${hello2.img_HRB} ::hello2.hrb
  DEPENDS 22_day_harib19g_img
)

add_dependencies(22_day_harib19g_run 22_day_harib19g_hello2_hrb)
add_custom_target(22_day_harib19g_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello2.img_SRCS} ${hello2_wine.img_OUTS}
)
add_custom_target(22_day_harib19g_hello2_od
  COMMAND ${OD} -t x1 ${hello2.img_OUTS}      > ${hello2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello2_wine.img_OUTS} > ${hello2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello2.img_OUTS}_f.txt ${hello2_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19g_hello2_od 22_day_harib19g_hello2)
add_dependencies(22_day_harib19g_hello2_od 22_day_harib19g_hello2_wine)

add_dependencies(images 22_day_harib19g_hello2)
add_dependencies(wine 22_day_harib19g_hello2_wine)
add_dependencies(od 22_day_harib19g_hello2_od)

#----------------------------------------------------------
########### next target ###############
set(hello5.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19g/hello5.nas)
set(hello5.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/hello5.img)
set(hello5_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/hello5_wine.img)

add_custom_target(22_day_harib19g_hello5
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_OUTS}
)
set(hello5.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19g/hello5.hrb)
add_custom_target(22_day_harib19g_hello5_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_HRB}
  COMMAND mcopy -i ${22_day_harib19g_OS} ${hello5.img_HRB} ::hello5.hrb
  DEPENDS 22_day_harib19g_img
)

add_dependencies(22_day_harib19g_run 22_day_harib19g_hello5_hrb)
add_custom_target(22_day_harib19g_hello5_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello5.img_SRCS} ${hello5_wine.img_OUTS}
)
add_custom_target(22_day_harib19g_hello5_od
  COMMAND ${OD} -t x1 ${hello5.img_OUTS}      > ${hello5.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello5_wine.img_OUTS} > ${hello5_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello5.img_OUTS}_f.txt ${hello5_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19g_hello5_od 22_day_harib19g_hello5)
add_dependencies(22_day_harib19g_hello5_od 22_day_harib19g_hello5_wine)

add_dependencies(images 22_day_harib19g_hello5)
add_dependencies(wine 22_day_harib19g_hello5_wine)
add_dependencies(od 22_day_harib19g_hello5_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19g/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/ipl10_wine.img)

add_custom_target(22_day_harib19g_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(22_day_harib19g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(22_day_harib19g_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19g_ipl10_od 22_day_harib19g_ipl10)
add_dependencies(22_day_harib19g_ipl10_od 22_day_harib19g_ipl10_wine)

add_dependencies(images 22_day_harib19g_ipl10)
add_dependencies(wine 22_day_harib19g_ipl10_wine)
add_dependencies(od 22_day_harib19g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19g/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19g/naskfunc_wine.img)

add_custom_target(22_day_harib19g_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(22_day_harib19g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(22_day_harib19g_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19g_naskfunc_od 22_day_harib19g_naskfunc)
add_dependencies(22_day_harib19g_naskfunc_od 22_day_harib19g_naskfunc_wine)

add_dependencies(images 22_day_harib19g_naskfunc)
add_dependencies(wine 22_day_harib19g_naskfunc_wine)
add_dependencies(od 22_day_harib19g_naskfunc_od)

#----------------------------------------------------------
