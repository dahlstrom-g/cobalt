# Copyright (c) 2009 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'use_system_sqlite%': 0,
    'required_sqlite_version': '3.6.1',
  },
  'target_defaults': {
    'defines': [
      'SQLITE_CORE',
      'SQLITE_ENABLE_BROKEN_FTS2',
      'SQLITE_ENABLE_FTS2',
      'SQLITE_ENABLE_FTS3',
      'SQLITE_ENABLE_ICU',
      'SQLITE_ENABLE_MEMORY_MANAGEMENT',
      'SQLITE_SECURE_DELETE',
      'THREADSAFE',
      '_HAS_EXCEPTIONS=0',
    ],
  },
  'targets': [
    {
      'target_name': 'sqlite',
      'conditions': [
        ['OS=="linux" and not use_system_sqlite', {
          'link_settings': {
            'libraries': [
              '-ldl',
            ],
          },
        }],
        ['(OS=="linux" or OS=="freebsd" or OS=="openbsd") and use_system_sqlite', {
          'type': 'settings',
          'direct_dependent_settings': {
            'cflags': [
              # This next command produces no output but it it will fail (and
              # cause GYP to fail) if we don't have a recent enough version of
              # sqlite.
              '<!@(pkg-config --atleast-version=<(required_sqlite_version) sqlite3)',

              '<!@(pkg-config --cflags sqlite3)',
            ],
            'defines': [
              'USE_SYSTEM_SQLITE',
            ],
          },
          'link_settings': {
            'ldflags': [
              '<!@(pkg-config --libs-only-L --libs-only-other sqlite3)',
            ],
            'libraries': [
              '<!@(pkg-config --libs-only-l sqlite3)',
            ],
          },
        }, { # else: OS != "linux" or ! use_system_sqlite
          'product_name': 'sqlite3',
          'type': '<(library)',
          'msvs_guid': '6EAD4A4B-2BBC-4974-8E45-BB5C16CC2AC9',
          'sources': [
            # This list contains all .h, .c, and .cc files in the directories
            # ext, preprocessed, and src, with the exception of src/lempar.c,
            # src/shell*, and src/test*.  Exclusions are applied below in the
            # sources/ and sources! sections.
            'ext/async/sqlite3async.c',
            'ext/async/sqlite3async.h',
            'ext/fts1/ft_hash.c',
            'ext/fts1/ft_hash.h',
            'ext/fts1/fts1.c',
            'ext/fts1/fts1.h',
            'ext/fts1/fts1_hash.c',
            'ext/fts1/fts1_hash.h',
            'ext/fts1/fts1_porter.c',
            'ext/fts1/fts1_tokenizer.h',
            'ext/fts1/fts1_tokenizer1.c',
            'ext/fts1/fulltext.c',
            'ext/fts1/fulltext.h',
            'ext/fts1/simple_tokenizer.c',
            'ext/fts1/tokenizer.h',
            'ext/fts2/fts2.c',
            'ext/fts2/fts2.h',
            'ext/fts2/fts2_hash.c',
            'ext/fts2/fts2_hash.h',
            'ext/fts2/fts2_icu.c',
            'ext/fts2/fts2_porter.c',
            'ext/fts2/fts2_tokenizer.c',
            'ext/fts2/fts2_tokenizer.h',
            'ext/fts2/fts2_tokenizer1.c',
            'ext/fts3/fts3.c',
            'ext/fts3/fts3.h',
            'ext/fts3/fts3_expr.c',
            'ext/fts3/fts3_expr.h',
            'ext/fts3/fts3_hash.c',
            'ext/fts3/fts3_hash.h',
            'ext/fts3/fts3_icu.c',
            'ext/fts3/fts3_porter.c',
            'ext/fts3/fts3_tokenizer.c',
            'ext/fts3/fts3_tokenizer.h',
            'ext/fts3/fts3_tokenizer1.c',
            'ext/icu/icu.c',
            'ext/icu/sqliteicu.h',
            'ext/rtree/rtree.c',
            'ext/rtree/rtree.h',
            'preprocessed/keywordhash.h',
            'preprocessed/opcodes.c',
            'preprocessed/opcodes.h',
            'preprocessed/parse.c',
            'preprocessed/parse.h',
            'preprocessed/sqlite3.h',
            'src/alter.c',
            'src/analyze.c',
            'src/attach.c',
            'src/auth.c',
            'src/backup.c',
            'src/bitvec.c',
            'src/btmutex.c',
            'src/btree.c',
            'src/btree.h',
            'src/btreeInt.h',
            'src/build.c',
            'src/callback.c',
            'src/complete.c',
            'src/date.c',
            'src/delete.c',
            'src/expr.c',
            'src/fault.c',
            'src/func.c',
            'src/global.c',
            'src/hash.c',
            'src/hash.h',
            'src/hwtime.h',
            'src/insert.c',
            'src/journal.c',
            'src/legacy.c',
            'src/loadext.c',
            'src/main.c',
            'src/malloc.c',
            'src/mem0.c',
            'src/mem1.c',
            'src/mem2.c',
            'src/mem3.c',
            'src/mem5.c',
            'src/memjournal.c',
            'src/mutex.c',
            'src/mutex.h',
            'src/mutex_noop.c',
            'src/mutex_os2.c',
            'src/mutex_unix.c',
            'src/mutex_w32.c',
            'src/notify.c',
            'src/os.c',
            'src/os.h',
            'src/os_common.h',
            'src/os_os2.c',
            'src/os_symbian.cc',
            'src/os_unix.c',
            'src/os_win.c',
            'src/pager.c',
            'src/pager.h',
            'src/pcache.c',
            'src/pcache.h',
            'src/pcache1.c',
            'src/pragma.c',
            'src/prepare.c',
            'src/printf.c',
            'src/random.c',
            'src/resolve.c',
            'src/rowset.c',
            'src/select.c',
            'src/sqlite3ext.h',
            'src/sqliteInt.h',
            'src/sqliteLimit.h',
            'src/status.c',
            'src/table.c',
            'src/tclsqlite.c',
            'src/tokenize.c',
            'src/trigger.c',
            'src/update.c',
            'src/utf.c',
            'src/util.c',
            'src/vacuum.c',
            'src/vdbe.c',
            'src/vdbe.h',
            'src/vdbeInt.h',
            'src/vdbeapi.c',
            'src/vdbeaux.c',
            'src/vdbeblob.c',
            'src/vdbemem.c',
            'src/vtab.c',
            'src/walker.c',
            'src/where.c',
          ],
          'sources/': [
            ['exclude', '^ext/(fts1|rtree)/'],
            ['exclude', '(symbian|os2|noop)\\.cc?$'],
          ],
          'sources!': [
            'src/journal.c',
            'src/tclsqlite.c',
          ],
          'include_dirs': [
            'ext/icu',
            'preprocessed',
            'src',
          ],
          'dependencies': [
            '../icu/icu.gyp:icui18n',
            '../icu/icu.gyp:icuuc',
          ],
          'direct_dependent_settings': {
            'include_dirs': [
              'preprocessed',
            ],
          },
          'msvs_disabled_warnings': [
              4018, 4244,
          ],
          'conditions': [
            ['OS=="win"', {
              'sources/': [['exclude', '_unix\\.cc?$']],
            }, {  # else: OS!="win"
              'sources/': [['exclude', '_(w32|win)\\.cc?$']],
            }],
          ],
        }],
      ],
    },
  ],
  'conditions': [
    ['(OS=="linux" or OS=="freebsd" or OS=="openbsd") and not use_system_sqlite', {
      'targets': [
        {
          'target_name': 'sqlite_shell',
          'type': 'executable',
          'dependencies': [
            '../icu/icu.gyp:icuuc',
            'sqlite',
          ],
          'sources': [
            'src/shell.c',
            'src/shell_icu_linux.c',
          ],
          'link_settings': {
            'link_languages': ['c++'],
          },
        },
      ],
    },]
  ],
}

# Local Variables:
# tab-width:2
# indent-tabs-mode:nil
# End:
# vim: set expandtab tabstop=2 shiftwidth=2:
