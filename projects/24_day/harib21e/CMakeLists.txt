#----------------------------------------------------------
message(STATUS "Entering directory projects/24_day/harib21e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(24_day_harib21e_OS    ${root_BINARY_DIR}/projects/24_day/harib21e/os.img)
set(24_day_harib21e_SYS	  ${root_BINARY_DIR}/projects/24_day/harib21e/os.sys)
set(24_day_harib21e_IPLB  ${root_BINARY_DIR}/projects/24_day/harib21e/ipl.bin)
set(24_day_harib21e_IPLS  ${root_SOURCE_DIR}/projects/24_day/harib21e/ipl10.nas)
set(24_day_harib21e_HEADB ${root_BINARY_DIR}/projects/24_day/harib21e/asmhead.bin)
set(24_day_harib21e_HEADS ${root_SOURCE_DIR}/projects/24_day/harib21e/asmhead.nas)
set(24_day_harib21e_BOOTB ${root_BINARY_DIR}/projects/24_day/harib21e/boot.bin)
set(24_day_harib21e_CCO	  ${root_BINARY_DIR}/projects/24_day/harib21e/boot.o)
set(24_day_harib21e_CCS	  ${root_SOURCE_DIR}/projects/24_day/harib21e/*.c)
set(24_day_harib21e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(24_day_harib21e_FUNCS ${root_SOURCE_DIR}/projects/24_day/harib21e/naskfunc.nas)
set(24_day_harib21e_FUNCO ${root_BINARY_DIR}/projects/24_day/harib21e/naskfunc.o)
set(24_day_harib21e_FONTS ${root_SOURCE_DIR}/projects/24_day/harib21e/hankaku.txt)
set(24_day_harib21e_FONTB ${root_BINARY_DIR}/projects/24_day/harib21e/hankaku.bin)
set(24_day_harib21e_FONTO ${root_BINARY_DIR}/projects/24_day/harib21e/hankaku.o)
set(24_day_harib21e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(24_day_harib21e_LIBGC ${root_BINARY_DIR}/projects/24_day/harib21e/libgo.o)
set(24_day_harib21e_WILDOBJ ${root_BINARY_DIR}/projects/24_day/harib21e/*.o)


add_custom_target(24_day_harib21e_a_nask_run
  COMMAND ${QEMU} ${QEMUOPT} ${24_day_harib21e_OS}
  DEPENDS 24_day_harib21e_img
)
add_custom_target(24_day_harib21e_a_nask_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${24_day_harib21e_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 24_day_harib21e_img
)
add_custom_target(24_day_harib21e_a_nask_clean
  COMMAND rm -f ${24_day_harib21e_OS}
  COMMAND rm -f ${24_day_harib21e_SYS}
  COMMAND rm -f ${24_day_harib21e_IPLB}
  COMMAND rm -f ${24_day_harib21e_HEADB}
  COMMAND rm -f ${24_day_harib21e_BOOTB}
  COMMAND rm -f ${24_day_harib21e_WILDOBJ}
)
add_custom_target(24_day_harib21e_a_nask_ipl
  COMMAND ${NASK} ${24_day_harib21e_IPLS} ${24_day_harib21e_IPLB}
)
add_custom_target(24_day_harib21e_a_nask_sys
  COMMAND ${NASK} ${24_day_harib21e_HEADS} ${24_day_harib21e_HEADB}
  COMMAND ${FONT} ${24_day_harib21e_FONTS} ${24_day_harib21e_FONTB}
  COMMAND ${B2O}  ${24_day_harib21e_FONTB} ${24_day_harib21e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${24_day_harib21e_LIBGE} ${24_day_harib21e_LIBGC}
  COMMAND ${NASK} ${24_day_harib21e_FUNCS} ${24_day_harib21e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_LDS} ${24_day_harib21e_CCS} ${24_day_harib21e_WILDOBJ} -o ${24_day_harib21e_BOOTB}
  COMMAND cat ${24_day_harib21e_HEADB} ${24_day_harib21e_BOOTB} > ${24_day_harib21e_SYS}
  DEPENDS 24_day_harib21e_ipl
)
add_custom_target(24_day_harib21e_a_nask_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${24_day_harib21e_IPLB} -i ${24_day_harib21e_OS}
  COMMAND mcopy -i ${24_day_harib21e_OS} ${24_day_harib21e_SYS} ::
  DEPENDS 24_day_harib21e_sys
)
########### next target ###############
set(a.imgk.img_SRCS ${root_SOURCE_DIR}/projects/24_day/harib21e/a_nask.nas)
set(a.imgk.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/a_nask.img)
set(a_wine.imgk_wine.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/a_nask_wine.img)

add_custom_target(24_day_harib21e_a_nask
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_OUTS}
)
set(a.imgk.img_HRB ${root_BINARY_DIR}/projects/24_day/harib21e/a_nask.hrb)
add_custom_target(24_day_harib21e_a_nask_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_HRB}
  COMMAND mcopy -i ${24_day_harib21e_OS} ${a.imgk.img_HRB} ::a.hrbk.hrb
  DEPENDS 24_day_harib21e_img
)

add_dependencies(24_day_harib21e_run 24_day_harib21e_a_nask_hrb)
add_custom_target(24_day_harib21e_a_nask_wine
  COMMAND ${WINE} ${WINE_NASK} ${a.imgk.img_SRCS} ${a_wine.imgk_wine.img_OUTS}
)
add_custom_target(24_day_harib21e_a_nask_od
  COMMAND ${OD} -t x1 ${a.imgk.img_OUTS}      > ${a.imgk.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${a_wine.imgk_wine.img_OUTS} > ${a_wine.imgk_wine.img_OUTS}_t.txt
  COMMAND diff -s ${a.imgk.img_OUTS}_f.txt ${a_wine.imgk_wine.img_OUTS}_t.txt
)

add_dependencies(24_day_harib21e_a_nask_od 24_day_harib21e_a_nask)
add_dependencies(24_day_harib21e_a_nask_od 24_day_harib21e_a_nask_wine)

add_dependencies(images 24_day_harib21e_a_nask)
add_dependencies(wine 24_day_harib21e_a_nask_wine)
add_dependencies(od 24_day_harib21e_a_nask_od)

#----------------------------------------------------------
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/24_day/harib21e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/asmhead_wine.img)

add_custom_target(24_day_harib21e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(24_day_harib21e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(24_day_harib21e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(24_day_harib21e_asmhead_od 24_day_harib21e_asmhead)
add_dependencies(24_day_harib21e_asmhead_od 24_day_harib21e_asmhead_wine)

add_dependencies(images 24_day_harib21e_asmhead)
add_dependencies(wine 24_day_harib21e_asmhead_wine)
add_dependencies(od 24_day_harib21e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/24_day/harib21e/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/hello_wine.img)

add_custom_target(24_day_harib21e_hello
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_OUTS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/24_day/harib21e/hello.hrb)
add_custom_target(24_day_harib21e_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_HRB}
  COMMAND mcopy -i ${24_day_harib21e_OS} ${hello.img_HRB} ::hello.hrb
  DEPENDS 24_day_harib21e_img
)

add_dependencies(24_day_harib21e_run 24_day_harib21e_hello_hrb)
add_custom_target(24_day_harib21e_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(24_day_harib21e_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(24_day_harib21e_hello_od 24_day_harib21e_hello)
add_dependencies(24_day_harib21e_hello_od 24_day_harib21e_hello_wine)

add_dependencies(images 24_day_harib21e_hello)
add_dependencies(wine 24_day_harib21e_hello_wine)
add_dependencies(od 24_day_harib21e_hello_od)

#----------------------------------------------------------
########### next target ###############
set(hello2.img_SRCS ${root_SOURCE_DIR}/projects/24_day/harib21e/hello2.nas)
set(hello2.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/hello2.img)
set(hello2_wine.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/hello2_wine.img)

add_custom_target(24_day_harib21e_hello2
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_OUTS}
)
set(hello2.img_HRB ${root_BINARY_DIR}/projects/24_day/harib21e/hello2.hrb)
add_custom_target(24_day_harib21e_hello2_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_HRB}
  COMMAND mcopy -i ${24_day_harib21e_OS} ${hello2.img_HRB} ::hello2.hrb
  DEPENDS 24_day_harib21e_img
)

add_dependencies(24_day_harib21e_run 24_day_harib21e_hello2_hrb)
add_custom_target(24_day_harib21e_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello2.img_SRCS} ${hello2_wine.img_OUTS}
)
add_custom_target(24_day_harib21e_hello2_od
  COMMAND ${OD} -t x1 ${hello2.img_OUTS}      > ${hello2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello2_wine.img_OUTS} > ${hello2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello2.img_OUTS}_f.txt ${hello2_wine.img_OUTS}_t.txt
)

add_dependencies(24_day_harib21e_hello2_od 24_day_harib21e_hello2)
add_dependencies(24_day_harib21e_hello2_od 24_day_harib21e_hello2_wine)

add_dependencies(images 24_day_harib21e_hello2)
add_dependencies(wine 24_day_harib21e_hello2_wine)
add_dependencies(od 24_day_harib21e_hello2_od)

#----------------------------------------------------------
########### next target ###############
set(hello5.img_SRCS ${root_SOURCE_DIR}/projects/24_day/harib21e/hello5.nas)
set(hello5.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/hello5.img)
set(hello5_wine.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/hello5_wine.img)

add_custom_target(24_day_harib21e_hello5
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_OUTS}
)
set(hello5.img_HRB ${root_BINARY_DIR}/projects/24_day/harib21e/hello5.hrb)
add_custom_target(24_day_harib21e_hello5_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello5.img_SRCS} ${hello5.img_HRB}
  COMMAND mcopy -i ${24_day_harib21e_OS} ${hello5.img_HRB} ::hello5.hrb
  DEPENDS 24_day_harib21e_img
)

add_dependencies(24_day_harib21e_run 24_day_harib21e_hello5_hrb)
add_custom_target(24_day_harib21e_hello5_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello5.img_SRCS} ${hello5_wine.img_OUTS}
)
add_custom_target(24_day_harib21e_hello5_od
  COMMAND ${OD} -t x1 ${hello5.img_OUTS}      > ${hello5.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello5_wine.img_OUTS} > ${hello5_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello5.img_OUTS}_f.txt ${hello5_wine.img_OUTS}_t.txt
)

add_dependencies(24_day_harib21e_hello5_od 24_day_harib21e_hello5)
add_dependencies(24_day_harib21e_hello5_od 24_day_harib21e_hello5_wine)

add_dependencies(images 24_day_harib21e_hello5)
add_dependencies(wine 24_day_harib21e_hello5_wine)
add_dependencies(od 24_day_harib21e_hello5_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/24_day/harib21e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/ipl10_wine.img)

add_custom_target(24_day_harib21e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(24_day_harib21e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(24_day_harib21e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(24_day_harib21e_ipl10_od 24_day_harib21e_ipl10)
add_dependencies(24_day_harib21e_ipl10_od 24_day_harib21e_ipl10_wine)

add_dependencies(images 24_day_harib21e_ipl10)
add_dependencies(wine 24_day_harib21e_ipl10_wine)
add_dependencies(od 24_day_harib21e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/24_day/harib21e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/24_day/harib21e/naskfunc_wine.img)

add_custom_target(24_day_harib21e_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(24_day_harib21e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(24_day_harib21e_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(24_day_harib21e_naskfunc_od 24_day_harib21e_naskfunc)
add_dependencies(24_day_harib21e_naskfunc_od 24_day_harib21e_naskfunc_wine)

add_dependencies(images 24_day_harib21e_naskfunc)
add_dependencies(wine 24_day_harib21e_naskfunc_wine)
add_dependencies(od 24_day_harib21e_naskfunc_od)

#----------------------------------------------------------
