#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17f_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17f/*.o)

# Full OS build rules
set(20_day_harib17f_OS    ${root_BINARY_DIR}/projects/20_day/harib17f/os.img)
set(20_day_harib17f_SYS   ${root_BINARY_DIR}/projects/20_day/harib17f/os.sys)
set(20_day_harib17f_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17f/ipl.bin)
set(20_day_harib17f_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17f/ipl10.nas)
set(20_day_harib17f_HEADB ${root_BINARY_DIR}/projects/20_day/harib17f/asmhead.bin)
set(20_day_harib17f_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17f/asmhead.nas)
set(20_day_harib17f_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17f/boot.bin)
set(20_day_harib17f_CCO   ${root_BINARY_DIR}/projects/20_day/harib17f/boot.o)
set(20_day_harib17f_CCS   ${root_SOURCE_DIR}/projects/20_day/harib17f/*.c)
set(20_day_harib17f_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17f_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17f/naskfunc.nas)
set(20_day_harib17f_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17f/naskfunc.o)
set(20_day_harib17f_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17f/hankaku.txt)
set(20_day_harib17f_FONTB ${root_BINARY_DIR}/projects/20_day/harib17f/hankaku.bin)
set(20_day_harib17f_FONTO ${root_BINARY_DIR}/projects/20_day/harib17f/hankaku.o)
set(20_day_harib17f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17f_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17f/libgo.o)

add_custom_target(20_day_harib17f_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17f_OS}
  DEPENDS 20_day_harib17f_img
)
add_custom_target(20_day_harib17f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17f_img
)
add_custom_target(20_day_harib17f_clean
  COMMAND rm -f ${20_day_harib17f_OS}
  COMMAND rm -f ${20_day_harib17f_SYS}
  COMMAND rm -f ${20_day_harib17f_IPLB}
  COMMAND rm -f ${20_day_harib17f_HEADB}
  COMMAND rm -f ${20_day_harib17f_BOOTB}
  COMMAND rm -f ${20_day_harib17f_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17f/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17f/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17f/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17f/*.txt
)
add_custom_target(20_day_harib17f_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17f_IPLS} ${20_day_harib17f_IPLB}
  DEPENDS ${20_day_harib17f_IPLS}
)
add_custom_command(
  OUTPUT ${20_day_harib17f_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17f_HEADS} ${20_day_harib17f_HEADB}
  DEPENDS ${20_day_harib17f_HEADS}
  COMMENT "Generating asmhead.bin for 20_day_harib17f"
)

add_custom_target(20_day_harib17f_sys
  COMMAND ${FONT} ${20_day_harib17f_FONTS} ${20_day_harib17f_FONTB}
  COMMAND ${B2O}  ${20_day_harib17f_FONTB} ${20_day_harib17f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17f_LIBGE} ${20_day_harib17f_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17f_FUNCS} ${20_day_harib17f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17f_LDS} ${20_day_harib17f_CCS} ${20_day_harib17f_WILDOBJ} -o ${20_day_harib17f_BOOTB}
  COMMAND cat ${20_day_harib17f_HEADB} ${20_day_harib17f_BOOTB} > ${20_day_harib17f_SYS}
  DEPENDS ${20_day_harib17f_HEADB} 20_day_harib17f_ipl_build
  DEPENDS ${20_day_harib17f_FUNCO}
  DEPENDS ${20_day_harib17f_FONTO}
)
add_custom_target(20_day_harib17f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17f_IPLB} -i ${20_day_harib17f_OS}
  COMMAND mcopy -i ${20_day_harib17f_OS} ${20_day_harib17f_SYS} ::
  DEPENDS 20_day_harib17f_sys
)

########### next target ###############
set(20_day_harib17f_asmhead_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17f/asmhead.nas)
set(20_day_harib17f_asmhead_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/asmhead.bin)

add_custom_target(20_day_harib17f_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17f_asmhead_SRCS} ${20_day_harib17f_asmhead_OUTS}
  DEPENDS ${20_day_harib17f_asmhead_SRCS}
)

set(20_day_harib17f_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/asmhead_wine.bin)
add_custom_target(20_day_harib17f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17f_asmhead_SRCS} ${20_day_harib17f_asmhead_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17f_asmhead_wine)

add_custom_target(20_day_harib17f_asmhead_od
  COMMAND ${OD} -t x1 ${20_day_harib17f_asmhead_OUTS}      > ${20_day_harib17f_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17f_asmhead_WINE_OUTS} > ${20_day_harib17f_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17f_asmhead_OUTS}_f.txt ${20_day_harib17f_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17f_asmhead_od 20_day_harib17f_asmhead)
add_dependencies(20_day_harib17f_asmhead_od 20_day_harib17f_asmhead_wine)
add_dependencies(od 20_day_harib17f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17f_hello_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17f/hello.nas)
set(20_day_harib17f_hello_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/hello.img)

add_custom_target(20_day_harib17f_hello
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17f_hello_SRCS} ${20_day_harib17f_hello_OUTS}
  DEPENDS ${20_day_harib17f_hello_SRCS}
)

add_dependencies(images 20_day_harib17f_hello)
set(20_day_harib17f_hello_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/hello_wine.img)
add_custom_target(20_day_harib17f_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17f_hello_SRCS} ${20_day_harib17f_hello_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17f_hello_wine)

add_custom_target(20_day_harib17f_hello_od
  COMMAND ${OD} -t x1 ${20_day_harib17f_hello_OUTS}      > ${20_day_harib17f_hello_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17f_hello_WINE_OUTS} > ${20_day_harib17f_hello_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17f_hello_OUTS}_f.txt ${20_day_harib17f_hello_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17f_hello_od 20_day_harib17f_hello)
add_dependencies(20_day_harib17f_hello_od 20_day_harib17f_hello_wine)
add_dependencies(od 20_day_harib17f_hello_od)

set(20_day_harib17f_hello_HRB ${root_BINARY_DIR}/projects/20_day/harib17f/hello.hrb)
add_custom_target(20_day_harib17f_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${20_day_harib17f_hello_SRCS} ${20_day_harib17f_hello_HRB}
  COMMAND mcopy -i ${20_day_harib17f_OS} ${20_day_harib17f_hello_HRB} ::hello.hrb
  DEPENDS 20_day_harib17f_img
)

add_dependencies(20_day_harib17f_run 20_day_harib17f_hello_hrb)
#----------------------------------------------------------
########### next target ###############
set(20_day_harib17f_ipl10_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17f/ipl10.nas)
set(20_day_harib17f_ipl10_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/ipl10.bin)

add_custom_target(20_day_harib17f_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17f_ipl10_SRCS} ${20_day_harib17f_ipl10_OUTS}
  DEPENDS ${20_day_harib17f_ipl10_SRCS}
)

set(20_day_harib17f_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/ipl10_wine.bin)
add_custom_target(20_day_harib17f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17f_ipl10_SRCS} ${20_day_harib17f_ipl10_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17f_ipl10_wine)

add_custom_target(20_day_harib17f_ipl10_od
  COMMAND ${OD} -t x1 ${20_day_harib17f_ipl10_OUTS}      > ${20_day_harib17f_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17f_ipl10_WINE_OUTS} > ${20_day_harib17f_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17f_ipl10_OUTS}_f.txt ${20_day_harib17f_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17f_ipl10_od 20_day_harib17f_ipl10)
add_dependencies(20_day_harib17f_ipl10_od 20_day_harib17f_ipl10_wine)
add_dependencies(od 20_day_harib17f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17f_naskfunc_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17f/naskfunc.nas)
set(20_day_harib17f_naskfunc_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/naskfunc.img)

add_custom_target(20_day_harib17f_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17f_naskfunc_SRCS} ${20_day_harib17f_naskfunc_OUTS}
  DEPENDS ${20_day_harib17f_naskfunc_SRCS}
)

add_dependencies(images 20_day_harib17f_naskfunc)
set(20_day_harib17f_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17f/naskfunc_wine.img)
add_custom_target(20_day_harib17f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17f_naskfunc_SRCS} ${20_day_harib17f_naskfunc_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17f_naskfunc_wine)

add_custom_target(20_day_harib17f_naskfunc_od
  COMMAND ${OD} -t x1 ${20_day_harib17f_naskfunc_OUTS}      > ${20_day_harib17f_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17f_naskfunc_WINE_OUTS} > ${20_day_harib17f_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17f_naskfunc_OUTS}_f.txt ${20_day_harib17f_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17f_naskfunc_od 20_day_harib17f_naskfunc)
add_dependencies(20_day_harib17f_naskfunc_od 20_day_harib17f_naskfunc_wine)
add_dependencies(od 20_day_harib17f_naskfunc_od)

#----------------------------------------------------------
