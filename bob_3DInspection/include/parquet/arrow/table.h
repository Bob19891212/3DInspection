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

#ifndef ARROW_TABLE_H
#define ARROW_TABLE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "arrow/util/visibility.h"

namespace arrow {

class Array;
class Column;
class Schema;
class Status;

// A record batch is a simpler and more rigid table data structure intended for
// use primarily in shared memory IPC. It contains a schema (metadata) and a
// corresponding sequence of equal-length Arrow arrays
class ARROW_EXPORT RecordBatch {
 public:
  // num_rows is a parameter to allow for record batches of a particular size not
  // having any materialized columns. Each array should have the same length as
  // num_rows
  RecordBatch(const std::shared_ptr<Schema>& schema, int32_t num_rows,
      const std::vector<std::shared_ptr<Array>>& columns);

  bool Equals(const RecordBatch& other) const;

  bool ApproxEquals(const RecordBatch& other) const;

  // @returns: the table's schema
  std::shared_ptr<Schema> schema() const { return schema_; }

  // @returns: the i-th column
  // Note: Does not boundscheck
  std::shared_ptr<Array> column(int i) const { return columns_[i]; }

  const std::vector<std::shared_ptr<Array>>& columns() const { return columns_; }

  const std::string& column_name(int i) const;

  // @returns: the number of columns in the table
  int num_columns() const { return columns_.size(); }

  // @returns: the number of rows (the corresponding length of each column)
  int32_t num_rows() const { return num_rows_; }

  /// Slice each of the arrays in the record batch and construct a new RecordBatch object
  std::shared_ptr<RecordBatch> Slice(int32_t offset);
  std::shared_ptr<RecordBatch> Slice(int32_t offset, int32_t length);

 private:
  std::shared_ptr<Schema> schema_;
  int32_t num_rows_;
  std::vector<std::shared_ptr<Array>> columns_;
};

// Immutable container of fixed-length columns conforming to a particular schema
class ARROW_EXPORT Table {
 public:
  // If columns is zero-length, the table's number of rows is zero
  Table(const std::string& name, const std::shared_ptr<Schema>& schema,
      const std::vector<std::shared_ptr<Column>>& columns);

  // num_rows is a parameter to allow for tables of a particular size not
  // having any materialized columns. Each column should therefore have the
  // same length as num_rows -- you can validate this using
  // Table::ValidateColumns
  Table(const std::string& name, const std::shared_ptr<Schema>& schema,
      const std::vector<std::shared_ptr<Column>>& columns, int64_t nubm_rows);

  // Construct table from RecordBatch, but only if all of the batch schemas are
  // equal. Returns Status::Invalid if there is some problem
  static Status FromRecordBatches(const std::string& name,
      const std::vector<std::shared_ptr<RecordBatch>>& batches,
      std::shared_ptr<Table>* table);

  // @returns: the table's name, if any (may be length 0)
  const std::string& name() const { return name_; }

  // @returns: the table's schema
  std::shared_ptr<Schema> schema() const { return schema_; }

  // Note: Does not boundscheck
  // @returns: the i-th column
  std::shared_ptr<Column> column(int i) const { return columns_[i]; }

  // @returns: the number of columns in the table
  int num_columns() const { return columns_.size(); }

  // @returns: the number of rows (the corresponding length of each column)
  int64_t num_rows() const { return num_rows_; }

  bool Equals(const Table& other) const;
  bool Equals(const std::shared_ptr<Table>& other) const;

  // After construction, perform any checks to validate the input arguments
  Status ValidateColumns() const;

 private:
  // The table's name, optional
  std::string name_;

  std::shared_ptr<Schema> schema_;
  std::vector<std::shared_ptr<Column>> columns_;

  int64_t num_rows_;
};

// Construct table from multiple input tables. Return Status::Invalid if
// schemas are not equal
Status ARROW_EXPORT ConcatenateTables(const std::string& output_name,
    const std::vector<std::shared_ptr<Table>>& tables, std::shared_ptr<Table>* table);

}  // namespace arrow

#endif  // ARROW_TABLE_H
