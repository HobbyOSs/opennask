#----------------------------------------------------------
message(STATUS "Entering directory projects/29_day/harib26d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(29_day_harib26d_OS    ${root_BINARY_DIR}/projects/29_day/harib26d/os.img)
set(29_day_harib26d_SYS	  ${root_BINARY_DIR}/projects/29_day/harib26d/os.sys)
set(29_day_harib26d_IPLB  ${root_BINARY_DIR}/projects/29_day/harib26d/ipl.bin)
set(29_day_harib26d_IPLS  ${root_SOURCE_DIR}/projects/29_day/harib26d/haribote/ipl20.nas)
set(29_day_harib26d_HEADB ${root_BINARY_DIR}/projects/29_day/harib26d/asmhead.bin)
set(29_day_harib26d_HEADS ${root_SOURCE_DIR}/projects/29_day/harib26d/haribote/asmhead.nas)
set(29_day_harib26d_BOOTB ${root_BINARY_DIR}/projects/29_day/harib26d/boot.bin)
set(29_day_harib26d_CCO	  ${root_BINARY_DIR}/projects/29_day/harib26d/boot.o)
file(GLOB 29_day_harib26d_CCS ${root_SOURCE_DIR}/projects/29_day/harib26d/haribote/*.c)
set(29_day_harib26d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(29_day_harib26d_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(29_day_harib26d_FUNCS ${root_SOURCE_DIR}/projects/29_day/harib26d/haribote/naskfunc.nas)
set(29_day_harib26d_FUNCO ${root_BINARY_DIR}/projects/29_day/harib26d/naskfunc.o)
set(29_day_harib26d_FONTS ${root_SOURCE_DIR}/projects/29_day/harib26d/haribote/hankaku.txt)
set(29_day_harib26d_FONTB ${root_BINARY_DIR}/projects/29_day/harib26d/hankaku.bin)
set(29_day_harib26d_FONTO ${root_BINARY_DIR}/projects/29_day/harib26d/hankaku.o)
set(29_day_harib26d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(29_day_harib26d_LIBGC ${root_BINARY_DIR}/projects/29_day/harib26d/libgo.o)
set(29_day_harib26d_WILDOBJ ${root_BINARY_DIR}/projects/29_day/harib26d/*.o)


add_custom_target(29_day_harib26d_run
  COMMAND ${QEMU} ${QEMUOPT} ${29_day_harib26d_OS}
  DEPENDS 29_day_harib26d_img
)
add_custom_target(29_day_harib26d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${29_day_harib26d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 29_day_harib26d_img
)
add_custom_target(29_day_harib26d_clean
  COMMAND rm -f ${29_day_harib26d_OS}
  COMMAND rm -f ${29_day_harib26d_SYS}
  COMMAND rm -f ${29_day_harib26d_IPLB}
  COMMAND rm -f ${29_day_harib26d_HEADB}
  COMMAND rm -f ${29_day_harib26d_BOOTB}
  COMMAND rm -f ${29_day_harib26d_WILDOBJ}
)
add_custom_target(29_day_harib26d_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26d_IPLS} ${29_day_harib26d_IPLB}
  DEPENDS ${29_day_harib26d_IPLS}
)
add_custom_command(
  OUTPUT ${29_day_harib26d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26d_HEADS} ${29_day_harib26d_HEADB}
  DEPENDS ${29_day_harib26d_HEADS}
  COMMENT "Generating asmhead.bin for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${29_day_harib26d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26d_FUNCS} ${29_day_harib26d_FUNCO}
  DEPENDS ${29_day_harib26d_FUNCS}
  COMMENT "Generating naskfunc.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${29_day_harib26d_FONTO}
  COMMAND ${FONT} ${29_day_harib26d_FONTS} ${29_day_harib26d_FONTB}
  COMMAND ${B2O}  ${29_day_harib26d_FONTB} ${29_day_harib26d_FONTO} _hankaku
  DEPENDS ${29_day_harib26d_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${29_day_harib26d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${29_day_harib26d_LIBGE} ${29_day_harib26d_LIBGC}
  DEPENDS ${29_day_harib26d_LIBGE}
  COMMENT "Converting golibc for 29_day_harib26d"
)

add_custom_target(29_day_harib26d_sys
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -T ${29_day_harib26d_LDS} ${29_day_harib26d_CCS} ${29_day_harib26d_WILDOBJ} -o ${29_day_harib26d_BOOTB}
  COMMAND cat ${29_day_harib26d_HEADB} ${29_day_harib26d_BOOTB} > ${29_day_harib26d_SYS}
  DEPENDS ${29_day_harib26d_HEADB} 29_day_harib26d_ipl
  DEPENDS ${29_day_harib26d_FUNCO}
  DEPENDS ${29_day_harib26d_FONTO} ${29_day_harib26d_LIBGC}
)
add_custom_target(29_day_harib26d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${29_day_harib26d_IPLB} -i ${29_day_harib26d_OS}
  COMMAND mcopy -i ${29_day_harib26d_OS} ${29_day_harib26d_SYS} ::
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/a.hrb ::a.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/bball.hrb ::bball.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/notrec.hrb ::notrec.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/type.hrb ::type.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/color.hrb ::color.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_BINARY_DIR}/projects/29_day/harib26d/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_SOURCE_DIR}/projects/29_day/harib26d/euc.txt ::euc.txt
  COMMAND mcopy -i ${29_day_harib26d_OS} ${root_SOURCE_DIR}/projects/29_day/harib26d/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 29_day_harib26d_sys ${root_BINARY_DIR}/projects/29_day/harib26d/a.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/hello3.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/hello4.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo2.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo3.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/star1.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/stars.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/stars2.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/lines.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/walk.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/bball.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/notrec.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/chklang.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/iroha.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/type.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/sosu3.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/sosu2.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/sosu.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/noodle.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/beepdown.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/color.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/color2.hrb ${root_BINARY_DIR}/projects/29_day/harib26d/hello5.hrb ${root_SOURCE_DIR}/projects/29_day/harib26d/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/29_day/harib26d/euc.txt
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(29_day_harib26d_APIO
  ${root_BINARY_DIR}/projects/29_day/harib26d/api001.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api002.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api003.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api004.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api005.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api006.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api007.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api008.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api009.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api010.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api011.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api012.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api013.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api014.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api015.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api016.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api017.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api018.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api019.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api020.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api021.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api022.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api023.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api024.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api025.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api026.o
  ${root_BINARY_DIR}/projects/29_day/harib26d/api027.o)
set(29_day_harib26d_RANDO ${root_BINARY_DIR}/projects/29_day/harib26d/rand.o)
add_custom_command(
  OUTPUT ${29_day_harib26d_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${29_day_harib26d_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api001.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api001.nas
  COMMENT "Generating api001.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api002.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api002.nas
  COMMENT "Generating api002.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api003.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api003.nas
  COMMENT "Generating api003.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api004.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api004.nas
  COMMENT "Generating api004.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api005.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api005.nas
  COMMENT "Generating api005.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api006.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api006.nas
  COMMENT "Generating api006.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api007.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api007.nas
  COMMENT "Generating api007.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api008.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api008.nas
  COMMENT "Generating api008.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api009.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api009.nas
  COMMENT "Generating api009.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api010.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api010.nas
  COMMENT "Generating api010.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api011.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api011.nas
  COMMENT "Generating api011.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api012.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api012.nas
  COMMENT "Generating api012.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api013.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api013.nas
  COMMENT "Generating api013.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api014.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api014.nas
  COMMENT "Generating api014.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api015.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api015.nas
  COMMENT "Generating api015.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api016.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api016.nas
  COMMENT "Generating api016.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api017.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api017.nas
  COMMENT "Generating api017.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api018.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api018.nas
  COMMENT "Generating api018.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api019.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api019.nas
  COMMENT "Generating api019.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api020.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api020.nas
  COMMENT "Generating api020.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api021.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api021.nas
  COMMENT "Generating api021.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api022.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api022.nas
  COMMENT "Generating api022.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api023.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api023.nas
  COMMENT "Generating api023.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api024.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api024.nas
  COMMENT "Generating api024.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api025.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api025.nas
  COMMENT "Generating api025.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api026.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api026.nas
  COMMENT "Generating api026.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api027.nas ${root_BINARY_DIR}/projects/29_day/harib26d/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/apilib/api027.nas
  COMMENT "Generating api027.o for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/a/a.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating a.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/hello3/hello3.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating hello3.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/hello4/hello4.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating hello4.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/winhelo/winhelo.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating winhelo.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/winhelo2/winhelo2.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating winhelo2.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/winhelo3/winhelo3.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating winhelo3.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/star1/star1.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating star1.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/stars/stars.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating stars.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/stars2/stars2.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating stars2.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/lines/lines.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating lines.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/walk/walk.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating walk.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/bball.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/bball/bball.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/bball.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/bball/bball.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating bball.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/notrec.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/notrec/notrec.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/notrec.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/notrec/notrec.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating notrec.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/chklang/chklang.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating chklang.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/iroha/iroha.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating iroha.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/type/type.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating type.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/sosu3/sosu3.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating sosu3.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/sosu2/sosu2.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating sosu2.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/sosu/sosu.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating sosu.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/color2/color2.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating color2.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/color/color.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating color.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/beepdown/beepdown.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating beepdown.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26d -T ${29_day_harib26d_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26d/noodle/noodle.c ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26d/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26d_APIO} ${29_day_harib26d_LIBGC} ${29_day_harib26d_RANDO}
  COMMENT "Generating noodle.hrb for 29_day_harib26d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26d/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26d/hello5/hello5.nas ${root_BINARY_DIR}/projects/29_day/harib26d/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26d/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 29_day_harib26d"
)
