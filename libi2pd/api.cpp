/*
* Copyright (c) 2013-2020, The PurpleI2P Project
*
* This file is part of Purple i2pd project and licensed under BSD3
*
* See full license text in LICENSE file at top of project tree
*/

#include <string>
#include <map>
#include <ClientContext.h>
#include "Config.h"
#include "Log.h"
#include "NetDb.hpp"
#include "Transports.h"
#include "Tunnel.h"
#include "RouterContext.h"
#include "Identity.h"
#include "Destination.h"
#include "Crypto.h"
#include "FS.h"
#include "api.h"

namespace i2p
{
namespace api
{
	void InitI2P (int argc, char* argv[], const char * appName)
	{
		i2p::config::Init ();
		i2p::config::ParseCmdline (argc, argv, true); // ignore unknown options and help
		i2p::config::Finalize ();

		std::string datadir; i2p::config::GetOption("datadir", datadir);

		i2p::fs::SetAppName (appName);
		i2p::fs::DetectDataDir(datadir, false);
		i2p::fs::Init();

		bool precomputation; i2p::config::GetOption("precomputation.elgamal", precomputation);
		bool aesni; i2p::config::GetOption("cpuext.aesni", aesni);
		bool avx; i2p::config::GetOption("cpuext.avx", avx);
		bool forceCpuExt; i2p::config::GetOption("cpuext.force", forceCpuExt);
		i2p::crypto::InitCrypto (precomputation, aesni, avx, forceCpuExt);

		int netID; i2p::config::GetOption("netid", netID);
		i2p::context.SetNetID (netID);

		i2p::context.Init ();
	}

	void TerminateI2P ()
	{
		i2p::crypto::TerminateCrypto ();
	}

	void StartI2P (std::shared_ptr<std::ostream> logStream)
	{
		if (logStream)
			i2p::log::Logger().SendTo (logStream);
		else
			i2p::log::Logger().SendTo (i2p::fs::DataDirPath (i2p::fs::GetAppName () + ".log"));
		i2p::log::Logger().Start ();
		LogPrint(eLogInfo, "API: starting NetDB");
		i2p::data::netdb.Start();
		LogPrint(eLogInfo, "API: starting Transports");
		i2p::transport::transports.Start();
		LogPrint(eLogInfo, "API: starting Tunnels");
		i2p::tunnel::tunnels.Start();
	}

	void StopI2P ()
	{
		LogPrint(eLogInfo, "API: shutting down");
		LogPrint(eLogInfo, "API: stopping Tunnels");
		i2p::tunnel::tunnels.Stop();
		LogPrint(eLogInfo, "API: stopping Transports");
		i2p::transport::transports.Stop();
		LogPrint(eLogInfo, "API: stopping NetDB");
		i2p::data::netdb.Stop();
		i2p::log::Logger().Stop ();
	}

	void RunPeerTest ()
	{
		i2p::transport::transports.PeerTest ();
	}

	std::shared_ptr<i2p::client::ClientDestination> CreateLocalDestination (const i2p::data::PrivateKeys& keys, bool isPublic,
		const std::map<std::string, std::string> * params)
	{
		auto localDestination = std::make_shared<i2p::client::RunnableClientDestination> (keys, isPublic, params);
		localDestination->Start ();
		return localDestination;
	}

	std::shared_ptr<i2p::client::ClientDestination> CreateLocalDestination (bool isPublic, i2p::data::SigningKeyType sigType,
		const std::map<std::string, std::string> * params)
	{
		i2p::data::PrivateKeys keys = i2p::data::PrivateKeys::CreateRandomKeys (sigType);
		auto localDestination = std::make_shared<i2p::client::RunnableClientDestination> (keys, isPublic, params);
		localDestination->Start ();
		return localDestination;
	}

	void DestroyLocalDestination (std::shared_ptr<i2p::client::ClientDestination> dest)
	{
		if (dest)
			dest->Stop ();
	}

	void RequestLeaseSet (std::shared_ptr<i2p::client::ClientDestination> dest, const i2p::data::IdentHash& remote)
	{
		if (dest)
			dest->RequestDestination (remote);
	}

	std::shared_ptr<i2p::stream::Stream> CreateStream (std::shared_ptr<i2p::client::ClientDestination> dest, const i2p::data::IdentHash& remote)
	{
		if (!dest) return nullptr;
		auto leaseSet = dest->FindLeaseSet (remote);
		if (leaseSet)
		{
			auto stream = dest->CreateStream (leaseSet);
			stream->Send (nullptr, 0); // connect
			return stream;
		}
		else
		{
			RequestLeaseSet (dest, remote);
			return nullptr;
		}
	}

	void AcceptStream (std::shared_ptr<i2p::client::ClientDestination> dest, const i2p::stream::StreamingDestination::Acceptor& acceptor)
	{
		if (dest)
			dest->AcceptStreams (acceptor);
	}

	void DestroyStream (std::shared_ptr<i2p::stream::Stream> stream)
	{
		if (stream)
			stream->Close ();
	}

    std::string LoadPrivateKeysFromFile (const std::string& filename, i2p::data::SigningKeyType sigType, i2p::data::CryptoKeyType cryptoType)
    {
        std::string fullPath = filename;    // filename is an absolute path
        std::string idenHashB32;
        i2p::data::PrivateKeys keys;

        std::ifstream s(fullPath, std::ifstream::binary);
        if (s.is_open ())
        {
            s.seekg (0, std::ios::end);
            size_t len = s.tellg();
            s.seekg (0, std::ios::beg);
            uint8_t * buf = new uint8_t[len];
            s.read ((char *)buf, len);
            if(!keys.FromBuffer (buf, len))
            {
                LogPrint (eLogError, "Clients: failed to load keyfile ", filename);
            }
            else
            {
                idenHashB32 = keys.GetPublic()->GetIdentHash().ToBase32();
                LogPrint (eLogInfo, "Clients: Local address ", idenHashB32, " loaded");
            }
            delete[] buf;
            return idenHashB32;
        }
        else
        {
            keys = i2p::data::PrivateKeys::CreateRandomKeys (sigType, cryptoType);
            std::ofstream f (fullPath, std::ofstream::binary | std::ofstream::out);
            size_t len = keys.GetFullLen ();
            uint8_t * buf = new uint8_t[len];
            len = keys.ToBuffer (buf, len);
            f.write ((char *)buf, len);
            idenHashB32 = keys.GetPublic()->GetIdentHash().ToBase32();
            LogPrint (eLogInfo, "Clients: New private keys file ", fullPath, " for ", idenHashB32, " created");
            return idenHashB32;
        }
    }
}
}
