#----------------------------------------------------------
message(STATUS "Entering directory projects/22_day/harib19b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(22_day_harib19b_OS    ${root_BINARY_DIR}/projects/22_day/harib19b/os.img)
set(22_day_harib19b_SYS	  ${root_BINARY_DIR}/projects/22_day/harib19b/os.sys)
set(22_day_harib19b_IPLB  ${root_BINARY_DIR}/projects/22_day/harib19b/ipl.bin)
set(22_day_harib19b_IPLS  ${root_SOURCE_DIR}/projects/22_day/harib19b/ipl10.nas)
set(22_day_harib19b_HEADB ${root_BINARY_DIR}/projects/22_day/harib19b/asmhead.bin)
set(22_day_harib19b_HEADS ${root_SOURCE_DIR}/projects/22_day/harib19b/asmhead.nas)
set(22_day_harib19b_BOOTB ${root_BINARY_DIR}/projects/22_day/harib19b/boot.bin)
set(22_day_harib19b_CCO	  ${root_BINARY_DIR}/projects/22_day/harib19b/boot.o)
file(GLOB 22_day_harib19b_CCS ${root_SOURCE_DIR}/projects/22_day/harib19b/*.c)
list(REMOVE_ITEM 22_day_harib19b_CCS
  ${root_SOURCE_DIR}/projects/22_day/harib19b/a.c
  ${root_SOURCE_DIR}/projects/22_day/harib19b/hello3.c
  ${root_SOURCE_DIR}/projects/22_day/harib19b/bug1.c)
set(22_day_harib19b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(22_day_harib19b_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(22_day_harib19b_FUNCS ${root_SOURCE_DIR}/projects/22_day/harib19b/naskfunc.nas)
set(22_day_harib19b_FUNCO ${root_BINARY_DIR}/projects/22_day/harib19b/naskfunc.o)
set(22_day_harib19b_FONTS ${root_SOURCE_DIR}/projects/22_day/harib19b/hankaku.txt)
set(22_day_harib19b_FONTB ${root_BINARY_DIR}/projects/22_day/harib19b/hankaku.bin)
set(22_day_harib19b_FONTO ${root_BINARY_DIR}/projects/22_day/harib19b/hankaku.o)
set(22_day_harib19b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(22_day_harib19b_LIBGC ${root_BINARY_DIR}/projects/22_day/harib19b/libgo.o)
set(22_day_harib19b_WILDOBJ ${root_BINARY_DIR}/projects/22_day/harib19b/*.o)


add_custom_target(22_day_harib19b_run
  COMMAND ${QEMU} ${QEMUOPT} ${22_day_harib19b_OS}
  DEPENDS 22_day_harib19b_img
)
add_custom_target(22_day_harib19b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${22_day_harib19b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 22_day_harib19b_img
)
add_custom_target(22_day_harib19b_clean
  COMMAND rm -f ${22_day_harib19b_OS}
  COMMAND rm -f ${22_day_harib19b_SYS}
  COMMAND rm -f ${22_day_harib19b_IPLB}
  COMMAND rm -f ${22_day_harib19b_HEADB}
  COMMAND rm -f ${22_day_harib19b_BOOTB}
  COMMAND rm -f ${22_day_harib19b_WILDOBJ}
)
add_custom_target(22_day_harib19b_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19b_IPLS} ${22_day_harib19b_IPLB}
  DEPENDS ${22_day_harib19b_IPLS}
)
add_custom_command(
  OUTPUT ${22_day_harib19b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19b_HEADS} ${22_day_harib19b_HEADB}
  DEPENDS ${22_day_harib19b_HEADS}
  COMMENT "Generating asmhead.bin for 22_day_harib19b"
)
add_custom_command(
  OUTPUT ${22_day_harib19b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19b_FUNCS} ${22_day_harib19b_FUNCO}
  DEPENDS ${22_day_harib19b_FUNCS}
  COMMENT "Generating naskfunc.o for 22_day_harib19b"
)
add_custom_command(
  OUTPUT ${22_day_harib19b_FONTO}
  COMMAND ${FONT} ${22_day_harib19b_FONTS} ${22_day_harib19b_FONTB}
  COMMAND ${B2O}  ${22_day_harib19b_FONTB} ${22_day_harib19b_FONTO} _hankaku
  DEPENDS ${22_day_harib19b_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 22_day_harib19b"
)
add_custom_command(
  OUTPUT ${22_day_harib19b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${22_day_harib19b_LIBGE} ${22_day_harib19b_LIBGC}
  DEPENDS ${22_day_harib19b_LIBGE}
  COMMENT "Converting golibc for 22_day_harib19b"
)

add_custom_target(22_day_harib19b_sys
  COMMAND gcc ${BINOPT} -T ${22_day_harib19b_LDS} ${22_day_harib19b_CCS} ${22_day_harib19b_WILDOBJ} -o ${22_day_harib19b_BOOTB}
  COMMAND cat ${22_day_harib19b_HEADB} ${22_day_harib19b_BOOTB} > ${22_day_harib19b_SYS}
  DEPENDS ${22_day_harib19b_HEADB} 22_day_harib19b_ipl
  DEPENDS ${22_day_harib19b_FUNCO}
  DEPENDS ${22_day_harib19b_FONTO} ${22_day_harib19b_LIBGC}
)
add_custom_target(22_day_harib19b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${22_day_harib19b_IPLB} -i ${22_day_harib19b_OS}
  COMMAND mcopy -i ${22_day_harib19b_OS} ${22_day_harib19b_SYS} ::
  COMMAND mcopy -i ${22_day_harib19b_OS} ${root_BINARY_DIR}/projects/22_day/harib19b/a.hrb ::a.hrb
  COMMAND mcopy -i ${22_day_harib19b_OS} ${root_BINARY_DIR}/projects/22_day/harib19b/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${22_day_harib19b_OS} ${root_BINARY_DIR}/projects/22_day/harib19b/bug1.hrb ::bug1.hrb
  COMMAND mcopy -i ${22_day_harib19b_OS} ${root_BINARY_DIR}/projects/22_day/harib19b/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${22_day_harib19b_OS} ${root_BINARY_DIR}/projects/22_day/harib19b/hello2.hrb ::hello2.hrb
  DEPENDS 22_day_harib19b_sys ${root_BINARY_DIR}/projects/22_day/harib19b/a.hrb ${root_BINARY_DIR}/projects/22_day/harib19b/hello3.hrb ${root_BINARY_DIR}/projects/22_day/harib19b/bug1.hrb ${root_BINARY_DIR}/projects/22_day/harib19b/hello.hrb ${root_BINARY_DIR}/projects/22_day/harib19b/hello2.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(22_day_harib19b_APIO ${root_BINARY_DIR}/projects/22_day/harib19b/a_nask.o)
add_custom_command(
  OUTPUT ${22_day_harib19b_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19b/a_nask.nas ${22_day_harib19b_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19b/a_nask.nas
  COMMENT "Generating a_nask.o for 22_day_harib19b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19b/a.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19b_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19b/a.c ${22_day_harib19b_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19b/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19b/a.c ${22_day_harib19b_APIO}
  COMMENT "Generating a.hrb for 22_day_harib19b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19b/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19b_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19b/hello3.c ${22_day_harib19b_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19b/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19b/hello3.c ${22_day_harib19b_APIO}
  COMMENT "Generating hello3.hrb for 22_day_harib19b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19b/bug1.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19b_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19b/bug1.c ${22_day_harib19b_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19b/bug1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19b/bug1.c ${22_day_harib19b_APIO}
  COMMENT "Generating bug1.hrb for 22_day_harib19b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19b/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19b/hello.nas ${root_BINARY_DIR}/projects/22_day/harib19b/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19b/hello.nas
  COMMENT "Generating hello.hrb for 22_day_harib19b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19b/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19b/hello2.nas ${root_BINARY_DIR}/projects/22_day/harib19b/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19b/hello2.nas
  COMMENT "Generating hello2.hrb for 22_day_harib19b"
)
