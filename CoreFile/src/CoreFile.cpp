//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : CoreFile.cpp                                                  //
//  Project   : CoreFile                                                      //
//  Date      : Aug 26, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

// Header
#include "../include/CoreFile.h"
// std
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iterator>
#include <sstream>
// CoreFile
#include "../include/Config.h"
// CoreFS
#include "CoreFS/CoreFS.h"
// CoreAssert
#include "CoreAssert/CoreAssert.h"

// COWTODO(n2omatt); Implement...
#define COREFILE_CHECK(_cond_, _code_, _exception_, _msg_, ...) \
    do {} while(0);

//----------------------------------------------------------------------------//
// Helper Functions                                                           //
//----------------------------------------------------------------------------//
// COWNOTE(n2omatt): Convert the time_t value to struct tm
//   This is needed because the pointers that localtime(3) or gmtime(3)
//   return might be reused in subsequent calls to the functions.
CoreFile::tm_t time_t_to_tm_t(time_t time, bool local)
{
    auto p_tm = (local) ? localtime(&time) : gmtime(&time);
    CoreFile::tm_t ret_tm = {0};
    memcpy(&ret_tm, p_tm, sizeof(ret_tm));

    return ret_tm;
}

std::fstream::openmode filemode_to_openmode(const std::string &filemode)
{
    //--------------------------------------------------------------------------
    // Text mode.
    if(filemode == CoreFile::FileMode::Text::kRead)
        return std::ios::in;

    if(filemode == CoreFile::FileMode::Text::kWrite)
        return std::ios::out;

    if(filemode == CoreFile::FileMode::Text::kReadWrite_Open)
        return std::ios::in | std::ios::out;
    if(filemode == CoreFile::FileMode::Text::kReadWrite_Truncate)
        return std::ios::in | std::ios::out | std::ios::trunc;

    if(filemode == CoreFile::FileMode::Text::kAppend)
        return std::ios::in | std::ios::app;
    if(filemode == CoreFile::FileMode::Text::kAppend_Truncate)
        return std::ios::in | std::ios::app | std::ios::trunc;

    //--------------------------------------------------------------------------
    // Binary mode.
    if(filemode == CoreFile::FileMode::Binary::kRead)
        return std::ios::in | std::ios::binary;

    if(filemode == CoreFile::FileMode::Binary::kWrite)
        return std::ios::out | std::ios::binary;

    if(filemode == CoreFile::FileMode::Binary::kReadWrite_Open)
        return std::ios::in | std::ios::out | std::ios::binary;
    if(filemode == CoreFile::FileMode::Binary::kReadWrite_Truncate)
        return std::ios::in | std::ios::out | std::ios::trunc | std::ios::binary;

    if(filemode == CoreFile::FileMode::Binary::kAppend)
        return std::ios::in | std::ios::app | std::ios::binary;
    if(filemode == CoreFile::FileMode::Binary::kAppend_Truncate)
        return std::ios::in | std::ios::app | std::ios::trunc | std::ios::binary;

    //--------------------------------------------------------------------------
    // Invalid filemode.
    COREASSERT_THROW_IF_NOT(
        false,
        std::invalid_argument,
        "Invalid filemode: (%s)",
        filemode.c_str()
    );
}


//----------------------------------------------------------------------------//
// Append                                                                     //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
void CoreFile::AppendAllLines(
    const std::string              &filename,
    const std::vector<std::string> &lines)
{
    std::stringstream ss;
    std::string new_line = CoreFS::NewLine();

    //--------------------------------------------------------------------------
    // Join the lines into a single string.
    std::copy(
        lines.begin(),
        lines.end(),
        std::ostream_iterator<std::string>(ss, new_line.c_str())
    );

    CoreFile::AppendAllText(filename, ss.str());
}

//------------------------------------------------------------------------------
void CoreFile::AppendAllText(
    const std::string &filename,
    const std::string &contents)
{
    auto p_file_stream = CoreFile::Open(filename, FileMode::Text::kAppend);
    (*p_file_stream) << contents;
}


//----------------------------------------------------------------------------//
// Copy                                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
void CoreFile::Copy(
    const std::string &src,
    const std::string &dst,
    bool               overwrite /* = false */)
{
    COREFILE_CHECK(
        CoreFS::Exists(src),   // What is to check...
        { return; },           // If check doesn't pass execute this block,
        std::invalid_argument, // OR throw this exception...
        "Source file doesn't exists - src: (%s)",
        src.c_str()
    );

    COREFILE_CHECK(
        (CoreFS::Exists(dst) && !overwrite),  // What is to check...
        { return; },                          // If check doesn't pass execute this block,
        std::runtime_error,                   // OR throw this exception...
        "Destination file exists and overwrite is false - dst: (%s)",
        dst.c_str()
    );

    CoreFile::WriteAllBytes(
        dst,
        CoreFile::ReadAllBytes(src)
    );
}


//----------------------------------------------------------------------------//
// Create                                                                     //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::unique_ptr<std::fstream> CoreFile::Create(const std::string &filename)
{
    return CoreFile::Open(filename, FileMode::Binary::kReadWrite_Truncate);
}

//------------------------------------------------------------------------------
std::unique_ptr<std::fstream> CoreFile::CreateText(const std::string &filename)
{
    return CoreFile::Open(filename, FileMode::Text::kReadWrite_Truncate);
}


//----------------------------------------------------------------------------//
// Delete                                                                     //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
void CoreFile::Delete(const std::string &filename)
{
    COREFILE_CHECK(
        (remove(filename.c_str()) == 0), // What is to check...
        { return; },                     // If check doesn't pass execute this block,
        std::runtime_error,              // OR throw this exception...
        "Failed to Delete file - filename (%s)",
        filename.c_str()
    );
}


//----------------------------------------------------------------------------//
// Exists                                                                     //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
bool CoreFile::Exist(const std::string &filename)
{
    return CoreFS::IsFile(filename);
}


//COWTODO: Gonna implement that???
//GetAttributes(const std::string &filename) ???


//----------------------------------------------------------------------------//
// Get * Time                                                                 //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
CoreFile::tm_t CoreFile::GetCreationTime(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetCTime(filename), true);
}

//------------------------------------------------------------------------------
CoreFile::tm_t CoreFile::GetCreationTimeUtc(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetCTime(filename), false);
}

//------------------------------------------------------------------------------
CoreFile::tm_t CoreFile::GetLastAccessTime(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetATime(filename), true);
}

//------------------------------------------------------------------------------
CoreFile::tm_t CoreFile::GetLastAccessTimeUtc(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetATime(filename), false);
}

//------------------------------------------------------------------------------
CoreFile::tm_t CoreFile::GetLastWriteTime(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetMTime(filename), true);
}

//------------------------------------------------------------------------------
CoreFile::tm_t CoreFile::GetLastWriteTimeUtc(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetMTime(filename), false);
}


//----------------------------------------------------------------------------//
// Move                                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
void CoreFile::Move(
    const std::string &src,
    const std::string &dst,
    bool               overwrite /* = false */)
{
    COREFILE_CHECK(
        (rename(src.c_str(), dst.c_str()) == 0), // What is to check...
        { return; },                             // If check doesn't pass execute this block,
        std::runtime_error,                      // OR throw this exception...
        "Failed to Move file - src (%s) dst (%s)",
        src.c_str(),
        dst.c_str()
    );

}


//----------------------------------------------------------------------------//
// Open                                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::unique_ptr<std::fstream> CoreFile::Open(
    const std::string &filename,
    const std::string &filemode)
{
    auto openmode = filemode_to_openmode(filemode);

    auto p_stream = std::unique_ptr<std::fstream>(new std::fstream());
    //COWTODO(n2omatt): Check if could alloc the p_stream.
    p_stream->open(filename.c_str(), openmode);

    COREASSERT_THROW_IF_NOT(
        p_stream->is_open(),
        std::ios::failure,
        "Failed to open stream - filename: (%s) - filemode (%s)",
        filename.c_str(),
        filemode.c_str()
    );

    return p_stream;
}

//------------------------------------------------------------------------------
std::unique_ptr<std::fstream> CoreFile::OpenRead(const std::string &filename)
{
    return Open(filename, FileMode::Binary::kRead);
}

//------------------------------------------------------------------------------
std::unique_ptr<std::fstream> CoreFile::OpenText(const std::string &filename)
{
    return Open(filename, FileMode::Text::kRead);
}

//------------------------------------------------------------------------------
std::unique_ptr<std::fstream>  CoreFile::OpenWrite(const std::string &filename)
{
    return Open(filename, FileMode::Binary::kWrite);
}


//----------------------------------------------------------------------------//
// Read                                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
std::vector<CoreFile::byte_t> CoreFile::ReadAllBytes(const std::string &filename)
{
    //COWTODO(n2omatt): How we gonna handle errors??
    std::vector<CoreFile::byte_t> ret_val;

    if(!CoreFile::Exist(filename))
        return ret_val;

    // Open the file and calculate it's size.
    auto p_file_stream = CoreFile::OpenRead(filename);
    auto size          = CoreFile::GetSize(*p_file_stream);

    // Play nice with memory.
    ret_val.resize(size);
    p_file_stream->read((char *)(&ret_val[0]), size);

    return ret_val;
}

//------------------------------------------------------------------------------
std::vector<std::string> CoreFile::ReadAllLines(const std::string &filename)
{
    // COWTODO(n2omatt): How we gonna handle errors??
    std::vector<std::string> ret_val;

    if(!CoreFile::Exist(filename))
        return ret_val;

    //Open the file and calculate it's size.
    auto p_file_stream = CoreFile::OpenText(filename);
    while(!p_file_stream->eof())
    {
        std::string line;
        std::getline(*p_file_stream, line);
        ret_val.push_back(line);
    }

    return ret_val;
}

//------------------------------------------------------------------------------
std::string CoreFile::ReadAllText(const std::string &filename)
{
    // COWTODO(n2omatt): How we gonna handle errors??
    std::string ret_val;

    if(!CoreFile::Exist(filename))
        return ret_val;

    auto bytes = CoreFile::ReadAllBytes(filename);
    std::copy(
        bytes.begin(),
        bytes.end  (),
        std::back_inserter(ret_val)
    );

    return ret_val;
}



//COWTODO(n2omatt): Check how to implement that....
//Replace(const std::string &filename, const std::string &filename, const std::string &filename, Boolean) ???


//----------------------------------------------------------------------------//
// Set * Time                                                                 //
//----------------------------------------------------------------------------//
void CoreFile::SetCreationTime(const std::string &filename, time_t dateTime)
{
    //COWTODO(n2omatt): Implement...
    //http://rosettacode.org/wiki/File_modification_time#C.2B.2B
    //utimensat
}

void CoreFile::SetCreationTimeUtc(const std::string &filename, time_t dateTime)
{
    //COWTODO(n2omatt): Implement...
}


void CoreFile::SetLastAccessTime(const std::string &filename, time_t dateTime)
{
    //COWTODO(n2omatt): Implement...
}

void CoreFile::SetLastAccessTimeUtc(const std::string &filename, time_t dateTime)
{
    //COWTODO(n2omatt): Implement...
}


void CoreFile::SetLastWriteTime(const std::string &filename, time_t dateTime)
{
    //COWTODO(n2omatt): Implement...
}

void CoreFile::SetLastWriteTimeUtc(const std::string &filename, time_t dateTime)
{
    //COWTODO(n2omatt): Implement...
}


//----------------------------------------------------------------------------//
// Size                                                                       //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
size_t CoreFile::GetSize(const std::string &filename)
{
    //COWTODO(n2omatt): How we gonna handle errors???
    if(!CoreFile::Exist(filename))
        return 0;

    auto p_stream = OpenRead(filename);
    return GetSize(*p_stream);
}

//------------------------------------------------------------------------------
size_t CoreFile::GetSize(std::fstream &fileStream)
{
    auto curr = fileStream.tellg();

    fileStream.seekg(0, std::ios::beg);
    auto beg = fileStream.tellg();

    fileStream.seekg(0, std::ios::end);
    auto end = fileStream.tellg();

    fileStream.seekg(curr, std::ios::beg);

    auto size = static_cast<size_t>(end - beg);
    return size;
}


//----------------------------------------------------------------------------//
// Write                                                                      //
//----------------------------------------------------------------------------//
//------------------------------------------------------------------------------
void CoreFile::WriteAllBytes(
    const std::string         &filename,
    const std::vector<byte_t> &bytes)
{
    //COWTODO(n2omatt): How we gonna handle errors??
    auto p_file_stream = CoreFile::Open(
        filename,
        FileMode::Binary::kReadWrite_Truncate
    );
    std::copy(
        bytes.begin(),
        bytes.end  (),
        std::ostream_iterator<CoreFile::byte_t>(*p_file_stream)
    );
}

//------------------------------------------------------------------------------
void CoreFile::WriteAllLines(
    const std::string              &filename,
    const std::vector<std::string> &lines)
{
    //COWTODO(n2omatt): How we gonna handle errors??
    auto p_file_stream = CoreFile::Open(
        filename,
        FileMode::Text::kReadWrite_Truncate
    );

    std::copy(
        lines.begin(),
        lines.end  (),
        std::ostream_iterator<std::string>(
            *p_file_stream,
            CoreFS::NewLine().c_str()
        )
    );
}

//------------------------------------------------------------------------------
void CoreFile::WriteAllText(
    const std::string &filename,
    const std::string &contents)
{
    //COWTODO(n2omatt): How we gonna handle errors??
    auto p_file_stream = CoreFile::Open(
        filename,
        FileMode::Text::kReadWrite_Truncate
    );

    std::copy(
        contents.begin(),
        contents.end  (),
        std::ostream_iterator<char>(*p_file_stream)
    );
}
