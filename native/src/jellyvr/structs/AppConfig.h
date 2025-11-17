#ifndef RETURNED_DATA_H
#define RETURNED_DATA_H

#include <string>
    
struct ReturnedData
{
    bool http_operation_success;
    std::string download_link;
    bool is_file_zip;
};

#endif