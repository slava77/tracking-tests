import FWCore.ParameterSet.Config as cms

process = cms.Process("TrkNtuple")

process.load("FWCore.MessageService.MessageLogger_cfi")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:./SingleMuPt1/step2.root'
        'file:./SingleMuPt10/step2.root'
        #'file:./ZTT/step2.root'
        #'file:./TTbar/step2.root'
        #'file:./TTbarPU10/step2_1000evt.root'
        #'file:./TTbarPU35/step2_100evt.root'
    ),
    #skipEvents=cms.untracked.uint32(136)
)

### conditions
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

### standard includes
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

#turn muon seeded steps off
process.muonSeededSeedsInOut.cut = 'pt < 0.000001'
process.muonSeededSeedsOutIn.cut = 'pt < 0.000001'

#mock
process.initialStepSeedLayers.layerList = cms.vstring('BPix1+BPix2+BPix3')
process.initialStepSeeds.RegionFactoryPSet.RegionPSet.ptMin = 0.5
process.initialStepTrackCandidates.doSeedingRegionRebuilding = False
process.initialStepTrackCandidates.TrajectoryBuilder = 'CkfTrajectoryBuilder'
process.initialStepTrackCandidates.TrajectoryBuilderPSet.refToPSet_ = 'CkfTrajectoryBuilder'
process.CkfTrajectoryBuilder.estimator = cms.string('initialStepChi2Est')
process.CkfTrajectoryBuilder.trajectoryFilter = cms.PSet(refToPSet_ = cms.string('initialStepTrajectoryFilter'))
process.CkfTrajectoryBuilder.maxCand = cms.int32(3)
#plus hack to navigation school to avoid moving from barrel to forward

#noSplit
#process.initialStepTrackCandidates.useHitsSplitting = False
#process.lowPtTripletStepTrackCandidates.useHitsSplitting = False
#process.mixedTripletStepTrackCandidates.useHitsSplitting = False
#process.pixelLessStepTrackCandidates.useHitsSplitting = False
#process.pixelPairStepTrackCandidates.useHitsSplitting = False
#process.tobTecStepTrackCandidates.useHitsSplitting = False
#process.detachedTripletStepTrackCandidates.useHitsSplitting = False
##otherwise there's a chance a matched hit in a track is not in the matched hit collection
#process.SiStripRecHitMatcherESProducer.NSigmaInside = 30.0

trackerSimHits = cms.VInputTag(cms.InputTag('g4SimHits','TrackerHitsTIBLowTof'),
                               cms.InputTag('g4SimHits','TrackerHitsTIBHighTof'),
                               cms.InputTag('g4SimHits','TrackerHitsTIDLowTof'),
                               cms.InputTag('g4SimHits','TrackerHitsTIDHighTof'),
                               cms.InputTag('g4SimHits','TrackerHitsTOBLowTof'),
                               cms.InputTag('g4SimHits','TrackerHitsTOBHighTof'),
                               cms.InputTag('g4SimHits','TrackerHitsTECLowTof'),
                               cms.InputTag('g4SimHits','TrackerHitsTECHighTof'),
                               cms.InputTag('g4SimHits','TrackerHitsPixelBarrelLowTof'),
                               cms.InputTag('g4SimHits','TrackerHitsPixelBarrelHighTof'),
                               cms.InputTag('g4SimHits','TrackerHitsPixelEndcapLowTof'),
                               cms.InputTag('g4SimHits','TrackerHitsPixelEndcapHighTof') )

process.load("SimTracker.TrackAssociation.quickTrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.LhcParametersDefinerForTP_cfi")
process.load("SimTracker.TrackerHitAssociation.clusterTpAssociationProducer_cfi")
process.load("SimGeneral.TrackingAnalysis.simHitTPAssociation_cfi")
process.simHitTPAssocProducer.simHitSrc = trackerSimHits

process.TFileService = cms.Service("TFileService",
        fileName = cms.string('ntuple_test.root'),
        closeFileFast = cms.untracked.bool(True)
)

allSeeds = cms.untracked.VInputTag(
    cms.untracked.InputTag('initialStepSeeds'),
    cms.untracked.InputTag('detachedTripletStepSeeds'),
    cms.untracked.InputTag('pixelPairStepSeeds'),
    cms.untracked.InputTag('lowPtTripletStepSeeds'),
    cms.untracked.InputTag('mixedTripletStepSeeds'),
    cms.untracked.InputTag('pixelLessStepSeeds'),
    cms.untracked.InputTag('tobTecStepSeeds'),
    cms.untracked.InputTag('jetCoreRegionalStepSeeds'),
    cms.untracked.InputTag('muonSeededSeedsInOut'),
    cms.untracked.InputTag('muonSeededSeedsOutIn')
    )

process.trkTree = cms.EDAnalyzer('TrackingNtuple',
                              #seeds  = cms.untracked.VInputTag(cms.untracked.InputTag('initialStepSeeds')),
                              seeds  = allSeeds,
                              tracks = cms.untracked.InputTag('generalTracks'),
                              TTRHBuilder = cms.string('WithTrackAngle'),
                              debug = cms.bool(False)
)

process.tracking = cms.Sequence(
    process.localreco*
    process.offlineBeamSpot*
    process.MeasurementTrackerEventPreSplitting* # unclear where to put this
    process.siPixelClusterShapeCachePreSplitting* # unclear where to put this
    process.standalonemuontracking*    
    process.iterTracking#process.trackingGlobalReco
)
 
process.p = cms.Path(process.RawToDigi*process.tracking*process.tpClusterProducer*process.simHitTPAssocProducer*process.trkTree)

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
process = customisePostLS1(process)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
