#include <Storages/System/StorageSystemBackups.h>
#include <Backups/BackupsWorker.h>
#include <DataTypes/DataTypeEnum.h>
#include <DataTypes/DataTypeString.h>
#include <DataTypes/DataTypesNumber.h>
#include <DataTypes/DataTypeUUID.h>
#include <Columns/ColumnString.h>
#include <Columns/ColumnsNumber.h>
#include <Interpreters/Context.h>


namespace DB
{

NamesAndTypesList StorageSystemBackups::getNamesAndTypes()
{
    NamesAndTypesList names_and_types{
        {"id", std::make_shared<DataTypeString>()},
        {"name", std::make_shared<DataTypeString>()},
        {"status", std::make_shared<DataTypeEnum8>(getBackupStatusEnumValues())},
        {"num_files", std::make_shared<DataTypeUInt64>()},
        {"uncompressed_size", std::make_shared<DataTypeUInt64>()},
        {"compressed_size", std::make_shared<DataTypeUInt64>()},
        {"error", std::make_shared<DataTypeString>()},
        {"start_time", std::make_shared<DataTypeDateTime>()},
        {"end_time", std::make_shared<DataTypeDateTime>()},
    };
    return names_and_types;
}


void StorageSystemBackups::fillData(MutableColumns & res_columns, ContextPtr context, const SelectQueryInfo &) const
{
    size_t column_index = 0;
    auto & column_id = assert_cast<ColumnString &>(*res_columns[column_index++]);
    auto & column_name = assert_cast<ColumnString &>(*res_columns[column_index++]);
    auto & column_status = assert_cast<ColumnInt8 &>(*res_columns[column_index++]);
    auto & column_num_files = assert_cast<ColumnUInt64 &>(*res_columns[column_index++]);
    auto & column_uncompressed_size = assert_cast<ColumnUInt64 &>(*res_columns[column_index++]);
    auto & column_compressed_size = assert_cast<ColumnUInt64 &>(*res_columns[column_index++]);
    auto & column_error = assert_cast<ColumnString &>(*res_columns[column_index++]);
    auto & column_start_time = assert_cast<ColumnUInt32 &>(*res_columns[column_index++]);
    auto & column_end_time = assert_cast<ColumnUInt32 &>(*res_columns[column_index++]);

    auto add_row = [&](const BackupsWorker::Info & info)
    {
        column_id.insertData(info.id.data(), info.id.size());
        column_name.insertData(info.name.data(), info.name.size());
        column_status.insertValue(static_cast<Int8>(info.status));
        column_num_files.insertValue(info.num_files);
        column_uncompressed_size.insertValue(info.uncompressed_size);
        column_compressed_size.insertValue(info.compressed_size);
        column_error.insertData(info.error_message.data(), info.error_message.size());
        column_start_time.insertValue(static_cast<UInt32>(std::chrono::system_clock::to_time_t(info.start_time)));
        column_end_time.insertValue(static_cast<UInt32>(std::chrono::system_clock::to_time_t(info.end_time)));
    };

    for (const auto & entry : context->getBackupsWorker().getAllInfos())
        add_row(entry);
}

}
