#----------------------------------------------------------
message(STATUS "Entering directory projects/22_day/harib19c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(22_day_harib19c_OS    ${root_BINARY_DIR}/projects/22_day/harib19c/os.img)
set(22_day_harib19c_SYS	  ${root_BINARY_DIR}/projects/22_day/harib19c/os.sys)
set(22_day_harib19c_IPLB  ${root_BINARY_DIR}/projects/22_day/harib19c/ipl.bin)
set(22_day_harib19c_IPLS  ${root_SOURCE_DIR}/projects/22_day/harib19c/ipl10.nas)
set(22_day_harib19c_HEADB ${root_BINARY_DIR}/projects/22_day/harib19c/asmhead.bin)
set(22_day_harib19c_HEADS ${root_SOURCE_DIR}/projects/22_day/harib19c/asmhead.nas)
set(22_day_harib19c_BOOTB ${root_BINARY_DIR}/projects/22_day/harib19c/boot.bin)
set(22_day_harib19c_CCO	  ${root_BINARY_DIR}/projects/22_day/harib19c/boot.o)
set(22_day_harib19c_CCS	  ${root_SOURCE_DIR}/projects/22_day/harib19c/*.c)
set(22_day_harib19c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(22_day_harib19c_FUNCS ${root_SOURCE_DIR}/projects/22_day/harib19c/naskfunc.nas)
set(22_day_harib19c_FUNCO ${root_BINARY_DIR}/projects/22_day/harib19c/naskfunc.o)
set(22_day_harib19c_FONTS ${root_SOURCE_DIR}/projects/22_day/harib19c/hankaku.txt)
set(22_day_harib19c_FONTB ${root_BINARY_DIR}/projects/22_day/harib19c/hankaku.bin)
set(22_day_harib19c_FONTO ${root_BINARY_DIR}/projects/22_day/harib19c/hankaku.o)
set(22_day_harib19c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(22_day_harib19c_LIBGC ${root_BINARY_DIR}/projects/22_day/harib19c/libgo.o)
set(22_day_harib19c_WILDOBJ ${root_BINARY_DIR}/projects/22_day/harib19c/*.o)


add_custom_target(22_day_harib19c_a_nask_run
  COMMAND ${QEMU} ${QEMUOPT} ${22_day_harib19c_OS}
  DEPENDS 22_day_harib19c_img
)
add_custom_target(22_day_harib19c_a_nask_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${22_day_harib19c_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 22_day_harib19c_img
)
add_custom_target(22_day_harib19c_a_nask_clean
  COMMAND rm -f ${22_day_harib19c_OS}
  COMMAND rm -f ${22_day_harib19c_SYS}
  COMMAND rm -f ${22_day_harib19c_IPLB}
  COMMAND rm -f ${22_day_harib19c_HEADB}
  COMMAND rm -f ${22_day_harib19c_BOOTB}
  COMMAND rm -f ${22_day_harib19c_WILDOBJ}
)
add_custom_target(22_day_harib19c_a_nask_ipl
  COMMAND ${NASK} ${22_day_harib19c_IPLS} ${22_day_harib19c_IPLB}
)
add_custom_target(22_day_harib19c_a_nask_sys
  COMMAND ${NASK} ${22_day_harib19c_HEADS} ${22_day_harib19c_HEADB}
  COMMAND ${FONT} ${22_day_harib19c_FONTS} ${22_day_harib19c_FONTB}
  COMMAND ${B2O}  ${22_day_harib19c_FONTB} ${22_day_harib19c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${22_day_harib19c_LIBGE} ${22_day_harib19c_LIBGC}
  COMMAND ${NASK} ${22_day_harib19c_FUNCS} ${22_day_harib19c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${22_day_harib19c_LDS} ${22_day_harib19c_CCS} ${22_day_harib19c_WILDOBJ} -o ${22_day_harib19c_BOOTB}
  COMMAND cat ${22_day_harib19c_HEADB} ${22_day_harib19c_BOOTB} > ${22_day_harib19c_SYS}
  DEPENDS 22_day_harib19c_ipl
)
add_custom_target(22_day_harib19c_a_nask_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${22_day_harib19c_IPLB} -i ${22_day_harib19c_OS}
  COMMAND mcopy -i ${22_day_harib19c_OS} ${22_day_harib19c_SYS} ::
  DEPENDS 22_day_harib19c_sys
)
########### next target ###############
set(a.imgk.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19c/a_nask.nas)
set(a.imgk.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/a_nask.img)
set(a_wine.imgk_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/a_nask_wine.img)

add_custom_target(22_day_harib19c_a_nask
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_OUTS}
)
set(a.imgk.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19c/a_nask.hrb)
add_custom_target(22_day_harib19c_a_nask_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_HRB}
  COMMAND mcopy -i ${22_day_harib19c_OS} ${a.imgk.img_HRB} ::a.hrbk.hrb
  DEPENDS 22_day_harib19c_img
)

add_dependencies(22_day_harib19c_run 22_day_harib19c_a_nask_hrb)
add_custom_target(22_day_harib19c_a_nask_wine
  COMMAND ${WINE} ${WINE_NASK} ${a.imgk.img_SRCS} ${a_wine.imgk_wine.img_OUTS}
)
add_custom_target(22_day_harib19c_a_nask_od
  COMMAND ${OD} -t x1 ${a.imgk.img_OUTS}      > ${a.imgk.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${a_wine.imgk_wine.img_OUTS} > ${a_wine.imgk_wine.img_OUTS}_t.txt
  COMMAND diff -s ${a.imgk.img_OUTS}_f.txt ${a_wine.imgk_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19c_a_nask_od 22_day_harib19c_a_nask)
add_dependencies(22_day_harib19c_a_nask_od 22_day_harib19c_a_nask_wine)

add_dependencies(images 22_day_harib19c_a_nask)
add_dependencies(wine 22_day_harib19c_a_nask_wine)
add_dependencies(od 22_day_harib19c_a_nask_od)

#----------------------------------------------------------
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/asmhead_wine.img)

add_custom_target(22_day_harib19c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(22_day_harib19c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(22_day_harib19c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19c_asmhead_od 22_day_harib19c_asmhead)
add_dependencies(22_day_harib19c_asmhead_od 22_day_harib19c_asmhead_wine)

add_dependencies(images 22_day_harib19c_asmhead)
add_dependencies(wine 22_day_harib19c_asmhead_wine)
add_dependencies(od 22_day_harib19c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19c/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/hello_wine.img)

add_custom_target(22_day_harib19c_hello
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_OUTS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19c/hello.hrb)
add_custom_target(22_day_harib19c_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_HRB}
  COMMAND mcopy -i ${22_day_harib19c_OS} ${hello.img_HRB} ::hello.hrb
  DEPENDS 22_day_harib19c_img
)

add_dependencies(22_day_harib19c_run 22_day_harib19c_hello_hrb)
add_custom_target(22_day_harib19c_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(22_day_harib19c_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19c_hello_od 22_day_harib19c_hello)
add_dependencies(22_day_harib19c_hello_od 22_day_harib19c_hello_wine)

add_dependencies(images 22_day_harib19c_hello)
add_dependencies(wine 22_day_harib19c_hello_wine)
add_dependencies(od 22_day_harib19c_hello_od)

#----------------------------------------------------------
########### next target ###############
set(hello2.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19c/hello2.nas)
set(hello2.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/hello2.img)
set(hello2_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/hello2_wine.img)

add_custom_target(22_day_harib19c_hello2
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_OUTS}
)
set(hello2.img_HRB ${root_BINARY_DIR}/projects/22_day/harib19c/hello2.hrb)
add_custom_target(22_day_harib19c_hello2_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_HRB}
  COMMAND mcopy -i ${22_day_harib19c_OS} ${hello2.img_HRB} ::hello2.hrb
  DEPENDS 22_day_harib19c_img
)

add_dependencies(22_day_harib19c_run 22_day_harib19c_hello2_hrb)
add_custom_target(22_day_harib19c_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello2.img_SRCS} ${hello2_wine.img_OUTS}
)
add_custom_target(22_day_harib19c_hello2_od
  COMMAND ${OD} -t x1 ${hello2.img_OUTS}      > ${hello2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello2_wine.img_OUTS} > ${hello2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello2.img_OUTS}_f.txt ${hello2_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19c_hello2_od 22_day_harib19c_hello2)
add_dependencies(22_day_harib19c_hello2_od 22_day_harib19c_hello2_wine)

add_dependencies(images 22_day_harib19c_hello2)
add_dependencies(wine 22_day_harib19c_hello2_wine)
add_dependencies(od 22_day_harib19c_hello2_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/ipl10_wine.img)

add_custom_target(22_day_harib19c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(22_day_harib19c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(22_day_harib19c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19c_ipl10_od 22_day_harib19c_ipl10)
add_dependencies(22_day_harib19c_ipl10_od 22_day_harib19c_ipl10_wine)

add_dependencies(images 22_day_harib19c_ipl10)
add_dependencies(wine 22_day_harib19c_ipl10_wine)
add_dependencies(od 22_day_harib19c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/22_day/harib19c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/22_day/harib19c/naskfunc_wine.img)

add_custom_target(22_day_harib19c_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(22_day_harib19c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(22_day_harib19c_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(22_day_harib19c_naskfunc_od 22_day_harib19c_naskfunc)
add_dependencies(22_day_harib19c_naskfunc_od 22_day_harib19c_naskfunc_wine)

add_dependencies(images 22_day_harib19c_naskfunc)
add_dependencies(wine 22_day_harib19c_naskfunc_wine)
add_dependencies(od 22_day_harib19c_naskfunc_od)

#----------------------------------------------------------
