#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void extract_zip_to(const std::string& zip_path, const fs::path& out_dir);

#if defined(_WIN32)
#include <Windows.h>

bool write_buffer_to_file(const wstring &filepath, const vector<uint8_t> &buffer, const wstring &filename, const wstring &file_extension);

#else
bool write_buffer_to_file(const string &filepath, const vector<uint8_t> &buffer, const string &filename, const string &file_extension);

#endif
#endif