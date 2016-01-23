# Auto generated configuration file
# using: 
# Revision: 1.20 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions PH2_1K_FB_V6::All --pileup_input SameAsInStep2 -n -1 --eventcontent RECOSIM -s RAW2DIGI,L1Reco,RECO --datatier GEN-SIM-RECO --pileup AVE_140_BX_25ns --customise SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2023HGCalMuon --geometry Extended2023HGCalMuon,Extended2023HGCalMuonReco --magField 38T_PostLS1 --no_exec --filein file:step2.root --fileout file:step3.root
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:step2.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('step3 nevts:-1'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('file:step3.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    )
)

# Additional output definition

# Other statements
process.mix.input.nbPileupEvents.averageNumber = cms.double(140.000000)
process.mix.bunchspace = cms.int32(25)
process.mix.minBunch = cms.int32(-12)
process.mix.maxBunch = cms.int32(3)
process.mix.input.fileNames = cms.untracked.vstring(SameAsInStep2)
process.mix.playback = True
process.mix.digitizers = cms.PSet()
for a in process.aliases: delattr(process, a)
process.RandomNumberGeneratorService.restoreStateLabel=cms.untracked.string("randomEngineStateProducer")
from SimGeneral.MixingModule.fullMixCustomize_cff import setCrossingFrameOn
process = setCrossingFrameOn(process)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'PH2_1K_FB_V6::All', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2023HGCalMuon 

#call to customisation function cust_2023HGCalMuon imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2023HGCalMuon(process)

# End of customisation functions

trackerSimHits = cms.VInputTag(cms.InputTag('g4SimHits','TrackerHitsPixelBarrelLowTof'),
                               cms.InputTag('g4SimHits','TrackerHitsPixelBarrelHighTof'),
                               cms.InputTag('g4SimHits','TrackerHitsPixelEndcapLowTof'),
                               cms.InputTag('g4SimHits','TrackerHitsPixelEndcapHighTof') )
xframes = cms.VInputTag( cms.InputTag("mix", "g4SimHitsTrackerHitsPixelBarrelLowTof"),
                         cms.InputTag("mix", "g4SimHitsTrackerHitsPixelBarrelHighTof"),
                         cms.InputTag("mix", "g4SimHitsTrackerHitsPixelEndcapLowTof"),
                         cms.InputTag("mix", "g4SimHitsTrackerHitsPixelEndcapHighTof")
    )
process.load("SimTracker.TrackAssociation.quickTrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.LhcParametersDefinerForTP_cfi")
process.load("SimTracker.TrackerHitAssociation.clusterTpAssociationProducer_cfi")
process.load("SimGeneral.TrackingAnalysis.simHitTPAssociation_cfi")
process.simHitTPAssocProducer.simHitSrc = trackerSimHits
process.simHitTPAssocProducer.xframesSrc = xframes

process.trkTree = cms.EDAnalyzer('TrackingNtuple',
                              seeds  = cms.untracked.VInputTag(),
                              tracks = cms.untracked.InputTag('generalTracks'),
                              TTRHBuilder = cms.string('WithTrackAngle'),
                              debug = cms.bool(False)
)

process.TFileService = cms.Service("TFileService",
        fileName = cms.string('ntuple.root'),
        closeFileFast = cms.untracked.bool(True)
)

process.trackVal_step = cms.Path(process.trackerlocalreco*process.offlineBeamSpot
                                 *cms.SequencePlaceholder("mix")*process.tpClusterProducer*process.simHitTPAssocProducer
                                 *process.trkTree)
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.trackVal_step,process.endjob_step)

#process.MessageLogger.destinations = cms.untracked.vstring("cout")
#process.MessageLogger.cout = cms.untracked.PSet(threshold = cms.untracked.string("INFO"))

