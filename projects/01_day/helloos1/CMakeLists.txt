#----------------------------------------------------------
message(STATUS "Entering directory projects/01_day/helloos1/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(01_day_helloos1_OS    ${root_BINARY_DIR}/projects/01_day/helloos1/os.img)
set(01_day_helloos1_SYS	  ${root_BINARY_DIR}/projects/01_day/helloos1/os.sys)
set(01_day_helloos1_IPLB  ${root_BINARY_DIR}/projects/01_day/helloos1/ipl.bin)
set(01_day_helloos1_IPLS  ${root_SOURCE_DIR}/projects/01_day/helloos1/ipl10.nas)
set(01_day_helloos1_HEADB ${root_BINARY_DIR}/projects/01_day/helloos1/asmhead.bin)
set(01_day_helloos1_HEADS ${root_SOURCE_DIR}/projects/01_day/helloos1/asmhead.nas)
set(01_day_helloos1_BOOTB ${root_BINARY_DIR}/projects/01_day/helloos1/boot.bin)
set(01_day_helloos1_CCO	  ${root_BINARY_DIR}/projects/01_day/helloos1/boot.o)
set(01_day_helloos1_CCS	  ${root_SOURCE_DIR}/projects/01_day/helloos1/*.c)

add_custom_target(01_day_helloos1_helloos_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${01_day_helloos1_OS}				
)											
add_custom_target(01_day_helloos1_helloos_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${01_day_helloos1_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(01_day_helloos1_helloos_ipl						
  COMMAND ${NASK} ${01_day_helloos1_IPLS} ${01_day_helloos1_IPLB}		
)											
add_custom_target(01_day_helloos1_helloos_sys						
  COMMAND ${NASK} ${01_day_helloos1_HEADS} ${01_day_helloos1_HEADB}		
  COMMAND gcc ${01_day_helloos1_CCS}  -c -o ${01_day_helloos1_CCO}	
  COMMAND ld -o ${01_day_helloos1_BOOTB} -e _HariMain ${01_day_helloos1_CCO} ${01_day_helloos1_FUNCO}	
  COMMAND cat ${01_day_helloos1_HEADB} ${01_day_helloos1_BOOTB} > ${01_day_helloos1_SYS}			
)											
add_custom_target(01_day_helloos1_helloos_img						
  COMMAND mformat -f 1440 -C -B ${01_day_helloos1_IPLB} -i ${01_day_helloos1_OS}	
  COMMAND mcopy ${01_day_helloos1_SYS} -i ${01_day_helloos1_OS}			
)											
########### next target ###############
set(helloos.img_SRCS ${root_SOURCE_DIR}/projects/01_day/helloos1/helloos.nas)
set(helloos.img_OUTS ${root_BINARY_DIR}/projects/01_day/helloos1/helloos.img)
set(helloos_wine.img_OUTS ${root_BINARY_DIR}/projects/01_day/helloos1/helloos_wine.img)

add_custom_target(01_day_helloos1_helloos
  COMMAND ${root_BINARY_DIR}/src/opennask ${helloos.img_SRCS} ${helloos.img_OUTS}
)
add_custom_target(01_day_helloos1_helloos_wine
  COMMAND ${WINE} ${WINE_NASK} ${helloos.img_SRCS} ${helloos_wine.img_OUTS}
)
add_custom_target(01_day_helloos1_helloos_od
  COMMAND ${OD} -t x1 ${helloos.img_OUTS}      > ${helloos.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${helloos_wine.img_OUTS} > ${helloos_wine.img_OUTS}_t.txt
)

add_dependencies(images 01_day_helloos1_helloos)
add_dependencies(wine 01_day_helloos1_helloos_wine)
add_dependencies(od 01_day_helloos1_helloos_od)

#----------------------------------------------------------
