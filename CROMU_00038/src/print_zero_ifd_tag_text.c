/*

Author: Steve Wood <swood@cromulence.co>

Copyright (c) 2014 Cromulence LLC

Permission is hereby granted, __free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <libcgc.h>
#include "stdlib.h"
#include "service.h"

void print_tag_text(unsigned short tag) {


    switch (tag) {

        case 0x100:

            __printf("ImageWidth");
            break;
        
        case 0x101:

            __printf("ImageLength");
            break;

        case 0x102:

            __printf("BitsPerSample");
            break;

        case 0x103:

            __printf("Compression");
            break;

        case 0x106:

            __printf("PhotometricInterpretation");
            break;

        case 0x112:

            __printf("Orientation");
            break;

        case 0x115:

            __printf("SamplesPerPixel");
            break;

        case 0x11c:

            __printf("PlanarConfiguration");
            break;

        case 0x212:

            __printf("YCbCrSubSampling");
            break;

        case 0x213:

            __printf("YCbCrPositioning");
            break;

        case 0x11a:

            __printf("XResolution");
            break;

        case 0x11b:

            __printf("YResolution");
            break;

        case 0x128:

            __printf("ResolutionUnit");
            break;

        case 0x111:

            __printf("StripOffsets");
            break;

        case 0x116:

            __printf("RowsPerStrip");
            break;

        case 0x117:

            __printf("StripByteCounts");
            break;

        case 0x201:

            __printf("InterchangeFormat");
            break;

        case 0x202:

            __printf("InterchangeFormatLength");
            break;

        case 0x12d:

            __printf("TransferFunction");
            break;

        case 0x13e:

            __printf("WhitePoint");
            break;

        case 0x13f:

            __printf("PrimaryChromaticities");
            break;

        case 0x211:

            __printf("YCbCrCoefficients");
            break;

        case 0x214:

            __printf("ReferenceBlackWhite");
            break;

        case 0x132:

            __printf("DateTime");
            break;

        case 0x10e:

            __printf("ImageDescription");
            break;

        case 0x10f:

            __printf("Make");
            break;

        case 0x110:

            __printf("Model");
            break;

        case 0x131:

            __printf("Software");
            break;

        case 0x13b:

            __printf("Artist");
            break;

        case 0x8298:

            __printf("Copyright");
            break;

        case 0x8825:

            __printf("GPS Info");
            break;

        case 0x8769:

            __printf("EXIF IFD Pointer");
            break;

        case 0xc4a5:

            __printf("");
            break;

    }
}

