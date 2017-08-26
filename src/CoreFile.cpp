//----------------------------------------------------------------------------//
//               █      █                                                     //
//               ████████                                                     //
//             ██        ██                                                   //
//            ███  █  █  ███        CoreFile.cpp                              //
//            █ █        █ █        CoreFile                                  //
//             ████████████                                                   //
//           █              █       Copyright (c) 2017                        //
//          █     █    █     █      AmazingCow - www.AmazingCow.com           //
//          █     █    █     █                                                //
//           █              █       N2OMatt - n2omatt@amazingcow.com          //
//             ████████████         www.amazingcow.com/n2omatt                //
//                                                                            //
//                  This software is licensed as GPLv3                        //
//                 CHECK THE COPYING FILE TO MORE DETAILS                     //
//                                                                            //
//    Permission is granted to anyone to use this software for any purpose,   //
//   including commercial applications, and to alter it and redistribute it   //
//               freely, subject to the following restrictions:               //
//                                                                            //
//     0. You **CANNOT** change the type of the license.                      //
//     1. The origin of this software must not be misrepresented;             //
//        you must not claim that you wrote the original software.            //
//     2. If you use this software in a product, an acknowledgment in the     //
//        product IS HIGHLY APPRECIATED, both in source and binary forms.     //
//        (See opensource.AmazingCow.com/acknowledgment.html for details).    //
//        If you will not acknowledge, just send us a email. We'll be         //
//        *VERY* happy to see our work being used by other people. :)         //
//        The email is: acknowledgment_opensource@AmazingCow.com              //
//     3. Altered source versions must be plainly marked as such,             //
//        and must not be misrepresented as being the original software.      //
//     4. This notice may not be removed or altered from any source           //
//        distribution.                                                       //
//     5. Most important, you must have fun. ;)                               //
//                                                                            //
//      Visit opensource.amazingcow.com for more open-source projects.        //
//                                                                            //
//                                  Enjoy :)                                  //
//----------------------------------------------------------------------------//

//Header
#include "../include/CoreFile.h"
//std
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <iterator>
#include <sstream>
//CoreFS
#include "CoreFS.h"


////////////////////////////////////////////////////////////////////////////////
// Helper Functions                                                           //
////////////////////////////////////////////////////////////////////////////////
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
    //Text mode.
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

    //Binary mode.
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
}


////////////////////////////////////////////////////////////////////////////////
// Append                                                                     //
////////////////////////////////////////////////////////////////////////////////
//Appends lines to a file, and then closes the file.
//If the specified file does not exist, this method creates a file,
//writes the specified lines to the file, and then closes the file.
void CoreFile::AppendAllLines(
    const std::string              &filename,
    const std::vector<std::string> &lines)
{
    //COWTODO(n2omatt): How handle errors?
    std::stringstream ss;
    std::string new_line = CoreFS::NewLine();

    std::copy(
        lines.begin(),
        lines.end(),
        std::ostream_iterator<std::string>(ss, new_line.c_str())
    );

    CoreFile::AppendAllText(filename, ss.str());
}

//Opens a file, appends the specified string to the file,
//and then closes the file.
//If the file does not exist, this method creates a file, writes
//the specified string to the file, then closes the file.
void CoreFile::AppendAllText(
    const std::string &filename,
    const std::string &contents)
{
    //COWTODO(n2omatt): Should we check errors or let the std crash itself???
    auto file_stream = CoreFile::Open(filename, FileMode::Text::kAppend);
    file_stream << contents;
}


////////////////////////////////////////////////////////////////////////////////
// Copy                                                                       //
////////////////////////////////////////////////////////////////////////////////
//Copies an existing file to a new file.
//Overwriting a file of the same name is allowed.
void CoreFile::Copy(
    const std::string &src,
    const std::string &dst,
    bool               overwrite)
{
    //COWTODO(n2omatt): How should we handle errors?
    //  Error codes, exceptions, return false, ignore????
    if(!CoreFS::Exists(src))
        return;

    if(CoreFS::Exists(dst) && !overwrite)
        return;

    CoreFile::WriteAllBytes(
        dst,
        CoreFile::ReadAllBytes(src)
    );
}


////////////////////////////////////////////////////////////////////////////////
// Create                                                                     //
////////////////////////////////////////////////////////////////////////////////
//Creates or overwrites a file in the specified path.
std::fstream CoreFile::Create(const std::string &filename)
{
    return CoreFile::Open(filename, FileMode::Binary::kReadWrite_Truncate);
}

//Creates or opens a file for writing encoded text.
std::fstream CoreFile::CreateText(const std::string &filename)
{
    return CoreFile::Open(filename, FileMode::Text::kReadWrite_Truncate);
}



////////////////////////////////////////////////////////////////////////////////
// Delete                                                                     //
////////////////////////////////////////////////////////////////////////////////
//Deletes the specified file.
void CoreFile::Delete(const std::string &filename)
{
    //COWTODO(n2omatt): How we gonna handle errors....
    remove(filename.c_str());
}


////////////////////////////////////////////////////////////////////////////////
// Exists                                                                     //
////////////////////////////////////////////////////////////////////////////////
//Determines whether the specified file exists.
bool CoreFile::Exist(const std::string &filename)
{
    return CoreFS::IsFile(filename);
}


//COWTODO: Gonna implement that???
//GetAttributes(const std::string &filename) ???

////////////////////////////////////////////////////////////////////////////////
// Get * Time                                                                 //
////////////////////////////////////////////////////////////////////////////////
//Returns the creation date and time of the specified file or directory.
CoreFile::tm_t CoreFile::GetCreationTime(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetCTime(filename), true);
}

//Returns the creation date and time, in coordinated universal time (UTC),
//of the specified file or directory
CoreFile::tm_t CoreFile::GetCreationTimeUtc(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetCTime(filename), false);
}

//Returns the date and time the specified file or directory was last accessed.
CoreFile::tm_t CoreFile::GetLastAccessTime(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetATime(filename), true);
}

//Returns the date and time, in coordinated universal time (UTC),
//that the specified file or directory was last accessed.
CoreFile::tm_t CoreFile::GetLastAccessTimeUtc(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetATime(filename), false);
}

//Returns the date and time the specified file or
//directory was last written to.
CoreFile::tm_t CoreFile::GetLastWriteTime(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetMTime(filename), true);
}

//Returns the date and time the specified file or
//directory was last written to.
CoreFile::tm_t CoreFile::GetLastWriteTimeUtc(const std::string &filename)
{
    return time_t_to_tm_t(CoreFS::GetMTime(filename), false);
}


////////////////////////////////////////////////////////////////////////////////
// Move                                                                       //
////////////////////////////////////////////////////////////////////////////////
//Moves a specified file to a new location, providing
//the option to specify a new file name.
void CoreFile::Move(const std::string &src, const std::string &dst)
{
    //COWTODO(n2omatt): How we gonna handle errors?
    rename(src.c_str(), dst.c_str());
}


////////////////////////////////////////////////////////////////////////////////
// Open                                                                       //
////////////////////////////////////////////////////////////////////////////////
//Opens a std::fstream on the specified path, with the specified mode.
std::fstream CoreFile::Open(
    const std::string &filename,
    const std::string &fileMode)
{
    //COWTODO(n2omatt): How we gonna handle errors???
    std::fstream stream;
    auto openmode = filemode_to_openmode(fileMode);
    stream.open(filename.c_str(), openmode);

    return stream;
}

//Opens an existing file for reading.
std::fstream CoreFile::OpenRead(const std::string &filename)
{
    //COWTODO(n2omatt): How we gonna handle errors???
    return Open(filename, FileMode::Binary::kRead);
}

//Opens an existing text file for reading.
std::fstream CoreFile::OpenText(const std::string &filename)
{
    //COWTODO(n2omatt): How we gonna handle errors???
    return Open(filename, FileMode::Text::kRead);
}

//Opens an existing file or creates a new file for writing.
std::fstream CoreFile::OpenWrite(const std::string &filename)
{
    //COWTODO(n2omatt): How we gonna handle errors???
    return Open(filename, FileMode::Binary::kWrite);
}


////////////////////////////////////////////////////////////////////////////////
// Read                                                                       //
////////////////////////////////////////////////////////////////////////////////
//Opens a binary file, reads the contents of the file into a byte array,
//and then closes the file.
std::vector<CoreFile::byte_t> CoreFile::ReadAllBytes(const std::string &filename)
{
    //COWTODO(n2omatt): How we gonna handle errors??
    std::vector<CoreFile::byte_t> ret_val;

    if(!CoreFile::Exist(filename))
        return ret_val;

    //Open the file and calculate it's size.
    auto file_stream = CoreFile::OpenRead(filename);
    auto size        = CoreFile::GetSize(file_stream);

    //Play nice with memory.
    ret_val.resize(size);
    file_stream.read((char *)(&ret_val[0]), size);

    return ret_val;
}

std::vector<std::string> CoreFile::ReadAllLines(const std::string &filename)
{
    //COWTODO(n2omatt): How we gonna handle errors??
    std::vector<std::string> ret_val;

    if(!CoreFile::Exist(filename))
        return ret_val;

    //Open the file and calculate it's size.
    auto file_stream = CoreFile::OpenText(filename);
    while(!file_stream.eof())
    {
        std::string line;
        std::getline(file_stream, line);
        ret_val.push_back(line);
    }

    return ret_val;
}

std::string CoreFile::ReadAllText(const std::string &filename)
{
    //COWTODO(n2omatt): How we gonna handle errors??
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


////////////////////////////////////////////////////////////////////////////////
// Set * Time                                                                 //
////////////////////////////////////////////////////////////////////////////////
void CoreFile::SetCreationTime(const std::string &filename, time_t dateTime)
{
    //http://rosettacode.org/wiki/File_modification_time#C.2B.2B
    //utimensat
}

void CoreFile::SetCreationTimeUtc(const std::string &filename, time_t dateTime)
{

}


void CoreFile::SetLastAccessTime(const std::string &filename, time_t dateTime)
{

}

void CoreFile::SetLastAccessTimeUtc(const std::string &filename, time_t dateTime)
{

}


void CoreFile::SetLastWriteTime(const std::string &filename, time_t dateTime)
{

}
void CoreFile::SetLastWriteTimeUtc(const std::string &filename, time_t dateTime)
{

}


////////////////////////////////////////////////////////////////////////////////
// Size                                                                       //
////////////////////////////////////////////////////////////////////////////////
size_t CoreFile::GetSize(const std::string &filename)
{
    //COWTODO(n2omatt): How we gonna handle errors???
    if(!CoreFile::Exist(filename))
        return 0;

    auto stream = OpenRead(filename);
    return GetSize(stream);
}

size_t CoreFile::GetSize(std::fstream &file_stream)
{
    auto curr = file_stream.tellg();

    file_stream.seekg(0, std::ios::beg);
    auto beg = file_stream.tellg();

    file_stream.seekg(0, std::ios::end);
    auto end = file_stream.tellg();

    file_stream.seekg(curr, std::ios::beg);

    auto size = static_cast<size_t>(end - beg);
    return size;
}


////////////////////////////////////////////////////////////////////////////////
// Write                                                                      //
////////////////////////////////////////////////////////////////////////////////
void CoreFile::WriteAllBytes(
    const std::string         &filename,
    const std::vector<byte_t> &bytes)
{
    //COWTODO(n2omatt): How we gonna handle errors??
    auto file_stream = CoreFile::Open(
        filename,
        FileMode::Binary::kReadWrite_Truncate
    );
    std::copy(
        bytes.begin(),
        bytes.end  (),
        std::ostream_iterator<CoreFile::byte_t>(file_stream)
    );
}

void CoreFile::WriteAllLines(
    const std::string              &filename,
    const std::vector<std::string> &lines)
{
    //COWTODO(n2omatt): How we gonna handle errors??
    auto file_stream = CoreFile::Open(
        filename,
        FileMode::Text::kReadWrite_Truncate
    );

    std::copy(
        lines.begin(),
        lines.end  (),
        std::ostream_iterator<std::string>(file_stream, CoreFS::NewLine().c_str())
    );
}

void CoreFile::WriteAllText(
    const std::string &filename,
    const std::string &contents)
{
    //COWTODO(n2omatt): How we gonna handle errors??
    auto file_stream = CoreFile::Open(
        filename,
        FileMode::Text::kReadWrite_Truncate
    );

    std::copy(
        contents.begin(),
        contents.end  (),
        std::ostream_iterator<char>(file_stream)
    );
}


