# Copyright 2014 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

{
  'variables': {
    'cobalt_code': 1,
  },
  'targets': [
    {
      'target_name': 'layout',
      'type': 'static_library',
      'sources': [
        'box.cc',
        'box.h',
        'box_generator.cc',
        'box_generator.h',
        'cascaded_style.cc',
        'cascaded_style.h',
        'computed_style.cc',
        'computed_style.h',
        'containing_block.cc',
        'containing_block.h',
        'declared_style.cc',
        'declared_style.h',
        'html_elements.cc',
        'html_elements.h',
        'initial_style.cc',
        'initial_style.h',
        'layout.cc',
        'layout.h',
        'layout_manager.cc',
        'layout_manager.h',
        'specified_style.cc',
        'specified_style.h',
        'text_box.cc',
        'text_box.h',
        'used_style.cc',
        'used_style.h',
      ],
      'dependencies': [
        '<(DEPTH)/cobalt/base/base.gyp:base',
        '<(DEPTH)/cobalt/dom/dom.gyp:dom',
        '<(DEPTH)/cobalt/render_tree/render_tree.gyp:render_tree',
        'embed_resources_as_header_files',
      ],
    },

    {
      # This target takes all files in the embedded_resources directory (e.g.
      # the user agent style sheet) and embeds them as header files for
      # inclusion into the binary.
      'target_name': 'embed_resources_as_header_files',
      'type': 'none',
      # Because we generate a header, we must set the hard_dependency flag.
      'hard_dependency': 1,
      'variables': {
        'script_path': '<(DEPTH)/lbshell/build/generate_data_header.py',
        'output_path': '<(SHARED_INTERMEDIATE_DIR)/cobalt/layout/embedded_resources.h',
        'input_directory': 'embedded_resources',
      },
      'sources': [
        '<(input_directory)/user_agent_style_sheet.css',
      ],
      'actions': [
        {
          'action_name': 'embed_resources_as_header_files',
          'inputs': [
            '<(script_path)',
            '<@(_sources)',
          ],
          'outputs': [
            '<(output_path)',
          ],
          'action': ['python', '<(script_path)', 'LayoutEmbeddedResources', '<(input_directory)', '<(output_path)'],
          'message': 'Embedding layout resources in "<(input_directory)" into header file, "<(output_path)".',
          'msvs_cygwin_shell': 1,
        },
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          '<(SHARED_INTERMEDIATE_DIR)',
        ],
      },
    },

    {
      'target_name': 'layout_test',
      'type': '<(gtest_target_type)',
      'sources': [
        'declared_style_test.cc',
        'layout_test.cc',
      ],
      'dependencies': [
        '<(DEPTH)/base/base.gyp:run_all_unittests',
        '<(DEPTH)/testing/gmock.gyp:gmock',
        '<(DEPTH)/testing/gtest.gyp:gtest',
        'layout',
      ],
    },

    {
      'target_name': 'layout_test_deploy',
      'type': 'none',
      'dependencies': [
        'layout_test',
      ],
      'variables': {
        'executable_name': 'layout_test',
      },
      'includes': [ '../build/deploy.gypi' ],
    },
  ],
}
