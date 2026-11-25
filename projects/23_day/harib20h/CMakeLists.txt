#----------------------------------------------------------
message(STATUS "Entering directory projects/23_day/harib20h/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(23_day_harib20h_OS    ${root_BINARY_DIR}/projects/23_day/harib20h/os.img)
set(23_day_harib20h_SYS	  ${root_BINARY_DIR}/projects/23_day/harib20h/os.sys)
set(23_day_harib20h_IPLB  ${root_BINARY_DIR}/projects/23_day/harib20h/ipl.bin)
set(23_day_harib20h_IPLS  ${root_SOURCE_DIR}/projects/23_day/harib20h/ipl10.nas)
set(23_day_harib20h_HEADB ${root_BINARY_DIR}/projects/23_day/harib20h/asmhead.bin)
set(23_day_harib20h_HEADS ${root_SOURCE_DIR}/projects/23_day/harib20h/asmhead.nas)
set(23_day_harib20h_BOOTB ${root_BINARY_DIR}/projects/23_day/harib20h/boot.bin)
set(23_day_harib20h_CCO	  ${root_BINARY_DIR}/projects/23_day/harib20h/boot.o)
file(GLOB 23_day_harib20h_CCS ${root_SOURCE_DIR}/projects/23_day/harib20h/*.c)
list(REMOVE_ITEM 23_day_harib20h_CCS
  ${root_SOURCE_DIR}/projects/23_day/harib20h/a.c
  ${root_SOURCE_DIR}/projects/23_day/harib20h/hello3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20h/hello4.c
  ${root_SOURCE_DIR}/projects/23_day/harib20h/winhelo.c
  ${root_SOURCE_DIR}/projects/23_day/harib20h/winhelo2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20h/winhelo3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20h/star1.c
  ${root_SOURCE_DIR}/projects/23_day/harib20h/stars.c
  ${root_SOURCE_DIR}/projects/23_day/harib20h/stars2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20h/lines.c
  ${root_SOURCE_DIR}/projects/23_day/harib20h/walk.c)
set(23_day_harib20h_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(23_day_harib20h_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(23_day_harib20h_FUNCS ${root_SOURCE_DIR}/projects/23_day/harib20h/naskfunc.nas)
set(23_day_harib20h_FUNCO ${root_BINARY_DIR}/projects/23_day/harib20h/naskfunc.o)
set(23_day_harib20h_FONTS ${root_SOURCE_DIR}/projects/23_day/harib20h/hankaku.txt)
set(23_day_harib20h_FONTB ${root_BINARY_DIR}/projects/23_day/harib20h/hankaku.bin)
set(23_day_harib20h_FONTO ${root_BINARY_DIR}/projects/23_day/harib20h/hankaku.o)
set(23_day_harib20h_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(23_day_harib20h_LIBGC ${root_BINARY_DIR}/projects/23_day/harib20h/libgo.o)
set(23_day_harib20h_WILDOBJ ${root_BINARY_DIR}/projects/23_day/harib20h/*.o)


add_custom_target(23_day_harib20h_run
  COMMAND ${QEMU} ${QEMUOPT} ${23_day_harib20h_OS}
  DEPENDS 23_day_harib20h_img
)
add_custom_target(23_day_harib20h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${23_day_harib20h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 23_day_harib20h_img
)
add_custom_target(23_day_harib20h_clean
  COMMAND rm -f ${23_day_harib20h_OS}
  COMMAND rm -f ${23_day_harib20h_SYS}
  COMMAND rm -f ${23_day_harib20h_IPLB}
  COMMAND rm -f ${23_day_harib20h_HEADB}
  COMMAND rm -f ${23_day_harib20h_BOOTB}
  COMMAND rm -f ${23_day_harib20h_WILDOBJ}
)
add_custom_target(23_day_harib20h_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20h_IPLS} ${23_day_harib20h_IPLB}
  DEPENDS ${23_day_harib20h_IPLS}
)
add_custom_command(
  OUTPUT ${23_day_harib20h_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20h_HEADS} ${23_day_harib20h_HEADB}
  DEPENDS ${23_day_harib20h_HEADS}
  COMMENT "Generating asmhead.bin for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${23_day_harib20h_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20h_FUNCS} ${23_day_harib20h_FUNCO}
  DEPENDS ${23_day_harib20h_FUNCS}
  COMMENT "Generating naskfunc.o for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${23_day_harib20h_FONTO}
  COMMAND ${FONT} ${23_day_harib20h_FONTS} ${23_day_harib20h_FONTB}
  COMMAND ${B2O}  ${23_day_harib20h_FONTB} ${23_day_harib20h_FONTO} _hankaku
  DEPENDS ${23_day_harib20h_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${23_day_harib20h_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${23_day_harib20h_LIBGE} ${23_day_harib20h_LIBGC}
  DEPENDS ${23_day_harib20h_LIBGE}
  COMMENT "Converting golibc for 23_day_harib20h"
)

add_custom_target(23_day_harib20h_sys
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_LDS} ${23_day_harib20h_CCS} ${23_day_harib20h_WILDOBJ} -o ${23_day_harib20h_BOOTB}
  COMMAND cat ${23_day_harib20h_HEADB} ${23_day_harib20h_BOOTB} > ${23_day_harib20h_SYS}
  DEPENDS ${23_day_harib20h_HEADB} 23_day_harib20h_ipl
  DEPENDS ${23_day_harib20h_FUNCO}
  DEPENDS ${23_day_harib20h_FONTO} ${23_day_harib20h_LIBGC}
)
add_custom_target(23_day_harib20h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${23_day_harib20h_IPLB} -i ${23_day_harib20h_OS}
  COMMAND mcopy -i ${23_day_harib20h_OS} ${23_day_harib20h_SYS} ::
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/a.hrb ::a.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${23_day_harib20h_OS} ${root_BINARY_DIR}/projects/23_day/harib20h/hello5.hrb ::hello5.hrb
  DEPENDS 23_day_harib20h_sys ${root_BINARY_DIR}/projects/23_day/harib20h/a.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/hello3.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/hello4.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo2.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo3.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/star1.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/stars.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/stars2.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/lines.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/walk.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/hello.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/hello2.hrb ${root_BINARY_DIR}/projects/23_day/harib20h/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(23_day_harib20h_APIO ${root_BINARY_DIR}/projects/23_day/harib20h/a_nask.o)
set(23_day_harib20h_RANDO ${root_BINARY_DIR}/projects/23_day/harib20h/rand.o)
add_custom_command(
  OUTPUT ${23_day_harib20h_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${23_day_harib20h_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${23_day_harib20h_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20h/a_nask.nas ${23_day_harib20h_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/a_nask.nas
  COMMENT "Generating a_nask.o for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/a.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/a.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/a.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating a.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/hello3.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating hello3.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/hello4.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating hello4.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/winhelo.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating winhelo.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/winhelo2.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating winhelo2.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/winhelo3.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating winhelo3.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/star1.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/star1.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/star1.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating star1.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/stars.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/stars.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/stars.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating stars.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/stars2.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating stars2.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/lines.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/lines.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/lines.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating lines.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/walk.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20h_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20h/walk.c ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20h/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/walk.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20h_APIO} ${23_day_harib20h_LIBGC} ${23_day_harib20h_RANDO}
  COMMENT "Generating walk.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20h/hello.nas ${root_BINARY_DIR}/projects/23_day/harib20h/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/hello.nas
  COMMENT "Generating hello.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20h/hello2.nas ${root_BINARY_DIR}/projects/23_day/harib20h/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/hello2.nas
  COMMENT "Generating hello2.hrb for 23_day_harib20h"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20h/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20h/hello5.nas ${root_BINARY_DIR}/projects/23_day/harib20h/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20h/hello5.nas
  COMMENT "Generating hello5.hrb for 23_day_harib20h"
)
