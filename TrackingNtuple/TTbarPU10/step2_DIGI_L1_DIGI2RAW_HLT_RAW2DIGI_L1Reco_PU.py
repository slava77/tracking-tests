# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step2 --conditions auto:run2_mc --pileup_input das:/RelValMinBias_13/CMSSW_7_4_0_pre7-MCRUN2_74_V7-v1/GEN-SIM -n 10 --eventcontent FEVTDEBUGHLT -s DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@frozen25ns,RAW2DIGI,L1Reco --datatier GEN-SIM-DIGI-RAW-HLTDEBUG --pileup AVE_35_BX_25ns --customise SLHCUpgradeSimulations/Configuration/postLS1Customs.customisePostLS1 --magField 38T_PostLS1 -n 10 --filein file:step1.root --fileout file:step2.root
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_25ns14e33_v1_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('file:step1.root'),
    inputCommands = cms.untracked.vstring('keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2 nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW-HLTDEBUG'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(1048576),
    fileName = cms.untracked.string('file:step2.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.mix.input.nbPileupEvents.averageNumber = cms.double(10.000000)
process.mix.bunchspace = cms.int32(25)
process.mix.minBunch = cms.int32(-2)
process.mix.maxBunch = cms.int32(0)
process.mix.input.fileNames = cms.untracked.vstring(['/store/relval/CMSSW_7_4_0_pre7/RelValMinBias_13/GEN-SIM/MCRUN2_74_V7-v1/00000/266DD8CB-EEB6-E411-82B8-0025905964BA.root', '/store/relval/CMSSW_7_4_0_pre7/RelValMinBias_13/GEN-SIM/MCRUN2_74_V7-v1/00000/64F6B3C9-EEB6-E411-9A31-0025905A6070.root', '/store/relval/CMSSW_7_4_0_pre7/RelValMinBias_13/GEN-SIM/MCRUN2_74_V7-v1/00000/7686F3C9-EEB6-E411-A5FA-0025905A608E.root', '/store/relval/CMSSW_7_4_0_pre7/RelValMinBias_13/GEN-SIM/MCRUN2_74_V7-v1/00000/802A09CA-EEB6-E411-A76A-0025905B85B2.root', '/store/relval/CMSSW_7_4_0_pre7/RelValMinBias_13/GEN-SIM/MCRUN2_74_V7-v1/00000/AC3E5DC7-EEB6-E411-9720-0025905A6092.root', '/store/relval/CMSSW_7_4_0_pre7/RelValMinBias_13/GEN-SIM/MCRUN2_74_V7-v1/00000/B4461BC7-EEB6-E411-9887-0025905A4964.root', '/store/relval/CMSSW_7_4_0_pre7/RelValMinBias_13/GEN-SIM/MCRUN2_74_V7-v1/00000/BE1984C7-EEB6-E411-99EE-0025905B8590.root', '/store/relval/CMSSW_7_4_0_pre7/RelValMinBias_13/GEN-SIM/MCRUN2_74_V7-v1/00000/BE6BEDCA-EEB6-E411-8572-0025905B855E.root'])
process.mix.digitizers = cms.PSet(process.theDigitizersValid)
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi_valid)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.raw2digi_step,process.L1Reco_step,process.endjob_step,process.FEVTDEBUGHLToutput_step])

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1 

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforFullSim 

#call to customisation function customizeHLTforFullSim imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforFullSim(process)

# End of customisation functions

