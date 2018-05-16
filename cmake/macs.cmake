#-------------------------------------------------
# CMake macros
#-------------------------------------------------

if(MSVC)
	set(DEFINE_PREFIX "/D")
else()
	set(DEFINE_PREFIX "-D")
endif()

macro(add_def tgt def)
	set(${tgt} "${${tgt}} ${DEFINE_PREFIX}${def}")
endmacro(add_def)


macro(add_flags tgt flag)
	set(${tgt} "${${tgt}} ${flag}")
endmacro(add_flags)



