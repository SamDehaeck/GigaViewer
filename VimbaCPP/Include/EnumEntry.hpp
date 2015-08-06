/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        EnumEntry.hpp

  Description: Inline wrapper functions for class AVT::VmbAPI::EnumEntry.

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

#ifndef AVT_VMBAPI_ENUMENTRY_HPP
#define AVT_VMBAPI_ENUMENTRY_HPP

//
// Inline wrapper functions that allocate memory for STL objects in the application's context
// and to pass data across DLL boundaries using arrays
//
inline VmbErrorType EnumEntry::GetName( std::string &rStrName ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetName( NULL, nLength );
    if ( VmbErrorSuccess == res )
    {
        if ( 0 < nLength )
        {
            rStrName.resize( nLength );
            res = GetName( &rStrName[0], nLength );
            if ( VmbErrorSuccess == res )
            {
                size_t nPos = rStrName.find( '\0' );
                if ( nLength-1 > nPos )
                {
                    rStrName.resize( nPos );
                }
            }
        }
        else
        {
            rStrName.clear();
        }
    }

    return res;
}

inline VmbErrorType EnumEntry::GetDisplayName( std::string &rStrDisplayName ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetDisplayName( NULL, nLength );
    if ( VmbErrorSuccess == res )
    {
        if ( 0 < nLength )
        {
            rStrDisplayName.resize( nLength );
            res = GetDisplayName( &rStrDisplayName[0], nLength );
            if ( VmbErrorSuccess == res )
            {
                size_t nPos = rStrDisplayName.find( '\0' );
                if ( nLength-1 > nPos )
                {
                    rStrDisplayName.resize( nPos );
                }
            }
        }
        else
        {
            rStrDisplayName.clear();
        }
    }

    return res;
}

inline VmbErrorType EnumEntry::GetDescription( std::string &rStrDescription ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetDescription( NULL, nLength );
    if ( VmbErrorSuccess == res )
    {
        if ( 0 < nLength )
        {
            rStrDescription.resize( nLength );
            res = GetDescription( &rStrDescription[0], nLength );
            if ( VmbErrorSuccess == res )
            {
                size_t nPos = rStrDescription.find( '\0' );
                if ( nLength-1 > nPos )
                {
                    rStrDescription.resize( nPos );
                }
            }
        }
        else
        {
            rStrDescription.clear();
        }
    }

    return res;
}

inline VmbErrorType EnumEntry::GetTooltip( std::string &rStrTooltip ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetTooltip( NULL, nLength );
    if ( VmbErrorSuccess == res )
    {
        if ( 0 < nLength )
        {
            rStrTooltip.resize( nLength );
            res = GetTooltip( &rStrTooltip[0], nLength );
            if ( VmbErrorSuccess == res )
            {
                size_t nPos = rStrTooltip.find( '\0' );
                if ( nLength-1 > nPos )
                {
                    rStrTooltip.resize( nPos );
                }
            }
        }
        else
        {
            rStrTooltip.clear();
        }
    }

    return res;
}

inline VmbErrorType EnumEntry::GetSFNCNamespace( std::string &rStrNamespace ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetSFNCNamespace( NULL, nLength );
    if ( VmbErrorSuccess == res )
    {
        if ( 0 < nLength )
        {
            rStrNamespace.resize( nLength );
            res = GetSFNCNamespace( &rStrNamespace[0], nLength );
            if ( VmbErrorSuccess == res )
            {
                size_t nPos = rStrNamespace.find( '\0' );
                if ( nLength-1 > nPos )
                {
                    rStrNamespace.resize( nPos );
                }
            }
        }
        else
        {
            rStrNamespace.clear();
        }
    }

    return res;
}

#endif
