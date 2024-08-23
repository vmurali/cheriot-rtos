-- Copyright Microsoft and CHERIoT Contributors.
-- SPDX-License-Identifier: MIT

set_project("CHERIoT Compartmentalised hello world (more secure)")
sdkdir = "../../sdk"
includes(sdkdir)
set_toolchains("cheriot-clang")

-- Support libraries
includes(path.join(sdkdir, "lib"))

option("board")
    set_default("sail")

compartment("service")
    -- This compartment uses C++ thread-safe static initialisation and so
    -- depends on the C++ runtime.
    add_files("service.cc")

compartment("caller1")
   add_files("caller1.cc")

compartment("caller2")
   add_files("caller2.cc")

-- Firmware image for the example.
firmware("sealing")
    -- Both compartments require memcpy
    add_deps("freestanding", "debug", "cxxrt", "crt", "locks")
    add_deps("caller1", "caller2", "service")
    on_load(function(target)
        target:values_set("board", "$(board)")
        target:values_set("threads", {
            {
                compartment = "caller1",
                priority = 1,
                entry_point = "entry",
                stack_size = 0x400,
                trusted_stack_frames = 3
            },
            {
                compartment = "caller2",
                priority = 1,
                entry_point = "entry",
                stack_size = 0x400,
                trusted_stack_frames = 3
            }
        }, {expand = false})
    end)
