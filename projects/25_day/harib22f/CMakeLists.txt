#----------------------------------------------------------
message(STATUS "Entering directory projects/25_day/harib22f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(25_day_harib22f_OS    ${root_BINARY_DIR}/projects/25_day/harib22f/os.img)
set(25_day_harib22f_SYS	  ${root_BINARY_DIR}/projects/25_day/harib22f/os.sys)
set(25_day_harib22f_IPLB  ${root_BINARY_DIR}/projects/25_day/harib22f/ipl.bin)
set(25_day_harib22f_IPLS  ${root_SOURCE_DIR}/projects/25_day/harib22f/ipl10.nas)
set(25_day_harib22f_HEADB ${root_BINARY_DIR}/projects/25_day/harib22f/asmhead.bin)
set(25_day_harib22f_HEADS ${root_SOURCE_DIR}/projects/25_day/harib22f/asmhead.nas)
set(25_day_harib22f_BOOTB ${root_BINARY_DIR}/projects/25_day/harib22f/boot.bin)
set(25_day_harib22f_CCO	  ${root_BINARY_DIR}/projects/25_day/harib22f/boot.o)
file(GLOB 25_day_harib22f_CCS ${root_SOURCE_DIR}/projects/25_day/harib22f/*.c)
list(REMOVE_ITEM 25_day_harib22f_CCS
  ${root_SOURCE_DIR}/projects/25_day/harib22f/a.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/hello3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/hello4.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/winhelo.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/winhelo2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/winhelo3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/star1.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/stars.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/stars2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/lines.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/walk.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/noodle.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/beepdown.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/color.c
  ${root_SOURCE_DIR}/projects/25_day/harib22f/color2.c)
set(25_day_harib22f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(25_day_harib22f_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(25_day_harib22f_FUNCS ${root_SOURCE_DIR}/projects/25_day/harib22f/naskfunc.nas)
set(25_day_harib22f_FUNCO ${root_BINARY_DIR}/projects/25_day/harib22f/naskfunc.o)
set(25_day_harib22f_FONTS ${root_SOURCE_DIR}/projects/25_day/harib22f/hankaku.txt)
set(25_day_harib22f_FONTB ${root_BINARY_DIR}/projects/25_day/harib22f/hankaku.bin)
set(25_day_harib22f_FONTO ${root_BINARY_DIR}/projects/25_day/harib22f/hankaku.o)
set(25_day_harib22f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(25_day_harib22f_LIBGC ${root_BINARY_DIR}/projects/25_day/harib22f/libgo.o)
set(25_day_harib22f_WILDOBJ ${root_BINARY_DIR}/projects/25_day/harib22f/*.o)


add_custom_target(25_day_harib22f_run
  COMMAND ${QEMU} ${QEMUOPT} ${25_day_harib22f_OS}
  DEPENDS 25_day_harib22f_img
)
add_custom_target(25_day_harib22f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${25_day_harib22f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 25_day_harib22f_img
)
add_custom_target(25_day_harib22f_clean
  COMMAND rm -f ${25_day_harib22f_OS}
  COMMAND rm -f ${25_day_harib22f_SYS}
  COMMAND rm -f ${25_day_harib22f_IPLB}
  COMMAND rm -f ${25_day_harib22f_HEADB}
  COMMAND rm -f ${25_day_harib22f_BOOTB}
  COMMAND rm -f ${25_day_harib22f_WILDOBJ}
)
add_custom_target(25_day_harib22f_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22f_IPLS} ${25_day_harib22f_IPLB}
  DEPENDS ${25_day_harib22f_IPLS}
)
add_custom_command(
  OUTPUT ${25_day_harib22f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22f_HEADS} ${25_day_harib22f_HEADB}
  DEPENDS ${25_day_harib22f_HEADS}
  COMMENT "Generating asmhead.bin for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${25_day_harib22f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22f_FUNCS} ${25_day_harib22f_FUNCO}
  DEPENDS ${25_day_harib22f_FUNCS}
  COMMENT "Generating naskfunc.o for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${25_day_harib22f_FONTO}
  COMMAND ${FONT} ${25_day_harib22f_FONTS} ${25_day_harib22f_FONTB}
  COMMAND ${B2O}  ${25_day_harib22f_FONTB} ${25_day_harib22f_FONTO} _hankaku
  DEPENDS ${25_day_harib22f_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${25_day_harib22f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${25_day_harib22f_LIBGE} ${25_day_harib22f_LIBGC}
  DEPENDS ${25_day_harib22f_LIBGE}
  COMMENT "Converting golibc for 25_day_harib22f"
)

add_custom_target(25_day_harib22f_sys
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_LDS} ${25_day_harib22f_CCS} ${25_day_harib22f_WILDOBJ} -o ${25_day_harib22f_BOOTB}
  COMMAND cat ${25_day_harib22f_HEADB} ${25_day_harib22f_BOOTB} > ${25_day_harib22f_SYS}
  DEPENDS ${25_day_harib22f_HEADB} 25_day_harib22f_ipl
  DEPENDS ${25_day_harib22f_FUNCO}
  DEPENDS ${25_day_harib22f_FONTO} ${25_day_harib22f_LIBGC}
)
add_custom_target(25_day_harib22f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${25_day_harib22f_IPLB} -i ${25_day_harib22f_OS}
  COMMAND mcopy -i ${25_day_harib22f_OS} ${25_day_harib22f_SYS} ::
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/a.hrb ::a.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/color.hrb ::color.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${25_day_harib22f_OS} ${root_BINARY_DIR}/projects/25_day/harib22f/hello5.hrb ::hello5.hrb
  DEPENDS 25_day_harib22f_sys ${root_BINARY_DIR}/projects/25_day/harib22f/a.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/hello3.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/hello4.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo2.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo3.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/star1.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/stars.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/stars2.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/lines.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/walk.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/noodle.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/beepdown.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/color.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/color2.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/hello.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/hello2.hrb ${root_BINARY_DIR}/projects/25_day/harib22f/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(25_day_harib22f_APIO ${root_BINARY_DIR}/projects/25_day/harib22f/a_nask.o)
set(25_day_harib22f_RANDO ${root_BINARY_DIR}/projects/25_day/harib22f/rand.o)
add_custom_command(
  OUTPUT ${25_day_harib22f_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${25_day_harib22f_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${25_day_harib22f_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22f/a_nask.nas ${25_day_harib22f_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/a_nask.nas
  COMMENT "Generating a_nask.o for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/a.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/a.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/a.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating a.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/hello3.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating hello3.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/hello4.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating hello4.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/winhelo.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating winhelo.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/winhelo2.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating winhelo2.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/winhelo3.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating winhelo3.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/star1.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/star1.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/star1.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating star1.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/stars.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/stars.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/stars.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating stars.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/stars2.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating stars2.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/lines.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/lines.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/lines.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating lines.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/walk.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/walk.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/walk.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating walk.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/color2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/color2.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/color2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating color2.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/color.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/color.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/color.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating color.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/beepdown.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating beepdown.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22f_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22f/noodle.c ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22f/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22f_APIO} ${25_day_harib22f_LIBGC} ${25_day_harib22f_RANDO}
  COMMENT "Generating noodle.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22f/hello.nas ${root_BINARY_DIR}/projects/25_day/harib22f/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/hello.nas
  COMMENT "Generating hello.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22f/hello2.nas ${root_BINARY_DIR}/projects/25_day/harib22f/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/hello2.nas
  COMMENT "Generating hello2.hrb for 25_day_harib22f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22f/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22f/hello5.nas ${root_BINARY_DIR}/projects/25_day/harib22f/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22f/hello5.nas
  COMMENT "Generating hello5.hrb for 25_day_harib22f"
)
