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


void print_xif_tag_text(unsigned short tag) 

{
    switch (tag) {

        case 0x9000:

            __printf("ExifVersion");
            break;

        case 0xa000:

            __printf("FlashpixVersion");
            break;

        case 0xa001:

            __printf("ColorSpace");
            break;

        case 0x9101:

            __printf("ComponentsConfiguration");
            break;

        case 0x9102:

            __printf("CompressedBitsPerPixel");
            break;

        case 0xa002:

            __printf("PixelXDimension");
            break;

        case 0xa003:

            __printf("PixelYDimension");
            break;

        case 0x927c:

            __printf("MakeNote");
            break;

        case 0x9286:

            __printf("UserComment");
            break;

        case 0xa004:

            __printf("RelatedSoundFile");
            break;

        case 0x9003:

            __printf("DateTimeOriginal");
            break;

        case 0x9004:

            __printf("DateTimeDigitized");
            break;

        case 0x9290:

            __printf("SubSecTime");
            break;

        case 0x9291:

            __printf("SubSecTimeOriginal");
            break;

        case 0x9292:

            __printf("SubSecTimeDigitized");
            break;

        case 0xa420:

            __printf("ImageUniqueID");
            break;

        case 0x829a:

            __printf("ExposureTime");
            break;

        case 0x829d:

            __printf("FNumber");
            break;

        case 0x8822:

            __printf("ExposureProgram");
            break;

        case 0x8824:

            __printf("SpectralSensitivity");
            break;

        case 0x8827:

            __printf("ISOSpeedRatings");
            break;

        case 0x8828:

            __printf("OECF");
            break;

        case 0x9201:

            __printf("ShutterSpeedValue");
            break;

        case 0x9202:

            __printf("ApertureValue");
            break;

        case 0x9203:

            __printf("BrightnessValue");
            break;

        case 0x9204:

            __printf("ExposureBiasValue");
            break;

        case 0x9205:

            __printf("MaxApertureValue");
            break;

        case 0x9206:

            __printf("SubjectDistance");
            break;

        case 0x9207:

            __printf("MeteringMode");
            break;

        case 0x9208:

            __printf("LightSource");
            break;

        case 0x9209:

            __printf("Flash");
            break;

        case 0x920a:

            __printf("FocalLength");
            break;

        case 0x9214:

            __printf("SubjectArea");
            break;

        case 0xa20b:

            __printf("FlashEnergy");
            break;

        case 0xa20c:

            __printf("SpatialFrequencyResponse");
            break;

        case 0xa20e:

            __printf("FocalPlaneXResolution");
            break;

        case 0xa20f:

            __printf("FocalPlaneYResolution");
            break;

        case 0xa210:

            __printf("FocalPlaneResolutionUnit");
            break;

        case 0xa214:

            __printf("SubjectLocation");
            break;

        case 0xa215:

            __printf("ExposureIndex");
            break;

        case 0xa217:

            __printf("SensingMethod");
            break;

        case 0xa300:

            __printf("FileSource");
            break;

        case 0xa301:

            __printf("SceneType");
            break;

        case 0xa302:

            __printf("CFAPattern");
            break;

        case 0xa401:

            __printf("CustomRendered");
            break;

        case 0xa402:

            __printf("ExposureMode");
            break;

        case 0xa403:

            __printf("WhiteBalance");
            break;

        case 0xa404:

            __printf("DigitalZoomRatio");
            break;

        case 0xa405:

            __printf("FocalLengthin35mmFilm");
            break;

        case 0xa406:

            __printf("SceneCaptureType");
            break;

        case 0xa407:

            __printf("GainControl");
            break;

        case 0xa408:

            __printf("Contrast");
            break;

        case 0xa409:

            __printf("Saturation");
            break;

        case 0xa40a:

            __printf("Sharpness");
            break;

        case 0xa40b:

            __printf("DeviceSettingDescription");
            break;

        case 0xa40c:

            __printf("SubjectDistanceRange");
            break;

    } //switch

} // print_xif_tag_text

