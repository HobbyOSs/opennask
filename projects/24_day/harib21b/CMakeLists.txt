#----------------------------------------------------------
message(STATUS "Entering directory projects/24_day/harib21b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(24_day_harib21b_OS    ${root_BINARY_DIR}/projects/24_day/harib21b/os.img)
set(24_day_harib21b_SYS	  ${root_BINARY_DIR}/projects/24_day/harib21b/os.sys)
set(24_day_harib21b_IPLB  ${root_BINARY_DIR}/projects/24_day/harib21b/ipl.bin)
set(24_day_harib21b_IPLS  ${root_SOURCE_DIR}/projects/24_day/harib21b/ipl10.nas)
set(24_day_harib21b_HEADB ${root_BINARY_DIR}/projects/24_day/harib21b/asmhead.bin)
set(24_day_harib21b_HEADS ${root_SOURCE_DIR}/projects/24_day/harib21b/asmhead.nas)
set(24_day_harib21b_BOOTB ${root_BINARY_DIR}/projects/24_day/harib21b/boot.bin)
set(24_day_harib21b_CCO	  ${root_BINARY_DIR}/projects/24_day/harib21b/boot.o)
file(GLOB 24_day_harib21b_CCS ${root_SOURCE_DIR}/projects/24_day/harib21b/*.c)
list(REMOVE_ITEM 24_day_harib21b_CCS
  ${root_SOURCE_DIR}/projects/24_day/harib21b/a.c
  ${root_SOURCE_DIR}/projects/24_day/harib21b/hello3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21b/hello4.c
  ${root_SOURCE_DIR}/projects/24_day/harib21b/winhelo.c
  ${root_SOURCE_DIR}/projects/24_day/harib21b/winhelo2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21b/winhelo3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21b/star1.c
  ${root_SOURCE_DIR}/projects/24_day/harib21b/stars.c
  ${root_SOURCE_DIR}/projects/24_day/harib21b/stars2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21b/lines.c
  ${root_SOURCE_DIR}/projects/24_day/harib21b/walk.c)
set(24_day_harib21b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(24_day_harib21b_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(24_day_harib21b_FUNCS ${root_SOURCE_DIR}/projects/24_day/harib21b/naskfunc.nas)
set(24_day_harib21b_FUNCO ${root_BINARY_DIR}/projects/24_day/harib21b/naskfunc.o)
set(24_day_harib21b_FONTS ${root_SOURCE_DIR}/projects/24_day/harib21b/hankaku.txt)
set(24_day_harib21b_FONTB ${root_BINARY_DIR}/projects/24_day/harib21b/hankaku.bin)
set(24_day_harib21b_FONTO ${root_BINARY_DIR}/projects/24_day/harib21b/hankaku.o)
set(24_day_harib21b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(24_day_harib21b_LIBGC ${root_BINARY_DIR}/projects/24_day/harib21b/libgo.o)
set(24_day_harib21b_WILDOBJ ${root_BINARY_DIR}/projects/24_day/harib21b/*.o)


add_custom_target(24_day_harib21b_run
  COMMAND ${QEMU} ${QEMUOPT} ${24_day_harib21b_OS}
  DEPENDS 24_day_harib21b_img
)
add_custom_target(24_day_harib21b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${24_day_harib21b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 24_day_harib21b_img
)
add_custom_target(24_day_harib21b_clean
  COMMAND rm -f ${24_day_harib21b_OS}
  COMMAND rm -f ${24_day_harib21b_SYS}
  COMMAND rm -f ${24_day_harib21b_IPLB}
  COMMAND rm -f ${24_day_harib21b_HEADB}
  COMMAND rm -f ${24_day_harib21b_BOOTB}
  COMMAND rm -f ${24_day_harib21b_WILDOBJ}
)
add_custom_target(24_day_harib21b_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21b_IPLS} ${24_day_harib21b_IPLB}
  DEPENDS ${24_day_harib21b_IPLS}
)
add_custom_command(
  OUTPUT ${24_day_harib21b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21b_HEADS} ${24_day_harib21b_HEADB}
  DEPENDS ${24_day_harib21b_HEADS}
  COMMENT "Generating asmhead.bin for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${24_day_harib21b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21b_FUNCS} ${24_day_harib21b_FUNCO}
  DEPENDS ${24_day_harib21b_FUNCS}
  COMMENT "Generating naskfunc.o for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${24_day_harib21b_FONTO}
  COMMAND ${FONT} ${24_day_harib21b_FONTS} ${24_day_harib21b_FONTB}
  COMMAND ${B2O}  ${24_day_harib21b_FONTB} ${24_day_harib21b_FONTO} _hankaku
  DEPENDS ${24_day_harib21b_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${24_day_harib21b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${24_day_harib21b_LIBGE} ${24_day_harib21b_LIBGC}
  DEPENDS ${24_day_harib21b_LIBGE}
  COMMENT "Converting golibc for 24_day_harib21b"
)

add_custom_target(24_day_harib21b_sys
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_LDS} ${24_day_harib21b_CCS} ${24_day_harib21b_WILDOBJ} -o ${24_day_harib21b_BOOTB}
  COMMAND cat ${24_day_harib21b_HEADB} ${24_day_harib21b_BOOTB} > ${24_day_harib21b_SYS}
  DEPENDS ${24_day_harib21b_HEADB} 24_day_harib21b_ipl
  DEPENDS ${24_day_harib21b_FUNCO}
  DEPENDS ${24_day_harib21b_FONTO} ${24_day_harib21b_LIBGC}
)
add_custom_target(24_day_harib21b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${24_day_harib21b_IPLB} -i ${24_day_harib21b_OS}
  COMMAND mcopy -i ${24_day_harib21b_OS} ${24_day_harib21b_SYS} ::
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/a.hrb ::a.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${24_day_harib21b_OS} ${root_BINARY_DIR}/projects/24_day/harib21b/hello5.hrb ::hello5.hrb
  DEPENDS 24_day_harib21b_sys ${root_BINARY_DIR}/projects/24_day/harib21b/a.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/hello3.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/hello4.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo2.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo3.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/star1.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/stars.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/stars2.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/lines.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/walk.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/hello.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/hello2.hrb ${root_BINARY_DIR}/projects/24_day/harib21b/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(24_day_harib21b_APIO ${root_BINARY_DIR}/projects/24_day/harib21b/a_nask.o)
set(24_day_harib21b_RANDO ${root_BINARY_DIR}/projects/24_day/harib21b/rand.o)
add_custom_command(
  OUTPUT ${24_day_harib21b_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${24_day_harib21b_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${24_day_harib21b_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21b/a_nask.nas ${24_day_harib21b_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/a_nask.nas
  COMMENT "Generating a_nask.o for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/a.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/a.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/a.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating a.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/hello3.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating hello3.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/hello4.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating hello4.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/winhelo.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating winhelo.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/winhelo2.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating winhelo2.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/winhelo3.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating winhelo3.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/star1.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/star1.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/star1.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating star1.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/stars.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/stars.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/stars.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating stars.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/stars2.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating stars2.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/lines.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/lines.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/lines.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating lines.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/walk.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21b_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21b/walk.c ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21b/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/walk.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21b_APIO} ${24_day_harib21b_LIBGC} ${24_day_harib21b_RANDO}
  COMMENT "Generating walk.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21b/hello.nas ${root_BINARY_DIR}/projects/24_day/harib21b/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/hello.nas
  COMMENT "Generating hello.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21b/hello2.nas ${root_BINARY_DIR}/projects/24_day/harib21b/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/hello2.nas
  COMMENT "Generating hello2.hrb for 24_day_harib21b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21b/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21b/hello5.nas ${root_BINARY_DIR}/projects/24_day/harib21b/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21b/hello5.nas
  COMMENT "Generating hello5.hrb for 24_day_harib21b"
)
