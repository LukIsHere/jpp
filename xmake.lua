add_rules("mode.release")

target("jpp")
    set_kind("static")
    add_includedirs(include/)
    add_files("src/*.cpp")
target_end()

target("jpp-test")
    set_kind("binary")
    add_includedirs(include/)
    add_files("test.cpp")
    add_packages("jpp")
target_end()