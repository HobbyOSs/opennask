cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(opennask_tools LANGUAGES C VERSION 1.0.0)

########### makefont ###############

set(makefont_SRCS
  makefont.c)

add_executable(makefont ${makefont_SRCS})

########### bin2obj ###############

set(bin2obj_SRCS
  bin2obj.c)

add_executable(bin2obj ${bin2obj_SRCS})