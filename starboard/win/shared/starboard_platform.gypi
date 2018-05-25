# Copyright 2016 Google Inc. All Rights Reserved.
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
  'includes': [
    '<(DEPTH)/starboard/shared/starboard/player/filter/player_filter.gypi',
  ],
  'variables': {
    'sb_pedantic_warnings': 1,
    'winrt%': 1,

    # TODO: Move this and the win32 dependencies below to a shared/win32/starboard_platform.gypi?
    'uwp_incompatible_win32': [
      '<(DEPTH)/starboard/shared/starboard/net_args.cc',
      '<(DEPTH)/starboard/shared/starboard/net_args.h',
      '<(DEPTH)/starboard/shared/win32/application_win32_key_event.cc',
      '<(DEPTH)/starboard/shared/win32/application_win32.cc',
      '<(DEPTH)/starboard/shared/win32/dialog.cc',
      '<(DEPTH)/starboard/shared/win32/get_home_directory.cc',
      '<(DEPTH)/starboard/shared/win32/log_file_impl.cc',
      '<(DEPTH)/starboard/shared/win32/log_file_impl.h',
      '<(DEPTH)/starboard/shared/win32/log_raw.cc',
      '<(DEPTH)/starboard/shared/win32/log_raw_format.cc',
      '<(DEPTH)/starboard/shared/win32/media_is_audio_supported.cc',
      '<(DEPTH)/starboard/shared/win32/media_is_video_supported.cc',
      '<(DEPTH)/starboard/shared/win32/minidump.cc',
      '<(DEPTH)/starboard/shared/win32/minidump.h',
      '<(DEPTH)/starboard/shared/win32/player_components_impl.cc',
      '<(DEPTH)/starboard/shared/win32/playready_license.cc',
      '<(DEPTH)/starboard/shared/win32/starboard_main.cc',
      '<(DEPTH)/starboard/shared/win32/system_clear_platform_error.cc',
      '<(DEPTH)/starboard/shared/win32/system_get_device_type.cc',
      '<(DEPTH)/starboard/shared/win32/system_get_property.cc',
      '<(DEPTH)/starboard/shared/win32/system_get_total_cpu_memory.cc',
      '<(DEPTH)/starboard/shared/win32/system_get_used_cpu_memory.cc',
      '<(DEPTH)/starboard/shared/win32/system_raise_platform_error.cc',
      '<(DEPTH)/starboard/shared/win32/system_symbolize.cc',
      '<(DEPTH)/starboard/shared/win32/window_create.cc',
      '<(DEPTH)/starboard/shared/win32/window_destroy.cc',
      '<(DEPTH)/starboard/shared/win32/window_get_platform_handle.cc',
      '<(DEPTH)/starboard/shared/win32/window_get_size.cc',
      '<(DEPTH)/starboard/shared/win32/window_internal.cc',
      '<(DEPTH)/starboard/shared/win32/window_intsdfdsfernal.h',
      '<(DEPTH)/starboard/shared/win32/window_set_default_options.cc',
    ],
    'win32_shared_drm_files': [
      '<(DEPTH)/starboard/shared/starboard/drm/drm_close_session.cc',
      '<(DEPTH)/starboard/shared/starboard/drm/drm_destroy_system.cc',
      '<(DEPTH)/starboard/shared/starboard/drm/drm_generate_session_update_request.cc',
      '<(DEPTH)/starboard/shared/starboard/drm/drm_system_internal.h',
      '<(DEPTH)/starboard/shared/starboard/drm/drm_update_session.cc',

      '<(DEPTH)/starboard/shared/stub/drm_is_server_certificate_updatable.cc',
      '<(DEPTH)/starboard/shared/stub/drm_update_server_certificate.cc',

      '<(DEPTH)/starboard/shared/win32/drm_create_system.cc',
      '<(DEPTH)/starboard/shared/win32/drm_system_playready.cc',
      '<(DEPTH)/starboard/shared/win32/drm_system_playready.h',
      '<(DEPTH)/starboard/shared/win32/wrm_header.cc',
      '<(DEPTH)/starboard/shared/win32/wrm_header.h',
    ],

    # Please keep this in sync with xb1/shared/starboard_platform.gypi.
    'win32_media_player_files': [
      '<(DEPTH)/starboard/shared/win32/atomic_queue.h',
      '<(DEPTH)/starboard/shared/win32/audio_decoder.cc',
      '<(DEPTH)/starboard/shared/win32/audio_decoder.h',
      '<(DEPTH)/starboard/shared/win32/audio_decoder_thread.cc',
      '<(DEPTH)/starboard/shared/win32/audio_decoder_thread.h',
      '<(DEPTH)/starboard/shared/win32/audio_transform.cc',
      '<(DEPTH)/starboard/shared/win32/audio_transform.h',
      '<(DEPTH)/starboard/shared/win32/decode_target_internal.cc',
      '<(DEPTH)/starboard/shared/win32/decode_target_internal.h',
      '<(DEPTH)/starboard/shared/win32/decrypting_decoder.cc',
      '<(DEPTH)/starboard/shared/win32/decrypting_decoder.h',
      '<(DEPTH)/starboard/shared/win32/dx_context_video_decoder.cc',
      '<(DEPTH)/starboard/shared/win32/dx_context_video_decoder.h',
      '<(DEPTH)/starboard/shared/win32/hardware_decode_target_internal.cc',
      '<(DEPTH)/starboard/shared/win32/hardware_decode_target_internal.h',
      '<(DEPTH)/starboard/shared/win32/media_common.cc',
      '<(DEPTH)/starboard/shared/win32/media_common.h',
      '<(DEPTH)/starboard/shared/win32/media_foundation_utils.cc',
      '<(DEPTH)/starboard/shared/win32/media_foundation_utils.h',
      '<(DEPTH)/starboard/shared/win32/media_is_supported.cc',
      '<(DEPTH)/starboard/shared/win32/media_transform.cc',
      '<(DEPTH)/starboard/shared/win32/media_transform.h',
      '<(DEPTH)/starboard/shared/win32/video_decoder.cc',
      '<(DEPTH)/starboard/shared/win32/video_decoder.h',
      '<(DEPTH)/starboard/shared/win32/win32_audio_decoder.cc',
      '<(DEPTH)/starboard/shared/win32/win32_audio_decoder.h',
    ],
    'win32_shared_media_player_files': [
      '<@(filter_based_player_sources)',
      '<(DEPTH)/starboard/shared/starboard/media/codec_util.cc',
      '<(DEPTH)/starboard/shared/starboard/media/codec_util.h',
      '<(DEPTH)/starboard/shared/starboard/media/media_can_play_mime_and_key_system.cc',
      '<(DEPTH)/starboard/shared/starboard/media/media_get_audio_output_count_stereo_only.cc',
      '<(DEPTH)/starboard/shared/starboard/media/media_util.cc',
      '<(DEPTH)/starboard/shared/starboard/media/media_util.h',
      '<(DEPTH)/starboard/shared/starboard/player/decoded_audio_internal.cc',
      '<(DEPTH)/starboard/shared/starboard/player/input_buffer_internal.cc',
      '<(DEPTH)/starboard/shared/starboard/player/input_buffer_internal.h',
      '<(DEPTH)/starboard/shared/starboard/player/job_queue.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_create.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_destroy.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_get_current_frame.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_get_info.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_get_info2.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_internal.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_output_mode_supported.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_output_mode_supported.h',
      '<(DEPTH)/starboard/shared/starboard/player/player_seek.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_seek2.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_set_bounds.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_set_playback_rate.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_set_volume.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_worker.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_write_end_of_stream.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_write_sample.cc',
      '<(DEPTH)/starboard/shared/starboard/player/player_write_sample2.cc',
      '<(DEPTH)/starboard/shared/stub/media_is_transfer_characteristics_supported.cc',
    ],

    'win32_shared_misc_files': [
      '<(DEPTH)/starboard/common/thread.cc',
      '<(DEPTH)/starboard/common/thread.h',
      '<(DEPTH)/starboard/shared/starboard/net_log.cc',
      '<(DEPTH)/starboard/shared/starboard/net_log.h',
    ],

    'starboard_platform_dependent_files': [
      '<@(win32_shared_misc_files)',
      '<@(win32_media_player_files)',
      '<@(win32_shared_drm_files)',
      '<@(win32_shared_media_player_files)',
    ]
  },
  'targets': [
    {
      'target_name': 'starboard_platform',
      'type': 'static_library',
      'hard_dependency': 1,
      'msvs_settings': {
        'VCCLCompilerTool': {
          'AdditionalIncludeDirectories': [
            '<(DEPTH)/third_party/angle/include',
            '<(DEPTH)/third_party/angle/include/EGL',
            '<(DEPTH)/third_party/angle/include/GLES2',
            '<(DEPTH)/third_party/angle/include/KHR',
          ],
          'AdditionalOptions': [
            '/EHsc',         # C++ exceptions (required with /ZW)
            '/FU"<(visual_studio_install_path)/lib/x86/store/references/platform.winmd"',
            '/FU"<(windows_sdk_path)/References/<(windows_sdk_version)/Windows.Foundation.FoundationContract/3.0.0.0/Windows.Foundation.FoundationContract.winmd"',
            '/FU"<(windows_sdk_path)/References/<(windows_sdk_version)/Windows.Foundation.UniversalApiContract/4.0.0.0/Windows.Foundation.UniversalApiContract.winmd"',
          ]
        }
      },
      'conditions': [
        ['winrt==1', {
          'msvs_settings': {
            'VCCLCompilerTool': {
              'AdditionalOptions': [
                '/ZW',           # Windows Runtime
                '/ZW:nostdlib',  # Windows Runtime, no default #using
              ],
            },
          },
          'defines': [
            # VS2017 always defines this for UWP apps
            'WINAPI_FAMILY=WINAPI_FAMILY_APP',
            # VS2017 always defines this for UWP apps
            '__WRL_NO_DEFAULT_LIB__',
          ]
        }]
      ],
      'sources': [
        '<(DEPTH)/starboard/shared/iso/character_is_alphanumeric.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_digit.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_hex_digit.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_space.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_upper.cc',
        '<(DEPTH)/starboard/shared/iso/character_to_lower.cc',
        '<(DEPTH)/starboard/shared/iso/character_to_upper.cc',
        '<(DEPTH)/starboard/shared/iso/double_absolute.cc',
        '<(DEPTH)/starboard/shared/iso/double_exponent.cc',
        '<(DEPTH)/starboard/shared/iso/double_floor.cc',
        '<(DEPTH)/starboard/shared/iso/double_is_finite.cc',
        '<(DEPTH)/starboard/shared/iso/double_is_nan.cc',
        '<(DEPTH)/starboard/shared/iso/memory_compare.cc',
        '<(DEPTH)/starboard/shared/iso/memory_copy.cc',
        '<(DEPTH)/starboard/shared/iso/memory_find_byte.cc',
        '<(DEPTH)/starboard/shared/iso/memory_move.cc',
        '<(DEPTH)/starboard/shared/iso/memory_set.cc',
        '<(DEPTH)/starboard/shared/iso/string_compare.cc',
        '<(DEPTH)/starboard/shared/iso/string_compare_all.cc',
        '<(DEPTH)/starboard/shared/iso/string_find_character.cc',
        '<(DEPTH)/starboard/shared/iso/string_find_last_character.cc',
        '<(DEPTH)/starboard/shared/iso/string_find_string.cc',
        '<(DEPTH)/starboard/shared/iso/string_get_length.cc',
        '<(DEPTH)/starboard/shared/iso/string_get_length_wide.cc',
        '<(DEPTH)/starboard/shared/iso/string_parse_double.cc',
        '<(DEPTH)/starboard/shared/iso/string_parse_signed_integer.cc',
        '<(DEPTH)/starboard/shared/iso/string_parse_uint64.cc',
        '<(DEPTH)/starboard/shared/iso/string_parse_unsigned_integer.cc',
        '<(DEPTH)/starboard/shared/iso/string_scan.cc',
        '<(DEPTH)/starboard/shared/iso/system_binary_search.cc',
        '<(DEPTH)/starboard/shared/iso/system_sort.cc',
        '<(DEPTH)/starboard/shared/nouser/user_get_current.cc',
        '<(DEPTH)/starboard/shared/nouser/user_get_property.cc',
        '<(DEPTH)/starboard/shared/nouser/user_get_signed_in.cc',
        '<(DEPTH)/starboard/shared/nouser/user_internal.cc',
        '<(DEPTH)/starboard/shared/starboard/application.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/audio_sink_create.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/audio_sink_destroy.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/audio_sink_internal.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/audio_sink_internal.h',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/audio_sink_is_valid.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/stub_audio_sink_type.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/stub_audio_sink_type.h',
        '<(DEPTH)/starboard/shared/starboard/command_line.cc',
        '<(DEPTH)/starboard/shared/starboard/command_line.h',
        '<(DEPTH)/starboard/shared/starboard/event_cancel.cc',
        '<(DEPTH)/starboard/shared/starboard/event_schedule.cc',
        '<(DEPTH)/starboard/shared/starboard/file_mode_string_to_flags.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_close_record.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_delete_record.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_get_record_size.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_open_record.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_read_record.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_write_record.cc',
        '<(DEPTH)/starboard/shared/starboard/log_message.cc',
        '<(DEPTH)/starboard/shared/starboard/media/mime_type.cc',
        '<(DEPTH)/starboard/shared/starboard/media/mime_type.h',
        '<(DEPTH)/starboard/shared/starboard/string_concat.cc',
        '<(DEPTH)/starboard/shared/starboard/string_concat_wide.cc',
        '<(DEPTH)/starboard/shared/starboard/string_copy.cc',
        '<(DEPTH)/starboard/shared/starboard/string_copy_wide.cc',
        '<(DEPTH)/starboard/shared/starboard/string_duplicate.cc',
        '<(DEPTH)/starboard/shared/stub/accessibility_get_display_settings.cc',
        '<(DEPTH)/starboard/shared/stub/accessibility_get_text_to_speech_settings.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_create_transformer.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_destroy_transformer.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_get_tag.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_set_authenticated_data.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_set_initialization_vector.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_transform.cc',
        '<(DEPTH)/starboard/shared/stub/image_decode.cc',
        '<(DEPTH)/starboard/shared/stub/image_is_decode_supported.cc',
        '<(DEPTH)/starboard/shared/stub/system_get_total_gpu_memory.cc',
        '<(DEPTH)/starboard/shared/stub/system_get_used_gpu_memory.cc',
        '<(DEPTH)/starboard/shared/stub/system_has_capability.cc',
        '<(DEPTH)/starboard/shared/stub/system_hide_splash_screen.cc',
        '<(DEPTH)/starboard/shared/stub/time_zone_get_dst_name.cc',
        '<(DEPTH)/starboard/shared/win32/adapter_utils.cc',
        '<(DEPTH)/starboard/shared/win32/adapter_utils.h',
        '<(DEPTH)/starboard/shared/win32/atomic_public.h',
        '<(DEPTH)/starboard/shared/win32/audio_sink.cc',
        '<(DEPTH)/starboard/shared/win32/audio_sink_get_max_channels.cc',
        '<(DEPTH)/starboard/shared/win32/audio_sink_get_nearest_supported_sample_frequency.cc',
        '<(DEPTH)/starboard/shared/win32/audio_sink_is_audio_frame_storage_type_supported.cc',
        '<(DEPTH)/starboard/shared/win32/audio_sink_is_audio_sample_type_supported.cc',
        '<(DEPTH)/starboard/shared/win32/auto_event_handle.h',
        '<(DEPTH)/starboard/shared/win32/byte_swap.cc',
        '<(DEPTH)/starboard/shared/win32/condition_variable_broadcast.cc',
        '<(DEPTH)/starboard/shared/win32/condition_variable_create.cc',
        '<(DEPTH)/starboard/shared/win32/condition_variable_destroy.cc',
        '<(DEPTH)/starboard/shared/win32/condition_variable_signal.cc',
        '<(DEPTH)/starboard/shared/win32/condition_variable_wait.cc',
        '<(DEPTH)/starboard/shared/win32/condition_variable_wait_timed.cc',
        '<(DEPTH)/starboard/shared/win32/directory_can_open.cc',
        '<(DEPTH)/starboard/shared/win32/directory_close.cc',
        '<(DEPTH)/starboard/shared/win32/directory_create.cc',
        '<(DEPTH)/starboard/shared/win32/directory_get_next.cc',
        '<(DEPTH)/starboard/shared/win32/directory_internal.cc',
        '<(DEPTH)/starboard/shared/win32/directory_internal.h',
        '<(DEPTH)/starboard/shared/win32/directory_open.cc',
        '<(DEPTH)/starboard/shared/win32/error_utils.cc',
        '<(DEPTH)/starboard/shared/win32/error_utils.h',
        '<(DEPTH)/starboard/shared/win32/file_can_open.cc',
        '<(DEPTH)/starboard/shared/win32/file_close.cc',
        '<(DEPTH)/starboard/shared/win32/file_delete.cc',
        '<(DEPTH)/starboard/shared/win32/file_exists.cc',
        '<(DEPTH)/starboard/shared/win32/file_flush.cc',
        '<(DEPTH)/starboard/shared/win32/file_get_info.cc',
        '<(DEPTH)/starboard/shared/win32/file_get_path_info.cc',
        '<(DEPTH)/starboard/shared/win32/file_internal.cc',
        '<(DEPTH)/starboard/shared/win32/file_internal.h',
        '<(DEPTH)/starboard/shared/win32/file_open.cc',
        '<(DEPTH)/starboard/shared/win32/file_read.cc',
        '<(DEPTH)/starboard/shared/win32/file_seek.cc',
        '<(DEPTH)/starboard/shared/win32/file_truncate.cc',
        '<(DEPTH)/starboard/shared/win32/file_write.cc',
        '<(DEPTH)/starboard/shared/win32/log.cc',
        '<(DEPTH)/starboard/shared/win32/log_flush.cc',
        '<(DEPTH)/starboard/shared/win32/log_format.cc',
        '<(DEPTH)/starboard/shared/win32/log_is_tty.cc',
        '<(DEPTH)/starboard/shared/win32/log_raw_dump_stack.cc',
        '<(DEPTH)/starboard/shared/win32/memory_allocate_aligned_unchecked.cc',
        '<(DEPTH)/starboard/shared/win32/memory_allocate_unchecked.cc',
        '<(DEPTH)/starboard/shared/win32/memory_flush.cc',
        '<(DEPTH)/starboard/shared/win32/memory_free.cc',
        '<(DEPTH)/starboard/shared/win32/memory_free_aligned.cc',
        '<(DEPTH)/starboard/shared/win32/memory_get_stack_bounds.cc',
        '<(DEPTH)/starboard/shared/win32/memory_map.cc',
        '<(DEPTH)/starboard/shared/win32/memory_protect.cc',
        '<(DEPTH)/starboard/shared/win32/memory_reallocate_unchecked.cc',
        '<(DEPTH)/starboard/shared/win32/memory_unmap.cc',
        '<(DEPTH)/starboard/shared/win32/mutex_acquire.cc',
        '<(DEPTH)/starboard/shared/win32/mutex_acquire_try.cc',
        '<(DEPTH)/starboard/shared/win32/mutex_create.cc',
        '<(DEPTH)/starboard/shared/win32/mutex_destroy.cc',
        '<(DEPTH)/starboard/shared/win32/mutex_release.cc',
        '<(DEPTH)/starboard/shared/win32/once.cc',
        '<(DEPTH)/starboard/shared/win32/set_non_blocking_internal.cc',
        '<(DEPTH)/starboard/shared/win32/set_non_blocking_internal.h',
        '<(DEPTH)/starboard/shared/win32/socket_accept.cc',
        '<(DEPTH)/starboard/shared/win32/socket_bind.cc',
        '<(DEPTH)/starboard/shared/win32/socket_clear_last_error.cc',
        '<(DEPTH)/starboard/shared/win32/socket_connect.cc',
        '<(DEPTH)/starboard/shared/win32/socket_create.cc',
        '<(DEPTH)/starboard/shared/win32/socket_destroy.cc',
        '<(DEPTH)/starboard/shared/win32/socket_free_resolution.cc',
        '<(DEPTH)/starboard/shared/win32/socket_get_interface_address.cc',
        '<(DEPTH)/starboard/shared/win32/socket_get_last_error.cc',
        '<(DEPTH)/starboard/shared/win32/socket_get_local_address.cc',
        '<(DEPTH)/starboard/shared/win32/socket_internal.cc',
        '<(DEPTH)/starboard/shared/win32/socket_internal.h',
        '<(DEPTH)/starboard/shared/win32/socket_is_connected.cc',
        '<(DEPTH)/starboard/shared/win32/socket_is_connected_and_idle.cc',
        '<(DEPTH)/starboard/shared/win32/socket_join_multicast_group.cc',
        '<(DEPTH)/starboard/shared/win32/socket_listen.cc',
        '<(DEPTH)/starboard/shared/win32/socket_receive_from.cc',
        '<(DEPTH)/starboard/shared/win32/socket_resolve.cc',
        '<(DEPTH)/starboard/shared/win32/socket_send_to.cc',
        '<(DEPTH)/starboard/shared/win32/socket_set_broadcast.cc',
        '<(DEPTH)/starboard/shared/win32/socket_set_receive_buffer_size.cc',
        '<(DEPTH)/starboard/shared/win32/socket_set_reuse_address.cc',
        '<(DEPTH)/starboard/shared/win32/socket_set_send_buffer_size.cc',
        '<(DEPTH)/starboard/shared/win32/socket_set_tcp_keep_alive.cc',
        '<(DEPTH)/starboard/shared/win32/socket_set_tcp_no_delay.cc',
        '<(DEPTH)/starboard/shared/win32/socket_set_tcp_window_scaling.cc',
        '<(DEPTH)/starboard/shared/win32/socket_waiter_add.cc',
        '<(DEPTH)/starboard/shared/win32/socket_waiter_create.cc',
        '<(DEPTH)/starboard/shared/win32/socket_waiter_destroy.cc',
        '<(DEPTH)/starboard/shared/win32/socket_waiter_internal.cc',
        '<(DEPTH)/starboard/shared/win32/socket_waiter_remove.cc',
        '<(DEPTH)/starboard/shared/win32/socket_waiter_wait.cc',
        '<(DEPTH)/starboard/shared/win32/socket_waiter_wait_timed.cc',
        '<(DEPTH)/starboard/shared/win32/socket_waiter_wake_up.cc',
        '<(DEPTH)/starboard/shared/win32/string_compare_no_case.cc',
        '<(DEPTH)/starboard/shared/win32/string_compare_no_case_n.cc',
        '<(DEPTH)/starboard/shared/win32/string_compare_wide.cc',
        '<(DEPTH)/starboard/shared/win32/string_format.cc',
        '<(DEPTH)/starboard/shared/win32/string_format_wide.cc',
        '<(DEPTH)/starboard/shared/win32/system_break_into_debugger.cc',
        '<(DEPTH)/starboard/shared/win32/system_clear_last_error.cc',
        '<(DEPTH)/starboard/shared/win32/system_get_connection_type.cc',
        '<(DEPTH)/starboard/shared/win32/system_get_error_string.cc',
        '<(DEPTH)/starboard/shared/win32/system_get_last_error.cc',
        '<(DEPTH)/starboard/shared/win32/system_get_locale_id.cc',
        '<(DEPTH)/starboard/shared/win32/system_get_number_of_processors.cc',
        '<(DEPTH)/starboard/shared/win32/system_get_random_data.cc',
        '<(DEPTH)/starboard/shared/win32/system_get_random_uint64.cc',
        '<(DEPTH)/starboard/shared/win32/system_get_stack.cc',
        '<(DEPTH)/starboard/shared/win32/system_is_debugger_attached.cc',
        '<(DEPTH)/starboard/shared/win32/thread_create.cc',
        '<(DEPTH)/starboard/shared/win32/thread_create_local_key.cc',
        '<(DEPTH)/starboard/shared/win32/thread_destroy_local_key.cc',
        '<(DEPTH)/starboard/shared/win32/thread_detach.cc',
        '<(DEPTH)/starboard/shared/win32/thread_get_current.cc',
        '<(DEPTH)/starboard/shared/win32/thread_get_id.cc',
        '<(DEPTH)/starboard/shared/win32/thread_get_local_value.cc',
        '<(DEPTH)/starboard/shared/win32/thread_get_name.cc',
        '<(DEPTH)/starboard/shared/win32/thread_is_equal.cc',
        '<(DEPTH)/starboard/shared/win32/thread_join.cc',
        '<(DEPTH)/starboard/shared/win32/thread_local_internal.cc',
        '<(DEPTH)/starboard/shared/win32/thread_local_internal.h',
        '<(DEPTH)/starboard/shared/win32/thread_private.cc',
        '<(DEPTH)/starboard/shared/win32/thread_private.h',
        '<(DEPTH)/starboard/shared/win32/thread_set_local_value.cc',
        '<(DEPTH)/starboard/shared/win32/thread_set_name.cc',
        '<(DEPTH)/starboard/shared/win32/thread_sleep.cc',
        '<(DEPTH)/starboard/shared/win32/thread_types_public.h',
        '<(DEPTH)/starboard/shared/win32/thread_yield.cc',
        '<(DEPTH)/starboard/shared/win32/thread_yield.h',
        '<(DEPTH)/starboard/shared/win32/time_get_monotonic_now.cc',
        '<(DEPTH)/starboard/shared/win32/time_get_now.cc',
        '<(DEPTH)/starboard/shared/win32/time_zone_get_current.cc',
        '<(DEPTH)/starboard/shared/win32/time_zone_get_name.cc',
        '<(DEPTH)/starboard/shared/win32/time_utils.h',
        '<(DEPTH)/starboard/shared/win32/wchar_utils.h',
        # Include private stubs, if present.
        '<!@(python "<(DEPTH)/starboard/tools/find_private_files.py" "<(DEPTH)" "shared/stub/*.cc")',
        '<@(starboard_platform_dependent_files)',
      ],
      'defines': [
        # This must be defined when building Starboard, and must not when
        # building Starboard client code.
        'STARBOARD_IMPLEMENTATION',
        # We assume most modern Windows PCs can handle 4k H264 and 8k VP9.
        # The latter is additionally gated on Media Foundation acceleration
        # support during runtime anyway.
        'ENABLE_H264_4K_SUPPORT',
        'ENABLE_VP9_8K_SUPPORT',
      ],
      'dependencies': [
        'convert_i18n_data',
      ],
    },
    {
      'target_name': 'convert_i18n_data',
      'type': 'none',
      'actions': [
        {
          'action_name': 'convert_i18n_data',
          'variables': {
            'input_files':
              '<!(find <(DEPTH)/cobalt/content/i18n/platform/xb1/*.xlb)',
          },
          'includes': [ '../../build/convert_i18n_data.gypi' ],
        },
      ],
    }
  ],
}
