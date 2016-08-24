find_package(Java REQUIRED)
include(UseJava)

macro(add_example example_name jar_name)
    set(example_src "${example_name}.java")
    set(example_jar "${CMAKE_CURRENT_BINARY_DIR}/../../src/${jar_name}/upm_${jar_name}.jar")

    add_jar(${example_name} SOURCES ${example_src} INCLUDE_JARS ${example_jar})
    add_dependencies(${example_name} javaupm_${jar_name})
endmacro()

macro(add_example_with_path example_name jar_path jar_name)
    set(example_src "${example_name}.java")
    set(example_jar "${CMAKE_CURRENT_BINARY_DIR}/../../src/${jar_path}/upm_${jar_name}.jar")

    add_jar(${example_name} SOURCES ${example_src} INCLUDE_JARS ${example_jar})
    add_dependencies(${example_name} javaupm_${jar_name})
endmacro()

add_example(BH1750_Example bh1750)
add_example(MMA7361_Example mma7361)
add_example(VK2828U7_Example vk2828u7)
