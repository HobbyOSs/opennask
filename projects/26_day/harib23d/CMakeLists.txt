#----------------------------------------------------------
message(STATUS "Entering directory projects/26_day/harib23d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(26_day_harib23d_OS    ${root_BINARY_DIR}/projects/26_day/harib23d/os.img)
set(26_day_harib23d_SYS	  ${root_BINARY_DIR}/projects/26_day/harib23d/os.sys)
set(26_day_harib23d_IPLB  ${root_BINARY_DIR}/projects/26_day/harib23d/ipl.bin)
set(26_day_harib23d_IPLS  ${root_SOURCE_DIR}/projects/26_day/harib23d/ipl10.nas)
set(26_day_harib23d_HEADB ${root_BINARY_DIR}/projects/26_day/harib23d/asmhead.bin)
set(26_day_harib23d_HEADS ${root_SOURCE_DIR}/projects/26_day/harib23d/asmhead.nas)
set(26_day_harib23d_BOOTB ${root_BINARY_DIR}/projects/26_day/harib23d/boot.bin)
set(26_day_harib23d_CCO	  ${root_BINARY_DIR}/projects/26_day/harib23d/boot.o)
file(GLOB 26_day_harib23d_CCS ${root_SOURCE_DIR}/projects/26_day/harib23d/*.c)
list(REMOVE_ITEM 26_day_harib23d_CCS
  ${root_SOURCE_DIR}/projects/26_day/harib23d/a.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/hello3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/hello4.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/winhelo.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/winhelo2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/winhelo3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/star1.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/stars.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/stars2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/lines.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/walk.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/noodle.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/beepdown.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/color.c
  ${root_SOURCE_DIR}/projects/26_day/harib23d/color2.c)
set(26_day_harib23d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(26_day_harib23d_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(26_day_harib23d_FUNCS ${root_SOURCE_DIR}/projects/26_day/harib23d/naskfunc.nas)
set(26_day_harib23d_FUNCO ${root_BINARY_DIR}/projects/26_day/harib23d/naskfunc.o)
set(26_day_harib23d_FONTS ${root_SOURCE_DIR}/projects/26_day/harib23d/hankaku.txt)
set(26_day_harib23d_FONTB ${root_BINARY_DIR}/projects/26_day/harib23d/hankaku.bin)
set(26_day_harib23d_FONTO ${root_BINARY_DIR}/projects/26_day/harib23d/hankaku.o)
set(26_day_harib23d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(26_day_harib23d_LIBGC ${root_BINARY_DIR}/projects/26_day/harib23d/libgo.o)
set(26_day_harib23d_WILDOBJ ${root_BINARY_DIR}/projects/26_day/harib23d/*.o)


add_custom_target(26_day_harib23d_run
  COMMAND ${QEMU} ${QEMUOPT} ${26_day_harib23d_OS}
  DEPENDS 26_day_harib23d_img
)
add_custom_target(26_day_harib23d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${26_day_harib23d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 26_day_harib23d_img
)
add_custom_target(26_day_harib23d_clean
  COMMAND rm -f ${26_day_harib23d_OS}
  COMMAND rm -f ${26_day_harib23d_SYS}
  COMMAND rm -f ${26_day_harib23d_IPLB}
  COMMAND rm -f ${26_day_harib23d_HEADB}
  COMMAND rm -f ${26_day_harib23d_BOOTB}
  COMMAND rm -f ${26_day_harib23d_WILDOBJ}
)
add_custom_target(26_day_harib23d_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23d_IPLS} ${26_day_harib23d_IPLB}
  DEPENDS ${26_day_harib23d_IPLS}
)
add_custom_command(
  OUTPUT ${26_day_harib23d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23d_HEADS} ${26_day_harib23d_HEADB}
  DEPENDS ${26_day_harib23d_HEADS}
  COMMENT "Generating asmhead.bin for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${26_day_harib23d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23d_FUNCS} ${26_day_harib23d_FUNCO}
  DEPENDS ${26_day_harib23d_FUNCS}
  COMMENT "Generating naskfunc.o for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${26_day_harib23d_FONTO}
  COMMAND ${FONT} ${26_day_harib23d_FONTS} ${26_day_harib23d_FONTB}
  COMMAND ${B2O}  ${26_day_harib23d_FONTB} ${26_day_harib23d_FONTO} _hankaku
  DEPENDS ${26_day_harib23d_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${26_day_harib23d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${26_day_harib23d_LIBGE} ${26_day_harib23d_LIBGC}
  DEPENDS ${26_day_harib23d_LIBGE}
  COMMENT "Converting golibc for 26_day_harib23d"
)

add_custom_target(26_day_harib23d_sys
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_LDS} ${26_day_harib23d_CCS} ${26_day_harib23d_WILDOBJ} -o ${26_day_harib23d_BOOTB}
  COMMAND cat ${26_day_harib23d_HEADB} ${26_day_harib23d_BOOTB} > ${26_day_harib23d_SYS}
  DEPENDS ${26_day_harib23d_HEADB} 26_day_harib23d_ipl
  DEPENDS ${26_day_harib23d_FUNCO}
  DEPENDS ${26_day_harib23d_FONTO} ${26_day_harib23d_LIBGC}
)
add_custom_target(26_day_harib23d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${26_day_harib23d_IPLB} -i ${26_day_harib23d_OS}
  COMMAND mcopy -i ${26_day_harib23d_OS} ${26_day_harib23d_SYS} ::
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/a.hrb ::a.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/color.hrb ::color.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${26_day_harib23d_OS} ${root_BINARY_DIR}/projects/26_day/harib23d/hello5.hrb ::hello5.hrb
  DEPENDS 26_day_harib23d_sys ${root_BINARY_DIR}/projects/26_day/harib23d/a.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/hello3.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/hello4.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo2.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo3.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/star1.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/stars.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/stars2.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/lines.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/walk.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/noodle.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/beepdown.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/color.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/color2.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/hello.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/hello2.hrb ${root_BINARY_DIR}/projects/26_day/harib23d/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(26_day_harib23d_APIO ${root_BINARY_DIR}/projects/26_day/harib23d/a_nask.o)
set(26_day_harib23d_RANDO ${root_BINARY_DIR}/projects/26_day/harib23d/rand.o)
add_custom_command(
  OUTPUT ${26_day_harib23d_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${26_day_harib23d_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${26_day_harib23d_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23d/a_nask.nas ${26_day_harib23d_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/a_nask.nas
  COMMENT "Generating a_nask.o for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/a.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/a.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/a.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating a.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/hello3.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating hello3.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/hello4.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating hello4.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/winhelo.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating winhelo.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/winhelo2.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating winhelo2.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/winhelo3.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating winhelo3.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/star1.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/star1.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/star1.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating star1.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/stars.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/stars.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/stars.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating stars.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/stars2.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating stars2.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/lines.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/lines.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/lines.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating lines.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/walk.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/walk.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/walk.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating walk.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/color2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/color2.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/color2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating color2.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/color.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/color.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/color.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating color.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/beepdown.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating beepdown.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23d_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23d/noodle.c ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23d/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23d_APIO} ${26_day_harib23d_LIBGC} ${26_day_harib23d_RANDO}
  COMMENT "Generating noodle.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23d/hello.nas ${root_BINARY_DIR}/projects/26_day/harib23d/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/hello.nas
  COMMENT "Generating hello.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23d/hello2.nas ${root_BINARY_DIR}/projects/26_day/harib23d/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/hello2.nas
  COMMENT "Generating hello2.hrb for 26_day_harib23d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23d/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23d/hello5.nas ${root_BINARY_DIR}/projects/26_day/harib23d/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23d/hello5.nas
  COMMENT "Generating hello5.hrb for 26_day_harib23d"
)
