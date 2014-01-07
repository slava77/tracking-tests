import FWCore.ParameterSet.Config as cms

process = cms.Process("MULTITRACKVALIDATOR")

# message logger
#process.MessageLogger = cms.Service("MessageLogger",
#     default = cms.untracked.PSet( limit = cms.untracked.int32(10) )
#)

# source
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring() 
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles)
readFiles.extend( ['file:/tas/cerati/TrackingTests/Samples/CMSSW_7_0_0_pre3/8TeV/step3_ttbar_20PU25ns_m2p0.root' ])
secFiles.extend( ['file:/tas/cerati/TrackingTests/Samples/CMSSW_7_0_0_pre3/8TeV/step2_ttbar_20PU25ns_m2p0.root'] )

process.source = source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

### conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = 'STARTUP3X_V14::All'

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

### standard includes
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

### validation-specific includes
#process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.quickTrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.load("Validation.RecoTrack.cuts_cff")
process.load("Validation.RecoTrack.MultiTrackValidator_cff")
#process.load("DQMServices.Components.EDMtoMEConverter_cff")
process.load("Validation.Configuration.postValidation_cff")
process.quickTrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

########### configuration MultiTrackValidator ########
process.multiTrackValidator.outputFile = 'mtv_pu20.root'
process.multiTrackValidator.associators = ['quickTrackAssociatorByHits']
process.multiTrackValidator.skipHistoFit = cms.untracked.bool(False)
process.multiTrackValidator.runStandalone = cms.bool(True)
process.multiTrackValidator.label = ['cutsRecoTracks']
process.multiTrackValidator.useLogPt=cms.untracked.bool(True)
process.multiTrackValidator.minpT = cms.double(0.1)
process.multiTrackValidator.maxpT = cms.double(3000.0)
process.multiTrackValidator.nintpT = cms.int32(40)
process.multiTrackValidator.UseAssociators = cms.bool(True)

process.multiTrackValidator.ptMinTP = cms.double(0.4)
process.multiTrackValidator.lipTP = cms.double(35.0)
process.multiTrackValidator.tipTP = cms.double(70.0)
process.multiTrackValidator.histoProducerAlgoBlock.generalTpSelector.ptMin = cms.double(0.4)
process.multiTrackValidator.histoProducerAlgoBlock.generalTpSelector.lip = cms.double(35.0)
process.multiTrackValidator.histoProducerAlgoBlock.generalTpSelector.tip = cms.double(70.0)
process.multiTrackValidator.histoProducerAlgoBlock.TpSelectorForEfficiencyVsEta  = process.multiTrackValidator.histoProducerAlgoBlock.generalTpSelector.clone()
process.multiTrackValidator.histoProducerAlgoBlock.TpSelectorForEfficiencyVsPhi  = process.multiTrackValidator.histoProducerAlgoBlock.generalTpSelector.clone()
process.multiTrackValidator.histoProducerAlgoBlock.TpSelectorForEfficiencyVsPt   = process.multiTrackValidator.histoProducerAlgoBlock.generalTpSelector.clone()
process.multiTrackValidator.histoProducerAlgoBlock.TpSelectorForEfficiencyVsVTXR = process.multiTrackValidator.histoProducerAlgoBlock.generalTpSelector.clone()
process.multiTrackValidator.histoProducerAlgoBlock.TpSelectorForEfficiencyVsVTXZ = process.multiTrackValidator.histoProducerAlgoBlock.generalTpSelector.clone()

#process.load("Validation.RecoTrack.cuts_cff")
#process.cutsRecoTracks.ptMin    = cms.double(0.5)
#process.cutsRecoTracks.minHit   = cms.int32(10)
#process.cutsRecoTracks.minRapidity  = cms.int32(-1.0)
#process.cutsRecoTracks.maxRapidity  = cms.int32(1.0)
process.cutsRecoTracks.quality = cms.vstring('highPurity')
#process.cutsRecoTracks.algorithm = cms.vstring('iter4')
#process.cutsRecoTracks.quality = cms.vstring('')
#process.cutsRecoTracks.copyExtras = True

process.quickTrackAssociatorByHits.useClusterTPAssociation = cms.bool(True)
process.load("SimTracker.TrackerHitAssociation.clusterTpAssociationProducer_cfi")

process.clustToHits = cms.Sequence(
    process.siPixelRecHits*process.siStripMatchedRecHits
)

process.tracking = cms.Sequence(
    process.trackingGlobalReco
)

process.validation = cms.Sequence(
    #process.cutsRecoTracks *
    process.multiTrackValidator
)

# paths
process.trk = cms.Path(
      process.clustToHits *
      process.tracking
)
process.val = cms.Path(
      process.tpClusterProducer *
      process.cutsRecoTracks *
      process.validation
)
process.schedule = cms.Schedule(
      process.trk,process.val
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
