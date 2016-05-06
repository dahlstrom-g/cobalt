# Copyright 2015 Google Inc. All Rights Reserved.
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
  'targets': [
    {
      'target_name': 'starboard_base_symbolize',
      'type': 'static_library',
      'sources': [
        '<(DEPTH)/base/third_party/symbolize/demangle.cc',
        '<(DEPTH)/base/third_party/symbolize/symbolize.cc',
      ],
    },
    {
      'target_name': 'starboard_platform',
      'type': 'static_library',
      'sources': [
        '<(DEPTH)/starboard/linux/shared/application_linux.cc',
        '<(DEPTH)/starboard/linux/shared/atomic_gcc.h',
        '<(DEPTH)/starboard/linux/shared/atomic_public.h',
        '<(DEPTH)/starboard/linux/shared/byte_swap.cc',
        '<(DEPTH)/starboard/linux/shared/configuration_public.h',
        '<(DEPTH)/starboard/linux/shared/get_home_directory.cc',
        '<(DEPTH)/starboard/linux/shared/main.cc',
        '<(DEPTH)/starboard/linux/shared/memory_get_stack_bounds.cc',
        '<(DEPTH)/starboard/linux/shared/page_internal.cc',
        '<(DEPTH)/starboard/linux/shared/socket_get_local_interface_address.cc',
        '<(DEPTH)/starboard/linux/shared/system_get_path.cc',
        '<(DEPTH)/starboard/linux/shared/system_get_property.cc',
        '<(DEPTH)/starboard/linux/shared/system_get_random_data.cc',
        '<(DEPTH)/starboard/linux/shared/system_get_stack.cc',
        '<(DEPTH)/starboard/linux/shared/system_get_total_memory.cc',
        '<(DEPTH)/starboard/linux/shared/system_has_capability.cc',
        '<(DEPTH)/starboard/linux/shared/system_is_debugger_attached.cc',
        '<(DEPTH)/starboard/linux/shared/system_symbolize.cc',
        '<(DEPTH)/starboard/linux/shared/thread_get_id.cc',
        '<(DEPTH)/starboard/linux/shared/thread_get_name.cc',
        '<(DEPTH)/starboard/linux/shared/thread_set_name.cc',
        '<(DEPTH)/starboard/linux/shared/window_create.cc',
        '<(DEPTH)/starboard/linux/shared/window_destroy.cc',
        '<(DEPTH)/starboard/linux/shared/window_get_platform_handle.cc',
        '<(DEPTH)/starboard/linux/shared/window_get_size.cc',
        '<(DEPTH)/starboard/linux/shared/window_internal.cc',
        '<(DEPTH)/starboard/linux/shared/window_set_default_options.cc',
        '<(DEPTH)/starboard/shared/dlmalloc/memory_allocate_aligned_unchecked.cc',
        '<(DEPTH)/starboard/shared/dlmalloc/memory_allocate_unchecked.cc',
        '<(DEPTH)/starboard/shared/dlmalloc/memory_free.cc',
        '<(DEPTH)/starboard/shared/dlmalloc/memory_free_aligned.cc',
        '<(DEPTH)/starboard/shared/dlmalloc/memory_map.cc',
        '<(DEPTH)/starboard/shared/dlmalloc/memory_reallocate_unchecked.cc',
        '<(DEPTH)/starboard/shared/dlmalloc/memory_unmap.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_alphanumeric.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_digit.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_hex_digit.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_space.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_upper.cc',
        '<(DEPTH)/starboard/shared/iso/character_to_lower.cc',
        '<(DEPTH)/starboard/shared/iso/character_to_upper.cc',
        '<(DEPTH)/starboard/shared/iso/directory_close.cc',
        '<(DEPTH)/starboard/shared/iso/directory_get_next.cc',
        '<(DEPTH)/starboard/shared/iso/directory_open.cc',
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
        '<(DEPTH)/starboard/shared/iso/string_parse_signed_integer.cc',
        '<(DEPTH)/starboard/shared/iso/string_parse_uint64.cc',
        '<(DEPTH)/starboard/shared/iso/string_parse_unsigned_integer.cc',
        '<(DEPTH)/starboard/shared/iso/string_scan.cc',
        '<(DEPTH)/starboard/shared/iso/system_binary_search.cc',
        '<(DEPTH)/starboard/shared/iso/system_sort.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_add.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_create.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_destroy.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_internal.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_remove.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_wait.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_wait_timed.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_wake_up.cc',
        '<(DEPTH)/starboard/shared/nouser/user_get_current.cc',
        '<(DEPTH)/starboard/shared/nouser/user_get_property.cc',
        '<(DEPTH)/starboard/shared/nouser/user_get_signed_in.cc',
        '<(DEPTH)/starboard/shared/nouser/user_internal.cc',
        '<(DEPTH)/starboard/shared/nouser/user_is_age_restricted.cc',
        '<(DEPTH)/starboard/shared/nouser/user_start_sign_in.cc',
        '<(DEPTH)/starboard/shared/posix/directory_create.cc',
        '<(DEPTH)/starboard/shared/posix/file_can_open.cc',
        '<(DEPTH)/starboard/shared/posix/file_close.cc',
        '<(DEPTH)/starboard/shared/posix/file_delete.cc',
        '<(DEPTH)/starboard/shared/posix/file_exists.cc',
        '<(DEPTH)/starboard/shared/posix/file_flush.cc',
        '<(DEPTH)/starboard/shared/posix/file_get_info.cc',
        '<(DEPTH)/starboard/shared/posix/file_get_path_info.cc',
        '<(DEPTH)/starboard/shared/posix/file_open.cc',
        '<(DEPTH)/starboard/shared/posix/file_read.cc',
        '<(DEPTH)/starboard/shared/posix/file_seek.cc',
        '<(DEPTH)/starboard/shared/posix/file_truncate.cc',
        '<(DEPTH)/starboard/shared/posix/file_write.cc',
        '<(DEPTH)/starboard/shared/posix/log.cc',
        '<(DEPTH)/starboard/shared/posix/log_flush.cc',
        '<(DEPTH)/starboard/shared/posix/log_format.cc',
        '<(DEPTH)/starboard/shared/posix/log_is_tty.cc',
        '<(DEPTH)/starboard/shared/posix/log_raw.cc',
        '<(DEPTH)/starboard/shared/posix/socket_accept.cc',
        '<(DEPTH)/starboard/shared/posix/socket_bind.cc',
        '<(DEPTH)/starboard/shared/posix/socket_clear_last_error.cc',
        '<(DEPTH)/starboard/shared/posix/socket_connect.cc',
        '<(DEPTH)/starboard/shared/posix/socket_create.cc',
        '<(DEPTH)/starboard/shared/posix/socket_destroy.cc',
        '<(DEPTH)/starboard/shared/posix/socket_free_resolution.cc',
        '<(DEPTH)/starboard/shared/posix/socket_get_last_error.cc',
        '<(DEPTH)/starboard/shared/posix/socket_get_local_address.cc',
        '<(DEPTH)/starboard/shared/posix/socket_internal.cc',
        '<(DEPTH)/starboard/shared/posix/socket_is_connected.cc',
        '<(DEPTH)/starboard/shared/posix/socket_is_connected_and_idle.cc',
        '<(DEPTH)/starboard/shared/posix/socket_join_multicast_group.cc',
        '<(DEPTH)/starboard/shared/posix/socket_listen.cc',
        '<(DEPTH)/starboard/shared/posix/socket_receive_from.cc',
        '<(DEPTH)/starboard/shared/posix/socket_resolve.cc',
        '<(DEPTH)/starboard/shared/posix/socket_send_to.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_broadcast.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_receive_buffer_size.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_reuse_address.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_send_buffer_size.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_tcp_keep_alive.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_tcp_no_delay.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_tcp_window_scaling.cc',
        '<(DEPTH)/starboard/shared/posix/string_compare_no_case.cc',
        '<(DEPTH)/starboard/shared/posix/string_compare_no_case_n.cc',
        '<(DEPTH)/starboard/shared/posix/string_compare_wide.cc',
        '<(DEPTH)/starboard/shared/posix/string_format.cc',
        '<(DEPTH)/starboard/shared/posix/string_format_wide.cc',
        '<(DEPTH)/starboard/shared/posix/system_break_into_debugger.cc',
        '<(DEPTH)/starboard/shared/posix/system_clear_last_error.cc',
        '<(DEPTH)/starboard/shared/posix/system_get_error_string.cc',
        '<(DEPTH)/starboard/shared/posix/system_get_last_error.cc',
        '<(DEPTH)/starboard/shared/posix/system_get_locale_id.cc',
        '<(DEPTH)/starboard/shared/posix/system_get_number_of_processors.cc',
        '<(DEPTH)/starboard/shared/posix/thread_sleep.cc',
        '<(DEPTH)/starboard/shared/posix/time_explode.cc',
        '<(DEPTH)/starboard/shared/posix/time_get_monotonic_now.cc',
        '<(DEPTH)/starboard/shared/posix/time_get_now.cc',
        '<(DEPTH)/starboard/shared/posix/time_implode.cc',
        '<(DEPTH)/starboard/shared/posix/time_zone_get_current.cc',
        '<(DEPTH)/starboard/shared/posix/time_zone_get_dst_name.cc',
        '<(DEPTH)/starboard/shared/posix/time_zone_get_name.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_broadcast.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_create.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_destroy.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_signal.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_wait.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_wait_timed.cc',
        '<(DEPTH)/starboard/shared/pthread/mutex_acquire.cc',
        '<(DEPTH)/starboard/shared/pthread/mutex_acquire_try.cc',
        '<(DEPTH)/starboard/shared/pthread/mutex_create.cc',
        '<(DEPTH)/starboard/shared/pthread/mutex_destroy.cc',
        '<(DEPTH)/starboard/shared/pthread/mutex_release.cc',
        '<(DEPTH)/starboard/shared/pthread/once.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_create.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_create_local_key.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_destroy_local_key.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_detach.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_get_current.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_get_local_value.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_is_equal.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_join.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_set_local_value.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_yield.cc',
        '<(DEPTH)/starboard/shared/starboard/application.cc',
        '<(DEPTH)/starboard/shared/starboard/directory_can_open.cc',
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
        '<(DEPTH)/starboard/shared/starboard/log_raw_dump_stack.cc',
        '<(DEPTH)/starboard/shared/starboard/log_raw_format.cc',
        '<(DEPTH)/starboard/shared/starboard/new.cc',
        '<(DEPTH)/starboard/shared/starboard/queue_application.cc',
        '<(DEPTH)/starboard/shared/starboard/string_concat.cc',
        '<(DEPTH)/starboard/shared/starboard/string_concat_wide.cc',
        '<(DEPTH)/starboard/shared/starboard/string_copy.cc',
        '<(DEPTH)/starboard/shared/starboard/string_copy_wide.cc',
        '<(DEPTH)/starboard/shared/starboard/string_duplicate.cc',
        '<(DEPTH)/starboard/shared/starboard/system_get_random_uint64.cc',
        '<(DEPTH)/starboard/shared/starboard/system_request_stop.cc',
      ],
      'defines': [
        # This must be defined when building Starboard, and must not when
        # building Starboard client code.
        'STARBOARD_IMPLEMENTATION',
      ],
      'dependencies': [
        '<(DEPTH)/third_party/dlmalloc/dlmalloc.gyp:dlmalloc',
        '<(DEPTH)/third_party/libevent/libevent.gyp:libevent',
        'starboard_base_symbolize',
      ],
    },
  ],
}
