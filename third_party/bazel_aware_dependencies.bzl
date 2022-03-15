load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

def load_bazel_dependencies():
    if not native.existing_rule("com_google_ortools"):
        native.local_repository(
            name = "com_google_ortools",
            path = "/home/james/git/or-tools",
        )
        # http_archive(
        #     name = "com_google_ortools",
        #     sha256 = "5337935ea1fa010bb62cf0fc8bedd6de07dda77bff3db7a0f6a36c84c7bd58db",
        #     strip_prefix = "or-tools-9.2",
        #     urls = ["https://github.com/google/or-tools/archive/v9.2.tar.gz"],
        # )

    if not native.existing_rule("com_google_googletest"):
        http_archive(
            name = "com_google_googletest",
            sha256 = "5cf189eb6847b4f8fc603a3ffff3b0771c08eec7dd4bd961bfd45477dd13eb73",
            strip_prefix = "googletest-609281088cfefc76f9d0ce82e1ff6c30cc3591e5",
            urls = ["https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip"],
        )

    if not native.existing_rule("com_github_gflags_gflags"):
        http_archive(
            name = "com_github_gflags_gflags",
            sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
            strip_prefix = "gflags-2.2.2",
            urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
        )

    if not native.existing_rule("com_github_google_glog"):
        http_archive(
            name = "com_github_google_glog",
            sha256 = "21bc744fb7f2fa701ee8db339ded7dce4f975d0d55837a97be7d46e8382dea5a",
            strip_prefix = "glog-0.5.0",
            urls = ["https://github.com/google/glog/archive/v0.5.0.zip"],
        )

    if not native.existing_rule("rules_proto"):
        http_archive(
            name = "rules_proto",
            sha256 = "66bfdf8782796239d3875d37e7de19b1d94301e8972b3cbd2446b332429b4df1",
            strip_prefix = "rules_proto-4.0.0",
            urls = [
                "https://mirror.bazel.build/github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz",
                "https://github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz",
            ],
        )

    if not native.existing_rule("com_google_absl"):
        git_repository(
            name = "com_google_absl",
            commit = "2151058", # release 20211102.0
            remote = "https://github.com/abseil/abseil-cpp.git",
        )

    if not native.existing_rule("org_libpng_libpng"):
        http_archive(
            name = "org_libpng_libpng",
            build_file = "//third_party/libpng:libpng.BUILD",
            sha256 = "505e70834d35383537b6491e7ae8641f1a4bed1876dbfe361201fc80868d88ca",
            strip_prefix = "libpng-1.6.37",
            urls = [
                "https://download.sourceforge.net/libpng/libpng-1.6.37.tar.xz",
            ],
            patch_cmds = [
                "cp scripts/pnglibconf.h.prebuilt pnglibconf.h",
            ],
            patch_cmds_win = [
                "copy scripts\\pnglibconf.h.prebuilt pnglibconf.h",
            ],
        )

    if not native.existing_rule("com_github_ocornut_imgui"):
        http_archive(
            name = "com_github_ocornut_imgui",
            build_file = "//third_party/imgui:imgui.BUILD",
            sha256 = "b54ceb35bda38766e36b87c25edf7a1cd8fd2cb8c485b245aedca6fb85645a20",
            strip_prefix = "imgui-1.87",
            urls = [
                "https://github.com/ocornut/imgui/archive/v1.87.tar.gz",
            ],
        )

    if not native.existing_rule("scip"):
        new_git_repository(
            name = "scip",
            build_file = "@com_google_ortools//bazel:scip.BUILD",
            patches = ["@com_google_ortools//bazel:scip.patch"],
            commit = "6acb7222e1b871041445bee75fc05bd1bcaed089", # master from Jul 19, 2021
            remote = "https://github.com/scipopt/scip.git",
        )

    if not native.existing_rule("bliss"):
        http_archive(
            name = "bliss",
            build_file = "@com_google_ortools//bazel:bliss.BUILD",
            patches = ["@com_google_ortools//bazel:bliss-0.73.patch"],
            sha256 = "f57bf32804140cad58b1240b804e0dbd68f7e6bf67eba8e0c0fa3a62fd7f0f84",
            url = "https://github.com/google/or-tools/releases/download/v9.0/bliss-0.73.zip",
            #url = "http://www.tcs.hut.fi/Software/bliss/bliss-0.73.zip",
        )
