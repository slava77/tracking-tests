import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/tas/cerati/TrackingTests/Samples/CMSSW_7_0_0_pre3/8TeV/step3_ttbar_20PU25ns_m2p0.root'
    )
)
#process.source.skipEvents=cms.untracked.uint32(3)

### conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

### standard includes
process.load('Configuration/StandardSequences/Services_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load("Configuration.StandardSequences.RawToDigi_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
#
#process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
process.load("RecoMuon.Configuration.RecoMuon_cff")
process.load("RecoTracker.Configuration.RecoTracker_cff")

#process.pixelLessStepSeedLayers.layerList = cms.vstring('TIB1+TIB2')

#process.pixelLessStepSeedLayers.layerList = cms.vstring('TIB1+TIB2+BPix3')
#process.pixelLessStepSeedLayers.BPix = cms.PSet(
#    HitProducer = cms.string('siPixelRecHits'),
#    hitErrorRZ = cms.double(0.006),
#    useErrorsFromParam = cms.bool(True),
#    TTRHBuilder = cms.string('TTRHBuilderWithoutAngle4MixedTriplets'),
#    skipClusters = cms.InputTag("pixelLessStepClusters"),
#    hitErrorRPhi = cms.double(0.0027)
#)

#process.pixelLessStepSeedLayers.layerList = cms.vstring('TIB1+TIB2+BPix3','TIB1+TIB2+TOB1')
#process.pixelLessStepSeedLayers.layerList = cms.vstring('TIB1+TIB2+TOB1')
#process.pixelLessStepSeedLayers.TOB = cms.PSet(
#    skipClusters = cms.InputTag("pixelLessStepClusters"),
#    matchedRecHits = cms.InputTag("siStripMatchedRecHits","matchedRecHit"),
#    TTRHBuilder = cms.string('WithTrackAngle')
#)
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.detIdsToDebug = cms.vint32(369125688,369141096,436233036)#track 3
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.detIdsToDebug = cms.vint32(369125688,369142152,436233072)#track 4
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.detIdsToDebug = cms.vint32(369124648,369141132,302193948)#track 7
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.detIdsToDebug = cms.vint32(369125716,369141124,302194452)#track 9
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.detIdsToDebug = cms.vint32(369124660,369141144,436233128)#track 10
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.detIdsToDebug = cms.vint32(369120308, 369137732, 302187796)#track 20
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.detIdsToDebug = cms.vint32(369121380,369136756,302188308)#track 22
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.debug = True
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.fnSigmaRZ = 0.0
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.extraPhiKDBox = 0.2
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.extraZKDBox = 5.0
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.maxChi2 = 5.0
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.refitHits = False
process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.propagator = cms.string("PropagatorWithMaterial")

#  these settings are ok for TIB1+TIB2+TOB1 (at least 1 event...)
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.fnSigmaRZ = 0.0
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.extraPhiKDBox = 0.025
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.extraZKDBox = 1.5
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.maxChi2 = 5.0
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.refitHits = True

# these settings are ok for TIB1+TIB2+BPix3 (at least 1 event...)
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.fnSigmaRZ = 0.0
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.extraPhiKDBox = 0.025
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.extraZKDBox = 0.4
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.maxChi2 = 5.0
#process.pixelLessStepSeeds.OrderedHitsFactoryPSet.GeneratorPSet.refitHits = True

#now add the quality flag to the track itself
import RecoTracker.FinalTrackSelectors.trackListMerger_cfi
process.pixelLessStepTracksWithQuality = RecoTracker.FinalTrackSelectors.trackListMerger_cfi.trackListMerger.clone(
    TrackProducers = cms.VInputTag(cms.InputTag('pixelLessStepTracks')),
    hasSelector=cms.vint32(1),
    indivShareFrac=cms.vdouble(1.0),
    selectedTrackQuals = cms.VInputTag(cms.InputTag("pixelLessStepSelector","pixelLessStep") ),
    setsToMerge = cms.VPSet( cms.PSet( tLists=cms.vint32(0), pQual=cms.bool(True) )  ),
    copyExtras = True,
    makeReKeyedSeeds = cms.untracked.bool(False)
)

process.tracking = cms.Sequence(
    process.siPixelRecHits*
    process.siStripMatchedRecHits*
    process.trackingGlobalReco*
    process.pixelLessStepTracksWithQuality
)

# this is the seeding presented here: https://indico.cern.ch/getFile.py/access?contribId=6&sessionId=1&resId=0&materialId=slides&confId=238024
# process.load("Demo.SeedAndTrackAnalyzer.StripTriplets_cff")
#process.demo = cms.EDAnalyzer('SeedAndTrackAnalyzer',
#                              seeds  = cms.untracked.InputTag('pixelLessStepSeeds'),
#                              tracks = cms.untracked.InputTag('pixelLessStepTracksWithQuality'),
#                              TTRHBuilder = cms.string('WithTrackAngle')
#)

process.TFileService = cms.Service("TFileService",
        #fileName = cms.string('testHPtracks_test.root'),
        #fileName = cms.string('testHPtracks_default_15eta19.root'),
        fileName = cms.string('testHPtracks_tib1tib2tib3_15eta19_mtec_newfit_v2.root'),
        #fileName = cms.string('testHPtracks_tib1tib2-tob1.root'),
        closeFileFast = cms.untracked.bool(True)
)
process.demo = cms.EDAnalyzer('SeedAndTrackAnalyzer',
                              seeds  = cms.untracked.VInputTag(cms.untracked.InputTag('pixelLessStepSeeds')),
                              tracks = cms.untracked.VInputTag(cms.untracked.InputTag('pixelLessStepTracksWithQuality')),
                              TTRHBuilder = cms.string('WithTrackAngle'),
                              debug = cms.bool(False),#fixme
                              minEta = cms.double(1.5),
                              maxEta = cms.double(1.9)
)


process.p = cms.Path(process.tracking*process.demo)
