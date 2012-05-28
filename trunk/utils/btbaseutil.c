/*-
 i Public platform independent Near Field Communication (NFC) library examples
 * 
 * Copyright (C) 2009, Roel Verdult
// * Copyright (C) 2010, Romuald Conty
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

/**
 * @file nfc-mfultralight.c
 * @brief MIFARE Ultralight dump/restore tool
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <string.h>
#include <ctype.h>

#include <nfc/nfc.h>

#include "../src/mifare.h"

static nfc_device *pnd;
static nfc_target nt;
static mifare_param mp;
static mifareul_tag mtDump;
static uint32_t uiBlocks = 0xF;

static const nfc_modulation nmMifare = {
  .nmt = NMT_ISO14443A,
  .nbr = NBR_106,
};

static unsigned char basetypeMajMin(const char* bt) {

    if (!strcmp(bt, "AMMO1")) {
        return (4<<4)+0;
    } else if (!strcmp(bt, "AMMO2")) {
        return (4<<4)+1;
    } else if (!strcmp(bt, "MED1")) {
        return (3<<4)+0;
    } else if (!strcmp(bt, "MED2")) {
        return (3<<4)+1;
    } else if (!strcmp(bt, "TBASE1")) {
        return (5<<4)+0;
    } else if (!strcmp(bt, "TBASE2")) {
        return (5<<4)+1;
    } else if (!strcmp(bt, "TBASE3")) {
        return (5<<4)+2;
    } else if (!strcmp(bt, "TBASE4")) {
        return (5<<4)+3;
    } else {
        exit(fprintf(stderr,"No basetype called %s", bt));
        return 0;
    }

}

static  bool
read_card (void)
{
  uint32_t page;
  bool    bFailure = false;

  for (page = 0; page <= uiBlocks; page += 4) {
    // Try to read out the data block
    if (nfc_initiator_mifare_cmd (pnd, MC_READ, page, &mp)) {
      memcpy (mtDump.amb[page / 4].mbd.abtData, mp.mpd.abtData, 16);
    } else {
      bFailure = true;
      break;
    }

  }
  printf ("Done reading BattleTag base...");
  fflush (stdout);

  return (!bFailure);
}

static  bool
write_card (unsigned char minor, unsigned char major)
{
  uint32_t uiBlock = 0;
  bool    bFailure = false;


  //printf ("Write OTP bytes ? [yN] ");
  //if (!fgets (buffer, BUFSIZ, stdin)) {
  //  ERR ("Unable to read standard input.");
  //}
  

  //Setup the memory
  unsigned char mm = (minor << 4) + major;
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
    memcpy (mp.mpd.abtData, mtDump.amb[uiBlock].mbd.abtData + ((page % 4) * 4), 16);
    if (!nfc_initiator_mifare_cmd (pnd, MC_WRITE, page, &mp)) {
      bFailure = true;
    }
  }

  return true;
}

int
main (int argc, const char *argv[])
{
  bool    bReadAction;
  FILE   *pfDump;

  if (argc < 3) {
    printf ("\n");
    printf ("%s r <dumpfile> | w [<major.minor> | <basetype>] \n", argv[0]);
    printf ("\n");
    printf ("r <dumpfile>    - Perform read from or write to card\n");
    printf ("w <major.minor> - Write a base with the given major|minor numbers\n");
    printf ("w <basetype>    - Write a base with <basetype> \n                  (AMMO1,AMMO2,MED1,MED2,TBASE1,TBASE2,TBASE3,TBASE4)\n");
    printf ("\n");
    return 1;
  }


  bReadAction = tolower ((int) ((unsigned char) *(argv[1])) == 'r');

  if (bReadAction) {
    memset (&mtDump, 0x00, sizeof (mtDump));
  }

  printf("1");
  nfc_init (NULL);
  printf("2");
  
  // Try to open the NFC device
  pnd = nfc_open (NULL, NULL);
  if (pnd == NULL) {
    exit(fprintf(stderr,"Error opening NFC device\n"));
    return 1;
  }

  if (nfc_initiator_init (pnd) < 0) {
    nfc_perror (pnd, "nfc_initiator_init");
    exit (EXIT_FAILURE);    
  }

  // Let the device only try once to find a tag
  if (nfc_device_set_property_bool (pnd, NP_INFINITE_SELECT, false) < 0) {
    nfc_perror (pnd, "nfc_device_set_property_bool");
    exit (EXIT_FAILURE);
  }

  //printf ("NFC device: %s opened\n", nfc_device_get_name (pnd));

  // Try to find a MIFARE Ultralight tag
  if (nfc_initiator_select_passive_target (pnd, nmMifare, NULL, 0, &nt) < 0) {
    exit(fprintf(stderr,"no tag was found\n"));
    nfc_close (pnd);
    nfc_exit (NULL);
    return 1;
  }

  // Test if we are dealing with a MIFARE compatible tag
  if (nt.nti.nai.abtAtqa[1] != 0x44) {
    exit(fprintf(stderr,"not a BattleTag base\n"));
    nfc_close (pnd);
    nfc_exit (NULL);
    return EXIT_FAILURE;
  }

  // Get the info from the current tag
  printf("Found potential BattleTag base...\n");

  //size_t  szPos;
  //for (szPos = 0; szPos < nt.nti.nai.szUidLen; szPos++) {
  //  printf ("%02x", nt.nti.nai.abtUid[szPos]);
  //}
  //printf("\n");
  //

  if (bReadAction) {

    if (read_card ()) {

      printf ("Writing BattleTag base info to file: %s\n", argv[2]);
      fflush (stdout);
      pfDump = fopen (argv[2], "wb");
      if (pfDump == NULL) {
        printf ("Could not open file: %s\n", argv[2]);
        return EXIT_FAILURE;
      }
      if (fwrite (&mtDump, 1, sizeof (mtDump), pfDump) != sizeof (mtDump)) {
        printf ("Could not write to file: %s\n", argv[2]);
        return EXIT_FAILURE;
      }
      fclose (pfDump);
      printf ("Done.\n");
    }
  } else {
    if (argv[2] && argv[2][0] >= '0' && argv[2][0] <= '9') {
      unsigned int major, minor;
      sscanf(argv[2], "%u.%u", &major, &minor);
      //printf("maj:%d,min:%d\n",major, minor);
      write_card ((unsigned char)major, (unsigned char)minor);
    } else if (argv[2]) {
        //printf("%s\n\n", argv[2]);
        unsigned char mm = basetypeMajMin(argv[2]);
        if (!mm) {
            nfc_close(pnd);
            nfc_exit(NULL);
            return EXIT_FAILURE;
        }

        //printf("maj:%d,min:%d",(mm >> 4), (mm & 0xf));
        write_card((unsigned char)(mm >> 4), (unsigned char)(mm & 0xf));   
    }
  }


  nfc_close (pnd);
  nfc_exit (NULL);
  return EXIT_SUCCESS;
}
