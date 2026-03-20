#----------------------------------------------------------
message(STATUS "Entering directory projects/21_day/harib18f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(21_day_harib18f_OS    ${root_BINARY_DIR}/projects/21_day/harib18f/os.img)
set(21_day_harib18f_SYS	  ${root_BINARY_DIR}/projects/21_day/harib18f/os.sys)
set(21_day_harib18f_IPLB  ${root_BINARY_DIR}/projects/21_day/harib18f/ipl.bin)
set(21_day_harib18f_IPLS  ${root_SOURCE_DIR}/projects/21_day/harib18f/ipl10.nas)
set(21_day_harib18f_HEADB ${root_BINARY_DIR}/projects/21_day/harib18f/asmhead.bin)
set(21_day_harib18f_HEADS ${root_SOURCE_DIR}/projects/21_day/harib18f/asmhead.nas)
set(21_day_harib18f_BOOTB ${root_BINARY_DIR}/projects/21_day/harib18f/boot.bin)
set(21_day_harib18f_CCO	  ${root_BINARY_DIR}/projects/21_day/harib18f/boot.o)
set(21_day_harib18f_CCS	  ${root_SOURCE_DIR}/projects/21_day/harib18f/*.c)
set(21_day_harib18f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(21_day_harib18f_FUNCS ${root_SOURCE_DIR}/projects/21_day/harib18f/naskfunc.nas)
set(21_day_harib18f_FUNCO ${root_BINARY_DIR}/projects/21_day/harib18f/naskfunc.o)
set(21_day_harib18f_FONTS ${root_SOURCE_DIR}/projects/21_day/harib18f/hankaku.txt)
set(21_day_harib18f_FONTB ${root_BINARY_DIR}/projects/21_day/harib18f/hankaku.bin)
set(21_day_harib18f_FONTO ${root_BINARY_DIR}/projects/21_day/harib18f/hankaku.o)
set(21_day_harib18f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(21_day_harib18f_LIBGC ${root_BINARY_DIR}/projects/21_day/harib18f/libgo.o)
set(21_day_harib18f_WILDOBJ ${root_BINARY_DIR}/projects/21_day/harib18f/*.o)


add_custom_target(21_day_harib18f_run
  COMMAND ${QEMU} ${QEMUOPT} ${21_day_harib18f_OS}
  DEPENDS 21_day_harib18f_img
)
add_custom_target(21_day_harib18f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${21_day_harib18f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 21_day_harib18f_img
)
add_custom_target(21_day_harib18f_clean
  COMMAND rm -f ${21_day_harib18f_OS}
  COMMAND rm -f ${21_day_harib18f_SYS}
  COMMAND rm -f ${21_day_harib18f_IPLB}
  COMMAND rm -f ${21_day_harib18f_HEADB}
  COMMAND rm -f ${21_day_harib18f_BOOTB}
  COMMAND rm -f ${21_day_harib18f_WILDOBJ}
)
add_custom_target(21_day_harib18f_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${21_day_harib18f_IPLS} ${21_day_harib18f_IPLB}
  DEPENDS ${21_day_harib18f_IPLS}
)
add_custom_command(
  OUTPUT ${21_day_harib18f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${21_day_harib18f_HEADS} ${21_day_harib18f_HEADB}
  DEPENDS ${21_day_harib18f_HEADS}
  COMMENT "Generating asmhead.bin for 21_day_harib18f"
)
add_custom_command(
  OUTPUT ${21_day_harib18f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${21_day_harib18f_FUNCS} ${21_day_harib18f_FUNCO}
  DEPENDS ${21_day_harib18f_FUNCS}
  COMMENT "Generating naskfunc.o for 21_day_harib18f"
)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
add_custom_command(
  OUTPUT ${21_day_harib18f_FONTO}
  COMMAND ${FONT} ${21_day_harib18f_FONTS} ${21_day_harib18f_FONTB}
  COMMAND ${B2O}  ${21_day_harib18f_FONTB} ${21_day_harib18f_FONTO} _hankaku
  DEPENDS ${21_day_harib18f_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 21_day_harib18f"
)
add_custom_command(
  OUTPUT ${21_day_harib18f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${21_day_harib18f_LIBGE} ${21_day_harib18f_LIBGC}
  DEPENDS ${21_day_harib18f_LIBGE}
  COMMENT "Converting golibc for 21_day_harib18f"
)

add_custom_target(21_day_harib18f_sys
  COMMAND gcc ${BINOPT} -T ${21_day_harib18f_LDS} ${21_day_harib18f_CCS} ${21_day_harib18f_WILDOBJ} -o ${21_day_harib18f_BOOTB}
  COMMAND cat ${21_day_harib18f_HEADB} ${21_day_harib18f_BOOTB} > ${21_day_harib18f_SYS}
  DEPENDS ${21_day_harib18f_HEADB} 21_day_harib18f_ipl
  DEPENDS ${21_day_harib18f_FUNCO}
  DEPENDS ${21_day_harib18f_FONTO} ${21_day_harib18f_LIBGC}
)
add_custom_target(21_day_harib18f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${21_day_harib18f_IPLB} -i ${21_day_harib18f_OS}
  COMMAND mcopy -i ${21_day_harib18f_OS} ${21_day_harib18f_SYS} ::
  DEPENDS 21_day_harib18f_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/asmhead_wine.img)

add_custom_target(21_day_harib18f_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${asmhead.img_SRCS} ${asmhead.img_OUTS}
  DEPENDS ${asmhead.img_SRCS}
)
add_custom_target(21_day_harib18f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(21_day_harib18f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18f_asmhead_od 21_day_harib18f_asmhead)
add_dependencies(21_day_harib18f_asmhead_od 21_day_harib18f_asmhead_wine)

add_dependencies(images 21_day_harib18f_asmhead)
add_dependencies(wine 21_day_harib18f_asmhead_wine)
add_dependencies(od 21_day_harib18f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(crack2.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18f/crack2.nas)
set(crack2.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/crack2.img)
set(crack2_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/crack2_wine.img)

add_custom_target(21_day_harib18f_crack2
  COMMAND ${RAKUSK_EXECUTABLE} ${crack2.img_SRCS} ${crack2.img_OUTS}
  DEPENDS ${crack2.img_SRCS}
)
set(crack2.img_HRB ${root_BINARY_DIR}/projects/21_day/harib18f/crack2.hrb)
add_custom_target(21_day_harib18f_crack2_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${crack2.img_SRCS} ${crack2.img_HRB}
  DEPENDS ${crack2.img_SRCS}
  COMMAND mcopy -i ${21_day_harib18f_OS} ${crack2.img_HRB} ::crack2.hrb
)

add_dependencies(21_day_harib18f_run 21_day_harib18f_crack2_hrb)
add_custom_target(21_day_harib18f_crack2_wine
  COMMAND ${WINE} ${WINE_NASK} ${crack2.img_SRCS} ${crack2_wine.img_OUTS}
)
add_custom_target(21_day_harib18f_crack2_od
  COMMAND ${OD} -t x1 ${crack2.img_OUTS}      > ${crack2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${crack2_wine.img_OUTS} > ${crack2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${crack2.img_OUTS}_f.txt ${crack2_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18f_crack2_od 21_day_harib18f_crack2)
add_dependencies(21_day_harib18f_crack2_od 21_day_harib18f_crack2_wine)

add_dependencies(images 21_day_harib18f_crack2)
add_dependencies(wine 21_day_harib18f_crack2_wine)
add_dependencies(od 21_day_harib18f_crack2_od)

#----------------------------------------------------------
########### next target ###############
set(hello.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18f/hello.nas)
set(hello.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/hello.img)
set(hello_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/hello_wine.img)

add_custom_target(21_day_harib18f_hello
  COMMAND ${RAKUSK_EXECUTABLE} ${hello.img_SRCS} ${hello.img_OUTS}
  DEPENDS ${hello.img_SRCS}
)
set(hello.img_HRB ${root_BINARY_DIR}/projects/21_day/harib18f/hello.hrb)
add_custom_target(21_day_harib18f_hello_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${hello.img_SRCS} ${hello.img_HRB}
  DEPENDS ${hello.img_SRCS}
  COMMAND mcopy -i ${21_day_harib18f_OS} ${hello.img_HRB} ::hello.hrb
)

add_dependencies(21_day_harib18f_run 21_day_harib18f_hello_hrb)
add_custom_target(21_day_harib18f_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello.img_SRCS} ${hello_wine.img_OUTS}
)
add_custom_target(21_day_harib18f_hello_od
  COMMAND ${OD} -t x1 ${hello.img_OUTS}      > ${hello.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello_wine.img_OUTS} > ${hello_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello.img_OUTS}_f.txt ${hello_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18f_hello_od 21_day_harib18f_hello)
add_dependencies(21_day_harib18f_hello_od 21_day_harib18f_hello_wine)

add_dependencies(images 21_day_harib18f_hello)
add_dependencies(wine 21_day_harib18f_hello_wine)
add_dependencies(od 21_day_harib18f_hello_od)

#----------------------------------------------------------
########### next target ###############
set(hello2.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18f/hello2.nas)
set(hello2.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/hello2.img)
set(hello2_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/hello2_wine.img)

add_custom_target(21_day_harib18f_hello2
  COMMAND ${RAKUSK_EXECUTABLE} ${hello2.img_SRCS} ${hello2.img_OUTS}
  DEPENDS ${hello2.img_SRCS}
)
set(hello2.img_HRB ${root_BINARY_DIR}/projects/21_day/harib18f/hello2.hrb)
add_custom_target(21_day_harib18f_hello2_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${hello2.img_SRCS} ${hello2.img_HRB}
  DEPENDS ${hello2.img_SRCS}
  COMMAND mcopy -i ${21_day_harib18f_OS} ${hello2.img_HRB} ::hello2.hrb
)

add_dependencies(21_day_harib18f_run 21_day_harib18f_hello2_hrb)
add_custom_target(21_day_harib18f_hello2_wine
  COMMAND ${WINE} ${WINE_NASK} ${hello2.img_SRCS} ${hello2_wine.img_OUTS}
)
add_custom_target(21_day_harib18f_hello2_od
  COMMAND ${OD} -t x1 ${hello2.img_OUTS}      > ${hello2.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hello2_wine.img_OUTS} > ${hello2_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hello2.img_OUTS}_f.txt ${hello2_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18f_hello2_od 21_day_harib18f_hello2)
add_dependencies(21_day_harib18f_hello2_od 21_day_harib18f_hello2_wine)

add_dependencies(images 21_day_harib18f_hello2)
add_dependencies(wine 21_day_harib18f_hello2_wine)
add_dependencies(od 21_day_harib18f_hello2_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/ipl10_wine.img)

add_custom_target(21_day_harib18f_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${ipl10.img_SRCS} ${ipl10.img_OUTS}
  DEPENDS ${ipl10.img_SRCS}
)
add_custom_target(21_day_harib18f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(21_day_harib18f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18f_ipl10_od 21_day_harib18f_ipl10)
add_dependencies(21_day_harib18f_ipl10_od 21_day_harib18f_ipl10_wine)

add_dependencies(images 21_day_harib18f_ipl10)
add_dependencies(wine 21_day_harib18f_ipl10_wine)
add_dependencies(od 21_day_harib18f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/naskfunc_wine.img)

add_custom_target(21_day_harib18f_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
  DEPENDS ${naskfunc.img_SRCS}
)
add_custom_target(21_day_harib18f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(21_day_harib18f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18f_naskfunc_od 21_day_harib18f_naskfunc)
add_dependencies(21_day_harib18f_naskfunc_od 21_day_harib18f_naskfunc_wine)

add_dependencies(images 21_day_harib18f_naskfunc)
add_dependencies(wine 21_day_harib18f_naskfunc_wine)
add_dependencies(od 21_day_harib18f_naskfunc_od)

#----------------------------------------------------------
########### next target ###############
set(a.imgk.img_SRCS ${root_SOURCE_DIR}/projects/21_day/harib18f/a_nask.nas)
set(a.imgk.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/a_nask.img)
set(a_wine.imgk_wine.img_OUTS ${root_BINARY_DIR}/projects/21_day/harib18f/a_nask_wine.img)

add_custom_target(21_day_harib18f_a_nask
  COMMAND ${RAKUSK_EXECUTABLE} ${a.imgk.img_SRCS} ${a.imgk.img_OUTS}
  DEPENDS ${a.imgk.img_SRCS}
)
set(a.imgk.img_HRB ${root_BINARY_DIR}/projects/21_day/harib18f/a_nask.hrb)
add_custom_target(21_day_harib18f_a_nask_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${a.imgk.img_SRCS} ${a.imgk.img_HRB}
  DEPENDS ${a.imgk.img_SRCS}
  COMMAND mcopy -i ${21_day_harib18f_OS} ${a.imgk.img_HRB} ::a.hrbk.hrb
)

add_dependencies(21_day_harib18f_run 21_day_harib18f_a_nask_hrb)
add_custom_target(21_day_harib18f_a_nask_wine
  COMMAND ${WINE} ${WINE_NASK} ${a.imgk.img_SRCS} ${a_wine.imgk_wine.img_OUTS}
)
add_custom_target(21_day_harib18f_a_nask_od
  COMMAND ${OD} -t x1 ${a.imgk.img_OUTS}      > ${a.imgk.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${a_wine.imgk_wine.img_OUTS} > ${a_wine.imgk_wine.img_OUTS}_t.txt
  COMMAND diff -s ${a.imgk.img_OUTS}_f.txt ${a_wine.imgk_wine.img_OUTS}_t.txt
)

add_dependencies(21_day_harib18f_a_nask_od 21_day_harib18f_a_nask)
add_dependencies(21_day_harib18f_a_nask_od 21_day_harib18f_a_nask_wine)

add_dependencies(images 21_day_harib18f_a_nask)
add_dependencies(wine 21_day_harib18f_a_nask_wine)
add_dependencies(od 21_day_harib18f_a_nask_od)

#----------------------------------------------------------
