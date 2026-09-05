#include "PWGEM/PhotonMeson/TableProducer/skimmerPrimaryElectronFromDalitzEE.h"

using namespace o2::framework;
using namespace o2::aod;
using namespace o2::framework::expressions;

WorkflowSpec defineDataProcessing(ConfigContext const& context)
{
  o2::pid::tof::TOFResponseImpl::metadataInfo.initMetadata(context);

  return WorkflowSpec{
    adaptAnalysisTask<skimmerPrimaryElectronFromDalitzEE<o2::aod::EMPrimaryElectronsFromDalitzTmp, o2::aod::EMPrimaryElectronsDeDxMCTmp>>(context, TaskName{"skimmer-primary-electron-from-dalitzee-tmptable"})};
}
