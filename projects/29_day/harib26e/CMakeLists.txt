#----------------------------------------------------------
message(STATUS "Entering directory projects/29_day/harib26e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(29_day_harib26e_OS    ${root_BINARY_DIR}/projects/29_day/harib26e/os.img)
set(29_day_harib26e_SYS	  ${root_BINARY_DIR}/projects/29_day/harib26e/os.sys)
set(29_day_harib26e_IPLB  ${root_BINARY_DIR}/projects/29_day/harib26e/ipl.bin)
set(29_day_harib26e_IPLS  ${root_SOURCE_DIR}/projects/29_day/harib26e/haribote/ipl20.nas)
set(29_day_harib26e_HEADB ${root_BINARY_DIR}/projects/29_day/harib26e/asmhead.bin)
set(29_day_harib26e_HEADS ${root_SOURCE_DIR}/projects/29_day/harib26e/haribote/asmhead.nas)
set(29_day_harib26e_BOOTB ${root_BINARY_DIR}/projects/29_day/harib26e/boot.bin)
set(29_day_harib26e_CCO	  ${root_BINARY_DIR}/projects/29_day/harib26e/boot.o)
file(GLOB 29_day_harib26e_CCS ${root_SOURCE_DIR}/projects/29_day/harib26e/haribote/*.c)
set(29_day_harib26e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(29_day_harib26e_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(29_day_harib26e_FUNCS ${root_SOURCE_DIR}/projects/29_day/harib26e/haribote/naskfunc.nas)
set(29_day_harib26e_FUNCO ${root_BINARY_DIR}/projects/29_day/harib26e/naskfunc.o)
set(29_day_harib26e_FONTS ${root_SOURCE_DIR}/projects/29_day/harib26e/haribote/hankaku.txt)
set(29_day_harib26e_FONTB ${root_BINARY_DIR}/projects/29_day/harib26e/hankaku.bin)
set(29_day_harib26e_FONTO ${root_BINARY_DIR}/projects/29_day/harib26e/hankaku.o)
set(29_day_harib26e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(29_day_harib26e_LIBGC ${root_BINARY_DIR}/projects/29_day/harib26e/libgo.o)
set(29_day_harib26e_WILDOBJ ${root_BINARY_DIR}/projects/29_day/harib26e/*.o)


add_custom_target(29_day_harib26e_run
  COMMAND ${QEMU} ${QEMUOPT} ${29_day_harib26e_OS}
  DEPENDS 29_day_harib26e_img
)
add_custom_target(29_day_harib26e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${29_day_harib26e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 29_day_harib26e_img
)
add_custom_target(29_day_harib26e_clean
  COMMAND rm -f ${29_day_harib26e_OS}
  COMMAND rm -f ${29_day_harib26e_SYS}
  COMMAND rm -f ${29_day_harib26e_IPLB}
  COMMAND rm -f ${29_day_harib26e_HEADB}
  COMMAND rm -f ${29_day_harib26e_BOOTB}
  COMMAND rm -f ${29_day_harib26e_WILDOBJ}
)
add_custom_target(29_day_harib26e_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26e_IPLS} ${29_day_harib26e_IPLB}
  DEPENDS ${29_day_harib26e_IPLS}
)
add_custom_command(
  OUTPUT ${29_day_harib26e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26e_HEADS} ${29_day_harib26e_HEADB}
  DEPENDS ${29_day_harib26e_HEADS}
  COMMENT "Generating asmhead.bin for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${29_day_harib26e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26e_FUNCS} ${29_day_harib26e_FUNCO}
  DEPENDS ${29_day_harib26e_FUNCS}
  COMMENT "Generating naskfunc.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${29_day_harib26e_FONTO}
  COMMAND ${FONT} ${29_day_harib26e_FONTS} ${29_day_harib26e_FONTB}
  COMMAND ${B2O}  ${29_day_harib26e_FONTB} ${29_day_harib26e_FONTO} _hankaku
  DEPENDS ${29_day_harib26e_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${29_day_harib26e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${29_day_harib26e_LIBGE} ${29_day_harib26e_LIBGC}
  DEPENDS ${29_day_harib26e_LIBGE}
  COMMENT "Converting golibc for 29_day_harib26e"
)

add_custom_target(29_day_harib26e_sys
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -T ${29_day_harib26e_LDS} ${29_day_harib26e_CCS} ${29_day_harib26e_WILDOBJ} -o ${29_day_harib26e_BOOTB}
  COMMAND cat ${29_day_harib26e_HEADB} ${29_day_harib26e_BOOTB} > ${29_day_harib26e_SYS}
  DEPENDS ${29_day_harib26e_HEADB} 29_day_harib26e_ipl
  DEPENDS ${29_day_harib26e_FUNCO}
  DEPENDS ${29_day_harib26e_FONTO} ${29_day_harib26e_LIBGC}
)
add_custom_target(29_day_harib26e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${29_day_harib26e_IPLB} -i ${29_day_harib26e_OS}
  COMMAND mcopy -i ${29_day_harib26e_OS} ${29_day_harib26e_SYS} ::
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/a.hrb ::a.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/invader.hrb ::invader.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/bball.hrb ::bball.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/notrec.hrb ::notrec.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/type.hrb ::type.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/color.hrb ::color.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_BINARY_DIR}/projects/29_day/harib26e/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_SOURCE_DIR}/projects/29_day/harib26e/euc.txt ::euc.txt
  COMMAND mcopy -i ${29_day_harib26e_OS} ${root_SOURCE_DIR}/projects/29_day/harib26e/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 29_day_harib26e_sys ${root_BINARY_DIR}/projects/29_day/harib26e/a.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/hello3.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/hello4.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo2.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo3.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/star1.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/stars.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/stars2.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/lines.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/walk.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/invader.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/bball.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/notrec.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/chklang.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/iroha.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/type.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/sosu3.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/sosu2.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/sosu.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/noodle.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/beepdown.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/color.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/color2.hrb ${root_BINARY_DIR}/projects/29_day/harib26e/hello5.hrb ${root_SOURCE_DIR}/projects/29_day/harib26e/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/29_day/harib26e/euc.txt
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(29_day_harib26e_APIO
  ${root_BINARY_DIR}/projects/29_day/harib26e/api001.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api002.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api003.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api004.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api005.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api006.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api007.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api008.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api009.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api010.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api011.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api012.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api013.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api014.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api015.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api016.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api017.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api018.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api019.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api020.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api021.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api022.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api023.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api024.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api025.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api026.o
  ${root_BINARY_DIR}/projects/29_day/harib26e/api027.o)
set(29_day_harib26e_RANDO ${root_BINARY_DIR}/projects/29_day/harib26e/rand.o)
add_custom_command(
  OUTPUT ${29_day_harib26e_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${29_day_harib26e_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api001.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api001.nas
  COMMENT "Generating api001.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api002.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api002.nas
  COMMENT "Generating api002.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api003.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api003.nas
  COMMENT "Generating api003.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api004.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api004.nas
  COMMENT "Generating api004.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api005.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api005.nas
  COMMENT "Generating api005.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api006.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api006.nas
  COMMENT "Generating api006.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api007.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api007.nas
  COMMENT "Generating api007.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api008.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api008.nas
  COMMENT "Generating api008.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api009.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api009.nas
  COMMENT "Generating api009.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api010.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api010.nas
  COMMENT "Generating api010.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api011.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api011.nas
  COMMENT "Generating api011.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api012.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api012.nas
  COMMENT "Generating api012.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api013.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api013.nas
  COMMENT "Generating api013.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api014.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api014.nas
  COMMENT "Generating api014.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api015.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api015.nas
  COMMENT "Generating api015.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api016.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api016.nas
  COMMENT "Generating api016.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api017.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api017.nas
  COMMENT "Generating api017.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api018.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api018.nas
  COMMENT "Generating api018.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api019.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api019.nas
  COMMENT "Generating api019.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api020.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api020.nas
  COMMENT "Generating api020.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api021.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api021.nas
  COMMENT "Generating api021.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api022.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api022.nas
  COMMENT "Generating api022.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api023.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api023.nas
  COMMENT "Generating api023.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api024.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api024.nas
  COMMENT "Generating api024.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api025.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api025.nas
  COMMENT "Generating api025.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api026.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api026.nas
  COMMENT "Generating api026.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api027.nas ${root_BINARY_DIR}/projects/29_day/harib26e/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/apilib/api027.nas
  COMMENT "Generating api027.o for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/a/a.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating a.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/hello3/hello3.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating hello3.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/hello4/hello4.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating hello4.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/winhelo/winhelo.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating winhelo.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/winhelo2/winhelo2.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating winhelo2.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/winhelo3/winhelo3.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating winhelo3.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/star1/star1.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating star1.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/stars/stars.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating stars.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/stars2/stars2.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating stars2.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/lines/lines.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating lines.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/walk/walk.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating walk.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/invader.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/invader/invader.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/invader.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/invader/invader.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating invader.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/bball.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/bball/bball.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/bball.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/bball/bball.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating bball.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/notrec.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/notrec/notrec.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/notrec.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/notrec/notrec.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating notrec.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/chklang/chklang.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating chklang.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/iroha/iroha.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating iroha.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/type/type.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating type.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/sosu3/sosu3.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating sosu3.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/sosu2/sosu2.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating sosu2.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/sosu/sosu.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating sosu.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/color2/color2.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating color2.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/color/color.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating color.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/beepdown/beepdown.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating beepdown.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26e -T ${29_day_harib26e_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26e/noodle/noodle.c ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26e/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26e_APIO} ${29_day_harib26e_LIBGC} ${29_day_harib26e_RANDO}
  COMMENT "Generating noodle.hrb for 29_day_harib26e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26e/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26e/hello5/hello5.nas ${root_BINARY_DIR}/projects/29_day/harib26e/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26e/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 29_day_harib26e"
)
