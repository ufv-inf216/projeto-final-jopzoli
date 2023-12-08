#
cmake_minimum_required (VERSION 3.27)

# configure CXX STANDARD version
set (CMAKE_CXX_STANDARD 20)

# SDL INIT NO SCREEN BUGFIX
set (CMAKE_SKIP_RPATH ON)
set (CMAKE_SKIP_RPATH ON CACHE BOOL "Skip RPATH" FORCE)

set (PROJECT_NAME "mygame")
set (PROJECT_SRC_ROOT_DIR "src")

project (${PROJECT_NAME} CXX)

set(SRC_FILES
	"src/actor/actor.cpp"
	"src/actor/actor.hpp"
	"src/actor/myactors/arac.cpp"
	"src/actor/myactors/arac.hpp"
	"src/actor/myactors/icosaedro.cpp"
	"src/actor/myactors/icosaedro.hpp"
	"src/actor/myactors/fireball.cpp"
	"src/actor/myactors/fireball.hpp"
	"src/actor/myactors/mage.cpp"
	"src/actor/myactors/mage.hpp"
	"src/actor/myactors/multiicos.cpp"
	"src/actor/myactors/multiicos.hpp"
	"src/actor/myactors/plane.cpp"
	"src/actor/myactors/plane.hpp"
	"src/actor/component/aabbcomponent.cpp"
	"src/actor/component/aabbcomponent.hpp"
	"src/actor/component/cameracomponent.cpp"
	"src/actor/component/cameracomponent.hpp"
	"src/actor/component/component.cpp"
	"src/actor/component/component.hpp"
	"src/actor/component/fsmcomponent.cpp"
	"src/actor/component/fsmcomponent.hpp"
	"src/actor/component/modelcomponent.cpp"
	"src/actor/component/modelcomponent.hpp"
	"src/actor/component/rigidbodycomponent.cpp"
	"src/actor/component/rigidbodycomponent.hpp"
	"src/actor/component/scenecomponent.cpp"
	"src/actor/component/scenecomponent.hpp"
	"src/game.cpp"
	"src/main.cpp"
	"src/math/math.hpp"
	"src/memory/memory.cpp"
	"src/memory/memory.hpp"
	"src/memory/monotonicstorage.cpp"
	"src/memory/monotonicstorage.hpp"
	"src/memory/refcounter.hpp"
	"src/object/object.cpp"
	"src/object/object.hpp"
	"src/scene/scene.cpp"
	"src/scene/scene.hpp"
	"src/scene/myscenes/mainscene.cpp"
	"src/scene/myscenes/mainscene.hpp"
	"src/thirdparty/eventfilter.cpp"
	"src/thirdparty/eventfilter.hpp"
	"src/thirdparty/input.cpp"
	"src/thirdparty/input.hpp"
	"src/thirdparty/modelloader.cpp"
	"src/thirdparty/modelloader.hpp"
	"src/thirdparty/renderer.cpp"
	"src/thirdparty/renderer.hpp"
	"src/thirdparty/thirdparty.cpp"
	"src/thirdparty/thirdparty.hpp"
	"src/thirdparty/shaderprogram.cpp"
	"src/thirdparty/shaderprogram.hpp"
	"src/thirdparty/staticmesh.cpp"
	"src/thirdparty/staticmesh.hpp"
	"src/utils/asserts.hpp"
	"src/utils/define.hpp"
	"src/utils/exception.hpp"
	"src/utils/hashsupport.hpp"
	"src/utils/logger.cpp"
	"src/utils/logger.hpp"
	"src/utils/type.hpp"
	"src/utils/rttisupport.cpp"
	"src/utils/rttisupport.hpp"
	"src/utils/util.hpp"
)

add_executable (${PROJECT_NAME} ${SRC_FILES})

# configure Glew
set (GLEW_VERSION "glew-2.2.0")
set (GLEW_BINARY_REDIST "${GLEW_VERSION}-win32.zip")
set (GLEW_EXTRACT_DIR "${CMAKE_BINARY_DIR}/glew")

if (NOT EXISTS ${GLEW_BINARY_REDIST})
	message (STATUS "Getting file ${GLEW_BINARY_REDIST}")
	file (DOWNLOAD "https://github.com/nigels-com/glew/releases/download/${GLEW_VERSION}/${GLEW_BINARY_REDIST}"
		${GLEW_BINARY_REDIST})
endif ()
if (NOT EXISTS ${GLEW_EXTRACT_DIR})
	message (STATUS "Extracting ${GLEW_BINARY_REDIST} to folder ${GLEW_EXTRACT_DIR}")
	file (ARCHIVE_EXTRACT INPUT ${GLEW_BINARY_REDIST} DESTINATION ${GLEW_EXTRACT_DIR})
endif ()
set (GLEW_LIBRARIES "${GLEW_EXTRACT_DIR}/${GLEW_VERSION}/lib/Release/x64/glew32.lib")
set (GLEW_INCLUDE_DIRS "${GLEW_EXTRACT_DIR}/${GLEW_VERSION}/include")

# configure GLM
set (GLM_BINARY_REDIST "glm-0.9.9.8.zip")
set (GLM_EXTRACT_DIR "${CMAKE_BINARY_DIR}/glm")

if (NOT EXISTS ${GLM_BINARY_REDIST})
	message (STATUS "Getting file ${GLM_BINARY_REDIST}")
	file (DOWNLOAD "https://github.com/g-truc/glm/releases/download/0.9.9.8/${GLM_BINARY_REDIST}"
		${GLM_BINARY_REDIST})
endif ()
if (NOT EXISTS ${GLM_EXTRACT_DIR})
	message (STATUS "Extracting ${GLM_BINARY_REDIST} to folder ${GLM_EXTRACT_DIR}")
	file (ARCHIVE_EXTRACT INPUT ${GLM_BINARY_REDIST} DESTINATION ${GLM_EXTRACT_DIR})
endif ()
set (glm_DIR "${GLM_EXTRACT_DIR}/glm/cmake/glm")

# configure Assimp
set (ASSIMP_INCLUDE_DIR "${assimp_DIR}/include")
file (COPY
	"src/.extern/assimp-vc143-mtd.lib"
	DESTINATION
	"${PROJECT_BINARY_DIR}"
)
set (ASSIMP_LIBRARIES "${PROJECT_BINARY_DIR}/assimp-vc143-mtd.lib")

# configure stb
set (STB_DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/stb")
set (STB_GET_FILES "stb_image.h")
set (STB_URL_SRCS "https://raw.githubusercontent.com/nothings/stb/master")
set (STB_INCLUDE_DIRS "${STB_DOWNLOAD_DIR}")
if (NOT EXISTS ${SDB_INCLUDE_DIRS})
	message (STATUS "Getting files ${STB_GET_FILES} from https://github.com/nothings/stb")
	foreach (FILE_SRC IN LISTS STB_GET_FILES)
		file (DOWNLOAD "${STB_URL_SRCS}/${FILE_SRC}" "${STB_DOWNLOAD_DIR}/${FILE_SRC}")
	endforeach ()
endif ()

# link libraries
find_package (SDL2 2.28.5 REQUIRED)
find_package (SDL2_mixer REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glm REQUIRED)
include_directories (${PROJECT_SRC_ROOT_DIR}
	${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIR}
	${STB_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
)
target_link_libraries (${PROJECT_NAME}
	${SDL2_LIBRARIES}
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${ASSIMP_LIBRARIES}
	${GLM_LIBRARIES}
	SDL2_mixer::SDL2_mixer
)

set (COMPILER_FLAGS "/fsanitize=address")
target_compile_options (${PROJECT_NAME} PUBLIC ${COMPILER_FLAGS})

# configure runtime dependencies generation
file (COPY
	"${GLEW_EXTRACT_DIR}/${GLEW_VERSION}/bin/Release/x64/glew32.dll"
	DESTINATION
	"${PROJECT_BINARY_DIR}"
)
file (COPY
	"src/.extern/assimp-vc143-mtd.dll"
	DESTINATION
	"${PROJECT_BINARY_DIR}"
)

# configure assets generation
set (SHADER_CONTENT_DELIM "//")
set (SHADER_GENERATED_DIR "${PROJECT_SRC_ROOT_DIR}/.generated/shaders")
set (SHADER_RESOURCE_FILE "${SHADER_GENERATED_DIR}/shader_resource.hpp")
function (include_shader_resource src)
	message (STATUS "Generating shader resource ${src}")
	get_filename_component (filename ${src} NAME)
	string (REPLACE "." "_" shader_resource_variable_name ${filename})

	file (READ "${src}" content)
	set (src_content "R\"${SHADER_CONTENT_DELIM}(${content})${SHADER_CONTENT_DELIM}\"")
	set (generated_file "${SHADER_GENERATED_DIR}/${filename}")
	file (WRITE ${generated_file} "${src_content}")

	set (generated_file_include "constexpr std::string_view ${shader_resource_variable_name} = \"\"\n#include \"${filename}\"\n;")
	file (APPEND ${SHADER_RESOURCE_FILE} "\n// **** automatically generated shader resource ****\n${generated_file_include}")
endfunction ()
include_directories (${SHADER_GENERATED_DIR})
file(GLOB ASSETS "src/assets/*")
file(COPY ${ASSETS} DESTINATION "${PROJECT_BINARY_DIR}/assets")

# add pre-compiled shader files
file (REMOVE ${SHADER_RESOURCE_FILE})
include_shader_resource ("src/shaders/basic.frag")
include_shader_resource ("src/shaders/basic.vert")
include_shader_resource ("src/shaders/notex.frag")
include_shader_resource ("src/shaders/notex.vert")
