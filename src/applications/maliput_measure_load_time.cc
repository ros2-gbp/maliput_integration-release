// BSD 3-Clause License
//
// Copyright (c) 2022, Woven Planet. All rights reserved.
// Copyright (c) 2021-2022, Toyota Research Institute. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/// @file maliput_measure_load_time.cc
///
/// Builds an api::RoadGeometry as many times as requested and calculates a mean with the time results. Possible
/// backends are `dragway`, `multilane` and `malidrive`.
///
/// @note
///   1. Allows to load a road geometry from different road geometry implementations.
///       The `maliput_backend` flag will determine the backend to be used.
///      - "dragway": The following flags are supported to use in order to create dragway road geometry:
///           -num_lanes, -length, -lane_width, -shoulder_width, -maximum_height.
///      - "multilane": yaml file path must be provided:
///           -yaml_file.
///      - "malidrive": xodr file path must be provided and other arguments are optional:
///           -xodr_file_path -linear_tolerance -build_policy -num_threads.
///   2. The applications allows you to load a xodr multiple times and calculate a mean.
///      The number of iterations could be changed using:
///      -iterations
///   3. The level of the logger is selected with `-log_level`.

#include <chrono>
#include <numeric>
#include <string>
#include <vector>

#include <gflags/gflags.h>
#include <maliput/common/logger.h>

#include "integration/tools.h"
#include "maliput_gflags.h"

namespace maliput {
namespace integration {
namespace {

MULTILANE_PROPERTIES_FLAGS();
DRAGWAY_PROPERTIES_FLAGS();
MALIDRIVE_PROPERTIES_FLAGS();
MALIPUT_APPLICATION_DEFINE_LOG_LEVEL_FLAG();

DEFINE_string(maliput_backend, "malidrive",
              "Whether to use <dragway>, <multilane> or <malidrive>. Default is malidrive.");
DEFINE_int32(iterations, 1, "Number of iterations for loading the Road Geometry.");

// Measure the time that it takes to create the RoadNetwork using the implementation that `maliput_implementation`
// describes. It is a wrapper around maliput::integration::LoadRoadNetwork() method.
//
// @param maliput_implementation One of MaliputImplementation. (kDragway, kMultilane, kMalidrive).
// @param dragway_build_properties Holds the properties to build a dragway RoadNetwork.
// @param multilane_build_properties Holds the properties to build a multilane RoadNetwork.
// @param malidrive_build_properties Holds the properties to build a malidrive RoadNetwork.
// @return the time in seconds.
//
// @throw maliput::common::assertion_error When `maliput_implementation` is unknown.
double MeasureLoadTime(MaliputImplementation maliput_implementation,
                       const DragwayBuildProperties& dragway_build_properties,
                       const MultilaneBuildProperties& multilane_build_properties,
                       const MalidriveBuildProperties& malidrive_build_properties) {
  const auto start = std::chrono::high_resolution_clock::now();
  const auto rn = LoadRoadNetwork(maliput_implementation, dragway_build_properties, multilane_build_properties,
                                  malidrive_build_properties);
  const auto end = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> duration = (end - start);
  return duration.count();
}

int Main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  maliput::common::set_log_level(FLAGS_log_level);

  log()->debug("Backend implementation selected is {} ", FLAGS_maliput_backend);
  const MaliputImplementation maliput_implementation{StringToMaliputImplementation(FLAGS_maliput_backend)};

  if (FLAGS_iterations < 1) {
    log()->error("Iterations: {}. The number of iterations must be greater than zero.", FLAGS_iterations);
    return 1;
  }
  std::vector<double> times;
  times.reserve(FLAGS_iterations);
  for (int i = 0; i < FLAGS_iterations; i++) {
    log()->info("Building RoadNetwork {} of {}.", i + 1, FLAGS_iterations);
    times.push_back(MeasureLoadTime(
        maliput_implementation,
        {FLAGS_num_lanes, FLAGS_length, FLAGS_lane_width, FLAGS_shoulder_width, FLAGS_maximum_height},
        {FLAGS_yaml_file},
        {FLAGS_xodr_file_path, GetLinearToleranceFlag(), GetMaxLinearToleranceFlag(), FLAGS_build_policy,
         FLAGS_num_threads, FLAGS_simplification_policy, FLAGS_standard_strictness_policy, FLAGS_omit_nondrivable_lanes,
         FLAGS_rule_registry_file, FLAGS_road_rule_book_file, FLAGS_traffic_light_book_file, FLAGS_phase_ring_book_file,
         FLAGS_intersection_book_file}));
  }
  const double mean_time = (std::accumulate(times.begin(), times.end(), 0.)) / static_cast<double>(times.size());
  maliput::log()->info("\tMean time was: {}s out of {} iterations.\n", mean_time, FLAGS_iterations);

  return 0;
}

}  // namespace
}  // namespace integration
}  // namespace maliput

int main(int argc, char* argv[]) { return maliput::integration::Main(argc, argv); }
