// Vita3K emulator project
// Copyright (C) 2018 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once

#include <kernel/state.h>
#include <mem/ptr.h>
#include <psp2/types.h>

#include <functional>
#include <memory>

struct CPUState;
struct ThreadState;

typedef std::function<void(uint32_t, SceUID)> CallImport;
typedef std::shared_ptr<ThreadState> ThreadStatePtr;

SceUID create_thread(Ptr<const void> entry_point, KernelState &kernel, MemState &mem, const char *name, int stackSize, CallImport call_import, bool log_code);
int start_thread(KernelState &kernel, const SceUID &thid, SceSize arglen, const Ptr<void> &argp);
bool run_thread(ThreadState &thread, bool callback);
bool run_callback(ThreadState &thread, Address &pc, Address &data);
