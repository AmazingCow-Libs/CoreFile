//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : CoreFile.h                                                    //
//  Project   : CoreFile                                                      //
//  Date      : Aug 26, 2017                                                  //
//  License   : GPLv3                                                         //
//  Author    : n2omatt <n2omatt@amazingcow.com>                              //
//  Copyright : AmazingCow - 2017                                             //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

#pragma once

//std
#include <fstream>
#include <string>
#include <vector>
//CoreFile
#include "CoreFile_Utils.h"


NS_COREFILE_BEGIN

////////////////////////////////////////////////////////////////////////////////
// Enums / Constants / Typedefs                                               //
////////////////////////////////////////////////////////////////////////////////
using byte_t = unsigned char;
using tm_t   = struct tm;

namespace FileMode
{
    namespace Text {
        constexpr auto kRead  = "r";
        constexpr auto kWrite = "w";

        constexpr auto kReadWrite_Open     = "r+";
        constexpr auto kReadWrite_Truncate = "w+";

        constexpr auto kAppend          = "a";
        constexpr auto kAppend_Truncate = "a+";
    }

    namespace Binary {
        constexpr auto kRead  = "rb";
        constexpr auto kWrite = "wb";

        constexpr auto kReadWrite_Open     = "r+b";
        constexpr auto kReadWrite_Truncate = "w+b";

        constexpr auto kAppend          = "ab";
        constexpr auto kAppend_Truncate = "a+b";
    }
}


////////////////////////////////////////////////////////////////////////////////
// Append                                                                     //
////////////////////////////////////////////////////////////////////////////////
//Appends lines to a file, and then closes the file.
//If the specified file does not exist, this method creates a file,
//writes the specified lines to the file, and then closes the file.
void AppendAllLines(
    const std::string              &filename,
    const std::vector<std::string> &lines);

//Opens a file, appends the specified string to the file,
//and then closes the file.
//If the file does not exist, this method creates a file, writes
//the specified string to the file, then closes the file.
void AppendAllText(const std::string &filename, const std::string &contents);


////////////////////////////////////////////////////////////////////////////////
// Copy                                                                       //
////////////////////////////////////////////////////////////////////////////////
//Copies an existing file to a new file.
//Overwriting a file of the same name is allowed.
void Copy(const std::string &src, const std::string &dst, bool overwrite);


////////////////////////////////////////////////////////////////////////////////
// Create                                                                     //
////////////////////////////////////////////////////////////////////////////////
//Creates or overwrites a file in the specified path.
std::fstream Create(const std::string &filename);
std::fstream CreateText(const std::string &filename);


////////////////////////////////////////////////////////////////////////////////
// Delete                                                                     //
////////////////////////////////////////////////////////////////////////////////
void Delete(const std::string &filename);


////////////////////////////////////////////////////////////////////////////////
// Exists                                                                     //
////////////////////////////////////////////////////////////////////////////////
bool Exist(const std::string &filename);

//COWTODO: Gonna implement that???
//GetAttributes(const std::string &filename) ???


////////////////////////////////////////////////////////////////////////////////
// Get * Time                                                                 //
////////////////////////////////////////////////////////////////////////////////
tm_t GetCreationTime(const std::string &filename);
tm_t GetCreationTimeUtc(const std::string &filename);

tm_t GetLastAccessTime(const std::string &filename);
tm_t GetLastAccessTimeUtc(const std::string &filename);

tm_t GetLastWriteTime(const std::string &filename);
tm_t GetLastWriteTimeUtc(const std::string &filename);


////////////////////////////////////////////////////////////////////////////////
// Move                                                                       //
////////////////////////////////////////////////////////////////////////////////
void Move(const std::string &src, const std::string &dst);


////////////////////////////////////////////////////////////////////////////////
// Open                                                                       //
////////////////////////////////////////////////////////////////////////////////
std::fstream Open(const std::string &filename, const std::string &fileMode);
std::fstream OpenRead(const std::string &filename);
std::fstream OpenText(const std::string &filename);
std::fstream OpenWrite(const std::string &filename);


////////////////////////////////////////////////////////////////////////////////
// Read                                                                       //
////////////////////////////////////////////////////////////////////////////////
std::vector<byte_t> ReadAllBytes(const std::string &filename);
std::vector<std::string> ReadAllLines(const std::string &filename);
std::string ReadAllText(const std::string &filename);


//COWTODO(n2omatt): Check how to implement that....
//Replace(const std::string &filename, const std::string &filename, const std::string &filename, Boolean) ???


////////////////////////////////////////////////////////////////////////////////
// Set * Time                                                                 //
////////////////////////////////////////////////////////////////////////////////
void SetCreationTime(const std::string &filename, time_t dateTime);
void SetCreationTimeUtc(const std::string &filename, time_t dateTime);

void SetLastAccessTime(const std::string &filename, time_t dateTime);
void SetLastAccessTimeUtc(const std::string &filename, time_t dateTime);

void SetLastWriteTime(const std::string &filename, time_t dateTime);
void SetLastWriteTimeUtc(const std::string &filename, time_t dateTime);


////////////////////////////////////////////////////////////////////////////////
// Size                                                                       //
////////////////////////////////////////////////////////////////////////////////
size_t GetSize(const std::string &filename);
size_t GetSize(std::fstream &file_stream);

////////////////////////////////////////////////////////////////////////////////
// Write                                                                      //
////////////////////////////////////////////////////////////////////////////////
void WriteAllBytes(
    const std::string         &filename,
    const std::vector<byte_t> &bytes);

void WriteAllLines(
    const std::string              &filename,
    const std::vector<std::string> &lines);

void WriteAllText(
    const std::string &filename,
    const std::string &contents);

NS_COREFILE_END
