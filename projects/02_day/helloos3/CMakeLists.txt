#----------------------------------------------------------
message(STATUS "Entering directory projects/02_day/helloos3/")

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

add_dependencies(images 02_day_helloos3_helloos)
add_dependencies(wine 02_day_helloos3_helloos_wine)

