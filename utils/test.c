#include <libbtbase.h>

#include <stdio.h>

int main(int argc, char**argv) {

    int setup_status = btbaselib_setup();

    if (setup_status != BSETUP_OK) {
        fprintf(stderr, "Could not initialize NFC device.");
    }

    //if (btbaselib_writebase(BASE_MED2) == BSETUP_NOBASE) {
    //    fprintf(stderr, "Could not write base!");
    //    btbaselib_close();
    //    return -1;
    //}
    
    int btype_int = btbaselib_readbase();

    if (btype_int == BSETUP_NOBASE)
        fprintf(stderr, "No base found!\n");
    else {
        //Lossy cast OK - if the read was good, there
        //will only be a value between 0 and 255 returned
        unsigned char btype = (unsigned char)btype_int;

        printf("Found base:");
        switch (btype) {
            case 0x30: printf("MED1"); break;
            case 0x31: printf("MED2"); break;
            case 0x40: printf("AMMO1"); break;
            case 0x41: printf("AMMO2"); break;
            case 0x50: printf("TBASE1"); break;
            case 0x51: printf("TBASE2"); break;
            case 0x52: printf("TBASE3"); break;
            case 0x53: printf("TBASE4"); break;
            default: printf("%x", btype);
        }
        printf("\n"); 
    }

    btbaselib_close();

    return 0;
}
    
