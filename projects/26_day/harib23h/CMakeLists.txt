#----------------------------------------------------------
message(STATUS "Entering directory projects/26_day/harib23h/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(26_day_harib23h_OS    ${root_BINARY_DIR}/projects/26_day/harib23h/os.img)
set(26_day_harib23h_SYS	  ${root_BINARY_DIR}/projects/26_day/harib23h/os.sys)
set(26_day_harib23h_IPLB  ${root_BINARY_DIR}/projects/26_day/harib23h/ipl.bin)
set(26_day_harib23h_IPLS  ${root_SOURCE_DIR}/projects/26_day/harib23h/ipl10.nas)
set(26_day_harib23h_HEADB ${root_BINARY_DIR}/projects/26_day/harib23h/asmhead.bin)
set(26_day_harib23h_HEADS ${root_SOURCE_DIR}/projects/26_day/harib23h/asmhead.nas)
set(26_day_harib23h_BOOTB ${root_BINARY_DIR}/projects/26_day/harib23h/boot.bin)
set(26_day_harib23h_CCO	  ${root_BINARY_DIR}/projects/26_day/harib23h/boot.o)
file(GLOB 26_day_harib23h_CCS ${root_SOURCE_DIR}/projects/26_day/harib23h/*.c)
list(REMOVE_ITEM 26_day_harib23h_CCS
  ${root_SOURCE_DIR}/projects/26_day/harib23h/a.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/hello3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/hello4.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/winhelo.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/winhelo2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/winhelo3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/star1.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/stars.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/stars2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/lines.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/walk.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/noodle.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/beepdown.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/color.c
  ${root_SOURCE_DIR}/projects/26_day/harib23h/color2.c)
set(26_day_harib23h_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(26_day_harib23h_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(26_day_harib23h_FUNCS ${root_SOURCE_DIR}/projects/26_day/harib23h/naskfunc.nas)
set(26_day_harib23h_FUNCO ${root_BINARY_DIR}/projects/26_day/harib23h/naskfunc.o)
set(26_day_harib23h_FONTS ${root_SOURCE_DIR}/projects/26_day/harib23h/hankaku.txt)
set(26_day_harib23h_FONTB ${root_BINARY_DIR}/projects/26_day/harib23h/hankaku.bin)
set(26_day_harib23h_FONTO ${root_BINARY_DIR}/projects/26_day/harib23h/hankaku.o)
set(26_day_harib23h_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(26_day_harib23h_LIBGC ${root_BINARY_DIR}/projects/26_day/harib23h/libgo.o)
set(26_day_harib23h_WILDOBJ ${root_BINARY_DIR}/projects/26_day/harib23h/*.o)


add_custom_target(26_day_harib23h_run
  COMMAND ${QEMU} ${QEMUOPT} ${26_day_harib23h_OS}
  DEPENDS 26_day_harib23h_img
)
add_custom_target(26_day_harib23h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${26_day_harib23h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 26_day_harib23h_img
)
add_custom_target(26_day_harib23h_clean
  COMMAND rm -f ${26_day_harib23h_OS}
  COMMAND rm -f ${26_day_harib23h_SYS}
  COMMAND rm -f ${26_day_harib23h_IPLB}
  COMMAND rm -f ${26_day_harib23h_HEADB}
  COMMAND rm -f ${26_day_harib23h_BOOTB}
  COMMAND rm -f ${26_day_harib23h_WILDOBJ}
)
add_custom_target(26_day_harib23h_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23h_IPLS} ${26_day_harib23h_IPLB}
  DEPENDS ${26_day_harib23h_IPLS}
)
add_custom_command(
  OUTPUT ${26_day_harib23h_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23h_HEADS} ${26_day_harib23h_HEADB}
  DEPENDS ${26_day_harib23h_HEADS}
  COMMENT "Generating asmhead.bin for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${26_day_harib23h_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23h_FUNCS} ${26_day_harib23h_FUNCO}
  DEPENDS ${26_day_harib23h_FUNCS}
  COMMENT "Generating naskfunc.o for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${26_day_harib23h_FONTO}
  COMMAND ${FONT} ${26_day_harib23h_FONTS} ${26_day_harib23h_FONTB}
  COMMAND ${B2O}  ${26_day_harib23h_FONTB} ${26_day_harib23h_FONTO} _hankaku
  DEPENDS ${26_day_harib23h_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${26_day_harib23h_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${26_day_harib23h_LIBGE} ${26_day_harib23h_LIBGC}
  DEPENDS ${26_day_harib23h_LIBGE}
  COMMENT "Converting golibc for 26_day_harib23h"
)

add_custom_target(26_day_harib23h_sys
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_LDS} ${26_day_harib23h_CCS} ${26_day_harib23h_WILDOBJ} -o ${26_day_harib23h_BOOTB}
  COMMAND cat ${26_day_harib23h_HEADB} ${26_day_harib23h_BOOTB} > ${26_day_harib23h_SYS}
  DEPENDS ${26_day_harib23h_HEADB} 26_day_harib23h_ipl
  DEPENDS ${26_day_harib23h_FUNCO}
  DEPENDS ${26_day_harib23h_FONTO} ${26_day_harib23h_LIBGC}
)
add_custom_target(26_day_harib23h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${26_day_harib23h_IPLB} -i ${26_day_harib23h_OS}
  COMMAND mcopy -i ${26_day_harib23h_OS} ${26_day_harib23h_SYS} ::
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/a.hrb ::a.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/color.hrb ::color.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${26_day_harib23h_OS} ${root_BINARY_DIR}/projects/26_day/harib23h/hello5.hrb ::hello5.hrb
  DEPENDS 26_day_harib23h_sys ${root_BINARY_DIR}/projects/26_day/harib23h/a.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/hello3.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/hello4.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo2.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo3.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/star1.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/stars.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/stars2.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/lines.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/walk.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/noodle.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/beepdown.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/color.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/color2.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/hello.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/hello2.hrb ${root_BINARY_DIR}/projects/26_day/harib23h/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(26_day_harib23h_APIO ${root_BINARY_DIR}/projects/26_day/harib23h/a_nask.o)
set(26_day_harib23h_RANDO ${root_BINARY_DIR}/projects/26_day/harib23h/rand.o)
add_custom_command(
  OUTPUT ${26_day_harib23h_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${26_day_harib23h_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${26_day_harib23h_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23h/a_nask.nas ${26_day_harib23h_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/a_nask.nas
  COMMENT "Generating a_nask.o for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/a.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/a.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/a.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating a.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/hello3.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating hello3.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/hello4.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating hello4.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/winhelo.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating winhelo.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/winhelo2.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating winhelo2.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/winhelo3.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating winhelo3.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/star1.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/star1.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/star1.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating star1.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/stars.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/stars.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/stars.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating stars.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/stars2.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating stars2.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/lines.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/lines.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/lines.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating lines.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/walk.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/walk.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/walk.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating walk.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/color2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/color2.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/color2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating color2.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/color.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/color.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/color.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating color.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/beepdown.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating beepdown.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23h_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23h/noodle.c ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23h/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23h_APIO} ${26_day_harib23h_LIBGC} ${26_day_harib23h_RANDO}
  COMMENT "Generating noodle.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23h/hello.nas ${root_BINARY_DIR}/projects/26_day/harib23h/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/hello.nas
  COMMENT "Generating hello.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23h/hello2.nas ${root_BINARY_DIR}/projects/26_day/harib23h/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/hello2.nas
  COMMENT "Generating hello2.hrb for 26_day_harib23h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23h/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23h/hello5.nas ${root_BINARY_DIR}/projects/26_day/harib23h/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23h/hello5.nas
  COMMENT "Generating hello5.hrb for 26_day_harib23h"
)
