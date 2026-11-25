#----------------------------------------------------------
message(STATUS "Entering directory projects/25_day/harib22i/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(25_day_harib22i_OS    ${root_BINARY_DIR}/projects/25_day/harib22i/os.img)
set(25_day_harib22i_SYS	  ${root_BINARY_DIR}/projects/25_day/harib22i/os.sys)
set(25_day_harib22i_IPLB  ${root_BINARY_DIR}/projects/25_day/harib22i/ipl.bin)
set(25_day_harib22i_IPLS  ${root_SOURCE_DIR}/projects/25_day/harib22i/ipl10.nas)
set(25_day_harib22i_HEADB ${root_BINARY_DIR}/projects/25_day/harib22i/asmhead.bin)
set(25_day_harib22i_HEADS ${root_SOURCE_DIR}/projects/25_day/harib22i/asmhead.nas)
set(25_day_harib22i_BOOTB ${root_BINARY_DIR}/projects/25_day/harib22i/boot.bin)
set(25_day_harib22i_CCO	  ${root_BINARY_DIR}/projects/25_day/harib22i/boot.o)
file(GLOB 25_day_harib22i_CCS ${root_SOURCE_DIR}/projects/25_day/harib22i/*.c)
list(REMOVE_ITEM 25_day_harib22i_CCS
  ${root_SOURCE_DIR}/projects/25_day/harib22i/a.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/hello3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/hello4.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/winhelo.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/winhelo2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/winhelo3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/star1.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/stars.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/stars2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/lines.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/walk.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/noodle.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/beepdown.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/color.c
  ${root_SOURCE_DIR}/projects/25_day/harib22i/color2.c)
set(25_day_harib22i_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(25_day_harib22i_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(25_day_harib22i_FUNCS ${root_SOURCE_DIR}/projects/25_day/harib22i/naskfunc.nas)
set(25_day_harib22i_FUNCO ${root_BINARY_DIR}/projects/25_day/harib22i/naskfunc.o)
set(25_day_harib22i_FONTS ${root_SOURCE_DIR}/projects/25_day/harib22i/hankaku.txt)
set(25_day_harib22i_FONTB ${root_BINARY_DIR}/projects/25_day/harib22i/hankaku.bin)
set(25_day_harib22i_FONTO ${root_BINARY_DIR}/projects/25_day/harib22i/hankaku.o)
set(25_day_harib22i_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(25_day_harib22i_LIBGC ${root_BINARY_DIR}/projects/25_day/harib22i/libgo.o)
set(25_day_harib22i_WILDOBJ ${root_BINARY_DIR}/projects/25_day/harib22i/*.o)


add_custom_target(25_day_harib22i_run
  COMMAND ${QEMU} ${QEMUOPT} ${25_day_harib22i_OS}
  DEPENDS 25_day_harib22i_img
)
add_custom_target(25_day_harib22i_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${25_day_harib22i_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 25_day_harib22i_img
)
add_custom_target(25_day_harib22i_clean
  COMMAND rm -f ${25_day_harib22i_OS}
  COMMAND rm -f ${25_day_harib22i_SYS}
  COMMAND rm -f ${25_day_harib22i_IPLB}
  COMMAND rm -f ${25_day_harib22i_HEADB}
  COMMAND rm -f ${25_day_harib22i_BOOTB}
  COMMAND rm -f ${25_day_harib22i_WILDOBJ}
)
add_custom_target(25_day_harib22i_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22i_IPLS} ${25_day_harib22i_IPLB}
  DEPENDS ${25_day_harib22i_IPLS}
)
add_custom_command(
  OUTPUT ${25_day_harib22i_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22i_HEADS} ${25_day_harib22i_HEADB}
  DEPENDS ${25_day_harib22i_HEADS}
  COMMENT "Generating asmhead.bin for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${25_day_harib22i_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22i_FUNCS} ${25_day_harib22i_FUNCO}
  DEPENDS ${25_day_harib22i_FUNCS}
  COMMENT "Generating naskfunc.o for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${25_day_harib22i_FONTO}
  COMMAND ${FONT} ${25_day_harib22i_FONTS} ${25_day_harib22i_FONTB}
  COMMAND ${B2O}  ${25_day_harib22i_FONTB} ${25_day_harib22i_FONTO} _hankaku
  DEPENDS ${25_day_harib22i_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${25_day_harib22i_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${25_day_harib22i_LIBGE} ${25_day_harib22i_LIBGC}
  DEPENDS ${25_day_harib22i_LIBGE}
  COMMENT "Converting golibc for 25_day_harib22i"
)

add_custom_target(25_day_harib22i_sys
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_LDS} ${25_day_harib22i_CCS} ${25_day_harib22i_WILDOBJ} -o ${25_day_harib22i_BOOTB}
  COMMAND cat ${25_day_harib22i_HEADB} ${25_day_harib22i_BOOTB} > ${25_day_harib22i_SYS}
  DEPENDS ${25_day_harib22i_HEADB} 25_day_harib22i_ipl
  DEPENDS ${25_day_harib22i_FUNCO}
  DEPENDS ${25_day_harib22i_FONTO} ${25_day_harib22i_LIBGC}
)
add_custom_target(25_day_harib22i_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${25_day_harib22i_IPLB} -i ${25_day_harib22i_OS}
  COMMAND mcopy -i ${25_day_harib22i_OS} ${25_day_harib22i_SYS} ::
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/a.hrb ::a.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/color.hrb ::color.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${25_day_harib22i_OS} ${root_BINARY_DIR}/projects/25_day/harib22i/hello5.hrb ::hello5.hrb
  DEPENDS 25_day_harib22i_sys ${root_BINARY_DIR}/projects/25_day/harib22i/a.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/hello3.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/hello4.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo2.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo3.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/star1.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/stars.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/stars2.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/lines.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/walk.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/noodle.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/beepdown.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/color.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/color2.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/hello.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/hello2.hrb ${root_BINARY_DIR}/projects/25_day/harib22i/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(25_day_harib22i_APIO ${root_BINARY_DIR}/projects/25_day/harib22i/a_nask.o)
set(25_day_harib22i_RANDO ${root_BINARY_DIR}/projects/25_day/harib22i/rand.o)
add_custom_command(
  OUTPUT ${25_day_harib22i_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${25_day_harib22i_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${25_day_harib22i_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22i/a_nask.nas ${25_day_harib22i_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/a_nask.nas
  COMMENT "Generating a_nask.o for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/a.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/a.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/a.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating a.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/hello3.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating hello3.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/hello4.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating hello4.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/winhelo.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating winhelo.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/winhelo2.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating winhelo2.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/winhelo3.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating winhelo3.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/star1.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/star1.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/star1.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating star1.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/stars.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/stars.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/stars.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating stars.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/stars2.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating stars2.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/lines.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/lines.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/lines.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating lines.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/walk.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/walk.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/walk.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating walk.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/color2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/color2.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/color2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating color2.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/color.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/color.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/color.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating color.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/beepdown.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating beepdown.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22i_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22i/noodle.c ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22i/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22i_APIO} ${25_day_harib22i_LIBGC} ${25_day_harib22i_RANDO}
  COMMENT "Generating noodle.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22i/hello.nas ${root_BINARY_DIR}/projects/25_day/harib22i/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/hello.nas
  COMMENT "Generating hello.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22i/hello2.nas ${root_BINARY_DIR}/projects/25_day/harib22i/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/hello2.nas
  COMMENT "Generating hello2.hrb for 25_day_harib22i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22i/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22i/hello5.nas ${root_BINARY_DIR}/projects/25_day/harib22i/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22i/hello5.nas
  COMMENT "Generating hello5.hrb for 25_day_harib22i"
)
