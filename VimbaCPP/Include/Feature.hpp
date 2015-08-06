/*=============================================================================
  Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        Feature.hpp

  Description: Inline wrapper functions for class AVT::VmbAPI::Feature.

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

#ifndef AVT_VMBAPI_FEATURE_HPP
#define AVT_VMBAPI_FEATURE_HPP

//
// Inline wrapper functions that allocate memory for STL objects in the application's context
// and to pass data across DLL boundaries using arrays
//
inline VmbErrorType Feature::GetValues( StringVector &rValues )
{
    VmbErrorType res;
    VmbUint32_t nSize;
    std::vector<const char*> data;

    res = GetValues( (const char **)NULL, nSize );
    if (    VmbErrorSuccess == res
        &&  0 < nSize )
    {
        data.resize( nSize );

        res = GetValues( &data[0], nSize );
    }

    if ( VmbErrorSuccess == res )
    {
        rValues.clear();

        for (   std::vector<const char*>::iterator iter = data.begin();
                data.end() != iter;
                ++iter )
        {
            rValues.push_back( std::string( *iter ));
        }
    }

    return res;
}

inline VmbErrorType Feature::GetEntries( EnumEntryVector &rEntries )
{
    VmbErrorType res;
    VmbUint32_t nSize;

    res = GetEntries( (EnumEntry*)NULL, nSize );
    if (    VmbErrorSuccess == res
         && 0 < nSize )
    {
        rEntries.resize( nSize );

        res = GetEntries( &rEntries[0], nSize );
    }

    return res;
}

inline VmbErrorType Feature::GetValues( Int64Vector &rValues )
{
    VmbErrorType res;
    VmbUint32_t nSize;

    res = GetValues( (VmbInt64_t*)NULL, nSize );
    if (    VmbErrorSuccess == res
         && 0 < nSize )
    {
        rValues.resize( nSize );

        res = GetValues( &rValues[0], nSize );
    }

    return res;
}

inline VmbErrorType Feature::GetValue( std::string &rStrValue ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetValue( (char * const)NULL, nLength );
    if ( VmbErrorSuccess == res )
    {
        if ( 0 < nLength )
        {
            rStrValue.resize( nLength );
            res = GetValue( &rStrValue[0], nLength );
            if ( VmbErrorSuccess == res )
            {
                size_t nPos = rStrValue.find( '\0' );
                if ( nLength-1 > nPos )
                {
                    rStrValue.resize( nPos );
                }
            }
        }
        else
        {
            rStrValue.clear();
        }
    }

    return res;
}

inline VmbErrorType Feature::GetValue( UcharVector &rValue ) const
{
    VmbUint32_t i;
    return GetValue( rValue, i );
}
inline VmbErrorType Feature::GetValue( UcharVector &rValue, VmbUint32_t &rnSizeFilled ) const
{
    VmbErrorType res;
    VmbUint32_t nSize;

    res = GetValue( NULL, nSize, rnSizeFilled );
    if ( VmbErrorSuccess == res )
    {
        rValue.resize( nSize );
        res = GetValue( &rValue[0], nSize, rnSizeFilled );
    }

    return res;
}

inline VmbErrorType Feature::SetValue( const UcharVector &rValue )
{
    if ( rValue.empty() )
    {
        return VmbErrorBadParameter;
    }
    return SetValue( &rValue[0], (VmbUint32_t)rValue.size() );
}

inline VmbErrorType Feature::GetName( std::string &rStrName ) const
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
        }
        else
        {
            rStrName.clear();
        }
    }

    return res;
}

inline VmbErrorType Feature::GetDisplayName( std::string &rStrDisplayName ) const
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
        }
        else
        {
            rStrDisplayName.clear();
        }
    }

    return res;
}

inline VmbErrorType Feature::GetCategory( std::string &rStrCategory ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetCategory( NULL, nLength );
    if ( VmbErrorSuccess == res )
    {
        if ( 0 < nLength )
        {
            rStrCategory.resize( nLength );
            res = GetCategory( &rStrCategory[0], nLength );
        }
        else
        {
            rStrCategory.clear();
        }
    }

    return res;
}

inline VmbErrorType Feature::GetUnit( std::string &rStrUnit ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetUnit( NULL, nLength );
    if ( VmbErrorSuccess == res )
    {
        if ( 0 < nLength )
        {
            rStrUnit.resize( nLength );
            res = GetUnit( &rStrUnit[0], nLength );
        }
        else
        {
            rStrUnit.clear();
        }
    }

    return res;
}

inline VmbErrorType Feature::GetRepresentation( std::string &rStrRepresentation ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetRepresentation( NULL, nLength );
    if ( VmbErrorSuccess == res )
    {
        if ( 0 < nLength )
        {
            rStrRepresentation.resize( nLength );
            res = GetRepresentation( &rStrRepresentation[0], nLength );
        }
        else
        {
            rStrRepresentation.clear();
        }
    }

    return res;
}

inline VmbErrorType Feature::GetToolTip( std::string &rStrToolTip ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetToolTip( NULL, nLength );
    if ( VmbErrorSuccess == res )
    {
        if ( 0 < nLength )
        {
            rStrToolTip.resize( nLength );
            res = GetToolTip( &rStrToolTip[0], nLength );
        }
        else
        {
            rStrToolTip.clear();
        }
    }

    return res;
}

inline VmbErrorType Feature::GetDescription( std::string &rStrDescription ) const
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
        }
        else
        {
            rStrDescription.clear();
        }
    }

    return res;
}

inline VmbErrorType Feature::GetSFNCNamespace( std::string &rStrSFNCNamespace ) const
{
    VmbErrorType res;
    VmbUint32_t nLength;

    res = GetSFNCNamespace( NULL, nLength );
    if ( VmbErrorSuccess == res )        
    {
        if ( 0 < nLength )
        {
            rStrSFNCNamespace.resize( nLength );
            res = GetSFNCNamespace( &rStrSFNCNamespace[0], nLength );
        }
        else
        {
            rStrSFNCNamespace.clear();
        }
    }

    return res;
}

inline VmbErrorType Feature::GetAffectedFeatures( FeaturePtrVector &rAffectedFeatures )
{
    VmbErrorType res;
    VmbUint32_t nSize;

    res = GetAffectedFeatures( NULL, nSize );
    if (    VmbErrorSuccess == res
        && 0 < nSize )
    {
        rAffectedFeatures.resize( nSize );
        res = GetAffectedFeatures( &rAffectedFeatures[0], nSize );
    }

    return res;
}

inline VmbErrorType Feature::GetSelectedFeatures( FeaturePtrVector &rselectedFeatures )
{
    VmbErrorType res;
    VmbUint32_t nSize;

    res = GetSelectedFeatures( NULL, nSize );
    if (    VmbErrorSuccess == res
         && 0 < nSize )
    {
        rselectedFeatures.resize( nSize );
        res = GetSelectedFeatures( &rselectedFeatures[0], nSize );
    }

    return res;
}

#endif
