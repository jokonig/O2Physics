// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

/// \file EventTables.h
/// \brief This header provides the table definitions to store photon meson event tables and ccdb tables
/// \author Marvin Hemmer (marvin.hemmer@cern.ch) - Goethe University Frankfurt

#ifndef PWGEM_PHOTONMESON_DATAMODEL_EVENTTABLES_H_
#define PWGEM_PHOTONMESON_DATAMODEL_EVENTTABLES_H_

#include "Common/CCDB/EventSelectionParams.h"
#include "Common/DataModel/EventSelection.h"

#include <DataFormatsParameters/GRPMagField.h>
#include <EMCALCalib/BadChannelMap.h>
#include <Framework/AnalysisDataModel.h>

#include <TBufferFile.h> // IWYU pragma: keep
#include <TClass.h>      // IWYU pragma: keep

#include <Rtypes.h> // for BIT

#include <cstdint>

namespace o2::aod
{

namespace pmevsel
{
// Event selection criteria. See O2Physics/Common/CCDB/EventSelectionParams.h
enum EventSelectionFlags {
  kIsTriggerTVX = 0,          // FT0 vertex (acceptable FT0C-FT0A time difference) at trigger level
  kNoITSROFrameBorder,        // bunch crossing is far from ITS RO Frame border
  kNoTimeFrameBorder,         // bunch crossing is far from Time Frame borders
  kNoSameBunchPileup,         // reject collisions in case of pileup with another collision in the same foundBC
  kIsGoodZvtxFT0vsPV,         // small difference between z-vertex from PV and from FT0
  kIsVertexITSTPC,            // at least one ITS-TPC track (reject vertices built from ITS-only tracks)
  kIsVertexTOFmatched,        // at least one of vertex contributors is matched to TOF
  kIsVertexTRDmatched,        // at least one of vertex contributors is matched to TRD
  kNoCollInTimeRangeNarrow,   // no other collisions in specified time range (narrower than Strict)
  kNoCollInTimeRangeStrict,   // no other collisions in specified time range
  kNoCollInTimeRangeStandard, // no other collisions in specified time range with per-collision multiplicity above threshold
  kNoCollInRofStrict,         // no other collisions in this Readout Frame
  kNoCollInRofStandard,       // no other collisions in this Readout Frame with per-collision multiplicity above threshold
  kNoHighMultCollInPrevRof,   // veto an event if FT0C amplitude in previous ITS ROF is above threshold
  kIsGoodITSLayer3,           // number of inactive chips on ITS layer 3 is below maximum allowed value
  kIsGoodITSLayer0123,        // numbers of inactive chips on ITS layers 0-3 are below maximum allowed values
  kIsGoodITSLayersAll,        // numbers of inactive chips on all ITS layers are below maximum allowed values
  kNsel                       // counter
};

DECLARE_SOA_BITMAP_COLUMN(Selection, selection, 32); //! Bitmask of selection flags
DECLARE_SOA_DYNAMIC_COLUMN(Sel8, sel8, [](uint32_t selection_bit, int runNumber = 500000) -> bool {
  return (selection_bit & BIT(o2::aod::pmevsel::kIsTriggerTVX)) && (selection_bit & BIT(o2::aod::pmevsel::kNoTimeFrameBorder)) && (runNumber < 568873 ? (selection_bit & BIT(o2::aod::pmevsel::kNoITSROFrameBorder)) : true);
});

enum EventAcceptanceBits {
  kAll = 0, // o2-linter: disable=magic-number (enum)
  kHasMCColl,
  kGoodZVtx,
  kIsFT0AND,
  kNoTFB,
  kITSROFB,
  kNoSameBunchPileUp,
  kGoodZVtxFTOPV,
  kNoCollInTimeRange,
  kGoodTrackOccupancy,
  kGoodFT0Occupancy,
  kTVXInEMC,
  kGoodCent,
  kGoodRCT,
  kGoodSel8,
  kSize
};

template <typename TBC>
uint32_t reduceSelectionBit(TBC const& bc)
{
  // input should be o2::aod::BcSels or o2::aod::EvSels.
  uint32_t bitMap = 0;
  if (bc.selection_bit(o2::aod::evsel::kIsTriggerTVX)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kIsTriggerTVX);
  }
  if (bc.selection_bit(o2::aod::evsel::kNoTimeFrameBorder)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kNoTimeFrameBorder);
  }
  if (bc.selection_bit(o2::aod::evsel::kNoITSROFrameBorder)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kNoITSROFrameBorder);
  }
  if (bc.selection_bit(o2::aod::evsel::kNoSameBunchPileup)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kNoSameBunchPileup);
  }
  if (bc.selection_bit(o2::aod::evsel::kIsGoodZvtxFT0vsPV)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kIsGoodZvtxFT0vsPV);
  }
  if (bc.selection_bit(o2::aod::evsel::kIsVertexITSTPC)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kIsVertexITSTPC);
  }
  if (bc.selection_bit(o2::aod::evsel::kIsVertexTRDmatched)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kIsVertexTRDmatched);
  }
  if (bc.selection_bit(o2::aod::evsel::kIsVertexTOFmatched)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kIsVertexTOFmatched);
  }
  if (bc.selection_bit(o2::aod::evsel::kNoCollInTimeRangeStandard)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kNoCollInTimeRangeStandard);
  }
  if (bc.selection_bit(o2::aod::evsel::kNoCollInTimeRangeStrict)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kNoCollInTimeRangeStrict);
  }
  if (bc.selection_bit(o2::aod::evsel::kNoCollInTimeRangeNarrow)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kNoCollInTimeRangeNarrow);
  }
  if (bc.selection_bit(o2::aod::evsel::kNoCollInRofStandard)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kNoCollInRofStandard);
  }
  if (bc.selection_bit(o2::aod::evsel::kNoCollInRofStrict)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kNoCollInRofStrict);
  }
  if (bc.selection_bit(o2::aod::evsel::kNoHighMultCollInPrevRof)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kNoHighMultCollInPrevRof);
  }
  if (bc.selection_bit(o2::aod::evsel::kIsGoodITSLayer3)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kIsGoodITSLayer3);
  }
  if (bc.selection_bit(o2::aod::evsel::kIsGoodITSLayer0123)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kIsGoodITSLayer0123);
  }
  if (bc.selection_bit(o2::aod::evsel::kIsGoodITSLayersAll)) {
    SETBIT(bitMap, o2::aod::pmevsel::EventSelectionFlags::kIsGoodITSLayersAll);
  }
  return bitMap;
}

} // namespace pmevsel

namespace pmevent
{
DECLARE_SOA_COLUMN(CollisionId, collisionId, int);

DECLARE_SOA_DYNAMIC_COLUMN(Sel8, sel8, [](uint64_t selection_bit, int runNumber = 500000) -> bool { return (selection_bit & BIT(o2::aod::evsel::kIsTriggerTVX)) && (selection_bit & BIT(o2::aod::evsel::kNoTimeFrameBorder)) && (runNumber < 568873 ? (selection_bit & BIT(o2::aod::evsel::kNoITSROFrameBorder)) : true); });

} // namespace pmevent

DECLARE_SOA_TABLE(PMEvents, "AOD", "PMEVENT", //!   Main event information table
                  o2::soa::Index<>, pmevent::CollisionId, bc::RunNumber, bc::GlobalBC, evsel::Selection, evsel::Rct, timestamp::Timestamp,
                  collision::PosZ,
                  collision::NumContrib, evsel::NumTracksInTimeRange, evsel::SumAmpFT0CInTimeRange, pmevent::Sel8<evsel::Selection, bc::RunNumber>);

using PMEvent = PMEvents::iterator;

// Tables for event selection and event bookkeeping

DECLARE_SOA_COLUMN(IsSelected, isSelected, bool); //! MB event selection info
DECLARE_SOA_TABLE(PMEvSels, "AOD", "PMEVSEL",     //! joinable to o2::aod::Collisions
                  IsSelected);
using PMEvSel = PMEvSels::iterator;

DECLARE_SOA_COLUMN(EventSelectionBit, eventSelectionBit, std::vector<uint64_t>); //! Event selection info stored in binned data for each DF
DECLARE_SOA_TABLE(PMEvSelBits, "AOD", "PMEVSELBITS",                             //! produces binned data that can be loaded in analysis task for event counting
                  EventSelectionBit);
using PMEvSelBit = PMEvSelBits::iterator;

namespace ccdbPcm
{
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
DECLARE_SOA_CCDB_COLUMN(GRPMagField, grpMagField, o2::parameters::GRPMagField, "GLO/Config/GRPMagField"); //!
} // namespace ccdbPcm

/// Full table — join with aod::BCsWithTimestamps to obtain all four objects.
DECLARE_SOA_TIMESTAMPED_TABLE(PcmObjects, aod::Timestamps, o2::aod::timestamp::Timestamp, 0, "PCMOBJECTS", //!
                              ccdbPcm::GRPMagField);

namespace em::ccdbMagField
{
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
DECLARE_SOA_CCDB_COLUMN(GRPMagField, grpMagField, o2::parameters::GRPMagField, "GLO/Config/GRPMagField"); //!
} // namespace em::ccdbMagField

/// Full table — join with aod::BCsWithTimestamps to obtain all four objects.
DECLARE_SOA_TIMESTAMPED_TABLE(EmMagFields, aod::PMEvents, o2::aod::timestamp::Timestamp, 0, "EMMAGFIELDS", //!
                              em::ccdbMagField::GRPMagField);

namespace em::ccdbEmcal
{
// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
DECLARE_SOA_CCDB_COLUMN(BadChannelMap, badChannelMap, o2::emcal::BadChannelMap, "EMC/Calib/BadChannelMap"); //! EMCal BadChannelMap for EmEvents
} // namespace em::ccdbEmcal

/// Full table — join with aod::BCsWithTimestamps to obtain all four objects.
DECLARE_SOA_TIMESTAMPED_TABLE(EmEmcalObjects, aod::PMEvents, o2::aod::timestamp::Timestamp, 0, "EMEMCALOBJECTS", //!
                              em::ccdbEmcal::BadChannelMap);

} // namespace o2::aod

#endif // PWGEM_PHOTONMESON_DATAMODEL_EVENTTABLES_H_
