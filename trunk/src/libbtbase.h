#ifndef BTBASELIB_H
#define BTBASELIB_H
/*
 * btbaselib.h - some useful functions and definitions
 * Copyright (C), Jonathan MacMillan 
 * 5/24/12
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  1) Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer. 
 *  2 )Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * Note that this license only applies on the examples, NFC library itself is under LGPL
 *
 */

#define BTBASELIB_MMPOS 24
#define BTBASELIB_03POS 27
//Return values from btbaselib_setup (start at 0x100 to avoid any of the 0xff base types)
enum {BSETUP_OK=0x100, BSETUP_FOUNDBASE, BSETUP_NODEV, BSETUP_NOBASE, BSETUP_FAIL};

//Basetypes
enum {  
        BASE_AMMO1  = (4<<4)+0, 
        BASE_AMMO2  = (4<<4)+1, 
        BASE_MED1   = (3<<4)+0,
        BASE_MED2   = (3<<4)+1,
        BASE_TBASE1 = (5<<4)+0,
        BASE_TBASE2 = (5<<4)+1,
        BASE_TBASE3 = (5<<4)+2,
        BASE_TBASE4 = (5<<4)+3
};

//Create a custom basetype
unsigned char btbaselib_custbt(int major, int minor);

//Get the major/minor number from a dump
//inline unsigned char btbaselib_getmajor(char *s) {
//    return (s[BTBASELIB_MMPOS] & 0x0f);
//}

//inline unsigned char btbaselib_getminor(char *s) {
//    return (s[BTBASELIB_MMPOS] >> 4);
//}

//Setup the NFC base reading
int btbaselib_setup();

//Close the NFC base reading
void btbaselib_close();

//Read/write
unsigned char btbaselib_readbase();
int btbaselib_writebase(unsigned char btype);

#endif
