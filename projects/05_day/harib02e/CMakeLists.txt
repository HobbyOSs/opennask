#----------------------------------------------------------
message(STATUS "Entering directory projects/05_day/harib02e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(05_day_harib02e_OS    ${root_BINARY_DIR}/projects/05_day/harib02e/os.img)
set(05_day_harib02e_SYS	  ${root_BINARY_DIR}/projects/05_day/harib02e/os.sys)
set(05_day_harib02e_IPLB  ${root_BINARY_DIR}/projects/05_day/harib02e/ipl.bin)
set(05_day_harib02e_IPLS  ${root_SOURCE_DIR}/projects/05_day/harib02e/ipl10.nas)
set(05_day_harib02e_HEADB ${root_BINARY_DIR}/projects/05_day/harib02e/asmhead.bin)
set(05_day_harib02e_HEADS ${root_SOURCE_DIR}/projects/05_day/harib02e/asmhead.nas)
set(05_day_harib02e_BOOTB ${root_BINARY_DIR}/projects/05_day/harib02e/boot.bin)
set(05_day_harib02e_CCO	  ${root_BINARY_DIR}/projects/05_day/harib02e/boot.o)
set(05_day_harib02e_CCS	  ${root_SOURCE_DIR}/projects/05_day/harib02e/*.c)

add_custom_target(05_day_harib02e_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${05_day_harib02e_OS}				
)											
add_custom_target(05_day_harib02e_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${05_day_harib02e_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(05_day_harib02e_ipl						
  COMMAND ${NASK} ${05_day_harib02e_IPLS} ${05_day_harib02e_IPLB}		
)											
add_custom_target(05_day_harib02e_sys						
  COMMAND ${NASK} ${05_day_harib02e_HEADS} ${05_day_harib02e_HEADB}		
  COMMAND gcc ${05_day_harib02e_CCS}  -c -o ${05_day_harib02e_CCO}	
  COMMAND ld -o ${05_day_harib02e_BOOTB} -e _HariMain ${05_day_harib02e_CCO} ${05_day_harib02e_FUNCO}	
  COMMAND cat ${05_day_harib02e_HEADB} ${05_day_harib02e_BOOTB} > ${05_day_harib02e_SYS}			
)											
add_custom_target(05_day_harib02e_img						
  COMMAND mformat -f 1440 -C -B ${05_day_harib02e_IPLB} -i ${05_day_harib02e_OS}	
  COMMAND mcopy ${05_day_harib02e_SYS} -i ${05_day_harib02e_OS}			
)											
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e/asmhead_wine.img)

add_custom_target(05_day_harib02e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(05_day_harib02e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(05_day_harib02e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02e_asmhead)
add_dependencies(wine 05_day_harib02e_asmhead_wine)
add_dependencies(od 05_day_harib02e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e/ipl10_wine.img)

add_custom_target(05_day_harib02e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(05_day_harib02e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(05_day_harib02e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02e_ipl10)
add_dependencies(wine 05_day_harib02e_ipl10_wine)
add_dependencies(od 05_day_harib02e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/05_day/harib02e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/05_day/harib02e_wine.imgkfunc_wine.img)

add_custom_target(05_day_harib02ekfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(05_day_harib02ekfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(05_day_harib02ekfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 05_day_harib02ekfunc)
add_dependencies(wine 05_day_harib02ekfunc_wine)
add_dependencies(od 05_day_harib02ekfunc_od)

#----------------------------------------------------------
