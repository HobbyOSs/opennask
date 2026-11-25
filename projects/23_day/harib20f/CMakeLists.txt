#----------------------------------------------------------
message(STATUS "Entering directory projects/23_day/harib20f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(23_day_harib20f_OS    ${root_BINARY_DIR}/projects/23_day/harib20f/os.img)
set(23_day_harib20f_SYS	  ${root_BINARY_DIR}/projects/23_day/harib20f/os.sys)
set(23_day_harib20f_IPLB  ${root_BINARY_DIR}/projects/23_day/harib20f/ipl.bin)
set(23_day_harib20f_IPLS  ${root_SOURCE_DIR}/projects/23_day/harib20f/ipl10.nas)
set(23_day_harib20f_HEADB ${root_BINARY_DIR}/projects/23_day/harib20f/asmhead.bin)
set(23_day_harib20f_HEADS ${root_SOURCE_DIR}/projects/23_day/harib20f/asmhead.nas)
set(23_day_harib20f_BOOTB ${root_BINARY_DIR}/projects/23_day/harib20f/boot.bin)
set(23_day_harib20f_CCO	  ${root_BINARY_DIR}/projects/23_day/harib20f/boot.o)
file(GLOB 23_day_harib20f_CCS ${root_SOURCE_DIR}/projects/23_day/harib20f/*.c)
list(REMOVE_ITEM 23_day_harib20f_CCS
  ${root_SOURCE_DIR}/projects/23_day/harib20f/a.c
  ${root_SOURCE_DIR}/projects/23_day/harib20f/hello3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20f/hello4.c
  ${root_SOURCE_DIR}/projects/23_day/harib20f/winhelo.c
  ${root_SOURCE_DIR}/projects/23_day/harib20f/winhelo2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20f/winhelo3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20f/star1.c
  ${root_SOURCE_DIR}/projects/23_day/harib20f/stars.c
  ${root_SOURCE_DIR}/projects/23_day/harib20f/stars2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20f/lines.c)
set(23_day_harib20f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(23_day_harib20f_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(23_day_harib20f_FUNCS ${root_SOURCE_DIR}/projects/23_day/harib20f/naskfunc.nas)
set(23_day_harib20f_FUNCO ${root_BINARY_DIR}/projects/23_day/harib20f/naskfunc.o)
set(23_day_harib20f_FONTS ${root_SOURCE_DIR}/projects/23_day/harib20f/hankaku.txt)
set(23_day_harib20f_FONTB ${root_BINARY_DIR}/projects/23_day/harib20f/hankaku.bin)
set(23_day_harib20f_FONTO ${root_BINARY_DIR}/projects/23_day/harib20f/hankaku.o)
set(23_day_harib20f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(23_day_harib20f_LIBGC ${root_BINARY_DIR}/projects/23_day/harib20f/libgo.o)
set(23_day_harib20f_WILDOBJ ${root_BINARY_DIR}/projects/23_day/harib20f/*.o)


add_custom_target(23_day_harib20f_run
  COMMAND ${QEMU} ${QEMUOPT} ${23_day_harib20f_OS}
  DEPENDS 23_day_harib20f_img
)
add_custom_target(23_day_harib20f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${23_day_harib20f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 23_day_harib20f_img
)
add_custom_target(23_day_harib20f_clean
  COMMAND rm -f ${23_day_harib20f_OS}
  COMMAND rm -f ${23_day_harib20f_SYS}
  COMMAND rm -f ${23_day_harib20f_IPLB}
  COMMAND rm -f ${23_day_harib20f_HEADB}
  COMMAND rm -f ${23_day_harib20f_BOOTB}
  COMMAND rm -f ${23_day_harib20f_WILDOBJ}
)
add_custom_target(23_day_harib20f_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20f_IPLS} ${23_day_harib20f_IPLB}
  DEPENDS ${23_day_harib20f_IPLS}
)
add_custom_command(
  OUTPUT ${23_day_harib20f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20f_HEADS} ${23_day_harib20f_HEADB}
  DEPENDS ${23_day_harib20f_HEADS}
  COMMENT "Generating asmhead.bin for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${23_day_harib20f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20f_FUNCS} ${23_day_harib20f_FUNCO}
  DEPENDS ${23_day_harib20f_FUNCS}
  COMMENT "Generating naskfunc.o for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${23_day_harib20f_FONTO}
  COMMAND ${FONT} ${23_day_harib20f_FONTS} ${23_day_harib20f_FONTB}
  COMMAND ${B2O}  ${23_day_harib20f_FONTB} ${23_day_harib20f_FONTO} _hankaku
  DEPENDS ${23_day_harib20f_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${23_day_harib20f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${23_day_harib20f_LIBGE} ${23_day_harib20f_LIBGC}
  DEPENDS ${23_day_harib20f_LIBGE}
  COMMENT "Converting golibc for 23_day_harib20f"
)

add_custom_target(23_day_harib20f_sys
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_LDS} ${23_day_harib20f_CCS} ${23_day_harib20f_WILDOBJ} -o ${23_day_harib20f_BOOTB}
  COMMAND cat ${23_day_harib20f_HEADB} ${23_day_harib20f_BOOTB} > ${23_day_harib20f_SYS}
  DEPENDS ${23_day_harib20f_HEADB} 23_day_harib20f_ipl
  DEPENDS ${23_day_harib20f_FUNCO}
  DEPENDS ${23_day_harib20f_FONTO} ${23_day_harib20f_LIBGC}
)
add_custom_target(23_day_harib20f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${23_day_harib20f_IPLB} -i ${23_day_harib20f_OS}
  COMMAND mcopy -i ${23_day_harib20f_OS} ${23_day_harib20f_SYS} ::
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/a.hrb ::a.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${23_day_harib20f_OS} ${root_BINARY_DIR}/projects/23_day/harib20f/hello5.hrb ::hello5.hrb
  DEPENDS 23_day_harib20f_sys ${root_BINARY_DIR}/projects/23_day/harib20f/a.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/hello3.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/hello4.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo2.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo3.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/star1.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/stars.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/stars2.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/lines.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/hello.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/hello2.hrb ${root_BINARY_DIR}/projects/23_day/harib20f/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(23_day_harib20f_APIO ${root_BINARY_DIR}/projects/23_day/harib20f/a_nask.o)
set(23_day_harib20f_RANDO ${root_BINARY_DIR}/projects/23_day/harib20f/rand.o)
add_custom_command(
  OUTPUT ${23_day_harib20f_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${23_day_harib20f_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${23_day_harib20f_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20f/a_nask.nas ${23_day_harib20f_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/a_nask.nas
  COMMENT "Generating a_nask.o for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/a.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20f/a.c ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20f/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/a.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO}
  COMMENT "Generating a.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20f/hello3.c ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20f/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO}
  COMMENT "Generating hello3.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20f/hello4.c ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20f/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO}
  COMMENT "Generating hello4.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20f/winhelo.c ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO}
  COMMENT "Generating winhelo.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20f/winhelo2.c ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO}
  COMMENT "Generating winhelo2.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20f/winhelo3.c ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20f/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO}
  COMMENT "Generating winhelo3.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/star1.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20f/star1.c ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20f/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/star1.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO}
  COMMENT "Generating star1.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/stars.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20f/stars.c ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20f/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/stars.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO}
  COMMENT "Generating stars.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20f/stars2.c ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20f/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO}
  COMMENT "Generating stars2.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/lines.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20f_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20f/lines.c ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20f/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/lines.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20f_APIO} ${23_day_harib20f_LIBGC} ${23_day_harib20f_RANDO}
  COMMENT "Generating lines.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20f/hello.nas ${root_BINARY_DIR}/projects/23_day/harib20f/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/hello.nas
  COMMENT "Generating hello.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20f/hello2.nas ${root_BINARY_DIR}/projects/23_day/harib20f/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/hello2.nas
  COMMENT "Generating hello2.hrb for 23_day_harib20f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20f/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20f/hello5.nas ${root_BINARY_DIR}/projects/23_day/harib20f/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20f/hello5.nas
  COMMENT "Generating hello5.hrb for 23_day_harib20f"
)
