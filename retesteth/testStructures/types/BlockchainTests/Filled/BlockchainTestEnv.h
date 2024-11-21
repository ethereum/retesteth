#pragma once
#include <retesteth/testStructures/types/StateTests/Base/StateTestEnvBase.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
// Blockchain test does not have Env section
// This represent env section consructed from blockchain test genesis header
struct BlockchainTestEnv : StateTestEnvBase
{
protected:
    void initializeCommonFields(DataObject const&);
private:
    virtual void define() const = 0; // BlockchainTestEnv is abstract
};

struct BlockchainTestEnvLegacy : BlockchainTestEnv
{
    BlockchainTestEnvLegacy(DataObject const& _data);

protected:
    void initializeLegacyFields(DataObject const&);
private:
    void define() const override {}
};

struct BlockchainTestEnv1559 : BlockchainTestEnv
{
    BlockchainTestEnv1559(DataObject const& _data);

protected:
    void initialize1559Fields(DataObject const&);
private:
    void define() const override {}
};

struct BlockchainTestEnvParis : BlockchainTestEnv
{
    BlockchainTestEnvParis(DataObject const& _data);

protected:
    void initializeParisFields(DataObject const&);
    BlockchainTestEnvParis(){};

private:
    void define() const override {}
};

struct BlockchainTestEnvShanghai : BlockchainTestEnvParis
{
    BlockchainTestEnvShanghai(DataObject const& _data);

protected:
    void initializeShanghaiFields(DataObject const&);
    BlockchainTestEnvShanghai(){};

private:
    void define() const override {}
};

struct BlockchainTestEnv4844 : BlockchainTestEnvShanghai
{
    BlockchainTestEnv4844(DataObject const& _data);

protected:
    BlockchainTestEnv4844(){};
    void initialize4844Fields(DataObject const&);

private:
    void define() const override {}
};

struct BlockchainTestEnvPrague : BlockchainTestEnv4844
{
    BlockchainTestEnvPrague(DataObject const& _data);

protected:
    void initializePragueFields(DataObject const&);

private:
    void define() const override {}
};

typedef GCP_SPointer<BlockchainTestEnv> spBlockchainTestEnv;


}  // namespace teststruct
