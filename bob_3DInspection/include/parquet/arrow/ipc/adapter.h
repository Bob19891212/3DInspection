// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

// Public API for writing and accessing (with zero copy, if possible) Arrow
// IPC binary formatted data (e.g. in shared memory, or from some other IO source)

#ifndef ARROW_IPC_ADAPTER_H
#define ARROW_IPC_ADAPTER_H

#include <cstdint>
#include <memory>
#include <vector>

#include "arrow/util/visibility.h"

namespace arrow {

class Array;
class MemoryPool;
class RecordBatch;
class Schema;
class Status;

namespace io {

class ReadableFileInterface;
class OutputStream;

}  // namespace io

namespace ipc {

class RecordBatchMetadata;

// ----------------------------------------------------------------------
// Write path
// We have trouble decoding flatbuffers if the size i > 70, so 64 is a nice round number
// TODO(emkornfield) investigate this more
constexpr int kMaxIpcRecursionDepth = 64;

// Write the RecordBatch (collection of equal-length Arrow arrays) to the
// output stream in a contiguous block. The record batch metadata is written as
// a flatbuffer (see format/Message.fbs -- the RecordBatch message type)
// prefixed by its size, followed by each of the memory buffers in the batch
// written end to end (with appropriate alignment and padding):
//
// <int32: metadata size> <uint8*: metadata> <buffers>
//
// Finally, the absolute offsets (relative to the start of the output stream)
// to the end of the body and end of the metadata / data header (suffixed by
// the header size) is returned in out-variables
//
// @param(in) buffer_start_offset: the start offset to use in the buffer metadata,
// default should be 0
//
// @param(out) metadata_length: the size of the length-prefixed flatbuffer
// including padding to a 64-byte boundary
//
// @param(out) body_length: the size of the contiguous buffer block plus
// padding bytes
Status ARROW_EXPORT WriteRecordBatch(const RecordBatch& batch,
    int64_t buffer_start_offset, io::OutputStream* dst, int32_t* metadata_length,
    int64_t* body_length, MemoryPool* pool,
    int max_recursion_depth = kMaxIpcRecursionDepth);

// Compute the precise number of bytes needed in a contiguous memory segment to
// write the record batch. This involves generating the complete serialized
// Flatbuffers metadata.
Status ARROW_EXPORT GetRecordBatchSize(const RecordBatch& batch, int64_t* size);

// ----------------------------------------------------------------------
// "Read" path; does not copy data if the input supports zero copy reads

// Read the record batch flatbuffer metadata starting at the indicated file offset
//
// The flatbuffer is expected to be length-prefixed, so the metadata_length
// includes at least the length prefix and the flatbuffer
Status ARROW_EXPORT ReadRecordBatchMetadata(int64_t offset, int32_t metadata_length,
    io::ReadableFileInterface* file, std::shared_ptr<RecordBatchMetadata>* metadata);

Status ARROW_EXPORT ReadRecordBatch(const std::shared_ptr<RecordBatchMetadata>& metadata,
    const std::shared_ptr<Schema>& schema, io::ReadableFileInterface* file,
    std::shared_ptr<RecordBatch>* out);

Status ARROW_EXPORT ReadRecordBatch(const std::shared_ptr<RecordBatchMetadata>& metadata,
    const std::shared_ptr<Schema>& schema, int max_recursion_depth,
    io::ReadableFileInterface* file, std::shared_ptr<RecordBatch>* out);

}  // namespace ipc
}  // namespace arrow

#endif  // ARROW_IPC_MEMORY_H
