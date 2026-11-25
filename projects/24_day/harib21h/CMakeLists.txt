#----------------------------------------------------------
message(STATUS "Entering directory projects/24_day/harib21h/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(24_day_harib21h_OS    ${root_BINARY_DIR}/projects/24_day/harib21h/os.img)
set(24_day_harib21h_SYS	  ${root_BINARY_DIR}/projects/24_day/harib21h/os.sys)
set(24_day_harib21h_IPLB  ${root_BINARY_DIR}/projects/24_day/harib21h/ipl.bin)
set(24_day_harib21h_IPLS  ${root_SOURCE_DIR}/projects/24_day/harib21h/ipl10.nas)
set(24_day_harib21h_HEADB ${root_BINARY_DIR}/projects/24_day/harib21h/asmhead.bin)
set(24_day_harib21h_HEADS ${root_SOURCE_DIR}/projects/24_day/harib21h/asmhead.nas)
set(24_day_harib21h_BOOTB ${root_BINARY_DIR}/projects/24_day/harib21h/boot.bin)
set(24_day_harib21h_CCO	  ${root_BINARY_DIR}/projects/24_day/harib21h/boot.o)
file(GLOB 24_day_harib21h_CCS ${root_SOURCE_DIR}/projects/24_day/harib21h/*.c)
list(REMOVE_ITEM 24_day_harib21h_CCS
  ${root_SOURCE_DIR}/projects/24_day/harib21h/a.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/hello3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/hello4.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/winhelo.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/winhelo2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/winhelo3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/star1.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/stars.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/stars2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/lines.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/walk.c
  ${root_SOURCE_DIR}/projects/24_day/harib21h/noodle.c)
set(24_day_harib21h_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(24_day_harib21h_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(24_day_harib21h_FUNCS ${root_SOURCE_DIR}/projects/24_day/harib21h/naskfunc.nas)
set(24_day_harib21h_FUNCO ${root_BINARY_DIR}/projects/24_day/harib21h/naskfunc.o)
set(24_day_harib21h_FONTS ${root_SOURCE_DIR}/projects/24_day/harib21h/hankaku.txt)
set(24_day_harib21h_FONTB ${root_BINARY_DIR}/projects/24_day/harib21h/hankaku.bin)
set(24_day_harib21h_FONTO ${root_BINARY_DIR}/projects/24_day/harib21h/hankaku.o)
set(24_day_harib21h_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(24_day_harib21h_LIBGC ${root_BINARY_DIR}/projects/24_day/harib21h/libgo.o)
set(24_day_harib21h_WILDOBJ ${root_BINARY_DIR}/projects/24_day/harib21h/*.o)


add_custom_target(24_day_harib21h_run
  COMMAND ${QEMU} ${QEMUOPT} ${24_day_harib21h_OS}
  DEPENDS 24_day_harib21h_img
)
add_custom_target(24_day_harib21h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${24_day_harib21h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 24_day_harib21h_img
)
add_custom_target(24_day_harib21h_clean
  COMMAND rm -f ${24_day_harib21h_OS}
  COMMAND rm -f ${24_day_harib21h_SYS}
  COMMAND rm -f ${24_day_harib21h_IPLB}
  COMMAND rm -f ${24_day_harib21h_HEADB}
  COMMAND rm -f ${24_day_harib21h_BOOTB}
  COMMAND rm -f ${24_day_harib21h_WILDOBJ}
)
add_custom_target(24_day_harib21h_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21h_IPLS} ${24_day_harib21h_IPLB}
  DEPENDS ${24_day_harib21h_IPLS}
)
add_custom_command(
  OUTPUT ${24_day_harib21h_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21h_HEADS} ${24_day_harib21h_HEADB}
  DEPENDS ${24_day_harib21h_HEADS}
  COMMENT "Generating asmhead.bin for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${24_day_harib21h_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21h_FUNCS} ${24_day_harib21h_FUNCO}
  DEPENDS ${24_day_harib21h_FUNCS}
  COMMENT "Generating naskfunc.o for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${24_day_harib21h_FONTO}
  COMMAND ${FONT} ${24_day_harib21h_FONTS} ${24_day_harib21h_FONTB}
  COMMAND ${B2O}  ${24_day_harib21h_FONTB} ${24_day_harib21h_FONTO} _hankaku
  DEPENDS ${24_day_harib21h_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${24_day_harib21h_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${24_day_harib21h_LIBGE} ${24_day_harib21h_LIBGC}
  DEPENDS ${24_day_harib21h_LIBGE}
  COMMENT "Converting golibc for 24_day_harib21h"
)

add_custom_target(24_day_harib21h_sys
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_LDS} ${24_day_harib21h_CCS} ${24_day_harib21h_WILDOBJ} -o ${24_day_harib21h_BOOTB}
  COMMAND cat ${24_day_harib21h_HEADB} ${24_day_harib21h_BOOTB} > ${24_day_harib21h_SYS}
  DEPENDS ${24_day_harib21h_HEADB} 24_day_harib21h_ipl
  DEPENDS ${24_day_harib21h_FUNCO}
  DEPENDS ${24_day_harib21h_FONTO} ${24_day_harib21h_LIBGC}
)
add_custom_target(24_day_harib21h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${24_day_harib21h_IPLB} -i ${24_day_harib21h_OS}
  COMMAND mcopy -i ${24_day_harib21h_OS} ${24_day_harib21h_SYS} ::
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/a.hrb ::a.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${24_day_harib21h_OS} ${root_BINARY_DIR}/projects/24_day/harib21h/hello5.hrb ::hello5.hrb
  DEPENDS 24_day_harib21h_sys ${root_BINARY_DIR}/projects/24_day/harib21h/a.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/hello3.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/hello4.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo2.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo3.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/star1.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/stars.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/stars2.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/lines.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/walk.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/noodle.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/hello.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/hello2.hrb ${root_BINARY_DIR}/projects/24_day/harib21h/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(24_day_harib21h_APIO ${root_BINARY_DIR}/projects/24_day/harib21h/a_nask.o)
set(24_day_harib21h_RANDO ${root_BINARY_DIR}/projects/24_day/harib21h/rand.o)
add_custom_command(
  OUTPUT ${24_day_harib21h_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${24_day_harib21h_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${24_day_harib21h_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21h/a_nask.nas ${24_day_harib21h_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/a_nask.nas
  COMMENT "Generating a_nask.o for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/a.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/a.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/a.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating a.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/hello3.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating hello3.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/hello4.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating hello4.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/winhelo.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating winhelo.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/winhelo2.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating winhelo2.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/winhelo3.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating winhelo3.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/star1.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/star1.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/star1.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating star1.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/stars.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/stars.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/stars.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating stars.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/stars2.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating stars2.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/lines.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/lines.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/lines.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating lines.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/walk.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/walk.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/walk.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating walk.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21h_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21h/noodle.c ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21h/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21h_APIO} ${24_day_harib21h_LIBGC} ${24_day_harib21h_RANDO}
  COMMENT "Generating noodle.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21h/hello.nas ${root_BINARY_DIR}/projects/24_day/harib21h/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/hello.nas
  COMMENT "Generating hello.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21h/hello2.nas ${root_BINARY_DIR}/projects/24_day/harib21h/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/hello2.nas
  COMMENT "Generating hello2.hrb for 24_day_harib21h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21h/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21h/hello5.nas ${root_BINARY_DIR}/projects/24_day/harib21h/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21h/hello5.nas
  COMMENT "Generating hello5.hrb for 24_day_harib21h"
)
