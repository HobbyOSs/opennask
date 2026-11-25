#----------------------------------------------------------
message(STATUS "Entering directory projects/28_day/harib25e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(28_day_harib25e_OS    ${root_BINARY_DIR}/projects/28_day/harib25e/os.img)
set(28_day_harib25e_SYS	  ${root_BINARY_DIR}/projects/28_day/harib25e/os.sys)
set(28_day_harib25e_IPLB  ${root_BINARY_DIR}/projects/28_day/harib25e/ipl.bin)
set(28_day_harib25e_IPLS  ${root_SOURCE_DIR}/projects/28_day/harib25e/haribote/ipl10.nas)
set(28_day_harib25e_HEADB ${root_BINARY_DIR}/projects/28_day/harib25e/asmhead.bin)
set(28_day_harib25e_HEADS ${root_SOURCE_DIR}/projects/28_day/harib25e/haribote/asmhead.nas)
set(28_day_harib25e_BOOTB ${root_BINARY_DIR}/projects/28_day/harib25e/boot.bin)
set(28_day_harib25e_CCO	  ${root_BINARY_DIR}/projects/28_day/harib25e/boot.o)
file(GLOB 28_day_harib25e_CCS ${root_SOURCE_DIR}/projects/28_day/harib25e/haribote/*.c)
set(28_day_harib25e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(28_day_harib25e_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(28_day_harib25e_FUNCS ${root_SOURCE_DIR}/projects/28_day/harib25e/haribote/naskfunc.nas)
set(28_day_harib25e_FUNCO ${root_BINARY_DIR}/projects/28_day/harib25e/naskfunc.o)
set(28_day_harib25e_FONTS ${root_SOURCE_DIR}/projects/28_day/harib25e/haribote/hankaku.txt)
set(28_day_harib25e_FONTB ${root_BINARY_DIR}/projects/28_day/harib25e/hankaku.bin)
set(28_day_harib25e_FONTO ${root_BINARY_DIR}/projects/28_day/harib25e/hankaku.o)
set(28_day_harib25e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(28_day_harib25e_LIBGC ${root_BINARY_DIR}/projects/28_day/harib25e/libgo.o)
set(28_day_harib25e_WILDOBJ ${root_BINARY_DIR}/projects/28_day/harib25e/*.o)


add_custom_target(28_day_harib25e_run
  COMMAND ${QEMU} ${QEMUOPT} ${28_day_harib25e_OS}
  DEPENDS 28_day_harib25e_img
)
add_custom_target(28_day_harib25e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${28_day_harib25e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 28_day_harib25e_img
)
add_custom_target(28_day_harib25e_clean
  COMMAND rm -f ${28_day_harib25e_OS}
  COMMAND rm -f ${28_day_harib25e_SYS}
  COMMAND rm -f ${28_day_harib25e_IPLB}
  COMMAND rm -f ${28_day_harib25e_HEADB}
  COMMAND rm -f ${28_day_harib25e_BOOTB}
  COMMAND rm -f ${28_day_harib25e_WILDOBJ}
)
add_custom_target(28_day_harib25e_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25e_IPLS} ${28_day_harib25e_IPLB}
  DEPENDS ${28_day_harib25e_IPLS}
)
add_custom_command(
  OUTPUT ${28_day_harib25e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25e_HEADS} ${28_day_harib25e_HEADB}
  DEPENDS ${28_day_harib25e_HEADS}
  COMMENT "Generating asmhead.bin for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${28_day_harib25e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25e_FUNCS} ${28_day_harib25e_FUNCO}
  DEPENDS ${28_day_harib25e_FUNCS}
  COMMENT "Generating naskfunc.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${28_day_harib25e_FONTO}
  COMMAND ${FONT} ${28_day_harib25e_FONTS} ${28_day_harib25e_FONTB}
  COMMAND ${B2O}  ${28_day_harib25e_FONTB} ${28_day_harib25e_FONTO} _hankaku
  DEPENDS ${28_day_harib25e_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${28_day_harib25e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${28_day_harib25e_LIBGE} ${28_day_harib25e_LIBGC}
  DEPENDS ${28_day_harib25e_LIBGE}
  COMMENT "Converting golibc for 28_day_harib25e"
)

add_custom_target(28_day_harib25e_sys
  COMMAND gcc ${BINOPT} -T ${28_day_harib25e_LDS} ${28_day_harib25e_CCS} ${28_day_harib25e_WILDOBJ} -o ${28_day_harib25e_BOOTB}
  COMMAND cat ${28_day_harib25e_HEADB} ${28_day_harib25e_BOOTB} > ${28_day_harib25e_SYS}
  DEPENDS ${28_day_harib25e_HEADB} 28_day_harib25e_ipl
  DEPENDS ${28_day_harib25e_FUNCO}
  DEPENDS ${28_day_harib25e_FONTO} ${28_day_harib25e_LIBGC}
)
add_custom_target(28_day_harib25e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${28_day_harib25e_IPLB} -i ${28_day_harib25e_OS}
  COMMAND mcopy -i ${28_day_harib25e_OS} ${28_day_harib25e_SYS} ::
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/a.hrb ::a.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/type.hrb ::type.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/typeipl.hrb ::typeipl.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/color.hrb ::color.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_BINARY_DIR}/projects/28_day/harib25e/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${28_day_harib25e_OS} ${root_SOURCE_DIR}/projects/28_day/harib25e/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 28_day_harib25e_sys ${root_BINARY_DIR}/projects/28_day/harib25e/a.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/hello3.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/hello4.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo2.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo3.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/star1.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/stars.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/stars2.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/lines.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/walk.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/iroha.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/type.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/typeipl.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/sosu3.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/sosu2.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/sosu.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/noodle.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/beepdown.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/color.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/color2.hrb ${root_BINARY_DIR}/projects/28_day/harib25e/hello5.hrb ${root_SOURCE_DIR}/projects/28_day/harib25e/nihongo/nihongo.fnt
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(28_day_harib25e_APIO
  ${root_BINARY_DIR}/projects/28_day/harib25e/api001.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api002.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api003.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api004.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api005.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api006.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api007.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api008.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api009.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api010.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api011.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api012.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api013.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api014.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api015.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api016.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api017.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api018.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api019.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api020.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api021.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api022.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api023.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api024.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api025.o
  ${root_BINARY_DIR}/projects/28_day/harib25e/api026.o)
set(28_day_harib25e_RANDO ${root_BINARY_DIR}/projects/28_day/harib25e/rand.o)
add_custom_command(
  OUTPUT ${28_day_harib25e_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${28_day_harib25e_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api001.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api001.nas
  COMMENT "Generating api001.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api002.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api002.nas
  COMMENT "Generating api002.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api003.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api003.nas
  COMMENT "Generating api003.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api004.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api004.nas
  COMMENT "Generating api004.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api005.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api005.nas
  COMMENT "Generating api005.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api006.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api006.nas
  COMMENT "Generating api006.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api007.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api007.nas
  COMMENT "Generating api007.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api008.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api008.nas
  COMMENT "Generating api008.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api009.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api009.nas
  COMMENT "Generating api009.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api010.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api010.nas
  COMMENT "Generating api010.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api011.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api011.nas
  COMMENT "Generating api011.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api012.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api012.nas
  COMMENT "Generating api012.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api013.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api013.nas
  COMMENT "Generating api013.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api014.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api014.nas
  COMMENT "Generating api014.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api015.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api015.nas
  COMMENT "Generating api015.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api016.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api016.nas
  COMMENT "Generating api016.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api017.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api017.nas
  COMMENT "Generating api017.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api018.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api018.nas
  COMMENT "Generating api018.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api019.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api019.nas
  COMMENT "Generating api019.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api020.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api020.nas
  COMMENT "Generating api020.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api021.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api021.nas
  COMMENT "Generating api021.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api022.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api022.nas
  COMMENT "Generating api022.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api023.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api023.nas
  COMMENT "Generating api023.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api024.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api024.nas
  COMMENT "Generating api024.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api025.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api025.nas
  COMMENT "Generating api025.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api026.nas ${root_BINARY_DIR}/projects/28_day/harib25e/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/apilib/api026.nas
  COMMENT "Generating api026.o for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/a/a.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating a.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/hello3/hello3.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating hello3.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/hello4/hello4.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating hello4.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/winhelo/winhelo.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating winhelo.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/winhelo2/winhelo2.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating winhelo2.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/winhelo3/winhelo3.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating winhelo3.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/star1/star1.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating star1.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/stars/stars.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating stars.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/stars2/stars2.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating stars2.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/lines/lines.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating lines.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/walk/walk.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating walk.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/iroha/iroha.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating iroha.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/type/type.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating type.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/typeipl.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/typeipl/typeipl.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/typeipl.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/typeipl/typeipl.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating typeipl.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/sosu3/sosu3.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating sosu3.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/sosu2/sosu2.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating sosu2.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/sosu/sosu.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating sosu.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/color2/color2.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating color2.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/color/color.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating color.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/beepdown/beepdown.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating beepdown.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25e -T ${28_day_harib25e_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25e/noodle/noodle.c ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25e/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25e_APIO} ${28_day_harib25e_LIBGC} ${28_day_harib25e_RANDO}
  COMMENT "Generating noodle.hrb for 28_day_harib25e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25e/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25e/hello5/hello5.nas ${root_BINARY_DIR}/projects/28_day/harib25e/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25e/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 28_day_harib25e"
)
