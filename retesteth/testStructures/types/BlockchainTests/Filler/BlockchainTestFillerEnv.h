#pragma once
#include <retesteth/testStructures/types/StateTests/Base/StateTestEnvBase.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

// Blockchain test does not have Env section
// This represent env section consructed from blockchain test genesis header
// Filler marks that fields are converted from dec to hex explicitly
struct BlockchainTestFillerEnv : StateTestEnvBase
{
protected:
    void initializeCommonFields(spDataObject const&, SealEngine);
private:
    virtual void define() const = 0; //BlockchainTestFillerEnv is abstract

};

struct BlockchainTestFillerEnvLegacy : BlockchainTestFillerEnv
{
    BlockchainTestFillerEnvLegacy(spDataObjectMove _data, SealEngine _sEngine);
    spDataObject asDataObject() const override;

protected:
    void initializeLegacyFields(DataObject const&);
private:
    void define() const override {}
};

struct BlockchainTestFillerEnv1559 : BlockchainTestFillerEnv
{
    BlockchainTestFillerEnv1559(spDataObjectMove _data, SealEngine _sEngine);
    spDataObject asDataObject() const override;

protected:
    void initialize1559Fields(DataObject const&);
private:
    void define() const override {}

};

struct BlockchainTestFillerEnvParis : BlockchainTestFillerEnv
{
    BlockchainTestFillerEnvParis(spDataObjectMove _data, SealEngine _sEngine);
    spDataObject asDataObject() const override;

protected:
    void initializeParisFields(DataObject const&);
    BlockchainTestFillerEnvParis(){};
private:
    void define() const override {}
};

struct BlockchainTestFillerEnvShanghai : BlockchainTestFillerEnvParis
{
    BlockchainTestFillerEnvShanghai(spDataObjectMove _data, SealEngine _sEngine);
    spDataObject asDataObject() const override;

protected:
    void initializeShanghaiFields(DataObject const&);
    BlockchainTestFillerEnvShanghai(){};
private:
    void define() const override {}
};

struct BlockchainTestFillerEnv4844 : BlockchainTestFillerEnvShanghai
{
    BlockchainTestFillerEnv4844(spDataObjectMove _data, SealEngine _sEngine);
    spDataObject asDataObject() const override;

protected:
    void initialize4844Fields(DataObject const&);
    BlockchainTestFillerEnv4844(){};
private:
    void define() const override {}
};


struct BlockchainTestFillerEnvPrague : BlockchainTestFillerEnv4844
{
    BlockchainTestFillerEnvPrague(spDataObjectMove _data, SealEngine _sEngine);
    spDataObject asDataObject() const override;

protected:
    void initializePragueFields(DataObject const&);
private:
    void define() const override {}
};


typedef GCP_SPointer<BlockchainTestFillerEnv> spBlockchainTestFillerEnv;
BlockchainTestFillerEnv* readBlockchainFillerTestEnv(spDataObjectMove _data, SealEngine _sEngine);

}  // namespace teststruct
