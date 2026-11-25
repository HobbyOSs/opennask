#----------------------------------------------------------
message(STATUS "Entering directory projects/26_day/harib23e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(26_day_harib23e_OS    ${root_BINARY_DIR}/projects/26_day/harib23e/os.img)
set(26_day_harib23e_SYS	  ${root_BINARY_DIR}/projects/26_day/harib23e/os.sys)
set(26_day_harib23e_IPLB  ${root_BINARY_DIR}/projects/26_day/harib23e/ipl.bin)
set(26_day_harib23e_IPLS  ${root_SOURCE_DIR}/projects/26_day/harib23e/ipl10.nas)
set(26_day_harib23e_HEADB ${root_BINARY_DIR}/projects/26_day/harib23e/asmhead.bin)
set(26_day_harib23e_HEADS ${root_SOURCE_DIR}/projects/26_day/harib23e/asmhead.nas)
set(26_day_harib23e_BOOTB ${root_BINARY_DIR}/projects/26_day/harib23e/boot.bin)
set(26_day_harib23e_CCO	  ${root_BINARY_DIR}/projects/26_day/harib23e/boot.o)
file(GLOB 26_day_harib23e_CCS ${root_SOURCE_DIR}/projects/26_day/harib23e/*.c)
list(REMOVE_ITEM 26_day_harib23e_CCS
  ${root_SOURCE_DIR}/projects/26_day/harib23e/a.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/hello3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/hello4.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/winhelo.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/winhelo2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/winhelo3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/star1.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/stars.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/stars2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/lines.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/walk.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/noodle.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/beepdown.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/color.c
  ${root_SOURCE_DIR}/projects/26_day/harib23e/color2.c)
set(26_day_harib23e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(26_day_harib23e_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(26_day_harib23e_FUNCS ${root_SOURCE_DIR}/projects/26_day/harib23e/naskfunc.nas)
set(26_day_harib23e_FUNCO ${root_BINARY_DIR}/projects/26_day/harib23e/naskfunc.o)
set(26_day_harib23e_FONTS ${root_SOURCE_DIR}/projects/26_day/harib23e/hankaku.txt)
set(26_day_harib23e_FONTB ${root_BINARY_DIR}/projects/26_day/harib23e/hankaku.bin)
set(26_day_harib23e_FONTO ${root_BINARY_DIR}/projects/26_day/harib23e/hankaku.o)
set(26_day_harib23e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(26_day_harib23e_LIBGC ${root_BINARY_DIR}/projects/26_day/harib23e/libgo.o)
set(26_day_harib23e_WILDOBJ ${root_BINARY_DIR}/projects/26_day/harib23e/*.o)


add_custom_target(26_day_harib23e_run
  COMMAND ${QEMU} ${QEMUOPT} ${26_day_harib23e_OS}
  DEPENDS 26_day_harib23e_img
)
add_custom_target(26_day_harib23e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${26_day_harib23e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 26_day_harib23e_img
)
add_custom_target(26_day_harib23e_clean
  COMMAND rm -f ${26_day_harib23e_OS}
  COMMAND rm -f ${26_day_harib23e_SYS}
  COMMAND rm -f ${26_day_harib23e_IPLB}
  COMMAND rm -f ${26_day_harib23e_HEADB}
  COMMAND rm -f ${26_day_harib23e_BOOTB}
  COMMAND rm -f ${26_day_harib23e_WILDOBJ}
)
add_custom_target(26_day_harib23e_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23e_IPLS} ${26_day_harib23e_IPLB}
  DEPENDS ${26_day_harib23e_IPLS}
)
add_custom_command(
  OUTPUT ${26_day_harib23e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23e_HEADS} ${26_day_harib23e_HEADB}
  DEPENDS ${26_day_harib23e_HEADS}
  COMMENT "Generating asmhead.bin for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${26_day_harib23e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23e_FUNCS} ${26_day_harib23e_FUNCO}
  DEPENDS ${26_day_harib23e_FUNCS}
  COMMENT "Generating naskfunc.o for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${26_day_harib23e_FONTO}
  COMMAND ${FONT} ${26_day_harib23e_FONTS} ${26_day_harib23e_FONTB}
  COMMAND ${B2O}  ${26_day_harib23e_FONTB} ${26_day_harib23e_FONTO} _hankaku
  DEPENDS ${26_day_harib23e_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${26_day_harib23e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${26_day_harib23e_LIBGE} ${26_day_harib23e_LIBGC}
  DEPENDS ${26_day_harib23e_LIBGE}
  COMMENT "Converting golibc for 26_day_harib23e"
)

add_custom_target(26_day_harib23e_sys
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_LDS} ${26_day_harib23e_CCS} ${26_day_harib23e_WILDOBJ} -o ${26_day_harib23e_BOOTB}
  COMMAND cat ${26_day_harib23e_HEADB} ${26_day_harib23e_BOOTB} > ${26_day_harib23e_SYS}
  DEPENDS ${26_day_harib23e_HEADB} 26_day_harib23e_ipl
  DEPENDS ${26_day_harib23e_FUNCO}
  DEPENDS ${26_day_harib23e_FONTO} ${26_day_harib23e_LIBGC}
)
add_custom_target(26_day_harib23e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${26_day_harib23e_IPLB} -i ${26_day_harib23e_OS}
  COMMAND mcopy -i ${26_day_harib23e_OS} ${26_day_harib23e_SYS} ::
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/a.hrb ::a.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/color.hrb ::color.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${26_day_harib23e_OS} ${root_BINARY_DIR}/projects/26_day/harib23e/hello5.hrb ::hello5.hrb
  DEPENDS 26_day_harib23e_sys ${root_BINARY_DIR}/projects/26_day/harib23e/a.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/hello3.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/hello4.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo2.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo3.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/star1.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/stars.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/stars2.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/lines.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/walk.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/noodle.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/beepdown.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/color.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/color2.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/hello.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/hello2.hrb ${root_BINARY_DIR}/projects/26_day/harib23e/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(26_day_harib23e_APIO ${root_BINARY_DIR}/projects/26_day/harib23e/a_nask.o)
set(26_day_harib23e_RANDO ${root_BINARY_DIR}/projects/26_day/harib23e/rand.o)
add_custom_command(
  OUTPUT ${26_day_harib23e_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${26_day_harib23e_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${26_day_harib23e_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23e/a_nask.nas ${26_day_harib23e_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/a_nask.nas
  COMMENT "Generating a_nask.o for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/a.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/a.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/a.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating a.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/hello3.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating hello3.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/hello4.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating hello4.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/winhelo.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating winhelo.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/winhelo2.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating winhelo2.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/winhelo3.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating winhelo3.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/star1.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/star1.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/star1.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating star1.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/stars.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/stars.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/stars.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating stars.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/stars2.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating stars2.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/lines.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/lines.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/lines.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating lines.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/walk.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/walk.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/walk.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating walk.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/color2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/color2.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/color2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating color2.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/color.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/color.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/color.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating color.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/beepdown.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating beepdown.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23e_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23e/noodle.c ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23e/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23e_APIO} ${26_day_harib23e_LIBGC} ${26_day_harib23e_RANDO}
  COMMENT "Generating noodle.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23e/hello.nas ${root_BINARY_DIR}/projects/26_day/harib23e/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/hello.nas
  COMMENT "Generating hello.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23e/hello2.nas ${root_BINARY_DIR}/projects/26_day/harib23e/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/hello2.nas
  COMMENT "Generating hello2.hrb for 26_day_harib23e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23e/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23e/hello5.nas ${root_BINARY_DIR}/projects/26_day/harib23e/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23e/hello5.nas
  COMMENT "Generating hello5.hrb for 26_day_harib23e"
)
