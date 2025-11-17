#include "File.h"

using namespace std;
namespace fs = std::filesystem;

#if defined(_WIN32)
#include <Windows.h>

bool write_buffer_to_file(const wstring &filepath, const vector<uint8_t> &buffer, const wstring &filename, const wstring &file_extension) {
    wstring full_path {filepath + filename + file_extension};
    wstring tmp_path = filepath + L".tmp";

    ofstream file(tmp_path, ios::out | ios::trunc | ios::binary);
    if(!file.is_open()) {
        return false;
    }

    file.write((const char*)buffer.data(), buffer.size());
    file.close();


    BOOL success = MoveFileExW(tmp_path.c_str(), full_path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH); // cmon windows you couldn't use bool like everybody else x')
    return success != 0; //return a WIN32 TRUE
}
#else

bool write_buffer_to_file(const string &filepath, const vector<uint8_t> &buffer, const string &filename, const string &file_extension) {
    string full_path {filepath + filename + file_extension};
    string tmp_path = filepath + ".tmp";

    ofstream file(tmp_path, ios::out | ios::trunc | ios::binary);
    if(file.is_open()) {
        file.write((const char*)buffer.data(), buffer.size());
        file.close();
        filesystem::rename(tmp_path, full_path);
    }
    return true;
}

#endif