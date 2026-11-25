#----------------------------------------------------------
message(STATUS "Entering directory projects/25_day/harib22h/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(25_day_harib22h_OS    ${root_BINARY_DIR}/projects/25_day/harib22h/os.img)
set(25_day_harib22h_SYS	  ${root_BINARY_DIR}/projects/25_day/harib22h/os.sys)
set(25_day_harib22h_IPLB  ${root_BINARY_DIR}/projects/25_day/harib22h/ipl.bin)
set(25_day_harib22h_IPLS  ${root_SOURCE_DIR}/projects/25_day/harib22h/ipl10.nas)
set(25_day_harib22h_HEADB ${root_BINARY_DIR}/projects/25_day/harib22h/asmhead.bin)
set(25_day_harib22h_HEADS ${root_SOURCE_DIR}/projects/25_day/harib22h/asmhead.nas)
set(25_day_harib22h_BOOTB ${root_BINARY_DIR}/projects/25_day/harib22h/boot.bin)
set(25_day_harib22h_CCO	  ${root_BINARY_DIR}/projects/25_day/harib22h/boot.o)
file(GLOB 25_day_harib22h_CCS ${root_SOURCE_DIR}/projects/25_day/harib22h/*.c)
list(REMOVE_ITEM 25_day_harib22h_CCS
  ${root_SOURCE_DIR}/projects/25_day/harib22h/a.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/hello3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/hello4.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/winhelo.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/winhelo2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/winhelo3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/star1.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/stars.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/stars2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/lines.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/walk.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/noodle.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/beepdown.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/color.c
  ${root_SOURCE_DIR}/projects/25_day/harib22h/color2.c)
set(25_day_harib22h_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(25_day_harib22h_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(25_day_harib22h_FUNCS ${root_SOURCE_DIR}/projects/25_day/harib22h/naskfunc.nas)
set(25_day_harib22h_FUNCO ${root_BINARY_DIR}/projects/25_day/harib22h/naskfunc.o)
set(25_day_harib22h_FONTS ${root_SOURCE_DIR}/projects/25_day/harib22h/hankaku.txt)
set(25_day_harib22h_FONTB ${root_BINARY_DIR}/projects/25_day/harib22h/hankaku.bin)
set(25_day_harib22h_FONTO ${root_BINARY_DIR}/projects/25_day/harib22h/hankaku.o)
set(25_day_harib22h_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(25_day_harib22h_LIBGC ${root_BINARY_DIR}/projects/25_day/harib22h/libgo.o)
set(25_day_harib22h_WILDOBJ ${root_BINARY_DIR}/projects/25_day/harib22h/*.o)


add_custom_target(25_day_harib22h_run
  COMMAND ${QEMU} ${QEMUOPT} ${25_day_harib22h_OS}
  DEPENDS 25_day_harib22h_img
)
add_custom_target(25_day_harib22h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${25_day_harib22h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 25_day_harib22h_img
)
add_custom_target(25_day_harib22h_clean
  COMMAND rm -f ${25_day_harib22h_OS}
  COMMAND rm -f ${25_day_harib22h_SYS}
  COMMAND rm -f ${25_day_harib22h_IPLB}
  COMMAND rm -f ${25_day_harib22h_HEADB}
  COMMAND rm -f ${25_day_harib22h_BOOTB}
  COMMAND rm -f ${25_day_harib22h_WILDOBJ}
)
add_custom_target(25_day_harib22h_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22h_IPLS} ${25_day_harib22h_IPLB}
  DEPENDS ${25_day_harib22h_IPLS}
)
add_custom_command(
  OUTPUT ${25_day_harib22h_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22h_HEADS} ${25_day_harib22h_HEADB}
  DEPENDS ${25_day_harib22h_HEADS}
  COMMENT "Generating asmhead.bin for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${25_day_harib22h_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22h_FUNCS} ${25_day_harib22h_FUNCO}
  DEPENDS ${25_day_harib22h_FUNCS}
  COMMENT "Generating naskfunc.o for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${25_day_harib22h_FONTO}
  COMMAND ${FONT} ${25_day_harib22h_FONTS} ${25_day_harib22h_FONTB}
  COMMAND ${B2O}  ${25_day_harib22h_FONTB} ${25_day_harib22h_FONTO} _hankaku
  DEPENDS ${25_day_harib22h_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${25_day_harib22h_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${25_day_harib22h_LIBGE} ${25_day_harib22h_LIBGC}
  DEPENDS ${25_day_harib22h_LIBGE}
  COMMENT "Converting golibc for 25_day_harib22h"
)

add_custom_target(25_day_harib22h_sys
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_LDS} ${25_day_harib22h_CCS} ${25_day_harib22h_WILDOBJ} -o ${25_day_harib22h_BOOTB}
  COMMAND cat ${25_day_harib22h_HEADB} ${25_day_harib22h_BOOTB} > ${25_day_harib22h_SYS}
  DEPENDS ${25_day_harib22h_HEADB} 25_day_harib22h_ipl
  DEPENDS ${25_day_harib22h_FUNCO}
  DEPENDS ${25_day_harib22h_FONTO} ${25_day_harib22h_LIBGC}
)
add_custom_target(25_day_harib22h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${25_day_harib22h_IPLB} -i ${25_day_harib22h_OS}
  COMMAND mcopy -i ${25_day_harib22h_OS} ${25_day_harib22h_SYS} ::
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/a.hrb ::a.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/color.hrb ::color.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${25_day_harib22h_OS} ${root_BINARY_DIR}/projects/25_day/harib22h/hello5.hrb ::hello5.hrb
  DEPENDS 25_day_harib22h_sys ${root_BINARY_DIR}/projects/25_day/harib22h/a.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/hello3.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/hello4.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo2.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo3.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/star1.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/stars.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/stars2.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/lines.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/walk.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/noodle.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/beepdown.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/color.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/color2.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/hello.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/hello2.hrb ${root_BINARY_DIR}/projects/25_day/harib22h/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(25_day_harib22h_APIO ${root_BINARY_DIR}/projects/25_day/harib22h/a_nask.o)
set(25_day_harib22h_RANDO ${root_BINARY_DIR}/projects/25_day/harib22h/rand.o)
add_custom_command(
  OUTPUT ${25_day_harib22h_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${25_day_harib22h_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${25_day_harib22h_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22h/a_nask.nas ${25_day_harib22h_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/a_nask.nas
  COMMENT "Generating a_nask.o for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/a.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/a.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/a.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating a.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/hello3.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating hello3.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/hello4.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating hello4.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/winhelo.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating winhelo.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/winhelo2.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating winhelo2.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/winhelo3.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating winhelo3.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/star1.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/star1.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/star1.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating star1.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/stars.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/stars.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/stars.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating stars.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/stars2.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating stars2.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/lines.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/lines.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/lines.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating lines.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/walk.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/walk.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/walk.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating walk.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/color2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/color2.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/color2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating color2.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/color.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/color.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/color.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating color.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/beepdown.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating beepdown.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22h_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22h/noodle.c ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22h/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22h_APIO} ${25_day_harib22h_LIBGC} ${25_day_harib22h_RANDO}
  COMMENT "Generating noodle.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22h/hello.nas ${root_BINARY_DIR}/projects/25_day/harib22h/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/hello.nas
  COMMENT "Generating hello.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22h/hello2.nas ${root_BINARY_DIR}/projects/25_day/harib22h/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/hello2.nas
  COMMENT "Generating hello2.hrb for 25_day_harib22h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22h/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22h/hello5.nas ${root_BINARY_DIR}/projects/25_day/harib22h/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22h/hello5.nas
  COMMENT "Generating hello5.hrb for 25_day_harib22h"
)
