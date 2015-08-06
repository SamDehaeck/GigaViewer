/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this header file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------
 
  File:        VmbCommonTypes.h

  Description: Main header file for the common types of the Vimba APIs.

-------------------------------------------------------------------------------

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
  NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
  DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, 
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED  
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#ifndef VMBCOMMONTYPES_H_INCLUDE_
#define VMBCOMMONTYPES_H_INCLUDE_

#ifdef __cplusplus
extern "C" {
#endif

// This file describes all necessary definitions for types used within AVT's 
// Vimba APIs. These type definitions are designed to be portable from other 
// languages and other operating systems.

#if defined (_MSC_VER)

    // 8 bit signed integer on Microsoft systems
    typedef __int8              VmbInt8_t;
    // 8 bit unsigned integer on Microsoft systems
    typedef unsigned __int8     VmbUint8_t;
    // 16 bit signed integer on Microsoft systems
    typedef __int16             VmbInt16_t;
    // 16 bit unsigned integer on Microsoft systems
    typedef unsigned __int16    VmbUint16_t;
    // 32 bit signed integer on Microsoft systems
    typedef __int32             VmbInt32_t;
    // 32 bit unsigned integer on Microsoft systems
    typedef unsigned __int32    VmbUint32_t;
    // 64 bit signed integer on Microsoft systems
    typedef __int64             VmbInt64_t;
    // 64 bit unsigned integer on Microsoft systems
    typedef unsigned __int64    VmbUint64_t;

#else // for non MS or GNU compilers without any warranty for the size

    //#pragma message("Compatibility warning: typedefs in " __FILE__ " may not have the correct number of bits")

    // 8 bit signed integer on non-Microsoft systems
    typedef signed char         VmbInt8_t;
    // 8 bit unsigned integer on non-Microsoft systems
    typedef unsigned char       VmbUint8_t;
    // 16 bit signed integer on non-Microsoft systems
    typedef short               VmbInt16_t;
    // 16 bit unsigned integer on non-Microsoft systems
    typedef unsigned short      VmbUint16_t;
    // 32 bit signed integer on non-Microsoft systems
    typedef int                 VmbInt32_t;
    // 32 bit signed integer on non-Microsoft systems
    typedef unsigned int        VmbUint32_t;
    // 64 bit signed integer on non-Microsoft systems
    typedef long long           VmbInt64_t;
    // 64 bit unsigned integer on non-Microsoft systems
    typedef unsigned long long  VmbUint64_t;

#endif

    // Handle; e.g. for a camera
    typedef void*               VmbHandle_t;

// Standard type for boolean values
#if defined(__cplusplus) || defined(__bool_true_false_are_defined)
    typedef bool                VmbBool_t;
#else
    // Boolean type (equivalent to char)
    typedef char                VmbBool_t;      // 1 means true and 0 means false
#endif
    //
    // enum for bool values
    //
    typedef enum VmbBoolVal
    {
        VmbBoolTrue = 1,
        VmbBoolFalse = 0,
    } VmbBoolVal;

    // char type
    typedef unsigned char       VmbUchar_t;

    //
    // Error codes, returned by most functions: (not yet complete)
    //
    typedef enum VmbErrorType
    {
        VmbErrorSuccess         =  0,           // No error
        VmbErrorInternalFault   = -1,           // Unexpected fault in VimbaC or driver
        VmbErrorApiNotStarted   = -2,           // VmbStartup() was not called before the current command
        VmbErrorNotFound        = -3,           // The designated instance (camera, feature etc.) cannot be found
        VmbErrorBadHandle       = -4,           // The given handle is not valid
        VmbErrorDeviceNotOpen   = -5,           // Device was not opened for usage
        VmbErrorInvalidAccess   = -6,           // Operation is invalid with the current access mode
        VmbErrorBadParameter    = -7,           // One of the parameters is invalid (usually an illegal pointer)
        VmbErrorStructSize      = -8,           // The given struct size is not valid for this version of the API
        VmbErrorMoreData        = -9,           // More data available in a string/list than space is provided
        VmbErrorWrongType       = -10,          // Wrong feature type for this access function 
        VmbErrorInvalidValue    = -11,          // The value is not valid; either out of bounds or not an increment of the minimum
        VmbErrorTimeout         = -12,          // Timeout during wait
        VmbErrorOther           = -13,          // Other error
        VmbErrorResources       = -14,          // Resources not available (e.g. memory)
        VmbErrorInvalidCall     = -15,          // Call is invalid in the current context (e.g. callback)
        VmbErrorNoTL            = -16,          // No transport layers are found 
        VmbErrorNotImplemented  = -17,          // API feature is not implemented
        VmbErrorNotSupported    = -18,          // API feature is not supported
        VmbErrorIncomplete      = -19,          // A multiple registers read or write is partially completed
    } VmbErrorType;
    typedef VmbInt32_t VmbError_t;              // Type for an error returned by API methods; for values see VmbErrorType

    //
    // Version information
    //
    typedef struct
    {
        VmbUint32_t             major;          // Major version number
        VmbUint32_t             minor;          // Minor version number
        VmbUint32_t             patch;          // Patch version number

    } VmbVersionInfo_t;

    //
    // Indicate if pixel is monochrome or RGB.
    //
    typedef enum VmbPixelType
    {
        VmbPixelMono  =         0x01000000,     // Monochrome pixel
        VmbPixelColor =         0x02000000      // Pixel bearing color information
    } VmbPixelType;

    //
    // Indicate number of bits for a pixel. Needed for building values ofVmbPixelFormatType
    //
    typedef enum VmbPixelOccupyType
    {
        VmbPixelOccupy8Bit  =   0x00080000,     // Pixel effectively occupies 8 bits
        VmbPixelOccupy12Bit =   0x000C0000,     // Pixel effectively occupies 12 bits
        VmbPixelOccupy16Bit =   0x00100000,     // Pixel effectively occupies 16 bits
        VmbPixelOccupy24Bit =   0x00180000,     // Pixel effectively occupies 24 bits
        VmbPixelOccupy32Bit =   0x00200000,     // Pixel effectively occupies 32 bits
        VmbPixelOccupy48Bit =   0x00300000      // Pixel effectively occupies 48 bits
    } VmbPixelOccupyType;

    //
    // Pixel format types.
    // As far as possible, the Pixel Naming Convention has been followed, allowing a few deviations.
    // If data spans more than one byte, it is always LSB aligned, except if stated differently.
    //
    typedef enum VmbPixelFormatType
    {
        VmbPixelFormatMono8           = VmbPixelMono  | VmbPixelOccupy8Bit  | 0x0001,  // Monochrome, 8 bits
        VmbPixelFormatMono10          = VmbPixelMono  | VmbPixelOccupy16Bit | 0x0003,  // Monochrome, 10 bits in 16 bits
        VmbPixelFormatMono12          = VmbPixelMono  | VmbPixelOccupy16Bit | 0x0005,  // Monochrome, 12 bits in 16 bits
        VmbPixelFormatMono12Packed    = VmbPixelMono  | VmbPixelOccupy12Bit | 0x0006,  // Monochrome, 2x12 bits in 24 bits
        VmbPixelFormatMono14          = VmbPixelMono  | VmbPixelOccupy16Bit | 0x0025,  // Monochrome, 14 bits in 16 bits
        VmbPixelFormatMono16          = VmbPixelMono  | VmbPixelOccupy16Bit | 0x0007,  // Monochrome, 16 bits
        VmbPixelFormatBayerGR8        = VmbPixelMono  | VmbPixelOccupy8Bit  | 0x0008,  // Bayer-color, 8 bits, starting with GR line
        VmbPixelFormatBayerRG8        = VmbPixelMono  | VmbPixelOccupy8Bit  | 0x0009,  // Bayer-color, 8 bits, starting with RG line
        VmbPixelFormatBayerGB8        = VmbPixelMono  | VmbPixelOccupy8Bit  | 0x000A,  // Bayer-color, 8 bits, starting with GB line
        VmbPixelFormatBayerBG8        = VmbPixelMono  | VmbPixelOccupy8Bit  | 0x000B,  // Bayer-color, 8 bits, starting with BG line
        VmbPixelFormatBayerGR10       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x000C,  // Bayer-color, 10 bits in 16 bits, starting with GR line
        VmbPixelFormatBayerRG10       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x000D,  // Bayer-color, 10 bits in 16 bits, starting with RG line
        VmbPixelFormatBayerGB10       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x000E,  // Bayer-color, 10 bits in 16 bits, starting with GB line
        VmbPixelFormatBayerBG10       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x000F,  // Bayer-color, 10 bits in 16 bits, starting with BG line
        VmbPixelFormatBayerGR12       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x0010,  // Bayer-color, 12 bits in 16 bits, starting with GR line
        VmbPixelFormatBayerRG12       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x0011,  // Bayer-color, 12 bits in 16 bits, starting with RG line
        VmbPixelFormatBayerGB12       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x0012,  // Bayer-color, 12 bits in 16 bits, starting with GB line
        VmbPixelFormatBayerBG12       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x0013,  // Bayer-color, 12 bits in 16 bits, starting with BG line
        VmbPixelFormatBayerGR12Packed = VmbPixelMono  | VmbPixelOccupy12Bit | 0x002A,  // Bayer-color, 2x12 bits in 24 bits, starting with GR line
        VmbPixelFormatBayerRG12Packed = VmbPixelMono  | VmbPixelOccupy12Bit | 0x002B,  // Bayer-color, 2x12 bits in 24 bits, starting with RG line
        VmbPixelFormatBayerGB12Packed = VmbPixelMono  | VmbPixelOccupy12Bit | 0x002C,  // Bayer-color, 2x12 bits in 24 bits, starting with GB line
        VmbPixelFormatBayerBG12Packed = VmbPixelMono  | VmbPixelOccupy12Bit | 0x002D,  // Bayer-color, 2x12 bits in 24 bits, starting with BG line
        VmbPixelFormatBayerGR16       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x002E,  // Bayer-color, 16 bits, starting with GR line
        VmbPixelFormatBayerRG16       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x002F,  // Bayer-color, 16 bits, starting with RG line
        VmbPixelFormatBayerGB16       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x0030,  // Bayer-color, 16 bits, starting with GB line
        VmbPixelFormatBayerBG16       = VmbPixelMono  | VmbPixelOccupy16Bit | 0x0031,  // Bayer-color, 16 bits, starting with BG line
        VmbPixelFormatRgb8            = VmbPixelColor | VmbPixelOccupy24Bit | 0x0014,  // RGB, 8 bits x 3
        VmbPixelFormatBgr8            = VmbPixelColor | VmbPixelOccupy24Bit | 0x0015,  // BGR, 8 bits x 3
        VmbPixelFormatArgb8           = VmbPixelColor | VmbPixelOccupy32Bit | 0x0016,  // ARGB, 8 bits x 4
        VmbPixelFormatRgba8           = VmbPixelFormatArgb8,                           // RGBA, 8 bits x 4, legacy name
        VmbPixelFormatBgra8           = VmbPixelColor | VmbPixelOccupy32Bit | 0x0017,  // BGRA, 8 bits x 4
        VmbPixelFormatRgb12           = VmbPixelColor | VmbPixelOccupy48Bit | 0x001A,  // RGB, 12 bits in 16 bits x 3
        VmbPixelFormatRgb16           = VmbPixelColor | VmbPixelOccupy48Bit | 0x0033,  // RGB, 16 bits x 3
        VmbPixelFormatYuv411          = VmbPixelColor | VmbPixelOccupy12Bit | 0x001E,  // YUV 411 with 8 bits
        VmbPixelFormatYuv422          = VmbPixelColor | VmbPixelOccupy16Bit | 0x001F,  // YUV 422 with 8 bits
        VmbPixelFormatYuv444          = VmbPixelColor | VmbPixelOccupy24Bit | 0x0020,  // YUV 444 with 8 bits
    } VmbPixelFormatType;
    typedef VmbUint32_t VmbPixelFormat_t;       // Type for the pixel format; for values see VmbPixelFormatType

#ifdef __cplusplus
}
#endif

#endif // VMBCOMMONTYPES_H_INCLUDE_
