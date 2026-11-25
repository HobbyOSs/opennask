#----------------------------------------------------------
message(STATUS "Entering directory projects/27_day/harib24f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(27_day_harib24f_OS    ${root_BINARY_DIR}/projects/27_day/harib24f/os.img)
set(27_day_harib24f_SYS	  ${root_BINARY_DIR}/projects/27_day/harib24f/os.sys)
set(27_day_harib24f_IPLB  ${root_BINARY_DIR}/projects/27_day/harib24f/ipl.bin)
set(27_day_harib24f_IPLS  ${root_SOURCE_DIR}/projects/27_day/harib24f/ipl10.nas)
set(27_day_harib24f_HEADB ${root_BINARY_DIR}/projects/27_day/harib24f/asmhead.bin)
set(27_day_harib24f_HEADS ${root_SOURCE_DIR}/projects/27_day/harib24f/asmhead.nas)
set(27_day_harib24f_BOOTB ${root_BINARY_DIR}/projects/27_day/harib24f/boot.bin)
set(27_day_harib24f_CCO	  ${root_BINARY_DIR}/projects/27_day/harib24f/boot.o)
file(GLOB 27_day_harib24f_CCS ${root_SOURCE_DIR}/projects/27_day/harib24f/*.c)
list(REMOVE_ITEM 27_day_harib24f_CCS
  ${root_SOURCE_DIR}/projects/27_day/harib24f/a.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/hello3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/hello4.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/winhelo.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/winhelo2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/winhelo3.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/star1.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/stars.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/stars2.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/lines.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/walk.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/noodle.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/beepdown.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/color.c
  ${root_SOURCE_DIR}/projects/27_day/harib24f/color2.c)
set(27_day_harib24f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(27_day_harib24f_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(27_day_harib24f_FUNCS ${root_SOURCE_DIR}/projects/27_day/harib24f/naskfunc.nas)
set(27_day_harib24f_FUNCO ${root_BINARY_DIR}/projects/27_day/harib24f/naskfunc.o)
set(27_day_harib24f_FONTS ${root_SOURCE_DIR}/projects/27_day/harib24f/hankaku.txt)
set(27_day_harib24f_FONTB ${root_BINARY_DIR}/projects/27_day/harib24f/hankaku.bin)
set(27_day_harib24f_FONTO ${root_BINARY_DIR}/projects/27_day/harib24f/hankaku.o)
set(27_day_harib24f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(27_day_harib24f_LIBGC ${root_BINARY_DIR}/projects/27_day/harib24f/libgo.o)
set(27_day_harib24f_WILDOBJ ${root_BINARY_DIR}/projects/27_day/harib24f/*.o)


add_custom_target(27_day_harib24f_run
  COMMAND ${QEMU} ${QEMUOPT} ${27_day_harib24f_OS}
  DEPENDS 27_day_harib24f_img
)
add_custom_target(27_day_harib24f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${27_day_harib24f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 27_day_harib24f_img
)
add_custom_target(27_day_harib24f_clean
  COMMAND rm -f ${27_day_harib24f_OS}
  COMMAND rm -f ${27_day_harib24f_SYS}
  COMMAND rm -f ${27_day_harib24f_IPLB}
  COMMAND rm -f ${27_day_harib24f_HEADB}
  COMMAND rm -f ${27_day_harib24f_BOOTB}
  COMMAND rm -f ${27_day_harib24f_WILDOBJ}
)
add_custom_target(27_day_harib24f_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24f_IPLS} ${27_day_harib24f_IPLB}
  DEPENDS ${27_day_harib24f_IPLS}
)
add_custom_command(
  OUTPUT ${27_day_harib24f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24f_HEADS} ${27_day_harib24f_HEADB}
  DEPENDS ${27_day_harib24f_HEADS}
  COMMENT "Generating asmhead.bin for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${27_day_harib24f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${27_day_harib24f_FUNCS} ${27_day_harib24f_FUNCO}
  DEPENDS ${27_day_harib24f_FUNCS}
  COMMENT "Generating naskfunc.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${27_day_harib24f_FONTO}
  COMMAND ${FONT} ${27_day_harib24f_FONTS} ${27_day_harib24f_FONTB}
  COMMAND ${B2O}  ${27_day_harib24f_FONTB} ${27_day_harib24f_FONTO} _hankaku
  DEPENDS ${27_day_harib24f_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${27_day_harib24f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${27_day_harib24f_LIBGE} ${27_day_harib24f_LIBGC}
  DEPENDS ${27_day_harib24f_LIBGE}
  COMMENT "Converting golibc for 27_day_harib24f"
)

add_custom_target(27_day_harib24f_sys
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_LDS} ${27_day_harib24f_CCS} ${27_day_harib24f_WILDOBJ} -o ${27_day_harib24f_BOOTB}
  COMMAND cat ${27_day_harib24f_HEADB} ${27_day_harib24f_BOOTB} > ${27_day_harib24f_SYS}
  DEPENDS ${27_day_harib24f_HEADB} 27_day_harib24f_ipl
  DEPENDS ${27_day_harib24f_FUNCO}
  DEPENDS ${27_day_harib24f_FONTO} ${27_day_harib24f_LIBGC}
)
add_custom_target(27_day_harib24f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${27_day_harib24f_IPLB} -i ${27_day_harib24f_OS}
  COMMAND mcopy -i ${27_day_harib24f_OS} ${27_day_harib24f_SYS} ::
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/a.hrb ::a.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/color.hrb ::color.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${27_day_harib24f_OS} ${root_BINARY_DIR}/projects/27_day/harib24f/hello5.hrb ::hello5.hrb
  DEPENDS 27_day_harib24f_sys ${root_BINARY_DIR}/projects/27_day/harib24f/a.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/hello3.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/hello4.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo2.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo3.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/star1.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/stars.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/stars2.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/lines.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/walk.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/noodle.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/beepdown.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/color.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/color2.hrb ${root_BINARY_DIR}/projects/27_day/harib24f/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(27_day_harib24f_APIO
  ${root_BINARY_DIR}/projects/27_day/harib24f/api001.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api002.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api003.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api004.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api005.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api006.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api007.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api008.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api009.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api010.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api011.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api012.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api013.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api014.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api015.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api016.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api017.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api018.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api019.o
  ${root_BINARY_DIR}/projects/27_day/harib24f/api020.o)
set(27_day_harib24f_RANDO ${root_BINARY_DIR}/projects/27_day/harib24f/rand.o)
add_custom_command(
  OUTPUT ${27_day_harib24f_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${27_day_harib24f_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api001.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api001.nas
  COMMENT "Generating api001.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api002.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api002.nas
  COMMENT "Generating api002.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api003.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api003.nas
  COMMENT "Generating api003.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api004.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api004.nas
  COMMENT "Generating api004.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api005.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api005.nas
  COMMENT "Generating api005.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api006.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api006.nas
  COMMENT "Generating api006.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api007.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api007.nas
  COMMENT "Generating api007.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api008.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api008.nas
  COMMENT "Generating api008.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api009.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api009.nas
  COMMENT "Generating api009.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api010.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api010.nas
  COMMENT "Generating api010.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api011.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api011.nas
  COMMENT "Generating api011.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api012.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api012.nas
  COMMENT "Generating api012.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api013.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api013.nas
  COMMENT "Generating api013.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api014.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api014.nas
  COMMENT "Generating api014.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api015.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api015.nas
  COMMENT "Generating api015.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api016.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api016.nas
  COMMENT "Generating api016.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api017.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api017.nas
  COMMENT "Generating api017.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api018.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api018.nas
  COMMENT "Generating api018.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api019.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api019.nas
  COMMENT "Generating api019.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/api020.nas ${root_BINARY_DIR}/projects/27_day/harib24f/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/api020.nas
  COMMENT "Generating api020.o for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/a.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/a.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/a.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating a.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/hello3.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating hello3.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/hello4.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating hello4.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/winhelo.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating winhelo.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/winhelo2.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating winhelo2.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/winhelo3.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating winhelo3.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/star1.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/star1.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/star1.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating star1.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/stars.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/stars.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/stars.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating stars.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/stars2.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating stars2.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/lines.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/lines.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/lines.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating lines.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/walk.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/walk.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/walk.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating walk.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/color2.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/color2.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/color2.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating color2.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/color.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/color.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/color.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating color.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/beepdown.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating beepdown.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${27_day_harib24f_APPLDS} ${root_SOURCE_DIR}/projects/27_day/harib24f/noodle.c ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO} -o ${root_BINARY_DIR}/projects/27_day/harib24f/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${27_day_harib24f_APIO} ${27_day_harib24f_LIBGC} ${27_day_harib24f_RANDO}
  COMMENT "Generating noodle.hrb for 27_day_harib24f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/27_day/harib24f/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/27_day/harib24f/hello5.nas ${root_BINARY_DIR}/projects/27_day/harib24f/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/27_day/harib24f/hello5.nas
  COMMENT "Generating hello5.hrb for 27_day_harib24f"
)
