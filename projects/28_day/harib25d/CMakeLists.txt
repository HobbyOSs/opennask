#----------------------------------------------------------
message(STATUS "Entering directory projects/28_day/harib25d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(28_day_harib25d_OS    ${root_BINARY_DIR}/projects/28_day/harib25d/os.img)
set(28_day_harib25d_SYS	  ${root_BINARY_DIR}/projects/28_day/harib25d/os.sys)
set(28_day_harib25d_IPLB  ${root_BINARY_DIR}/projects/28_day/harib25d/ipl.bin)
set(28_day_harib25d_IPLS  ${root_SOURCE_DIR}/projects/28_day/harib25d/haribote/ipl10.nas)
set(28_day_harib25d_HEADB ${root_BINARY_DIR}/projects/28_day/harib25d/asmhead.bin)
set(28_day_harib25d_HEADS ${root_SOURCE_DIR}/projects/28_day/harib25d/haribote/asmhead.nas)
set(28_day_harib25d_BOOTB ${root_BINARY_DIR}/projects/28_day/harib25d/boot.bin)
set(28_day_harib25d_CCO	  ${root_BINARY_DIR}/projects/28_day/harib25d/boot.o)
file(GLOB 28_day_harib25d_CCS ${root_SOURCE_DIR}/projects/28_day/harib25d/haribote/*.c)
set(28_day_harib25d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(28_day_harib25d_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(28_day_harib25d_FUNCS ${root_SOURCE_DIR}/projects/28_day/harib25d/haribote/naskfunc.nas)
set(28_day_harib25d_FUNCO ${root_BINARY_DIR}/projects/28_day/harib25d/naskfunc.o)
set(28_day_harib25d_FONTS ${root_SOURCE_DIR}/projects/28_day/harib25d/haribote/hankaku.txt)
set(28_day_harib25d_FONTB ${root_BINARY_DIR}/projects/28_day/harib25d/hankaku.bin)
set(28_day_harib25d_FONTO ${root_BINARY_DIR}/projects/28_day/harib25d/hankaku.o)
set(28_day_harib25d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(28_day_harib25d_LIBGC ${root_BINARY_DIR}/projects/28_day/harib25d/libgo.o)
set(28_day_harib25d_WILDOBJ ${root_BINARY_DIR}/projects/28_day/harib25d/*.o)


add_custom_target(28_day_harib25d_run
  COMMAND ${QEMU} ${QEMUOPT} ${28_day_harib25d_OS}
  DEPENDS 28_day_harib25d_img
)
add_custom_target(28_day_harib25d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${28_day_harib25d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 28_day_harib25d_img
)
add_custom_target(28_day_harib25d_clean
  COMMAND rm -f ${28_day_harib25d_OS}
  COMMAND rm -f ${28_day_harib25d_SYS}
  COMMAND rm -f ${28_day_harib25d_IPLB}
  COMMAND rm -f ${28_day_harib25d_HEADB}
  COMMAND rm -f ${28_day_harib25d_BOOTB}
  COMMAND rm -f ${28_day_harib25d_WILDOBJ}
)
add_custom_target(28_day_harib25d_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25d_IPLS} ${28_day_harib25d_IPLB}
  DEPENDS ${28_day_harib25d_IPLS}
)
add_custom_command(
  OUTPUT ${28_day_harib25d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25d_HEADS} ${28_day_harib25d_HEADB}
  DEPENDS ${28_day_harib25d_HEADS}
  COMMENT "Generating asmhead.bin for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${28_day_harib25d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25d_FUNCS} ${28_day_harib25d_FUNCO}
  DEPENDS ${28_day_harib25d_FUNCS}
  COMMENT "Generating naskfunc.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${28_day_harib25d_FONTO}
  COMMAND ${FONT} ${28_day_harib25d_FONTS} ${28_day_harib25d_FONTB}
  COMMAND ${B2O}  ${28_day_harib25d_FONTB} ${28_day_harib25d_FONTO} _hankaku
  DEPENDS ${28_day_harib25d_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${28_day_harib25d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${28_day_harib25d_LIBGE} ${28_day_harib25d_LIBGC}
  DEPENDS ${28_day_harib25d_LIBGE}
  COMMENT "Converting golibc for 28_day_harib25d"
)

add_custom_target(28_day_harib25d_sys
  COMMAND gcc ${BINOPT} -T ${28_day_harib25d_LDS} ${28_day_harib25d_CCS} ${28_day_harib25d_WILDOBJ} -o ${28_day_harib25d_BOOTB}
  COMMAND cat ${28_day_harib25d_HEADB} ${28_day_harib25d_BOOTB} > ${28_day_harib25d_SYS}
  DEPENDS ${28_day_harib25d_HEADB} 28_day_harib25d_ipl
  DEPENDS ${28_day_harib25d_FUNCO}
  DEPENDS ${28_day_harib25d_FONTO} ${28_day_harib25d_LIBGC}
)
add_custom_target(28_day_harib25d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${28_day_harib25d_IPLB} -i ${28_day_harib25d_OS}
  COMMAND mcopy -i ${28_day_harib25d_OS} ${28_day_harib25d_SYS} ::
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/a.hrb ::a.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/type.hrb ::type.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/typeipl.hrb ::typeipl.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/color.hrb ::color.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${28_day_harib25d_OS} ${root_BINARY_DIR}/projects/28_day/harib25d/hello5.hrb ::hello5.hrb
  DEPENDS 28_day_harib25d_sys ${root_BINARY_DIR}/projects/28_day/harib25d/a.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/hello3.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/hello4.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo2.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo3.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/star1.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/stars.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/stars2.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/lines.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/walk.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/type.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/typeipl.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/sosu3.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/sosu2.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/sosu.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/noodle.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/beepdown.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/color.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/color2.hrb ${root_BINARY_DIR}/projects/28_day/harib25d/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(28_day_harib25d_APIO
  ${root_BINARY_DIR}/projects/28_day/harib25d/api001.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api002.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api003.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api004.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api005.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api006.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api007.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api008.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api009.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api010.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api011.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api012.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api013.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api014.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api015.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api016.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api017.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api018.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api019.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api020.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api021.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api022.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api023.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api024.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api025.o
  ${root_BINARY_DIR}/projects/28_day/harib25d/api026.o)
set(28_day_harib25d_RANDO ${root_BINARY_DIR}/projects/28_day/harib25d/rand.o)
add_custom_command(
  OUTPUT ${28_day_harib25d_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${28_day_harib25d_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api001.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api001.nas
  COMMENT "Generating api001.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api002.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api002.nas
  COMMENT "Generating api002.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api003.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api003.nas
  COMMENT "Generating api003.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api004.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api004.nas
  COMMENT "Generating api004.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api005.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api005.nas
  COMMENT "Generating api005.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api006.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api006.nas
  COMMENT "Generating api006.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api007.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api007.nas
  COMMENT "Generating api007.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api008.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api008.nas
  COMMENT "Generating api008.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api009.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api009.nas
  COMMENT "Generating api009.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api010.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api010.nas
  COMMENT "Generating api010.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api011.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api011.nas
  COMMENT "Generating api011.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api012.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api012.nas
  COMMENT "Generating api012.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api013.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api013.nas
  COMMENT "Generating api013.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api014.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api014.nas
  COMMENT "Generating api014.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api015.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api015.nas
  COMMENT "Generating api015.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api016.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api016.nas
  COMMENT "Generating api016.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api017.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api017.nas
  COMMENT "Generating api017.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api018.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api018.nas
  COMMENT "Generating api018.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api019.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api019.nas
  COMMENT "Generating api019.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api020.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api020.nas
  COMMENT "Generating api020.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api021.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api021.nas
  COMMENT "Generating api021.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api022.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api022.nas
  COMMENT "Generating api022.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api023.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api023.nas
  COMMENT "Generating api023.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api024.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api024.nas
  COMMENT "Generating api024.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api025.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api025.nas
  COMMENT "Generating api025.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api026.nas ${root_BINARY_DIR}/projects/28_day/harib25d/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/apilib/api026.nas
  COMMENT "Generating api026.o for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/a/a.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating a.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/hello3/hello3.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating hello3.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/hello4/hello4.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating hello4.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/winhelo/winhelo.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating winhelo.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/winhelo2/winhelo2.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating winhelo2.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/winhelo3/winhelo3.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating winhelo3.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/star1/star1.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating star1.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/stars/stars.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating stars.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/stars2/stars2.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating stars2.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/lines/lines.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating lines.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/walk/walk.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating walk.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/type/type.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating type.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/typeipl.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/typeipl/typeipl.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/typeipl.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/typeipl/typeipl.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating typeipl.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/sosu3/sosu3.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating sosu3.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/sosu2/sosu2.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating sosu2.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/sosu/sosu.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating sosu.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/color2/color2.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating color2.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/color/color.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating color.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/beepdown/beepdown.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating beepdown.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25d -T ${28_day_harib25d_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25d/noodle/noodle.c ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25d/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25d_APIO} ${28_day_harib25d_LIBGC} ${28_day_harib25d_RANDO}
  COMMENT "Generating noodle.hrb for 28_day_harib25d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25d/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25d/hello5/hello5.nas ${root_BINARY_DIR}/projects/28_day/harib25d/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25d/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 28_day_harib25d"
)
