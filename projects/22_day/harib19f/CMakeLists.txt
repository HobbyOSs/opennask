#----------------------------------------------------------
message(STATUS "Entering directory projects/22_day/harib19f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(22_day_harib19f_OS    ${root_BINARY_DIR}/projects/22_day/harib19f/os.img)
set(22_day_harib19f_SYS	  ${root_BINARY_DIR}/projects/22_day/harib19f/os.sys)
set(22_day_harib19f_IPLB  ${root_BINARY_DIR}/projects/22_day/harib19f/ipl.bin)
set(22_day_harib19f_IPLS  ${root_SOURCE_DIR}/projects/22_day/harib19f/ipl10.nas)
set(22_day_harib19f_HEADB ${root_BINARY_DIR}/projects/22_day/harib19f/asmhead.bin)
set(22_day_harib19f_HEADS ${root_SOURCE_DIR}/projects/22_day/harib19f/asmhead.nas)
set(22_day_harib19f_BOOTB ${root_BINARY_DIR}/projects/22_day/harib19f/boot.bin)
set(22_day_harib19f_CCO	  ${root_BINARY_DIR}/projects/22_day/harib19f/boot.o)
file(GLOB 22_day_harib19f_CCS ${root_SOURCE_DIR}/projects/22_day/harib19f/*.c)
list(REMOVE_ITEM 22_day_harib19f_CCS
  ${root_SOURCE_DIR}/projects/22_day/harib19f/a.c
  ${root_SOURCE_DIR}/projects/22_day/harib19f/hello3.c
  ${root_SOURCE_DIR}/projects/22_day/harib19f/bug1.c
  ${root_SOURCE_DIR}/projects/22_day/harib19f/bug2.c
  ${root_SOURCE_DIR}/projects/22_day/harib19f/bug3.c
  ${root_SOURCE_DIR}/projects/22_day/harib19f/hello4.c
  ${root_SOURCE_DIR}/projects/22_day/harib19f/winhelo.c)
set(22_day_harib19f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(22_day_harib19f_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(22_day_harib19f_FUNCS ${root_SOURCE_DIR}/projects/22_day/harib19f/naskfunc.nas)
set(22_day_harib19f_FUNCO ${root_BINARY_DIR}/projects/22_day/harib19f/naskfunc.o)
set(22_day_harib19f_FONTS ${root_SOURCE_DIR}/projects/22_day/harib19f/hankaku.txt)
set(22_day_harib19f_FONTB ${root_BINARY_DIR}/projects/22_day/harib19f/hankaku.bin)
set(22_day_harib19f_FONTO ${root_BINARY_DIR}/projects/22_day/harib19f/hankaku.o)
set(22_day_harib19f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(22_day_harib19f_LIBGC ${root_BINARY_DIR}/projects/22_day/harib19f/libgo.o)
set(22_day_harib19f_WILDOBJ ${root_BINARY_DIR}/projects/22_day/harib19f/*.o)


add_custom_target(22_day_harib19f_run
  COMMAND ${QEMU} ${QEMUOPT} ${22_day_harib19f_OS}
  DEPENDS 22_day_harib19f_img
)
add_custom_target(22_day_harib19f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${22_day_harib19f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 22_day_harib19f_img
)
add_custom_target(22_day_harib19f_clean
  COMMAND rm -f ${22_day_harib19f_OS}
  COMMAND rm -f ${22_day_harib19f_SYS}
  COMMAND rm -f ${22_day_harib19f_IPLB}
  COMMAND rm -f ${22_day_harib19f_HEADB}
  COMMAND rm -f ${22_day_harib19f_BOOTB}
  COMMAND rm -f ${22_day_harib19f_WILDOBJ}
)
add_custom_target(22_day_harib19f_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19f_IPLS} ${22_day_harib19f_IPLB}
  DEPENDS ${22_day_harib19f_IPLS}
)
add_custom_command(
  OUTPUT ${22_day_harib19f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19f_HEADS} ${22_day_harib19f_HEADB}
  DEPENDS ${22_day_harib19f_HEADS}
  COMMENT "Generating asmhead.bin for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${22_day_harib19f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19f_FUNCS} ${22_day_harib19f_FUNCO}
  DEPENDS ${22_day_harib19f_FUNCS}
  COMMENT "Generating naskfunc.o for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${22_day_harib19f_FONTO}
  COMMAND ${FONT} ${22_day_harib19f_FONTS} ${22_day_harib19f_FONTB}
  COMMAND ${B2O}  ${22_day_harib19f_FONTB} ${22_day_harib19f_FONTO} _hankaku
  DEPENDS ${22_day_harib19f_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${22_day_harib19f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${22_day_harib19f_LIBGE} ${22_day_harib19f_LIBGC}
  DEPENDS ${22_day_harib19f_LIBGE}
  COMMENT "Converting golibc for 22_day_harib19f"
)

add_custom_target(22_day_harib19f_sys
  COMMAND gcc ${BINOPT} -T ${22_day_harib19f_LDS} ${22_day_harib19f_CCS} ${22_day_harib19f_WILDOBJ} -o ${22_day_harib19f_BOOTB}
  COMMAND cat ${22_day_harib19f_HEADB} ${22_day_harib19f_BOOTB} > ${22_day_harib19f_SYS}
  DEPENDS ${22_day_harib19f_HEADB} 22_day_harib19f_ipl
  DEPENDS ${22_day_harib19f_FUNCO}
  DEPENDS ${22_day_harib19f_FONTO} ${22_day_harib19f_LIBGC}
)
add_custom_target(22_day_harib19f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${22_day_harib19f_IPLB} -i ${22_day_harib19f_OS}
  COMMAND mcopy -i ${22_day_harib19f_OS} ${22_day_harib19f_SYS} ::
  COMMAND mcopy -i ${22_day_harib19f_OS} ${root_BINARY_DIR}/projects/22_day/harib19f/a.hrb ::a.hrb
  COMMAND mcopy -i ${22_day_harib19f_OS} ${root_BINARY_DIR}/projects/22_day/harib19f/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${22_day_harib19f_OS} ${root_BINARY_DIR}/projects/22_day/harib19f/bug1.hrb ::bug1.hrb
  COMMAND mcopy -i ${22_day_harib19f_OS} ${root_BINARY_DIR}/projects/22_day/harib19f/bug2.hrb ::bug2.hrb
  COMMAND mcopy -i ${22_day_harib19f_OS} ${root_BINARY_DIR}/projects/22_day/harib19f/bug3.hrb ::bug3.hrb
  COMMAND mcopy -i ${22_day_harib19f_OS} ${root_BINARY_DIR}/projects/22_day/harib19f/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${22_day_harib19f_OS} ${root_BINARY_DIR}/projects/22_day/harib19f/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${22_day_harib19f_OS} ${root_BINARY_DIR}/projects/22_day/harib19f/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${22_day_harib19f_OS} ${root_BINARY_DIR}/projects/22_day/harib19f/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${22_day_harib19f_OS} ${root_BINARY_DIR}/projects/22_day/harib19f/hello5.hrb ::hello5.hrb
  DEPENDS 22_day_harib19f_sys ${root_BINARY_DIR}/projects/22_day/harib19f/a.hrb ${root_BINARY_DIR}/projects/22_day/harib19f/hello3.hrb ${root_BINARY_DIR}/projects/22_day/harib19f/bug1.hrb ${root_BINARY_DIR}/projects/22_day/harib19f/bug2.hrb ${root_BINARY_DIR}/projects/22_day/harib19f/bug3.hrb ${root_BINARY_DIR}/projects/22_day/harib19f/hello4.hrb ${root_BINARY_DIR}/projects/22_day/harib19f/winhelo.hrb ${root_BINARY_DIR}/projects/22_day/harib19f/hello.hrb ${root_BINARY_DIR}/projects/22_day/harib19f/hello2.hrb ${root_BINARY_DIR}/projects/22_day/harib19f/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(22_day_harib19f_APIO ${root_BINARY_DIR}/projects/22_day/harib19f/a_nask.o)
add_custom_command(
  OUTPUT ${22_day_harib19f_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19f/a_nask.nas ${22_day_harib19f_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/a_nask.nas
  COMMENT "Generating a_nask.o for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19f/a.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19f_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19f/a.c ${22_day_harib19f_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19f/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/a.c ${22_day_harib19f_APIO}
  COMMENT "Generating a.hrb for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19f/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19f_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19f/hello3.c ${22_day_harib19f_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19f/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/hello3.c ${22_day_harib19f_APIO}
  COMMENT "Generating hello3.hrb for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19f/bug1.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19f_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19f/bug1.c ${22_day_harib19f_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19f/bug1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/bug1.c ${22_day_harib19f_APIO}
  COMMENT "Generating bug1.hrb for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19f/bug2.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19f_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19f/bug2.c ${22_day_harib19f_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19f/bug2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/bug2.c ${22_day_harib19f_APIO}
  COMMENT "Generating bug2.hrb for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19f/bug3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19f_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19f/bug3.c ${22_day_harib19f_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19f/bug3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/bug3.c ${22_day_harib19f_APIO}
  COMMENT "Generating bug3.hrb for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19f/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19f_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19f/hello4.c ${22_day_harib19f_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19f/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/hello4.c ${22_day_harib19f_APIO}
  COMMENT "Generating hello4.hrb for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19f/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19f_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19f/winhelo.c ${22_day_harib19f_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19f/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/winhelo.c ${22_day_harib19f_APIO}
  COMMENT "Generating winhelo.hrb for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19f/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19f/hello.nas ${root_BINARY_DIR}/projects/22_day/harib19f/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/hello.nas
  COMMENT "Generating hello.hrb for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19f/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19f/hello2.nas ${root_BINARY_DIR}/projects/22_day/harib19f/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/hello2.nas
  COMMENT "Generating hello2.hrb for 22_day_harib19f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19f/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19f/hello5.nas ${root_BINARY_DIR}/projects/22_day/harib19f/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19f/hello5.nas
  COMMENT "Generating hello5.hrb for 22_day_harib19f"
)
