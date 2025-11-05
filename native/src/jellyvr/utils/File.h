#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include <string>
#include <vector>

using namespace std;

#if defined(_WIN32)
#include <Windows.h>

bool write_buffer_to_file(const wstring &filepath, const vector<uint8_t> &buffer, const wstring &filename, const wstring &file_extension);

#else
bool write_buffer_to_file(const string &filepath, const vector<uint8_t> &buffer, const string &filename, const string &file_extension);

#endif
#endif