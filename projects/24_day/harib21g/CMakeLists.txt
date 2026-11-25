#----------------------------------------------------------
message(STATUS "Entering directory projects/24_day/harib21g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(24_day_harib21g_OS    ${root_BINARY_DIR}/projects/24_day/harib21g/os.img)
set(24_day_harib21g_SYS	  ${root_BINARY_DIR}/projects/24_day/harib21g/os.sys)
set(24_day_harib21g_IPLB  ${root_BINARY_DIR}/projects/24_day/harib21g/ipl.bin)
set(24_day_harib21g_IPLS  ${root_SOURCE_DIR}/projects/24_day/harib21g/ipl10.nas)
set(24_day_harib21g_HEADB ${root_BINARY_DIR}/projects/24_day/harib21g/asmhead.bin)
set(24_day_harib21g_HEADS ${root_SOURCE_DIR}/projects/24_day/harib21g/asmhead.nas)
set(24_day_harib21g_BOOTB ${root_BINARY_DIR}/projects/24_day/harib21g/boot.bin)
set(24_day_harib21g_CCO	  ${root_BINARY_DIR}/projects/24_day/harib21g/boot.o)
file(GLOB 24_day_harib21g_CCS ${root_SOURCE_DIR}/projects/24_day/harib21g/*.c)
list(REMOVE_ITEM 24_day_harib21g_CCS
  ${root_SOURCE_DIR}/projects/24_day/harib21g/a.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/hello3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/hello4.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/winhelo.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/winhelo2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/winhelo3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/star1.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/stars.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/stars2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/lines.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/walk.c
  ${root_SOURCE_DIR}/projects/24_day/harib21g/noodle.c)
set(24_day_harib21g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(24_day_harib21g_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(24_day_harib21g_FUNCS ${root_SOURCE_DIR}/projects/24_day/harib21g/naskfunc.nas)
set(24_day_harib21g_FUNCO ${root_BINARY_DIR}/projects/24_day/harib21g/naskfunc.o)
set(24_day_harib21g_FONTS ${root_SOURCE_DIR}/projects/24_day/harib21g/hankaku.txt)
set(24_day_harib21g_FONTB ${root_BINARY_DIR}/projects/24_day/harib21g/hankaku.bin)
set(24_day_harib21g_FONTO ${root_BINARY_DIR}/projects/24_day/harib21g/hankaku.o)
set(24_day_harib21g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(24_day_harib21g_LIBGC ${root_BINARY_DIR}/projects/24_day/harib21g/libgo.o)
set(24_day_harib21g_WILDOBJ ${root_BINARY_DIR}/projects/24_day/harib21g/*.o)


add_custom_target(24_day_harib21g_run
  COMMAND ${QEMU} ${QEMUOPT} ${24_day_harib21g_OS}
  DEPENDS 24_day_harib21g_img
)
add_custom_target(24_day_harib21g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${24_day_harib21g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 24_day_harib21g_img
)
add_custom_target(24_day_harib21g_clean
  COMMAND rm -f ${24_day_harib21g_OS}
  COMMAND rm -f ${24_day_harib21g_SYS}
  COMMAND rm -f ${24_day_harib21g_IPLB}
  COMMAND rm -f ${24_day_harib21g_HEADB}
  COMMAND rm -f ${24_day_harib21g_BOOTB}
  COMMAND rm -f ${24_day_harib21g_WILDOBJ}
)
add_custom_target(24_day_harib21g_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21g_IPLS} ${24_day_harib21g_IPLB}
  DEPENDS ${24_day_harib21g_IPLS}
)
add_custom_command(
  OUTPUT ${24_day_harib21g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21g_HEADS} ${24_day_harib21g_HEADB}
  DEPENDS ${24_day_harib21g_HEADS}
  COMMENT "Generating asmhead.bin for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${24_day_harib21g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21g_FUNCS} ${24_day_harib21g_FUNCO}
  DEPENDS ${24_day_harib21g_FUNCS}
  COMMENT "Generating naskfunc.o for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${24_day_harib21g_FONTO}
  COMMAND ${FONT} ${24_day_harib21g_FONTS} ${24_day_harib21g_FONTB}
  COMMAND ${B2O}  ${24_day_harib21g_FONTB} ${24_day_harib21g_FONTO} _hankaku
  DEPENDS ${24_day_harib21g_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${24_day_harib21g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${24_day_harib21g_LIBGE} ${24_day_harib21g_LIBGC}
  DEPENDS ${24_day_harib21g_LIBGE}
  COMMENT "Converting golibc for 24_day_harib21g"
)

add_custom_target(24_day_harib21g_sys
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_LDS} ${24_day_harib21g_CCS} ${24_day_harib21g_WILDOBJ} -o ${24_day_harib21g_BOOTB}
  COMMAND cat ${24_day_harib21g_HEADB} ${24_day_harib21g_BOOTB} > ${24_day_harib21g_SYS}
  DEPENDS ${24_day_harib21g_HEADB} 24_day_harib21g_ipl
  DEPENDS ${24_day_harib21g_FUNCO}
  DEPENDS ${24_day_harib21g_FONTO} ${24_day_harib21g_LIBGC}
)
add_custom_target(24_day_harib21g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${24_day_harib21g_IPLB} -i ${24_day_harib21g_OS}
  COMMAND mcopy -i ${24_day_harib21g_OS} ${24_day_harib21g_SYS} ::
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/a.hrb ::a.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${24_day_harib21g_OS} ${root_BINARY_DIR}/projects/24_day/harib21g/hello5.hrb ::hello5.hrb
  DEPENDS 24_day_harib21g_sys ${root_BINARY_DIR}/projects/24_day/harib21g/a.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/hello3.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/hello4.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo2.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo3.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/star1.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/stars.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/stars2.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/lines.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/walk.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/noodle.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/hello.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/hello2.hrb ${root_BINARY_DIR}/projects/24_day/harib21g/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(24_day_harib21g_APIO ${root_BINARY_DIR}/projects/24_day/harib21g/a_nask.o)
set(24_day_harib21g_RANDO ${root_BINARY_DIR}/projects/24_day/harib21g/rand.o)
add_custom_command(
  OUTPUT ${24_day_harib21g_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${24_day_harib21g_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${24_day_harib21g_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21g/a_nask.nas ${24_day_harib21g_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/a_nask.nas
  COMMENT "Generating a_nask.o for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/a.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/a.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/a.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating a.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/hello3.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating hello3.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/hello4.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating hello4.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/winhelo.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating winhelo.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/winhelo2.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating winhelo2.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/winhelo3.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating winhelo3.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/star1.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/star1.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/star1.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating star1.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/stars.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/stars.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/stars.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating stars.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/stars2.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating stars2.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/lines.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/lines.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/lines.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating lines.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/walk.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/walk.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/walk.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating walk.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21g_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21g/noodle.c ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21g/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21g_APIO} ${24_day_harib21g_LIBGC} ${24_day_harib21g_RANDO}
  COMMENT "Generating noodle.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21g/hello.nas ${root_BINARY_DIR}/projects/24_day/harib21g/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/hello.nas
  COMMENT "Generating hello.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21g/hello2.nas ${root_BINARY_DIR}/projects/24_day/harib21g/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/hello2.nas
  COMMENT "Generating hello2.hrb for 24_day_harib21g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21g/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21g/hello5.nas ${root_BINARY_DIR}/projects/24_day/harib21g/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21g/hello5.nas
  COMMENT "Generating hello5.hrb for 24_day_harib21g"
)
