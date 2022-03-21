/*
* Copyright (c) 2021-2022, The PurpleI2P Project
*
* This file is part of Purple i2pd project and licensed under BSD3
*
* See full license text in LICENSE file at top of project tree
*/

#include "../libi2pd/api.h"
#include "capi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static std::string RET_STR;

#ifdef __cplusplus
extern "C" {
#endif

void C_InitI2P (int argc, char *argv[], const char * appName)
{
	const char* delim = " ";
	char* vargs = strdup(argv);
	char** args = str_split(vargs, *delim);
//    std::shared_ptr<std::ostream> p_cout(&std::cout, [](std::ostream*){});
	return i2p::api::InitI2P(argc, args, appName, nullptr);
}

void C_StartI2P ()
{
	return i2p::api::StartI2P();
}

void C_CloseAcceptsTunnels () {
    return i2p::api::CloseAcceptsTunnels();
}

void C_StopI2P ()
{
	return i2p::api::StopI2P();
}

void C_RunPeerTest ()
{
	return i2p::api::RunPeerTest();
}

const char * C_GetNetworkStatus ()
{
    int ret = i2p::api::GetNetworkStatus(RET_STR);
    if (ret == 0)
    {
        return RET_STR.c_str();
    } else
    {
        return NULL;
    };
}

int C_GetTunnelCreationSuccessRate ()
{
    int ret = i2p::api::GetTunnelCreationSuccessRate();
    return ret;
}

uint64_t C_GetReceivedByte ()
{
    uint64_t ret = i2p::api::GetReceivedByte();
    return ret;
}

uint32_t C_GetInBandwidth ()
{
    uint32_t ret = i2p::api::GetInBandwidth();
    return ret;
}

uint64_t C_GetSentByte ()
{
    uint64_t ret = i2p::api::GetSentByte();
    return ret;
}

uint32_t C_GetOutBandwidth ()
{
    uint32_t ret = i2p::api::GetOutBandwidth();
    return ret;
}

uint64_t C_GetTransitByte ()
{
    uint64_t ret = i2p::api::GetTransitByte();
    return ret;
}

uint32_t C_GetTransitBandwidth ()
{
    uint32_t ret = i2p::api::GetTransitBandwidth();
    return ret;
}

int C_IsHTTPProxyEnabled ()
{
    int ret = i2p::api::IsHTTPProxyEnabled();
    return ret;
}

int C_IsSOCKSProxyEnabled ()
{
    int ret = i2p::api::IsSOCKSProxyEnabled();
    return ret;
}

int C_IsBOBEnabled ()
{
    int ret = i2p::api::IsBOBEnabled();
    return ret;
}

int C_IsSAMEnabled ()
{
    int ret = i2p::api::IsSAMEnabled();
    return ret;
}

int C_IsI2CPEnabled ()
{
    int ret = i2p::api::IsI2CPEnabled();
    return ret;
}

int C_GetClientTunnelsCount ()
{
    int ret = i2p::api::GetClientTunnelsCount();
    return ret;
}

int C_GetServerTunnelsCount ()
{
    int ret = i2p::api::GetServerTunnelsCount();
    return ret;
}

const char * C_GetClientTunnelsName (int index)
{
    if (index < C_GetClientTunnelsCount())
    {
        int ret = i2p::api::GetClientTunnelsName(RET_STR, index);
        if (ret == 0)
        {
            return RET_STR.c_str();
        } else
        {
            return NULL;
        };
    }
    return NULL;
}

const char * C_GetClientTunnelsIdent (int index)
{
    if (index < C_GetClientTunnelsCount())
    {
        int ret = i2p::api::GetClientTunnelsIdent(RET_STR, index);
        if (ret == 0)
        {
            return RET_STR.c_str();
        } else
        {
            return NULL;
        };
    }
    return NULL;
}

const char * C_GetHTTPProxyIdent ()
{
    int ret = i2p::api::GetHTTPProxyIdent(RET_STR);
    if (ret == 0)
    {
        return RET_STR.c_str();
    } else
    {
        return NULL;
    };
}

const char * C_GetSOCKSProxyIdent ()
{
    int ret = i2p::api::GetSOCKSProxyIdent(RET_STR);
    if (ret == 0)
    {
        return RET_STR.c_str();
    } else
    {
        return NULL;
    };
}

const char * C_GetServerTunnelsName (int index)
{
    if (index < C_GetServerTunnelsCount())
    {
        int ret = i2p::api::GetServerTunnelsName(RET_STR, index);
        if (ret == 0)
        {
            return RET_STR.c_str();
        } else
        {
            return NULL;
        };
    }
    return NULL;
}

const char * C_GetServerTunnelsIdent (int index)
{
    if (index < C_GetServerTunnelsCount())
    {
        int ret = i2p::api::GetServerTunnelsIdent(RET_STR, index);
        if (ret == 0)
        {
            return RET_STR.c_str();
        } else
        {
            return NULL;
        };
    }
    return NULL;
}

int C_GetInboundTunnelsCount ()
{
    int ret = i2p::api::GetInboundTunnelsCount ();
    return ret;
}

int C_GetOutboundTunnelsCount ()
{
    int ret = i2p::api::GetOutboundTunnelsCount ();
    return ret;
}

const char * C_GetInboundTunnelsFormattedInfo (int index)
{
    if (index < C_GetInboundTunnelsCount())
    {
        int ret = i2p::api::GetInboundTunnelsFormattedInfo(RET_STR, index);
        if (ret == 0)
        {
            return RET_STR.c_str();
        } else
        {
            return NULL;
        };
    }
    return NULL;
}

const char * C_GetOutboundTunnelsFormattedInfo (int index)
{
    if (index < C_GetOutboundTunnelsCount())
    {
        int ret = i2p::api::GetOutboundTunnelsFormattedInfo(RET_STR, index);
        if (ret == 0)
        {
            return RET_STR.c_str();
        } else
        {
            return NULL;
        };
    }
    return NULL;
}

const char * C_GenerateIdentToFile (const char * filename, const char * sk, uint16_t sigType, uint16_t cryptoType)
{
    size_t SK_LENGTH = 64; // 64 bytes
    uint8_t uint_sk[SK_LENGTH];
    memcpy(uint_sk, sk, SK_LENGTH);
    std::string str_filename(filename);
    int ret = i2p::api::GenerateIdentToFile(RET_STR, str_filename, uint_sk, sigType, cryptoType);
    if (ret == 0)
    {
        return RET_STR.c_str();
    } else
    {
        return NULL;
    };
}

#ifdef __cplusplus
}
#endif
