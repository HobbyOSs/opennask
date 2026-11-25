#----------------------------------------------------------
message(STATUS "Entering directory projects/23_day/harib20e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(23_day_harib20e_OS    ${root_BINARY_DIR}/projects/23_day/harib20e/os.img)
set(23_day_harib20e_SYS	  ${root_BINARY_DIR}/projects/23_day/harib20e/os.sys)
set(23_day_harib20e_IPLB  ${root_BINARY_DIR}/projects/23_day/harib20e/ipl.bin)
set(23_day_harib20e_IPLS  ${root_SOURCE_DIR}/projects/23_day/harib20e/ipl10.nas)
set(23_day_harib20e_HEADB ${root_BINARY_DIR}/projects/23_day/harib20e/asmhead.bin)
set(23_day_harib20e_HEADS ${root_SOURCE_DIR}/projects/23_day/harib20e/asmhead.nas)
set(23_day_harib20e_BOOTB ${root_BINARY_DIR}/projects/23_day/harib20e/boot.bin)
set(23_day_harib20e_CCO	  ${root_BINARY_DIR}/projects/23_day/harib20e/boot.o)
file(GLOB 23_day_harib20e_CCS ${root_SOURCE_DIR}/projects/23_day/harib20e/*.c)
list(REMOVE_ITEM 23_day_harib20e_CCS
  ${root_SOURCE_DIR}/projects/23_day/harib20e/a.c
  ${root_SOURCE_DIR}/projects/23_day/harib20e/hello3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20e/hello4.c
  ${root_SOURCE_DIR}/projects/23_day/harib20e/winhelo.c
  ${root_SOURCE_DIR}/projects/23_day/harib20e/winhelo2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20e/winhelo3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20e/star1.c
  ${root_SOURCE_DIR}/projects/23_day/harib20e/stars.c
  ${root_SOURCE_DIR}/projects/23_day/harib20e/stars2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20e/lines.c)
set(23_day_harib20e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(23_day_harib20e_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(23_day_harib20e_FUNCS ${root_SOURCE_DIR}/projects/23_day/harib20e/naskfunc.nas)
set(23_day_harib20e_FUNCO ${root_BINARY_DIR}/projects/23_day/harib20e/naskfunc.o)
set(23_day_harib20e_FONTS ${root_SOURCE_DIR}/projects/23_day/harib20e/hankaku.txt)
set(23_day_harib20e_FONTB ${root_BINARY_DIR}/projects/23_day/harib20e/hankaku.bin)
set(23_day_harib20e_FONTO ${root_BINARY_DIR}/projects/23_day/harib20e/hankaku.o)
set(23_day_harib20e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(23_day_harib20e_LIBGC ${root_BINARY_DIR}/projects/23_day/harib20e/libgo.o)
set(23_day_harib20e_WILDOBJ ${root_BINARY_DIR}/projects/23_day/harib20e/*.o)


add_custom_target(23_day_harib20e_run
  COMMAND ${QEMU} ${QEMUOPT} ${23_day_harib20e_OS}
  DEPENDS 23_day_harib20e_img
)
add_custom_target(23_day_harib20e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${23_day_harib20e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 23_day_harib20e_img
)
add_custom_target(23_day_harib20e_clean
  COMMAND rm -f ${23_day_harib20e_OS}
  COMMAND rm -f ${23_day_harib20e_SYS}
  COMMAND rm -f ${23_day_harib20e_IPLB}
  COMMAND rm -f ${23_day_harib20e_HEADB}
  COMMAND rm -f ${23_day_harib20e_BOOTB}
  COMMAND rm -f ${23_day_harib20e_WILDOBJ}
)
add_custom_target(23_day_harib20e_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20e_IPLS} ${23_day_harib20e_IPLB}
  DEPENDS ${23_day_harib20e_IPLS}
)
add_custom_command(
  OUTPUT ${23_day_harib20e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20e_HEADS} ${23_day_harib20e_HEADB}
  DEPENDS ${23_day_harib20e_HEADS}
  COMMENT "Generating asmhead.bin for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${23_day_harib20e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20e_FUNCS} ${23_day_harib20e_FUNCO}
  DEPENDS ${23_day_harib20e_FUNCS}
  COMMENT "Generating naskfunc.o for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${23_day_harib20e_FONTO}
  COMMAND ${FONT} ${23_day_harib20e_FONTS} ${23_day_harib20e_FONTB}
  COMMAND ${B2O}  ${23_day_harib20e_FONTB} ${23_day_harib20e_FONTO} _hankaku
  DEPENDS ${23_day_harib20e_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${23_day_harib20e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${23_day_harib20e_LIBGE} ${23_day_harib20e_LIBGC}
  DEPENDS ${23_day_harib20e_LIBGE}
  COMMENT "Converting golibc for 23_day_harib20e"
)

add_custom_target(23_day_harib20e_sys
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_LDS} ${23_day_harib20e_CCS} ${23_day_harib20e_WILDOBJ} -o ${23_day_harib20e_BOOTB}
  COMMAND cat ${23_day_harib20e_HEADB} ${23_day_harib20e_BOOTB} > ${23_day_harib20e_SYS}
  DEPENDS ${23_day_harib20e_HEADB} 23_day_harib20e_ipl
  DEPENDS ${23_day_harib20e_FUNCO}
  DEPENDS ${23_day_harib20e_FONTO} ${23_day_harib20e_LIBGC}
)
add_custom_target(23_day_harib20e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${23_day_harib20e_IPLB} -i ${23_day_harib20e_OS}
  COMMAND mcopy -i ${23_day_harib20e_OS} ${23_day_harib20e_SYS} ::
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/a.hrb ::a.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${23_day_harib20e_OS} ${root_BINARY_DIR}/projects/23_day/harib20e/hello5.hrb ::hello5.hrb
  DEPENDS 23_day_harib20e_sys ${root_BINARY_DIR}/projects/23_day/harib20e/a.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/hello3.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/hello4.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo2.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo3.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/star1.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/stars.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/stars2.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/lines.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/hello.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/hello2.hrb ${root_BINARY_DIR}/projects/23_day/harib20e/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(23_day_harib20e_APIO ${root_BINARY_DIR}/projects/23_day/harib20e/a_nask.o)
set(23_day_harib20e_RANDO ${root_BINARY_DIR}/projects/23_day/harib20e/rand.o)
add_custom_command(
  OUTPUT ${23_day_harib20e_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${23_day_harib20e_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${23_day_harib20e_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20e/a_nask.nas ${23_day_harib20e_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/a_nask.nas
  COMMENT "Generating a_nask.o for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/a.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20e/a.c ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20e/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/a.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO}
  COMMENT "Generating a.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20e/hello3.c ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20e/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO}
  COMMENT "Generating hello3.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20e/hello4.c ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20e/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO}
  COMMENT "Generating hello4.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20e/winhelo.c ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO}
  COMMENT "Generating winhelo.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20e/winhelo2.c ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO}
  COMMENT "Generating winhelo2.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20e/winhelo3.c ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20e/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO}
  COMMENT "Generating winhelo3.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/star1.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20e/star1.c ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20e/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/star1.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO}
  COMMENT "Generating star1.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/stars.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20e/stars.c ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20e/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/stars.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO}
  COMMENT "Generating stars.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20e/stars2.c ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20e/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO}
  COMMENT "Generating stars2.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/lines.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20e_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20e/lines.c ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20e/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/lines.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20e_APIO} ${23_day_harib20e_LIBGC} ${23_day_harib20e_RANDO}
  COMMENT "Generating lines.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20e/hello.nas ${root_BINARY_DIR}/projects/23_day/harib20e/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/hello.nas
  COMMENT "Generating hello.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20e/hello2.nas ${root_BINARY_DIR}/projects/23_day/harib20e/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/hello2.nas
  COMMENT "Generating hello2.hrb for 23_day_harib20e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20e/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20e/hello5.nas ${root_BINARY_DIR}/projects/23_day/harib20e/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20e/hello5.nas
  COMMENT "Generating hello5.hrb for 23_day_harib20e"
)
