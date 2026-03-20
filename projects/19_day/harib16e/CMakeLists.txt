#----------------------------------------------------------
message(STATUS "Entering directory projects/19_day/harib16e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(19_day_harib16e_WILDOBJ ${root_BINARY_DIR}/projects/19_day/harib16e/*.o)

# Full OS build rules
set(19_day_harib16e_OS    ${root_BINARY_DIR}/projects/19_day/harib16e/os.img)
set(19_day_harib16e_SYS   ${root_BINARY_DIR}/projects/19_day/harib16e/os.sys)
set(19_day_harib16e_IPLB  ${root_BINARY_DIR}/projects/19_day/harib16e/ipl.bin)
set(19_day_harib16e_IPLS  ${root_SOURCE_DIR}/projects/19_day/harib16e/ipl10.nas)
set(19_day_harib16e_HEADB ${root_BINARY_DIR}/projects/19_day/harib16e/asmhead.bin)
set(19_day_harib16e_HEADS ${root_SOURCE_DIR}/projects/19_day/harib16e/asmhead.nas)
set(19_day_harib16e_BOOTB ${root_BINARY_DIR}/projects/19_day/harib16e/boot.bin)
set(19_day_harib16e_CCO   ${root_BINARY_DIR}/projects/19_day/harib16e/boot.o)
set(19_day_harib16e_CCS   ${root_SOURCE_DIR}/projects/19_day/harib16e/*.c)
set(19_day_harib16e_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(19_day_harib16e_FUNCS ${root_SOURCE_DIR}/projects/19_day/harib16e/naskfunc.nas)
set(19_day_harib16e_FUNCO ${root_BINARY_DIR}/projects/19_day/harib16e/naskfunc.o)
set(19_day_harib16e_FONTS ${root_SOURCE_DIR}/projects/19_day/harib16e/hankaku.txt)
set(19_day_harib16e_FONTB ${root_BINARY_DIR}/projects/19_day/harib16e/hankaku.bin)
set(19_day_harib16e_FONTO ${root_BINARY_DIR}/projects/19_day/harib16e/hankaku.o)
set(19_day_harib16e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(19_day_harib16e_LIBGC ${root_BINARY_DIR}/projects/19_day/harib16e/libgo.o)

add_custom_target(19_day_harib16e_run
  COMMAND ${QEMU} ${QEMUOPT} ${19_day_harib16e_OS}
  DEPENDS 19_day_harib16e_img
)
add_custom_target(19_day_harib16e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${19_day_harib16e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 19_day_harib16e_img
)
add_custom_target(19_day_harib16e_clean
  COMMAND rm -f ${19_day_harib16e_OS}
  COMMAND rm -f ${19_day_harib16e_SYS}
  COMMAND rm -f ${19_day_harib16e_IPLB}
  COMMAND rm -f ${19_day_harib16e_HEADB}
  COMMAND rm -f ${19_day_harib16e_BOOTB}
  COMMAND rm -f ${19_day_harib16e_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16e/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16e/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16e/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/19_day/harib16e/*.txt
)
add_custom_target(19_day_harib16e_ipl_build
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16e_IPLS} ${19_day_harib16e_IPLB}
  DEPENDS ${19_day_harib16e_IPLS}
)
add_custom_command(
  OUTPUT ${19_day_harib16e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16e_HEADS} ${19_day_harib16e_HEADB}
  DEPENDS ${19_day_harib16e_HEADS}
  COMMENT "Generating asmhead.bin for 19_day_harib16e"
)

add_custom_command(
  OUTPUT ${19_day_harib16e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16e_FUNCS} ${19_day_harib16e_FUNCO}
  DEPENDS ${19_day_harib16e_FUNCS}
  COMMENT "Generating naskfunc.o for 19_day_harib16e"
)
add_custom_command(
  OUTPUT ${19_day_harib16e_FONTO}
  COMMAND ${FONT} ${19_day_harib16e_FONTS} ${19_day_harib16e_FONTB}
  COMMAND ${B2O}  ${19_day_harib16e_FONTB} ${19_day_harib16e_FONTO} _hankaku
  DEPENDS ${19_day_harib16e_FONTS}
  COMMENT "Generating hankaku.o for 19_day_harib16e"
)
add_custom_command(
  OUTPUT ${19_day_harib16e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${19_day_harib16e_LIBGE} ${19_day_harib16e_LIBGC}
  DEPENDS ${19_day_harib16e_LIBGE}
  COMMENT "Converting golibc for 19_day_harib16e"
)
add_custom_target(19_day_harib16e_sys
  COMMAND gcc ${BINOPT} -T ${19_day_harib16e_LDS} ${19_day_harib16e_CCS} ${19_day_harib16e_WILDOBJ} -o ${19_day_harib16e_BOOTB}
  COMMAND cat ${19_day_harib16e_HEADB} ${19_day_harib16e_BOOTB} > ${19_day_harib16e_SYS}
  DEPENDS ${19_day_harib16e_HEADB} 19_day_harib16e_ipl_build
  DEPENDS ${19_day_harib16e_FUNCO}
  DEPENDS ${19_day_harib16e_FONTO} ${19_day_harib16e_LIBGC}
)
add_custom_target(19_day_harib16e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${19_day_harib16e_IPLB} -i ${19_day_harib16e_OS}
  COMMAND mcopy -i ${19_day_harib16e_OS} ${19_day_harib16e_SYS} ::
  DEPENDS 19_day_harib16e_sys
)

########### next target ###############
set(19_day_harib16e_asmhead_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16e/asmhead.nas)
set(19_day_harib16e_asmhead_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/asmhead.bin)

add_custom_target(19_day_harib16e_asmhead
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16e_asmhead_SRCS} ${19_day_harib16e_asmhead_OUTS}
  DEPENDS ${19_day_harib16e_asmhead_SRCS}
)

set(19_day_harib16e_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/asmhead_wine.bin)
add_custom_target(19_day_harib16e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16e_asmhead_SRCS} ${19_day_harib16e_asmhead_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16e_asmhead_wine)

add_custom_target(19_day_harib16e_asmhead_od
  COMMAND ${OD} -t x1 ${19_day_harib16e_asmhead_OUTS}      > ${19_day_harib16e_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16e_asmhead_WINE_OUTS} > ${19_day_harib16e_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16e_asmhead_OUTS}_f.txt ${19_day_harib16e_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16e_asmhead_od 19_day_harib16e_asmhead)
add_dependencies(19_day_harib16e_asmhead_od 19_day_harib16e_asmhead_wine)
add_dependencies(od 19_day_harib16e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(19_day_harib16e_hlt_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16e/hlt.nas)
set(19_day_harib16e_hlt_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/hlt.img)

add_custom_target(19_day_harib16e_hlt
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16e_hlt_SRCS} ${19_day_harib16e_hlt_OUTS}
  DEPENDS ${19_day_harib16e_hlt_SRCS}
)

add_dependencies(images 19_day_harib16e_hlt)
set(19_day_harib16e_hlt_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/hlt_wine.img)
add_custom_target(19_day_harib16e_hlt_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16e_hlt_SRCS} ${19_day_harib16e_hlt_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16e_hlt_wine)

add_custom_target(19_day_harib16e_hlt_od
  COMMAND ${OD} -t x1 ${19_day_harib16e_hlt_OUTS}      > ${19_day_harib16e_hlt_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16e_hlt_WINE_OUTS} > ${19_day_harib16e_hlt_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16e_hlt_OUTS}_f.txt ${19_day_harib16e_hlt_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16e_hlt_od 19_day_harib16e_hlt)
add_dependencies(19_day_harib16e_hlt_od 19_day_harib16e_hlt_wine)
add_dependencies(od 19_day_harib16e_hlt_od)

set(19_day_harib16e_hlt_HRB ${root_BINARY_DIR}/projects/19_day/harib16e/hlt.hrb)
add_custom_target(19_day_harib16e_hlt_hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16e_hlt_SRCS} ${19_day_harib16e_hlt_HRB}
  COMMAND mcopy -i ${19_day_harib16e_OS} ${19_day_harib16e_hlt_HRB} ::hlt.hrb
  DEPENDS 19_day_harib16e_img
)

add_dependencies(19_day_harib16e_run 19_day_harib16e_hlt_hrb)
#----------------------------------------------------------
########### next target ###############
set(19_day_harib16e_ipl10_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16e/ipl10.nas)
set(19_day_harib16e_ipl10_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/ipl10.bin)

add_custom_target(19_day_harib16e_ipl10
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16e_ipl10_SRCS} ${19_day_harib16e_ipl10_OUTS}
  DEPENDS ${19_day_harib16e_ipl10_SRCS}
)

set(19_day_harib16e_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/ipl10_wine.bin)
add_custom_target(19_day_harib16e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16e_ipl10_SRCS} ${19_day_harib16e_ipl10_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16e_ipl10_wine)

add_custom_target(19_day_harib16e_ipl10_od
  COMMAND ${OD} -t x1 ${19_day_harib16e_ipl10_OUTS}      > ${19_day_harib16e_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16e_ipl10_WINE_OUTS} > ${19_day_harib16e_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16e_ipl10_OUTS}_f.txt ${19_day_harib16e_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16e_ipl10_od 19_day_harib16e_ipl10)
add_dependencies(19_day_harib16e_ipl10_od 19_day_harib16e_ipl10_wine)
add_dependencies(od 19_day_harib16e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(19_day_harib16e_naskfunc_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16e/naskfunc.nas)
set(19_day_harib16e_naskfunc_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/naskfunc.img)

add_custom_target(19_day_harib16e_naskfunc
  COMMAND ${RAKUSK_EXECUTABLE} ${19_day_harib16e_naskfunc_SRCS} ${19_day_harib16e_naskfunc_OUTS}
  DEPENDS ${19_day_harib16e_naskfunc_SRCS}
)

add_dependencies(images 19_day_harib16e_naskfunc)
set(19_day_harib16e_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/naskfunc_wine.img)
add_custom_target(19_day_harib16e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${19_day_harib16e_naskfunc_SRCS} ${19_day_harib16e_naskfunc_WINE_OUTS}
)
add_dependencies(wine 19_day_harib16e_naskfunc_wine)

add_custom_target(19_day_harib16e_naskfunc_od
  COMMAND ${OD} -t x1 ${19_day_harib16e_naskfunc_OUTS}      > ${19_day_harib16e_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${19_day_harib16e_naskfunc_WINE_OUTS} > ${19_day_harib16e_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${19_day_harib16e_naskfunc_OUTS}_f.txt ${19_day_harib16e_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(19_day_harib16e_naskfunc_od 19_day_harib16e_naskfunc)
add_dependencies(19_day_harib16e_naskfunc_od 19_day_harib16e_naskfunc_wine)
add_dependencies(od 19_day_harib16e_naskfunc_od)

#----------------------------------------------------------
