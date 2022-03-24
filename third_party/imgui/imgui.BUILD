cc_library(
    name = "interface",
    hdrs = [
        "imconfig.h",
        "imgui.h",
        "imgui_internal.h",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
    ],
    includes = ["."],
)

cc_library(
    name = "opengl3_backend",
    srcs = [
        "backends/imgui_impl_opengl3.cpp",
    ],
    hdrs = [
        "backends/imgui_impl_opengl3.h",
        "backends/imgui_impl_opengl3_loader.h",
    ],
    strip_include_prefix = "backends/",
    deps = [
        ":interface",
    ],
)

cc_library(
    name = "glfw_backend",
    srcs = [
        "backends/imgui_impl_glfw.cpp",
    ],
    hdrs = [
        "backends/imgui_impl_glfw.h",
    ],
    strip_include_prefix = "backends/",
    deps = [
        ":interface",
        "@conancenter_glfw//:glfw",
    ],
)

cc_library(
    name = "opengl3_glfw",
    srcs = [
        "imgui.cpp",
        "imgui_demo.cpp",
        "imgui_draw.cpp",
        "imgui_tables.cpp",
        "imgui_widgets.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":glfw_backend",
        ":interface",
        ":opengl3_backend",
    ],
)
