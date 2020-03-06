
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if(CMAKE_VERSION VERSION_LESS 3.11)
	set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")

	include(DownloadProject)
	message(STATUS "Downloading PyBind11 if needed")
	download_project(PROJ                pybind
			GIT_REPOSITORY      https://github.com/pybind/pybind11.git
			GIT_TAG             v2.3.0
			UPDATE_DISCONNECTED 1
			QUIET
			)

	# Exports pybind11::module
	# And provides pybind11_add_module
	set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "")
	add_subdirectory(${pybind_SOURCE_DIR} ${pybind_SOURCE_DIR})
	unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
else()
	include(FetchContent)
	FetchContent_Declare(pybind
			GIT_REPOSITORY      https://github.com/pybind/pybind11.git
			GIT_TAG             v2.3.0)
	FetchContent_GetProperties(pybind)
	if(NOT pybind_POPULATED)
		FetchContent_Populate(pybind)
		set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE BOOL "")
		add_subdirectory(${pybind_SOURCE_DIR} ${pybind_BINARY_DIR} EXCLUDE_FROM_ALL)
		unset(CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
	endif()
endif()
