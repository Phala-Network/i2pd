/*
* Copyright (c) 2013-2020, The PurpleI2P Project
*
* This file is part of Purple i2pd project and licensed under BSD3
*
* See full license text in LICENSE file at top of project tree
*/

#ifndef CAPI_H__
#define CAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// initialization start and stop
void C_InitI2P (int argc, char argv[], const char * appName);
// void C_InitI2P (int argc, char** argv, const char * appName);
void C_StartI2P ();
// write system log to logStream, if not specified to <appName>.log in application's folder
void C_CloseAcceptsTunnels ();
void C_StopI2P ();
void C_RunPeerTest (); // should be called after UPnP
// fetch tunnels info
int C_GetClientTunnelsCount ();
int C_GetServerTunnelsCount ();
const char * C_GetClientTunnelsName (int index);
const char * C_GetClientTunnelsIdent (int index);
const char * C_GetServerTunnelsName (int index);
const char * C_GetServerTunnelsIdent (int index);
// key related
const char * C_LoadPrivateKeysFromFile (const char * filename, uint16_t sigType, uint16_t cryptoType);

#ifdef __cplusplus
}
#endif

#endif
