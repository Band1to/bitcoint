// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}


//
// Main network
//
class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x2a;
        pchMessageStart[1] = 0x1b;
        pchMessageStart[2] = 0x0c;
        pchMessageStart[3] = 0xfa;

        vAlertPubKey = ParseHex("02848b2b4a0898ee715d998fa1eb74aaaa07e8498877b224db6ee1747872d9f12a");
        nDefaultPort = 15047;
        nRPCPort = 15048;

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 20);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        //
        // genesis
        // CBlock(hash=00000fe804f4811169424161e0e896cbfc20ca5a5c04f04f997c7e7776cf5613, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=11c47b76e8e52e603e08c07105fa6fa093379401082640d69f0f25b8177fb65b, nTime=1429801140, nBits=1e0fffff, nNonce=1095958, vtx=1, vchBlockSig=)
        //   Coinbase(hash=11c47b76e8e52e603e08c07105fa6fa093379401082640d69f0f25b8177fb65b, nTime=1429801140, ver=1, vin.size=1, vout.size=1, nLockTime=0)
        //     CTxIn(COutPoint(0000000000, 4294967295), coinbase 00012a2b323031352f30342f32332031343a35393a303020474d542e20537461697277617920746f2048656176656e)
        //     CTxOut(empty)

        const char* pszTimestamp = "2015/04/23 14:59:00 GMT. Stairway to Heaven";
        CTransaction txNew;
        txNew.nTime = 1429801200 - 60; // 2015/04/23 14:59:00 GMT
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].SetEmpty();
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = txNew.nTime;
        genesis.nBits = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 1095958;

        hashGenesisBlock = uint256("0x00000fe804f4811169424161e0e896cbfc20ca5a5c04f04f997c7e7776cf5613");

        if (false && (genesis.GetHash() != hashGenesisBlock))
        {
            uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
            while (genesis.GetHash() > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    printf("NONCE WRAPPED, incrementing time");
                    ++genesis.nTime;
                }
            }
            cout << "genesis: \n" << genesis.ToString() << endl;
            cout << "genesis.GetHash(): " << genesis.GetHash().ToString() << endl;
            cout << "genesis.hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() << endl;
            cout << "genesis.nTime: " << genesis.nTime << endl;
            cout << "genesis.nNonce: " << genesis.nNonce << endl;
        }

        assert(genesis.GetHash() == hashGenesisBlock);
        assert(genesis.hashMerkleRoot == uint256("0x11c47b76e8e52e603e08c07105fa6fa093379401082640d69f0f25b8177fb65b"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(85);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(101);
        base58Prefixes[SECRET_KEY] =     list_of(117);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        vSeeds.push_back(CDNSSeedData("bitcoint.net", "seed1.bitcoint.net"));
        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        nTargetSpacing = 60;
        nTargetTimespan = 20 * 60; // 20 mins
        nCoinbaseMaturity = 30;
        nCoinbaseMaturitySuperStake = 1440;
        nLastPoWBlock = 2880;
        nLaunchTime = genesis.nTime + 60;
        nSuperStakeStartTime = nLaunchTime + 60 * 60 * 24 * 2;
        nSuperStakeEndTime = nSuperStakeStartTime + 60 * 60 * 24 * 30;
        nStakeMinAge = 1 * 60 * 60; // 1 hour
        nStakeMaxAge = -1; // infinity
        nModifierInterval = 1 * 60; // 1 minute
        nPoSSwitch = nLastPoWBlock - 50;
        cnPoWReward = 7500 * COIN;
        cnPoSCoinReward = 20 * CENT;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;

//
// Testnet
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x2a;
        pchMessageStart[1] = 0x1b;
        pchMessageStart[2] = 0x0c;
        pchMessageStart[3] = 0xf9;

        vAlertPubKey = ParseHex("02848b2b4a0898ee715d998fa1eb74aaaa07e8498877b224db6ee1747872d9f12a");
        nDefaultPort = 25047;
        nRPCPort = 25048;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1429801200 - 3600;
        genesis.nBits = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 9687;

        hashGenesisBlock = uint256("0x0000f1509825aae54b0242ea1e836e3604dc1b7375f79d5e8ca2ee2433fc86f1");

        if (false && (genesis.GetHash() != hashGenesisBlock))
        {
            uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
            while (genesis.GetHash() > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    cout << "NONCE WRAPPED, incrementing time\n";
                    ++genesis.nTime;
                }
            }
            cout << "genesis: \n" << genesis.ToString() << endl;
            cout << "genesis.GetHash(): " << genesis.GetHash().ToString() << endl;
            cout << "genesis.hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() << endl;
            cout << "genesis.nTime: " << genesis.nTime << endl;
            cout << "genesis.nNonce: " << genesis.nNonce << endl;
        }

        assert(genesis.GetHash() == hashGenesisBlock);
        assert(genesis.hashMerkleRoot == uint256("0x11c47b76e8e52e603e08c07105fa6fa093379401082640d69f0f25b8177fb65b"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(65);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(97);
        base58Prefixes[SECRET_KEY]     = list_of(129);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);

        vFixedSeeds.clear();
        vSeeds.clear();
        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        nTargetSpacing = 10;
        nTargetTimespan = 20 * 60;  // 20 minutes
        nCoinbaseMaturity = 30;
        nCoinbaseMaturitySuperStake = 1440;
        nLastPoWBlock = 2880;
        nLaunchTime = genesis.nTime + 60;
        nSuperStakeStartTime = nLaunchTime + 60 * 60 * 24 * 2;
        nSuperStakeEndTime = nSuperStakeStartTime + 60 * 60 * 24 * 3560;
        nStakeMinAge = 30 * 60; // 30 minutes
        nStakeMaxAge = -1; // infinity
        nModifierInterval = 1 * 30; // 30 seconds
        nPoSSwitch = nLastPoWBlock - 50;
        cnPoWReward = 7500 * COIN;
        cnPoSCoinReward = 20 * CENT;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

const CChainParams &GetParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            return mainParams;
            break;
        case CChainParams::TESTNET:
            return testNetParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return mainParams;;
    }
}

bool SelectParamsFromCommandLine() {
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
