//~---------------------------------------------------------------------------//
//                     _______  _______  _______  _     _                     //
//                    |   _   ||       ||       || | _ | |                    //
//                    |  |_|  ||       ||   _   || || || |                    //
//                    |       ||       ||  | |  ||       |                    //
//                    |       ||      _||  |_|  ||       |                    //
//                    |   _   ||     |_ |       ||   _   |                    //
//                    |__| |__||_______||_______||__| |__|                    //
//                             www.amazingcow.com                             //
//  File      : CoreFile_Utils.h                                              //
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

//----------------------------------------------------------------------------//
// Namespaces                                                                 //
//----------------------------------------------------------------------------//
// All classes of this core is placed inside this namespace.
// We use MACROS so is easier to change if needed.
// Is (in our opinion) more explicit.
// And finally the editors will not reformat the code.
#define NS_COREFILE_BEGIN namespace CoreFile {
#define NS_COREFILE_END   }
#define USING_NS_COREFILE using namespace CoreFile

//----------------------------------------------------------------------------//
// Version                                                                    //
//----------------------------------------------------------------------------//
#define COW_COREFILE_VERSION_MAJOR    "0"
#define COW_COREFILE_VERSION_MINOR    "0"
#define COW_COREFILE_VERSION_REVISION "0"

#define COW_COREFILE_VERSION       \
    COW_COREFILE_VERSION_MAJOR "." \
    COW_COREFILE_VERSION_MINOR "." \
    COW_COREFILE_VERSION_REVISION
