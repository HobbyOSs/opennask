#----------------------------------------------------------
message(STATUS "Entering directory projects/02_day/helloos3/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(02_day_helloos3_OS    ${root_BINARY_DIR}/projects/02_day/helloos3/os.img)
set(02_day_helloos3_SYS	  ${root_BINARY_DIR}/projects/02_day/helloos3/os.sys)
set(02_day_helloos3_IPLB  ${root_BINARY_DIR}/projects/02_day/helloos3/ipl.bin)
set(02_day_helloos3_IPLS  ${root_SOURCE_DIR}/projects/02_day/helloos3/ipl10.nas)
set(02_day_helloos3_HEADB ${root_BINARY_DIR}/projects/02_day/helloos3/asmhead.bin)
set(02_day_helloos3_HEADS ${root_SOURCE_DIR}/projects/02_day/helloos3/asmhead.nas)
set(02_day_helloos3_BOOTB ${root_BINARY_DIR}/projects/02_day/helloos3/boot.bin)
set(02_day_helloos3_CCO	  ${root_BINARY_DIR}/projects/02_day/helloos3/boot.o)
set(02_day_helloos3_CCS	  ${root_SOURCE_DIR}/projects/02_day/helloos3/*.c)

add_custom_target(02_day_helloos3_helloos_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${02_day_helloos3_OS}				
)											
add_custom_target(02_day_helloos3_helloos_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${02_day_helloos3_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(02_day_helloos3_helloos_ipl						
  COMMAND ${NASK} ${02_day_helloos3_IPLS} ${02_day_helloos3_IPLB}		
)											
add_custom_target(02_day_helloos3_helloos_sys						
  COMMAND ${NASK} ${02_day_helloos3_HEADS} ${02_day_helloos3_HEADB}		
  COMMAND gcc ${02_day_helloos3_CCS}  -c -o ${02_day_helloos3_CCO}	
  COMMAND ld -o ${02_day_helloos3_BOOTB} -e _HariMain ${02_day_helloos3_CCO} ${02_day_helloos3_FUNCO}	
  COMMAND cat ${02_day_helloos3_HEADB} ${02_day_helloos3_BOOTB} > ${02_day_helloos3_SYS}			
)											
add_custom_target(02_day_helloos3_helloos_img						
  COMMAND mformat -f 1440 -C -B ${02_day_helloos3_IPLB} -i ${02_day_helloos3_OS}	
  COMMAND mcopy ${02_day_helloos3_SYS} -i ${02_day_helloos3_OS}			
)											
########### next target ###############
set(helloos.img_SRCS ${root_SOURCE_DIR}/projects/02_day/helloos3/helloos.nas)
set(helloos.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos3/helloos.img)
set(helloos_wine.img_OUTS ${root_BINARY_DIR}/projects/02_day/helloos3/helloos_wine.img)

add_custom_target(02_day_helloos3_helloos
  COMMAND ${root_BINARY_DIR}/src/opennask ${helloos.img_SRCS} ${helloos.img_OUTS}
)
add_custom_target(02_day_helloos3_helloos_wine
  COMMAND ${WINE} ${WINE_NASK} ${helloos.img_SRCS} ${helloos_wine.img_OUTS}
)
add_custom_target(02_day_helloos3_helloos_od
  COMMAND ${OD} -t x1 ${helloos.img_OUTS}      > ${helloos.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${helloos_wine.img_OUTS} > ${helloos_wine.img_OUTS}_t.txt
)

add_dependencies(images 02_day_helloos3_helloos)
add_dependencies(wine 02_day_helloos3_helloos_wine)
add_dependencies(od 02_day_helloos3_helloos_od)

#----------------------------------------------------------
