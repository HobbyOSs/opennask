#----------------------------------------------------------
message(STATUS "Entering directory projects/21_day/harib18g/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(21_day_harib18g_OS    ${root_BINARY_DIR}/projects/21_day/harib18g/os.img)
set(21_day_harib18g_SYS	  ${root_BINARY_DIR}/projects/21_day/harib18g/os.sys)
set(21_day_harib18g_IPLB  ${root_BINARY_DIR}/projects/21_day/harib18g/ipl.bin)
set(21_day_harib18g_IPLS  ${root_SOURCE_DIR}/projects/21_day/harib18g/ipl10.nas)
set(21_day_harib18g_HEADB ${root_BINARY_DIR}/projects/21_day/harib18g/asmhead.bin)
set(21_day_harib18g_HEADS ${root_SOURCE_DIR}/projects/21_day/harib18g/asmhead.nas)
set(21_day_harib18g_BOOTB ${root_BINARY_DIR}/projects/21_day/harib18g/boot.bin)
set(21_day_harib18g_CCO	  ${root_BINARY_DIR}/projects/21_day/harib18g/boot.o)
set(21_day_harib18g_CCS	  ${root_SOURCE_DIR}/projects/21_day/harib18g/*.c)
set(21_day_harib18g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(21_day_harib18g_FUNCS ${root_SOURCE_DIR}/projects/21_day/harib18g/naskfunc.nas)
set(21_day_harib18g_FUNCO ${root_BINARY_DIR}/projects/21_day/harib18g/naskfunc.o)
set(21_day_harib18g_FONTS ${root_SOURCE_DIR}/projects/21_day/harib18g/hankaku.txt)
set(21_day_harib18g_FONTB ${root_BINARY_DIR}/projects/21_day/harib18g/hankaku.bin)
set(21_day_harib18g_FONTO ${root_BINARY_DIR}/projects/21_day/harib18g/hankaku.o)
set(21_day_harib18g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(21_day_harib18g_LIBGC ${root_BINARY_DIR}/projects/21_day/harib18g/libgo.o)
set(21_day_harib18g_WILDOBJ ${root_BINARY_DIR}/projects/21_day/harib18g/*.o)


add_custom_target(21_day_harib18g_run
  COMMAND ${QEMU} ${QEMUOPT} ${21_day_harib18g_OS}
  DEPENDS 21_day_harib18g_img
)
add_custom_target(21_day_harib18g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${21_day_harib18g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 21_day_harib18g_img
)
add_custom_target(21_day_harib18g_clean
  COMMAND rm -f ${21_day_harib18g_OS}
  COMMAND rm -f ${21_day_harib18g_SYS}
  COMMAND rm -f ${21_day_harib18g_IPLB}
  COMMAND rm -f ${21_day_harib18g_HEADB}
  COMMAND rm -f ${21_day_harib18g_BOOTB}
  COMMAND rm -f ${21_day_harib18g_WILDOBJ}
)
add_custom_target(21_day_harib18g_ipl
  COMMAND ${NASK} ${21_day_harib18g_IPLS} ${21_day_harib18g_IPLB}
)
add_custom_target(21_day_harib18g_sys
  COMMAND ${NASK} ${21_day_harib18g_HEADS} ${21_day_harib18g_HEADB}
  COMMAND ${FONT} ${21_day_harib18g_FONTS} ${21_day_harib18g_FONTB}
  COMMAND ${B2O}  ${21_day_harib18g_FONTB} ${21_day_harib18g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${21_day_harib18g_LIBGE} ${21_day_harib18g_LIBGC}
  COMMAND ${NASK} ${21_day_harib18g_FUNCS} ${21_day_harib18g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${21_day_harib18g_LDS} ${21_day_harib18g_CCS} ${21_day_harib18g_WILDOBJ} -o ${21_day_harib18g_BOOTB}
  COMMAND cat ${21_day_harib18g_HEADB} ${21_day_harib18g_BOOTB} > ${21_day_harib18g_SYS}
  DEPENDS 21_day_harib18g_ipl
)
add_custom_target(21_day_harib18g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${21_day_harib18g_IPLB} -i ${21_day_harib18g_OS}
  COMMAND mcopy -i ${21_day_harib18g_OS} ${21_day_harib18g_SYS} ::
  DEPENDS 21_day_harib18g_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18g/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/asmhead_wine.img)

add_custom_target(21_day_harib18g_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(21_day_harib18g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(21_day_harib18g_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18g_asmhead_od 21_day_harib18g_asmhead)
add_dependencies(21_day_harib18g_asmhead_od 21_day_harib18g_asmhead_wine)

add_dependencies(images 21_day_harib18g_asmhead)
add_dependencies(wine 21_day_harib18g_asmhead_wine)
add_dependencies(od 21_day_harib18g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(crack2.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18g/crack2.nas)
set(crack2.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/crack2.img)
set(crack2_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/crack2_wine.img)

add_custom_target(21_day_harib18g_crack2
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack2.img_SRCS} ${crack2.img_OUTS}
)
set(crack2.img_HRB ${root_BINARY_DIR}/projects/21_day/harib18g/crack2.hrb)
add_custom_target(21_day_harib18g_crack2_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${crack2.img_SRCS} ${crack2.img_HRB}
  COMMAND mcopy -i ${21_day_harib18g_OS} ${crack2.img_HRB} ::crack2.hrb
  DEPENDS 21_day_harib18g_img
)

add_dependencies(21_day_harib18g_run 21_day_harib18g_crack2_hrb)
add_custom_target(21_day_harib18g_crack2_wine
  COMMAND ${WINE} ${WINE_NASK} ${crack2.img_SRCS} ${crack2_wine.img_OUTS}
)
add_custom_target(21_day_harib18g_crack2_od
  COMMAND ${OD} -t x1 ${crack2.img_OUTS}      > ${crack2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${crack2_wine.img_OUTS} > ${crack2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${crack2.img_OUTS}_f.txt ${crack2_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18g_crack2_od 21_day_harib18g_crack2)
add_dependencies(21_day_harib18g_crack2_od 21_day_harib18g_crack2_wine)

add_dependencies(images 21_day_harib18g_crack2)
add_dependencies(wine 21_day_harib18g_crack2_wine)
add_dependencies(od 21_day_harib18g_crack2_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18g/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/hello_wine.img)

add_custom_target(21_day_harib18g_hello
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_OUTS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/21_day/harib18g/hello.hrb)
add_custom_target(21_day_harib18g_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello.img_SRCS} ${hello.img_HRB}
  COMMAND mcopy -i ${21_day_harib18g_OS} ${hello.img_HRB} ::hello.hrb
  DEPENDS 21_day_harib18g_img
)

add_dependencies(21_day_harib18g_run 21_day_harib18g_hello_hrb)
add_custom_target(21_day_harib18g_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(21_day_harib18g_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18g_hello_od 21_day_harib18g_hello)
add_dependencies(21_day_harib18g_hello_od 21_day_harib18g_hello_wine)

add_dependencies(images 21_day_harib18g_hello)
add_dependencies(wine 21_day_harib18g_hello_wine)
add_dependencies(od 21_day_harib18g_hello_od)

#----------------------------------------------------------
########### next target ###############
set(hello2.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18g/hello2.nas)
set(hello2.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/hello2.img)
set(hello2_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/hello2_wine.img)

add_custom_target(21_day_harib18g_hello2
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_OUTS}
)
set(hello2.img_HRB ${root_BINARY_DIR}/projects/21_day/harib18g/hello2.hrb)
add_custom_target(21_day_harib18g_hello2_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${hello2.img_SRCS} ${hello2.img_HRB}
  COMMAND mcopy -i ${21_day_harib18g_OS} ${hello2.img_HRB} ::hello2.hrb
  DEPENDS 21_day_harib18g_img
)

add_dependencies(21_day_harib18g_run 21_day_harib18g_hello2_hrb)
add_custom_target(21_day_harib18g_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello2.img_SRCS} ${hello2_wine.img_OUTS}
)
add_custom_target(21_day_harib18g_hello2_od
  COMMAND ${OD} -t x1 ${hello2.img_OUTS}      > ${hello2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello2_wine.img_OUTS} > ${hello2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello2.img_OUTS}_f.txt ${hello2_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18g_hello2_od 21_day_harib18g_hello2)
add_dependencies(21_day_harib18g_hello2_od 21_day_harib18g_hello2_wine)

add_dependencies(images 21_day_harib18g_hello2)
add_dependencies(wine 21_day_harib18g_hello2_wine)
add_dependencies(od 21_day_harib18g_hello2_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18g/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/ipl10_wine.img)

add_custom_target(21_day_harib18g_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(21_day_harib18g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(21_day_harib18g_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18g_ipl10_od 21_day_harib18g_ipl10)
add_dependencies(21_day_harib18g_ipl10_od 21_day_harib18g_ipl10_wine)

add_dependencies(images 21_day_harib18g_ipl10)
add_dependencies(wine 21_day_harib18g_ipl10_wine)
add_dependencies(od 21_day_harib18g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18g/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/naskfunc_wine.img)

add_custom_target(21_day_harib18g_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(21_day_harib18g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(21_day_harib18g_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18g_naskfunc_od 21_day_harib18g_naskfunc)
add_dependencies(21_day_harib18g_naskfunc_od 21_day_harib18g_naskfunc_wine)

add_dependencies(images 21_day_harib18g_naskfunc)
add_dependencies(wine 21_day_harib18g_naskfunc_wine)
add_dependencies(od 21_day_harib18g_naskfunc_od)

#----------------------------------------------------------
########### next target ###############
set(a.imgk.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18g/a_nask.nas)
set(a.imgk.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/a_nask.img)
set(a_wine.imgk_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18g/a_nask_wine.img)

add_custom_target(21_day_harib18g_a_nask
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_OUTS}
)
set(a.imgk.img_HRB ${root_BINARY_DIR}/projects/21_day/harib18g/a_nask.hrb)
add_custom_target(21_day_harib18g_a_nask_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${a.imgk.img_SRCS} ${a.imgk.img_HRB}
  COMMAND mcopy -i ${21_day_harib18g_OS} ${a.imgk.img_HRB} ::a.hrbk.hrb
  DEPENDS 21_day_harib18g_img
)

add_dependencies(21_day_harib18g_run 21_day_harib18g_a_nask_hrb)
add_custom_target(21_day_harib18g_a_nask_wine
  COMMAND ${WINE} ${WINE_NASK} ${a.imgk.img_SRCS} ${a_wine.imgk_wine.img_OUTS}
)
add_custom_target(21_day_harib18g_a_nask_od
  COMMAND ${OD} -t x1 ${a.imgk.img_OUTS}      > ${a.imgk.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${a_wine.imgk_wine.img_OUTS} > ${a_wine.imgk_wine.img_OUTS}_t.txt
  COMMAND diff -s ${a.imgk.img_OUTS}_f.txt ${a_wine.imgk_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18g_a_nask_od 21_day_harib18g_a_nask)
add_dependencies(21_day_harib18g_a_nask_od 21_day_harib18g_a_nask_wine)

add_dependencies(images 21_day_harib18g_a_nask)
add_dependencies(wine 21_day_harib18g_a_nask_wine)
add_dependencies(od 21_day_harib18g_a_nask_od)

#----------------------------------------------------------
