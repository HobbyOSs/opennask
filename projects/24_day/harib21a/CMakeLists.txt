#----------------------------------------------------------
message(STATUS "Entering directory projects/24_day/harib21a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(24_day_harib21a_OS    ${root_BINARY_DIR}/projects/24_day/harib21a/os.img)
set(24_day_harib21a_SYS	  ${root_BINARY_DIR}/projects/24_day/harib21a/os.sys)
set(24_day_harib21a_IPLB  ${root_BINARY_DIR}/projects/24_day/harib21a/ipl.bin)
set(24_day_harib21a_IPLS  ${root_SOURCE_DIR}/projects/24_day/harib21a/ipl10.nas)
set(24_day_harib21a_HEADB ${root_BINARY_DIR}/projects/24_day/harib21a/asmhead.bin)
set(24_day_harib21a_HEADS ${root_SOURCE_DIR}/projects/24_day/harib21a/asmhead.nas)
set(24_day_harib21a_BOOTB ${root_BINARY_DIR}/projects/24_day/harib21a/boot.bin)
set(24_day_harib21a_CCO	  ${root_BINARY_DIR}/projects/24_day/harib21a/boot.o)
file(GLOB 24_day_harib21a_CCS ${root_SOURCE_DIR}/projects/24_day/harib21a/*.c)
list(REMOVE_ITEM 24_day_harib21a_CCS
  ${root_SOURCE_DIR}/projects/24_day/harib21a/a.c
  ${root_SOURCE_DIR}/projects/24_day/harib21a/hello3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21a/hello4.c
  ${root_SOURCE_DIR}/projects/24_day/harib21a/winhelo.c
  ${root_SOURCE_DIR}/projects/24_day/harib21a/winhelo2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21a/winhelo3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21a/star1.c
  ${root_SOURCE_DIR}/projects/24_day/harib21a/stars.c
  ${root_SOURCE_DIR}/projects/24_day/harib21a/stars2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21a/lines.c
  ${root_SOURCE_DIR}/projects/24_day/harib21a/walk.c)
set(24_day_harib21a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(24_day_harib21a_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(24_day_harib21a_FUNCS ${root_SOURCE_DIR}/projects/24_day/harib21a/naskfunc.nas)
set(24_day_harib21a_FUNCO ${root_BINARY_DIR}/projects/24_day/harib21a/naskfunc.o)
set(24_day_harib21a_FONTS ${root_SOURCE_DIR}/projects/24_day/harib21a/hankaku.txt)
set(24_day_harib21a_FONTB ${root_BINARY_DIR}/projects/24_day/harib21a/hankaku.bin)
set(24_day_harib21a_FONTO ${root_BINARY_DIR}/projects/24_day/harib21a/hankaku.o)
set(24_day_harib21a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(24_day_harib21a_LIBGC ${root_BINARY_DIR}/projects/24_day/harib21a/libgo.o)
set(24_day_harib21a_WILDOBJ ${root_BINARY_DIR}/projects/24_day/harib21a/*.o)


add_custom_target(24_day_harib21a_run
  COMMAND ${QEMU} ${QEMUOPT} ${24_day_harib21a_OS}
  DEPENDS 24_day_harib21a_img
)
add_custom_target(24_day_harib21a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${24_day_harib21a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 24_day_harib21a_img
)
add_custom_target(24_day_harib21a_clean
  COMMAND rm -f ${24_day_harib21a_OS}
  COMMAND rm -f ${24_day_harib21a_SYS}
  COMMAND rm -f ${24_day_harib21a_IPLB}
  COMMAND rm -f ${24_day_harib21a_HEADB}
  COMMAND rm -f ${24_day_harib21a_BOOTB}
  COMMAND rm -f ${24_day_harib21a_WILDOBJ}
)
add_custom_target(24_day_harib21a_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21a_IPLS} ${24_day_harib21a_IPLB}
  DEPENDS ${24_day_harib21a_IPLS}
)
add_custom_command(
  OUTPUT ${24_day_harib21a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21a_HEADS} ${24_day_harib21a_HEADB}
  DEPENDS ${24_day_harib21a_HEADS}
  COMMENT "Generating asmhead.bin for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${24_day_harib21a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21a_FUNCS} ${24_day_harib21a_FUNCO}
  DEPENDS ${24_day_harib21a_FUNCS}
  COMMENT "Generating naskfunc.o for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${24_day_harib21a_FONTO}
  COMMAND ${FONT} ${24_day_harib21a_FONTS} ${24_day_harib21a_FONTB}
  COMMAND ${B2O}  ${24_day_harib21a_FONTB} ${24_day_harib21a_FONTO} _hankaku
  DEPENDS ${24_day_harib21a_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${24_day_harib21a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${24_day_harib21a_LIBGE} ${24_day_harib21a_LIBGC}
  DEPENDS ${24_day_harib21a_LIBGE}
  COMMENT "Converting golibc for 24_day_harib21a"
)

add_custom_target(24_day_harib21a_sys
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_LDS} ${24_day_harib21a_CCS} ${24_day_harib21a_WILDOBJ} -o ${24_day_harib21a_BOOTB}
  COMMAND cat ${24_day_harib21a_HEADB} ${24_day_harib21a_BOOTB} > ${24_day_harib21a_SYS}
  DEPENDS ${24_day_harib21a_HEADB} 24_day_harib21a_ipl
  DEPENDS ${24_day_harib21a_FUNCO}
  DEPENDS ${24_day_harib21a_FONTO} ${24_day_harib21a_LIBGC}
)
add_custom_target(24_day_harib21a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${24_day_harib21a_IPLB} -i ${24_day_harib21a_OS}
  COMMAND mcopy -i ${24_day_harib21a_OS} ${24_day_harib21a_SYS} ::
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/a.hrb ::a.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${24_day_harib21a_OS} ${root_BINARY_DIR}/projects/24_day/harib21a/hello5.hrb ::hello5.hrb
  DEPENDS 24_day_harib21a_sys ${root_BINARY_DIR}/projects/24_day/harib21a/a.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/hello3.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/hello4.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo2.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo3.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/star1.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/stars.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/stars2.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/lines.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/walk.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/hello.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/hello2.hrb ${root_BINARY_DIR}/projects/24_day/harib21a/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(24_day_harib21a_APIO ${root_BINARY_DIR}/projects/24_day/harib21a/a_nask.o)
set(24_day_harib21a_RANDO ${root_BINARY_DIR}/projects/24_day/harib21a/rand.o)
add_custom_command(
  OUTPUT ${24_day_harib21a_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${24_day_harib21a_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${24_day_harib21a_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21a/a_nask.nas ${24_day_harib21a_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/a_nask.nas
  COMMENT "Generating a_nask.o for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/a.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/a.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/a.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating a.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/hello3.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating hello3.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/hello4.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating hello4.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/winhelo.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating winhelo.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/winhelo2.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating winhelo2.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/winhelo3.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating winhelo3.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/star1.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/star1.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/star1.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating star1.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/stars.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/stars.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/stars.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating stars.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/stars2.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating stars2.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/lines.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/lines.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/lines.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating lines.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/walk.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21a_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21a/walk.c ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21a/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/walk.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21a_APIO} ${24_day_harib21a_LIBGC} ${24_day_harib21a_RANDO}
  COMMENT "Generating walk.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21a/hello.nas ${root_BINARY_DIR}/projects/24_day/harib21a/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/hello.nas
  COMMENT "Generating hello.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21a/hello2.nas ${root_BINARY_DIR}/projects/24_day/harib21a/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/hello2.nas
  COMMENT "Generating hello2.hrb for 24_day_harib21a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21a/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21a/hello5.nas ${root_BINARY_DIR}/projects/24_day/harib21a/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21a/hello5.nas
  COMMENT "Generating hello5.hrb for 24_day_harib21a"
)
