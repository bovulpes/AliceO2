// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

///
/// \file    raw-to-digits-workflow.cxx
/// \author  Andrea Ferrero
///
/// \brief This is an executable that runs the decoding via DPL.
///
/// This is an executable that takes a raw buffer from the Data Processing Layer, runs the decoding and sends the digits via the Data Processing Layer.
/// The decoder expects an input buffer in the format generated by o2-raw-file-reader-workflow
///

#include "Framework/WorkflowSpec.h"
#include "Framework/DataSpecUtils.h"
#include "Framework/CallbackService.h"
#include "Framework/ControlService.h"
#include "Framework/Task.h"

#include "Framework/ConfigParamSpec.h"
#include "Framework/CompletionPolicyHelpers.h"
#include "CommonUtils/ConfigurableParam.h"
#include "DetectorsRaw/HBFUtilsInitializer.h"
#include "MCHWorkflow/DataDecoderSpec.h"

using namespace o2::framework;

void customize(std::vector<ConfigParamSpec>& workflowOptions)
{
  std::vector<ConfigParamSpec> options{
    {"configKeyValues", VariantType::String, "", {"Semicolon separated key=value strings"}}};

  o2::raw::HBFUtilsInitializer::addConfigOption(options);

  std::swap(workflowOptions, options);
  workflowOptions.push_back(ConfigParamSpec{"dataspec", VariantType::String, "TF:MCH/RAWDATA", {"selection string for the input data"}});
}

#include "Framework/runDataProcessing.h"

using namespace o2;
using namespace o2::framework;

WorkflowSpec defineDataProcessing(const ConfigContext& configcontext)
{
  o2::conf::ConfigurableParam::updateFromString(configcontext.options().get<std::string>("configKeyValues"));

  auto inputSpec = configcontext.options().get<std::string>("dataspec");
  WorkflowSpec wf{o2::mch::raw::getDecodingSpec(inputSpec)};

  // configure dpl timer to inject correct firstTFOrbit: start from the 1st orbit of TF containing 1st sampled orbit
  o2::raw::HBFUtilsInitializer hbfIni(configcontext, wf);

  return std::move(wf);
}
