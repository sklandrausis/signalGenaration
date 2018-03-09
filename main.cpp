#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "vdifio.h"

using namespace std;

int main(int argc, char **argv){


    char buffer[MAX_VDIF_FRAME_BYTES];
    FILE * input;
    FILE * output;
    int readbytes, framebytes, stationid, numchannels, bitspersample;

    long long framesread;
    vdif_header *header;

    // Ievad un izvad faila lasisana
    input = fopen(argv[1], "r");
    if(input == NULL){
        fprintf(stderr, "Cannot open input file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    output = fopen(argv[2], "w");
    if(output == NULL){
        fprintf(stderr, "Cannot open output file %s\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    float delay = atof(argv[3]);

    header = (vdif_header*)&buffer;
    framesread = 0;

    readbytes = fread(buffer, 1, 0, input);
    while(!feof(input)) {

    readbytes = fread(buffer, 1, VDIF_HEADER_BYTES, input); //read the VDIF header
    if (readbytes < VDIF_HEADER_BYTES){
      break;
    }

    stationid = getVDIFStationID(header);
    bitspersample = getVDIFBitsPerSample(header);
    numchannels = getVDIFNumChannels(header);
    framebytes = getVDIFFrameBytes(header);

    readbytes = fwrite(buffer, 1, VDIF_HEADER_BYTES, output); //write out the VDIF header
    readbytes = fread(buffer, 1, framebytes-VDIF_HEADER_BYTES, input); //read the VDIF data segment
    readbytes = fwrite(buffer, 1, framebytes-VDIF_HEADER_BYTES, output); //write out the VDIF data segment

    framesread++;
    }

    for (int i = 0; i<10; i++){
        cout<<buffer[i]<<endl;
    }

    printf("Read and wrote %lld frames\n", framesread);
    fclose(input);
    fclose(output);

    return EXIT_SUCCESS;
}
