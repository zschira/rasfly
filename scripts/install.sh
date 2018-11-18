#!/bin/sh

declare -a HEADERS=("rasfly.h" "rasfly_config.h" "rasfly_types.h")
declare -a OBJECTS=("libRASFLY.so")

declare lib_dir="/usr/local/lib/"
declare header_dir="/usr/local/include"

cp "../rasfly.config" "${HOME}"

for header in "${HEADERS[@]}"
do
	cp "../include/${header}" "${header_dir}"	
done	

for obj in "${OBJECTS[@]}"
do
	cp "../lib/${obj}" "${lib_dir}"	
done	