# Note: The buildbots evaluate this file with CWD set to the parent
# directory and assume that the root of the checkout is in ./v8/, so
# all paths in here must match this assumption.

use_relative_paths = True

gclient_gn_args_file = 'build/config/gclient_args.gni'
gclient_gn_args = [
  # TODO(https://crbug.com/1137662, https://crbug.com/1080854)
  # Remove when migration is complete.
  'checkout_fuchsia_for_arm64_host',
  'checkout_google_benchmark',
]

vars = {
  # Fetches only the SDK boot images which match at least one of the whitelist
  # entries in a comma-separated list.
  #
  # Only the X64 and ARM64 QEMU images are downloaded by default. Developers
  # that need to boot on other target architectures or devices can opt to
  # download more boot images. Example of images include:
  #
  # Emulation:
  #   qemu.x64, qemu.arm64
  # Hardware:
  #   generic.x64, generic.arm64
  #
  # Wildcards are supported (e.g. "qemu.*").
  'checkout_fuchsia_boot_images': "qemu.x64,qemu.arm64",

  # TODO(https://crbug.com/1137662, https://crbug.com/1080854)
  # Remove when migration is complete.
  # By default, do not check out files required to run fuchsia tests in
  # qemu on linux-arm64 machines.
  'checkout_fuchsia_for_arm64_host': False,

  'checkout_instrumented_libraries': False,
  'checkout_ittapi': False,
  # Fetch clang-tidy into the same bin/ directory as our clang binary.
  'checkout_clang_tidy': False,
  'chromium_url': 'https://chromium.googlesource.com',
  'android_url': 'https://android.googlesource.com',
  'download_gcmole': False,
  'download_jsfunfuzz': False,
  'check_v8_header_includes': False,

  'checkout_google_benchmark' : False,

  # GN CIPD package version.
  'gn_version': 'git_revision:dfcbc6fed0a8352696f92d67ccad54048ad182b3',

  # luci-go CIPD package version.
  'luci_go': 'git_revision:fd10124659e991321df2f8a5d3749687b54ceb0a',

  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling android_sdk_build-tools_version
  # and whatever else without interference from each other.
  'android_sdk_build-tools_version': '8LZujEmLjSh0g3JciDA3cslSptxKs9HOa_iUPXkOeYQC',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling android_sdk_emulator_version
  # and whatever else without interference from each other.
  'android_sdk_emulator_version': 'A4EvXZUIuQho0QRDJopMUpgyp6NA3aiDQjGKPUKbowMC',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling android_sdk_extras_version
  # and whatever else without interference from each other.
  'android_sdk_extras_version': 'ppQ4TnqDvBHQ3lXx5KPq97egzF5X2FFyOrVHkGmiTMQC',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling android_sdk_patcher_version
  # and whatever else without interference from each other.
  'android_sdk_patcher_version': 'I6FNMhrXlpB-E1lOhMlvld7xt9lBVNOO83KIluXDyA0C',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling android_sdk_platform-tools_version
  # and whatever else without interference from each other.
  'android_sdk_platform-tools_version': '8tF0AOj7Dwlv4j7_nfkhxWB0jzrvWWYjEIpirt8FIWYC',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling android_sdk_platforms_version
  # and whatever else without interference from each other.
  'android_sdk_platforms_version': 'YMUu9EHNZ__2Xcxl-KsaSf-dI5TMt_P62IseUVsxktMC',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling android_sdk_sources_version
  # and whatever else without interference from each other.
  'android_sdk_sources_version': '4gxhM8E62bvZpQs7Q3d0DinQaW0RLCIefhXrQBFkNy8C',
  # Three lines of non-changing comments so that
  # the commit queue can handle CLs rolling android_sdk_tools-lint_version
  # and whatever else without interference from each other.
  'android_sdk_cmdline-tools_version': 'V__2Ycej-H2-6AcXX5A3gi7sIk74SuN44PBm2uC_N1sC',
}

deps = {
  'build':
    Var('chromium_url') + '/chromium/src/build.git' + '@' + '446bf3e5a00bfe4fd99d91cb76ec3b3a7b34d226',
  'third_party/depot_tools':
    Var('chromium_url') + '/chromium/tools/depot_tools.git' + '@' + '5fe664f150beaf71104ce7787560fabdb55ebf5b',
  'third_party/icu':
    Var('chromium_url') + '/chromium/deps/icu.git' + '@' + 'e05b663d1c50b4e9ecc3ff9325f5158f1d071471',
  'third_party/instrumented_libraries':
    Var('chromium_url') + '/chromium/src/third_party/instrumented_libraries.git' + '@' + '0964a78c832d1d0f2669b020b073c38f67509cf2',
  'buildtools':
    Var('chromium_url') + '/chromium/src/buildtools.git' + '@' + '4c78ef9c38b683c5c5cbac70445378c2362cebfc',
  'buildtools/clang_format/script':
    Var('chromium_url') + '/external/github.com/llvm/llvm-project/clang/tools/clang-format.git' + '@' + '99803d74e35962f63a775f29477882afd4d57d94',
  'buildtools/linux64': {
    'packages': [
      {
        'package': 'gn/gn/linux-amd64',
        'version': Var('gn_version'),
      }
    ],
    'dep_type': 'cipd',
    'condition': 'host_os == "linux"',
  },
  'buildtools/mac': {
    'packages': [
      {
        'package': 'gn/gn/mac-amd64',
        'version': Var('gn_version'),
      }
    ],
    'dep_type': 'cipd',
    'condition': 'host_os == "mac"',
  },
  'buildtools/third_party/libc++/trunk':
    Var('chromium_url') + '/external/github.com/llvm/llvm-project/libcxx.git' + '@' + '8fa87946779682841e21e2da977eccfb6cb3bded',
  'buildtools/third_party/libc++abi/trunk':
    Var('chromium_url') + '/external/github.com/llvm/llvm-project/libcxxabi.git' + '@' + '196ba1aaa8ac285d94f4ea8d9836390a45360533',
  'buildtools/third_party/libunwind/trunk':
    Var('chromium_url') + '/external/github.com/llvm/llvm-project/libunwind.git' + '@' + 'a2cc4f8c554dedcb0c64cac5511b19c43f1f3d32',
  'buildtools/win': {
    'packages': [
      {
        'package': 'gn/gn/windows-amd64',
        'version': Var('gn_version'),
      }
    ],
    'dep_type': 'cipd',
    'condition': 'host_os == "win"',
  },
  'base/trace_event/common':
    Var('chromium_url') + '/chromium/src/base/trace_event/common.git' + '@' + '7af6071eddf11ad91fbd5df54138f9d3c6d980d5',
  'third_party/android_ndk': {
    'url': Var('chromium_url') + '/android_ndk.git' + '@' + '401019bf85744311b26c88ced255cd53401af8b7',
    'condition': 'checkout_android',
  },
  'third_party/android_platform': {
    'url': Var('chromium_url') + '/chromium/src/third_party/android_platform.git' + '@' + 'fdaa5e5b9f6384c3011ca8479d672e47528f743f',
    'condition': 'checkout_android',
  },
  'third_party/android_sdk/public': {
      'packages': [
          {
              'package': 'chromium/third_party/android_sdk/public/build-tools/30.0.1',
              'version': Var('android_sdk_build-tools_version'),
          },
          {
              'package': 'chromium/third_party/android_sdk/public/emulator',
              'version': Var('android_sdk_emulator_version'),
          },
          {
              'package': 'chromium/third_party/android_sdk/public/extras',
              'version': Var('android_sdk_extras_version'),
          },
          {
              'package': 'chromium/third_party/android_sdk/public/patcher',
              'version': Var('android_sdk_patcher_version'),
          },
          {
              'package': 'chromium/third_party/android_sdk/public/platform-tools',
              'version': Var('android_sdk_platform-tools_version'),
          },
          {
              'package': 'chromium/third_party/android_sdk/public/platforms/android-30',
              'version': Var('android_sdk_platforms_version'),
          },
          {
              'package': 'chromium/third_party/android_sdk/public/sources/android-29',
              'version': Var('android_sdk_sources_version'),
          },
          {
              'package': 'chromium/third_party/android_sdk/public/cmdline-tools',
              'version': Var('android_sdk_cmdline-tools_version'),
          },
      ],
      'condition': 'checkout_android',
      'dep_type': 'cipd',
  },
  'third_party/catapult': {
    'url': Var('chromium_url') + '/catapult.git' + '@' + '81c9d30d7f1b3c1ab0f1856761f738cc81741322',
    'condition': 'checkout_android',
  },
  'third_party/colorama/src': {
    'url': Var('chromium_url') + '/external/colorama.git' + '@' + '799604a1041e9b3bc5d2789ecbd7e8db2e18e6b8',
    'condition': 'checkout_android',
  },
  'third_party/fuchsia-sdk': {
    'url': Var('chromium_url') + '/chromium/src/third_party/fuchsia-sdk.git' + '@' + 'efa46583d89ea8c97523281d9f52a0d96472114d',
    'condition': 'checkout_fuchsia',
  },
  'third_party/googletest/src':
    Var('chromium_url') + '/external/github.com/google/googletest.git' + '@' + '1e315c5b1a62707fac9b8f1d4e03180ee7507f98',
  'third_party/google_benchmark/src': {
    'url': Var('chromium_url') + '/external/github.com/google/benchmark.git' + '@' + '7f27afe83b82f3a98baf58ef595814b9d42a5b2b',
    'condition': 'checkout_google_benchmark',
  },
  'third_party/jinja2':
    Var('chromium_url') + '/chromium/src/third_party/jinja2.git' + '@' + '11b6b3e5971d760bd2d310f77643f55a818a6d25',
  'third_party/markupsafe':
    Var('chromium_url') + '/chromium/src/third_party/markupsafe.git' + '@' + '0944e71f4b2cb9a871bcbe353f95e889b64a611a',
  'tools/swarming_client':
    Var('chromium_url') + '/infra/luci/client-py.git' + '@' + 'a32a1607f6093d338f756c7e7c7b4333b0c50c9c',
  'test/benchmarks/data':
    Var('chromium_url') + '/v8/deps/third_party/benchmarks.git' + '@' + '05d7188267b4560491ff9155c5ee13e207ecd65f',
  'test/mozilla/data':
    Var('chromium_url') + '/v8/deps/third_party/mozilla-tests.git' + '@' + 'f6c578a10ea707b1a8ab0b88943fe5115ce2b9be',
  'test/test262/data':
    Var('chromium_url') + '/external/github.com/tc39/test262.git' + '@' + 'f6034ebe9fb92d4d3dea644b9225bdc18b44a7ab',
  'test/test262/harness':
    Var('chromium_url') + '/external/github.com/test262-utils/test262-harness-py.git' + '@' + '278bcfaed0dcaa13936831fb1769d15e7c1e3b2b',
  'third_party/qemu-linux-x64': {
      'packages': [
          {
              'package': 'fuchsia/qemu/linux-amd64',
              'version': '9cc486c5b18a0be515c39a280ca9a309c54cf994'
          },
      ],
      'condition': 'host_os == "linux" and checkout_fuchsia',
      'dep_type': 'cipd',
  },
  'third_party/qemu-mac-x64': {
      'packages': [
          {
              'package': 'fuchsia/qemu/mac-amd64',
              'version': '2d3358ae9a569b2d4a474f498b32b202a152134f'
          },
      ],
      'condition': 'host_os == "mac" and checkout_fuchsia',
      'dep_type': 'cipd',
  },
  'third_party/aemu-linux-x64': {
      'packages': [
          {
              'package': 'fuchsia/third_party/aemu/linux-amd64',
              'version': 'qI8e328VwkWv64EapCvG3Xj9_hDpKQFuJWeVdUHz7W0C'
          },
      ],
      'condition': 'host_os == "linux" and checkout_fuchsia',
      'dep_type': 'cipd',
  },
  'third_party/aemu-mac-x64': {
      'packages': [
          {
              'package': 'fuchsia/third_party/aemu/mac-amd64',
              'version': 'T9bWxf8aUC5TwCFgPxpuW29Mfy-7Z9xCfXB9QO8MfU0C'
          },
      ],
      'condition': 'host_os == "mac" and checkout_fuchsia',
      'dep_type': 'cipd',
  },
  'tools/clang':
    Var('chromium_url') + '/chromium/src/tools/clang.git' + '@' + 'cfd0f628093b7382ac054fb33e23fa9d9a278bc3',
  'tools/luci-go': {
      'packages': [
        {
          'package': 'infra/tools/luci/isolate/${{platform}}',
          'version': Var('luci_go'),
        },
        {
          'package': 'infra/tools/luci/isolated/${{platform}}',
          'version': Var('luci_go'),
        },
        {
          'package': 'infra/tools/luci/swarming/${{platform}}',
          'version': Var('luci_go'),
        },
      ],
      'condition': 'host_cpu != "s390" and host_os != "aix"',
      'dep_type': 'cipd',
  },
  'tools/clang/dsymutil': {
    'packages': [
      {
        'package': 'chromium/llvm-build-tools/dsymutil',
        'version': 'M56jPzDv1620Rnm__jTMYS62Zi8rxHVq7yw0qeBFEgkC',
      }
    ],
    'condition': 'checkout_mac',
    'dep_type': 'cipd',
  },
  'third_party/perfetto':
    Var('android_url') + '/platform/external/perfetto.git' + '@' + 'aa4385bc5997ecad4c633885e1b331b1115012fb',
  'third_party/protobuf':
    Var('chromium_url') + '/external/github.com/google/protobuf'+ '@' + '6a59a2ad1f61d9696092f79b6d74368b4d7970a3',
  'third_party/zlib':
    Var('chromium_url') + '/chromium/src/third_party/zlib.git'+ '@' + '348acca950b1d6de784a954f4fda0952046c652c',
  'third_party/jsoncpp/source':
    Var('chromium_url') + '/external/github.com/open-source-parsers/jsoncpp.git'+ '@' + '9059f5cad030ba11d37818847443a53918c327b1',
  'third_party/ittapi': {
    # Force checkout ittapi libraries to pass v8 header includes check on
    # bots that has check_v8_header_includes enabled.
    'url': Var('chromium_url') + '/external/github.com/intel/ittapi' + '@' + 'b4ae0122ba749163096058b4f1bb065bf4a7de94',
    'condition': "checkout_ittapi or check_v8_header_includes",
  },
  'third_party/requests': {
      'url': Var('chromium_url') + '/external/github.com/kennethreitz/requests.git' + '@' + 'bfb93d4b7d269a8735f1b216093e7e9a9fdc4517',
      'condition': 'checkout_android',
  },
}

include_rules = [
  # Everybody can use some things.
  '+include',
  '+unicode',
  '+third_party/fdlibm',
  '+third_party/ittapi/include'
]

# checkdeps.py shouldn't check for includes in these directories:
skip_child_includes = [
  'build',
  'third_party',
]

hooks = [
  {
    # Ensure that the DEPS'd "depot_tools" has its self-update capability
    # disabled.
    'name': 'disable_depot_tools_selfupdate',
    'pattern': '.',
    'action': [
        'python',
        'third_party/depot_tools/update_depot_tools_toggle.py',
        '--disable',
    ],
  },
  {
    # This clobbers when necessary (based on get_landmines.py). It must be the
    # first hook so that other things that get/generate into the output
    # directory will not subsequently be clobbered.
    'name': 'landmines',
    'pattern': '.',
    'action': [
        'python',
        'build/landmines.py',
        '--landmine-scripts',
        'tools/get_landmines.py',
    ],
  },
  # Pull clang-format binaries using checked-in hashes.
  {
    'name': 'clang_format_win',
    'pattern': '.',
    'condition': 'host_os == "win"',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=win32',
                '--no_auth',
                '--bucket', 'chromium-clang-format',
                '-s', 'buildtools/win/clang-format.exe.sha1',
    ],
  },
  {
    'name': 'clang_format_mac',
    'pattern': '.',
    'condition': 'host_os == "mac"',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=darwin',
                '--no_auth',
                '--bucket', 'chromium-clang-format',
                '-s', 'buildtools/mac/clang-format.sha1',
    ],
  },
  {
    'name': 'clang_format_linux',
    'pattern': '.',
    'condition': 'host_os == "linux"',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--platform=linux*',
                '--no_auth',
                '--bucket', 'chromium-clang-format',
                '-s', 'buildtools/linux64/clang-format.sha1',
    ],
  },
  {
    'name': 'gcmole',
    'pattern': '.',
    'condition': 'download_gcmole',
    'action': [ 'download_from_google_storage',
                '--bucket', 'chrome-v8-gcmole',
                '-u', '--no_resume',
                '-s', 'tools/gcmole/gcmole-tools.tar.gz.sha1',
                '--platform=linux*',
    ],
  },
  {
    'name': 'jsfunfuzz',
    'pattern': '.',
    'condition': 'download_jsfunfuzz',
    'action': [ 'download_from_google_storage',
                '--bucket', 'chrome-v8-jsfunfuzz',
                '-u', '--no_resume',
                '-s', 'tools/jsfunfuzz/jsfunfuzz.tar.gz.sha1',
                '--platform=linux*',
    ],
  },
  {
    'name': 'wasm_spec_tests',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--no_auth',
                '-u',
                '--bucket', 'v8-wasm-spec-tests',
                '-s', 'test/wasm-spec-tests/tests.tar.gz.sha1',
    ],
  },
  {
    'name': 'wasm_js',
    'pattern': '.',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--no_auth',
                '-u',
                '--bucket', 'v8-wasm-spec-tests',
                '-s', 'test/wasm-js/tests.tar.gz.sha1',
    ],
  },
  {
    'name': 'sysroot_arm',
    'pattern': '.',
    'condition': '(checkout_linux and checkout_arm)',
    'action': ['python', 'build/linux/sysroot_scripts/install-sysroot.py',
               '--arch=arm'],
  },
  {
    'name': 'sysroot_arm64',
    'pattern': '.',
    'condition': '(checkout_linux and checkout_arm64)',
    'action': ['python', 'build/linux/sysroot_scripts/install-sysroot.py',
               '--arch=arm64'],
  },
  {
    'name': 'sysroot_x86',
    'pattern': '.',
    'condition': '(checkout_linux and (checkout_x86 or checkout_x64))',
    'action': ['python', 'build/linux/sysroot_scripts/install-sysroot.py',
               '--arch=x86'],
  },
  {
    'name': 'sysroot_x64',
    'pattern': '.',
    'condition': 'checkout_linux and checkout_x64',
    'action': ['python', 'build/linux/sysroot_scripts/install-sysroot.py',
               '--arch=x64'],
  },
  {
    'name': 'msan_chained_origins',
    'pattern': '.',
    'condition': 'checkout_instrumented_libraries',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--no_auth',
                '--bucket', 'chromium-instrumented-libraries',
                '-s', 'third_party/instrumented_libraries/binaries/msan-chained-origins-trusty.tgz.sha1',
              ],
  },
  {
    'name': 'msan_no_origins',
    'pattern': '.',
    'condition': 'checkout_instrumented_libraries',
    'action': [ 'download_from_google_storage',
                '--no_resume',
                '--no_auth',
                '--bucket', 'chromium-instrumented-libraries',
                '-s', 'third_party/instrumented_libraries/binaries/msan-no-origins-trusty.tgz.sha1',
              ],
  },
  {
    # Update the Windows toolchain if necessary.
    'name': 'win_toolchain',
    'pattern': '.',
    'condition': 'checkout_win',
    'action': ['python', 'build/vs_toolchain.py', 'update'],
  },
  {
    # Update the Mac toolchain if necessary.
    'name': 'mac_toolchain',
    'pattern': '.',
    'condition': 'checkout_mac',
    'action': ['python', 'build/mac_toolchain.py'],
  },
  {
    # Note: On Win, this should run after win_toolchain, as it may use it.
    'name': 'clang',
    'pattern': '.',
    # clang not supported on aix
    'condition': 'host_os != "aix"',
    'action': ['python', 'tools/clang/scripts/update.py'],
  },
  {
    'name': 'clang_tidy',
    'pattern': '.',
    'condition': 'checkout_clang_tidy',
    'action': ['python', 'tools/clang/scripts/update.py',
               '--package=clang-tidy'],
  },
  {
    # Update LASTCHANGE.
    'name': 'lastchange',
    'pattern': '.',
    'action': ['python', 'build/util/lastchange.py',
               '-o', 'build/util/LASTCHANGE'],
  },
  {
    'name': 'Download Fuchsia SDK',
    'pattern': '.',
    'condition': 'checkout_fuchsia',
    'action': [
      'python',
      'build/fuchsia/update_sdk.py',
    ],
  },
  {
    'name': 'Download Fuchsia system images',
    'pattern': '.',
    'condition': 'checkout_fuchsia',
    'action': [
      'python',
      'build/fuchsia/update_images.py',
      '--boot-images={checkout_fuchsia_boot_images}',
    ],
  },
  {
    # Mac doesn't use lld so it's not included in the default clang bundle
    # there. However, lld is need in Fuchsia cross builds, so
    # download it there.
    # Should run after the clang hook.
    'name': 'lld/mac',
    'pattern': '.',
    'condition': 'host_os == "mac" and checkout_fuchsia',
    'action': ['python', 'tools/clang/scripts/update.py',
               '--package=lld_mac'],
  },
  {
      # Mac does not have llvm-objdump, download it for cross builds in Fuchsia.
    'name': 'llvm-objdump',
    'pattern': '.',
    'condition': 'host_os == "mac" and checkout_fuchsia',
    'action': ['python', 'tools/clang/scripts/update.py',
               '--package=objdump'],
  },
  # Download and initialize "vpython" VirtualEnv environment packages.
  {
    'name': 'vpython_common',
    'pattern': '.',
    'condition': 'checkout_android',
    'action': [ 'vpython',
                '-vpython-spec', '.vpython',
                '-vpython-tool', 'install',
    ],
  },
  {
    'name': 'check_v8_header_includes',
    'pattern': '.',
    'condition': 'check_v8_header_includes',
    'action': [
      'python',
      'tools/generate-header-include-checks.py',
    ],
  },
]
