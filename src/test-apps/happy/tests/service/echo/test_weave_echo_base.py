#!/usr/bin/env python3


#
#    Copyright (c) 2016-2017 Nest Labs, Inc.
#    All rights reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

#
#    @file
#       base class for weave echo between mock device and real service.
#

from __future__ import absolute_import
from __future__ import print_function
import itertools
import os
import sys
import unittest
import set_test_path

from happy.Utils import *
import WeavePing
import WeaveTunnelStart
import WeaveTunnelStop
import plugins.testrail.TestrailResultOutput

from topologies.dynamic.thread_wifi_ap_internet_configurable_topology import thread_wifi_ap_internet_configurable_topology
from six.moves import range

TEST_OPTION_QUIET = True
DEFAULT_FABRIC_SEED = "00000"
TESTRAIL_SECTION_NAME = "Weave Echo between Mock-Client and Real-Service"
TESTRAIL_SUFFIX = "_TESTRAIL.json"
ERROR_THRESHOLD_PERCENTAGE = 5

class test_weave_echo_base(unittest.TestCase):
    def setUp(self):
        self.tap = None
        self.tap_id = None
        self.quiet = TEST_OPTION_QUIET
        self.options = None

        self.topology_setup_required = int(os.environ.get("TOPOLOGY", "1")) == 1

        self.count = os.environ.get("ECHO_COUNT", "400")

        fabric_seed = os.environ.get("FABRIC_SEED", DEFAULT_FABRIC_SEED)

        if "FABRIC_OFFSET" in list(os.environ.keys()):
            self.fabric_id = format(int(fabric_seed, 16) + int(os.environ["FABRIC_OFFSET"]), 'x').zfill(5)
        else:
            self.fabric_id = fabric_seed

        self.eui64_prefix = os.environ.get("EUI64_PREFIX", '18:B4:30:AA:00:')

        self.customized_eui64_seed = self.eui64_prefix + self.fabric_id[0:2] + ':' + self.fabric_id[2:4] + ':' + self.fabric_id[4:]

        self.device_numbers = int(os.environ.get("DEVICE_NUMBERS", 1))

        self.test_timeout = int(os.environ.get("TEST_TIMEOUT", 60 * 30))

        # TODO: Once LwIP bugs for tunnel are fix, enable this test on LwIP
        if "WEAVE_SYSTEM_CONFIG_USE_LWIP" in list(os.environ.keys()) and os.environ["WEAVE_SYSTEM_CONFIG_USE_LWIP"] == "1":
            self.tap = True
            self.tap_id = "wpan0"
            return
        else:
            self.tap = False

        self.case = int(os.environ.get("CASE", "0")) == 1

        self.use_service_dir = int(os.environ.get("USE_SERVICE_DIR", "0")) == 1

        if self.topology_setup_required:
            self.topology = thread_wifi_ap_internet_configurable_topology(quiet=self.quiet,
                                                                          fabric_id=self.fabric_id,
                                                                          customized_eui64_seed=self.customized_eui64_seed,
                                                                          tap=self.tap,
                                                                          dns=None,
                                                                          device_numbers=self.device_numbers)
            self.topology.createTopology()
        else:
            print("topology set up not required")

        self.weave_wdm = None
        # Wait for a second to ensure that Weave ULA addresses passed dad
        # and are no longer tentative
        delayExecution(2)

        # topology has nodes: ThreadNode, BorderRouter, onhub and NestService instance
        # we run tunnel between BorderRouter and NestService

        # Start tunnel
        value, data = self.__start_tunnel_from("BorderRouter")
        delayExecution(1)



    def tearDown(self):
        delayExecution(1)
        # Stop tunnel
        value, data = self.__stop_tunnel_from("BorderRouter")
        if self.topology_setup_required:
            self.topology.destroyTopology()
            self.topology = None


    def weave_echo_base(self, echo_args):
        default_options = {'test_case_name' : [],
                           'timeout' : self.test_timeout,
                           'quiet': False,
                           'case' : self.case,
                           'case_shared' : False,
                           'tap' : self.tap_id
                           }

        default_options.update(echo_args)
        self.__dict__.update(default_options)
        self.default_options = default_options

        # topology has nodes: ThreadNode, BorderRouter, onhub and NestService instance

        value, data = self.__run_ping_test_between("ThreadNode", "service")
        self.__process_result("ThreadNode", "service", value, data)


    def __start_tunnel_from(self, gateway):
        options = WeaveTunnelStart.option()
        options["quiet"] = False
        options["border_gateway"] = gateway
        options["tap"] = self.tap_id

        options["case"] = self.case

        options["service_dir"] = self.use_service_dir

        weave_tunnel = WeaveTunnelStart.WeaveTunnelStart(options)
        ret = weave_tunnel.run()

        value = ret.Value()
        data = ret.Data()

        return value, data


    def __run_ping_test_between(self, nodeA, nodeB):
        options = WeavePing.option()
        options.update(self.default_options)
        options["server"] = nodeB
        options["clients"] = [nodeA + str(index + 1).zfill(2) for index in range(self.device_numbers)]
        options["device_numbers"] = self.device_numbers
        self.weave_ping = WeavePing.WeavePing(options)
        ret = self.weave_ping.run()

        value = ret.Value()
        data = ret.Data()

        return value, data


    def __process_result(self, nodeA, nodeB, value, all_data):
        success = True
        for data in all_data:
            print("ping from " + data['client'] + " to " + nodeB + " ", end=' ')

            if value > ERROR_THRESHOLD_PERCENTAGE:
                success = False
                print(hred("Failed"))
            else:
                print(hgreen("Passed"))

            try:
                self.assertTrue(value < ERROR_THRESHOLD_PERCENTAGE, "%s < %d %%" % (str(value), ERROR_THRESHOLD_PERCENTAGE))
            except AssertionError as e:
                print(str(e))
                print("Captured experiment result:")

                print("Client Output: ")
                for line in data["client_output"].split("\n"):
                   print("\t" + line)

            test_results = []

            test_results.append({
                'testName': self.test_case_name[0],
                'testStatus': 'success' if success else 'failure',
                'testDescription': "echo loss percentage is %f" % value ,
                'success_iterations_count': (100-value)/100.0 * int(self.count),
                'total_iterations_count': int(self.count)
            })

            # output the test result to a json file
            output_data = {
                'sectionName': TESTRAIL_SECTION_NAME,
                'testResults': test_results
            }

            output_file_name = self.weave_ping.process_log_prefix + data['client'] + self.test_tag.upper() + TESTRAIL_SUFFIX

            self.__output_test_result(output_file_name, output_data)

        if not success:
            raise ValueError('test failure')

    def __output_test_result(self, file_path, output_data):
        options = plugins.testrail.TestrailResultOutput.option()
        options["quiet"] = TEST_OPTION_QUIET
        options["file_path"] = file_path
        options["output_data"] = output_data

        output_test = plugins.testrail.TestrailResultOutput.TestrailResultOutput(options)
        output_test.run()

    def __stop_tunnel_from(self, gateway):
        options = WeaveTunnelStop.option()
        options["quiet"] = False
        options["border_gateway"] = gateway

        options["service_dir"] = self.use_service_dir

        weave_tunnel = WeaveTunnelStop.WeaveTunnelStop(options)
        ret = weave_tunnel.run()

        value = ret.Value()
        data = ret.Data()

        return value, data


if __name__ == "__main__":
    unittest.main()


