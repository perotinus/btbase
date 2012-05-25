/*  Public platform independent Near Field Communication (NFC) library examples
 * 
 * Copyright (C) 2009, Roel Verdult
 * Copyright (C) 2010, Romuald Conty
 * Copyright (C) 2012, Jonathan MacMillan
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
 * Note that this license only applies on the examples, NFC library itself is 
 * under LGPL
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <string.h>
#include <ctype.h>

#include <nfc/nfc.h>

#include "nfc-utils.h"
#include "mifare.h"

#include "libbtbase.h"

static nfc_device *pnd;
static nfc_target nt;
static mifare_param mp;
static mifareul_tag mtDump;
static uint32_t uiBlocks = 0xF;

static const nfc_modulation nmMifare = {
    .nmt = NMT_ISO14443A,
    .nbr = NBR_106,
};


//Read a Mifare Ultralight card
static  bool
read_card(void)
{
    uint32_t page;
    bool    bFailure = false;

    for (page = 0; page <= uiBlocks; page += 4) {
        // Try to read out the data block
        if (nfc_initiator_mifare_cmd(pnd, MC_READ, page, &mp)) {
            memcpy(mtDump.amb[page / 4].mbd.abtData, mp.mpd.abtData, 16);
        } else {
            bFailure = true;
            break;
        }
    }

    return (!bFailure);
}

//Write a Mifare Ultralight card
static bool
write_card(unsigned char mm)
{
    uint32_t uiBlock = 0;
    bool bFailure    = false;


    //Setup the memory
    mtDump.amb[1].mbd.abtData[0] = 0xff;
    mtDump.amb[1].mbd.abtData[1] = 0xff;
    mtDump.amb[1].mbd.abtData[2] = 0xff;
    mtDump.amb[1].mbd.abtData[3] = 0xff;
    mtDump.amb[1].mbd.abtData[4] = 0x0;
    mtDump.amb[1].mbd.abtData[5] = 0x0;
    mtDump.amb[1].mbd.abtData[6] = 0x0;
    mtDump.amb[1].mbd.abtData[7] = 0x0;
    mtDump.amb[1].mbd.abtData[8] = mm;
    mtDump.amb[1].mbd.abtData[9] = 0x00;
    mtDump.amb[1].mbd.abtData[10] = 0x00;
    mtDump.amb[1].mbd.abtData[11] = 0x03;

    int page;
    for (page=0x04; page<=0x06; page++) {
    
        // For the Mifare Ultralight, this write command can be used
        // in compatibility mode, which only actually writes the first 
        // page (4 bytes). The Ultralight-specific Write command only
        // writes one page at a time.
        uiBlock = page / 4;
        memcpy(mp.mpd.abtData,mtDump.amb[uiBlock].mbd.abtData+((page%4)*4), 16);
        if (!nfc_initiator_mifare_cmd(pnd, MC_WRITE, page, &mp)) {
            bFailure = true;
        }
    }

    return true;
}

unsigned char btbaselib_custbt(int major, int minor) {
    return (unsigned char) ((minor << 4) + major & 0x0f);
}

int
btbaselib_setup()
{
    bool    bReadAction;
    FILE   *pfDump;

    nfc_init(NULL);
  
    //Try to open the NFC device
    pnd = nfc_open (NULL, NULL);
    if (pnd == NULL) {
        return BSETUP_NODEV;
    }

    if (nfc_initiator_init (pnd) < 0) {
        return BSETUP_FAIL;
    }

    return BSETUP_OK;
}


int 
checktag() 
{
    //Let the device only try once to find a tag
    if (nfc_device_set_property_bool(pnd, NP_INFINITE_SELECT, false) < 0) {
        nfc_perror(pnd, "nfc_device_set_property_bool");
        return BSETUP_NOBASE;
    }

    //Try to find a MIFARE Ultralight tag
    if (nfc_initiator_select_passive_target (pnd, nmMifare, NULL, 0, &nt) < 0) {
        return BSETUP_NOBASE;
    }

    //Test if we are dealing with a MIFARE compatible tag
    if (nt.nti.nai.abtAtqa[1] != 0x44) {
        return BSETUP_NOBASE;
    }

    return BSETUP_FOUNDBASE;
}

unsigned char 
btbaselib_readbase() 
{
    if (checktag() == BSETUP_NOBASE) {
        return BSETUP_NOBASE;
    }

    unsigned char basedata[64]; 
    if (read_card ()) {
        memcpy(basedata, &mtDump, sizeof (mtDump));
    }
    
    return basedata[BTBASELIB_MMPOS];
}

int 
btbaselib_writebase(unsigned char btype) 
{
    if (checktag() == BSETUP_NOBASE) {
        return BSETUP_NOBASE;
    }

    write_card(btype);   

    return BSETUP_FOUNDBASE;
}


void
btbaselib_close() 
{
    nfc_close(pnd);
    nfc_exit(NULL);
}

