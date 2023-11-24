#

function (include_shader_resource src)
	get_filename_component (filename ${src} NAME)
	string (REPLACE "." "_" shader_resource_variable_name ${filename})

	file (READ "${src}" content)
	set (src_content "R\"${SHADER_CONTENT_DELIM}(${content})${SHADER_CONTENT_DELIM}\"")
	set (generated_file "${SHADER_GENERATED_DIR}/${filename}")
	file (WRITE ${generated_file} "${src_content}")

	set (generated_file_include "constexpr std::string_view ${shader_resource_variable_name} = \"\"\n#include \"${filename}\"\n;")
	file (APPEND ${SHADER_RESOURCE_FILE} "\n// **** automatically generated shader resource ****\n${generated_file_include}")
endfunction ()
