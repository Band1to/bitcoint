// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAIN_PARAMS_H
#define BITCOIN_CHAIN_PARAMS_H

#include "bignum.h"
#include "uint256.h"
#include "util.h"

#include <vector>

using namespace std;

#define MESSAGE_START_SIZE 4
typedef unsigned char MessageStartChars[MESSAGE_START_SIZE];

class CAddress;
class CBlock;

struct CDNSSeedData {
    string name, host;
    CDNSSeedData(const string &strName, const string &strHost) : name(strName), host(strHost) {}
};

/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * Bitcoin system. There are three: the main network on which people trade goods
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams
{
public:
    enum Network {
        MAIN,
        TESTNET,
        REGTEST,

        MAX_NETWORK_TYPES
    };

    enum Base58Type {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,
        EXT_PUBLIC_KEY,
        EXT_SECRET_KEY,

        MAX_BASE58_TYPES
    };

    enum SuperStakes {
        SUPER_STAKE_5K  = (1 << 10), // SuperStake 5K block
        SUPER_STAKE_15K = (1 << 11), // SuperStake 15K block
        SUPER_STAKE_60K = (1 << 12), // SuperStake 60K block
    };

    const uint256& HashGenesisBlock() const { return hashGenesisBlock; }
    const MessageStartChars& MessageStart() const { return pchMessageStart; }
    const vector<unsigned char>& AlertKey() const { return vAlertPubKey; }
    int GetDefaultPort() const { return nDefaultPort; }
    const CBigNum& ProofOfWorkLimit() const { return bnProofOfWorkLimit; }
    const CBigNum& ProofOfStakeLimit() const { return bnProofOfStakeLimit; }
    int SubsidyHalvingInterval() const { return nSubsidyHalvingInterval; }
    virtual const CBlock& GenesisBlock() const = 0;
    virtual bool RequireRPCPassword() const { return true; }
    const string& DataDir() const { return strDataDir; }
    virtual Network NetworkID() const = 0;
    const vector<CDNSSeedData>& DNSSeeds() const { return vSeeds; }
    const std::vector<unsigned char> &Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    virtual const vector<CAddress>& FixedSeeds() const = 0;
    unsigned int TargetSpacing() const { return nTargetSpacing; }
    int64_t TargetTimespan() const { return nTargetTimespan; }
    unsigned int CoinbaseMaturity() const { return nCoinbaseMaturity; }
    unsigned int CoinbaseMaturitySuperStake() const { return nCoinbaseMaturitySuperStake; }
    int64_t StakeMinAge() const { return nStakeMinAge; }
    int64_t StakeMaxAge() const { return nStakeMaxAge; }
    unsigned int ModifierInterval() const { return nModifierInterval; }
    int RPCPort() const { return nRPCPort; }
    int LaunchTime() const { return nLaunchTime; }
    unsigned int SuperStakeStartTime() const { return nSuperStakeStartTime; }
    unsigned int SuperStakeEndTime() const { return nSuperStakeEndTime; }
    int LastPoWBlock() const { return nLastPoWBlock; }
    int PoSSwitch() const { return nPoSSwitch; }
    int64_t PoWReward() const { return cnPoWReward; }
    int64_t PoSCoinReward() const { return cnPoSCoinReward; }
    int64_t SuperStakeMinValueIn(unsigned int nSuperStake) const
    {
        if (nSuperStake & SUPER_STAKE_5K)
            return 50000 * COIN;
        else if (nSuperStake & SUPER_STAKE_15K)
            return 100000 * COIN;
        else if (nSuperStake & SUPER_STAKE_60K)
            return 300000 * COIN;

        return 2000000000 * COIN; // MAX_MONEY
    }
    int64_t SuperStakeReward(unsigned int nSuperStake) const
    {
        if (nSuperStake & SUPER_STAKE_5K)
            return 5000 * COIN;
        else if (nSuperStake & SUPER_STAKE_15K)
            return 15000 * COIN;
        else if (nSuperStake & SUPER_STAKE_60K)
            return 60000 * COIN;

        return 0 * COIN;
    }
    int64_t SuperStakeTimespan(unsigned int nSuperStake) const
    {
        if (nSuperStake & SUPER_STAKE_5K)
            return 1 * 60 * 60; // 1 hour
        else if (nSuperStake & SUPER_STAKE_15K)
            return 8 * 60 * 60; // 8 hours
        else if (nSuperStake & SUPER_STAKE_60K)
            return 24 * 60 * 60; // 24 hours

        return -1;
    }

protected:
    CChainParams() {};

    uint256 hashGenesisBlock;
    MessageStartChars pchMessageStart;
    // Raw pub key bytes for the broadcast alert signing key.
    vector<unsigned char> vAlertPubKey;
    int nDefaultPort;
    int nRPCPort;
    CBigNum bnProofOfWorkLimit;
    CBigNum bnProofOfStakeLimit;
    int nSubsidyHalvingInterval;
    string strDataDir;
    vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    unsigned int nTargetSpacing;
    int64_t nTargetTimespan;
    unsigned int nLaunchTime;
    unsigned int nSuperStakeStartTime;
    unsigned int nSuperStakeEndTime;
    unsigned int nCoinbaseMaturity;
    unsigned int nCoinbaseMaturitySuperStake;
    int64_t nStakeMinAge;
    int64_t nStakeMaxAge;
    unsigned int nModifierInterval;
    int nLastPoWBlock;
    int nPoSSwitch;
    int64_t cnPoWReward;
    int64_t cnPoSCoinReward;
};

/**
 * Return the currently selected parameters. This won't change after app startup
 * outside of the unit tests.
 */
const CChainParams &Params();

/** Sets the params returned by Params() to those for the given network. */
void SelectParams(CChainParams::Network network);

/** Gets the parameters for the given network. */
const CChainParams &GetParams(CChainParams::Network network);

/**
 * Looks for -regtest or -testnet and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectParamsFromCommandLine();

inline bool TestNet() {
    // Note: it's deliberate that this returns "false" for regression test mode.
    return Params().NetworkID() == CChainParams::TESTNET;
}

#endif
