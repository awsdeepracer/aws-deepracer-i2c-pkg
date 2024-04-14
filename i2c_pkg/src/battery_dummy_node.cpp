///////////////////////////////////////////////////////////////////////////////////
//   Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.          //
//                                                                               //
//   Licensed under the Apache License, Version 2.0 (the "License").             //
//   You may not use this file except in compliance with the License.            //
//   You may obtain a copy of the License at                                     //
//                                                                               //
//       http://www.apache.org/licenses/LICENSE-2.0                              //
//                                                                               //
//   Unless required by applicable law or agreed to in writing, software         //
//   distributed under the License is distributed on an "AS IS" BASIS,           //
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    //
//   See the License for the specific language governing permissions and         //
//   limitations under the License.                                              //
///////////////////////////////////////////////////////////////////////////////////

#include "rclcpp/rclcpp.hpp"
#include <filesystem>
#include "i2c_pkg/i2c.hpp"
#include "deepracer_interfaces_pkg/srv/battery_level_srv.hpp"


namespace BoardChips {

    // The level conversion was given by pegatron, we may have more fidelity.
    // Implemented as vector for in order iteration.
    std::vector<std::pair<uint8_t, int>> levelMap = { {0xec, 11},
                                                      {0xe0, 10},
                                                      {0xd9, 9},
                                                      {0xd3, 8},
                                                      {0xcf, 7},
                                                      {0xcb, 6},
                                                      {0xc8, 5},
                                                      {0xc5, 4},
                                                      {0xc3, 3},
                                                      {0xc0, 2},
                                                      {0xb4, 1},
                                                      {0x8c, 0} };
    class BatteryDummyNodeMgr : public rclcpp::Node
    {
    public:
        BatteryDummyNodeMgr(const std::string & nodeName)
        : Node(nodeName),
          level_(-1)
        {
            RCLCPP_INFO(this->get_logger(), "%s started", nodeName.c_str());
            monitorBatteryService_ = this->create_service<deepracer_interfaces_pkg::srv::BatteryLevelSrv>("battery_level", std::bind(&BatteryDummyNodeMgr::getBatteryLevel, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        }
        ~BatteryDummyNodeMgr() = default;
    private:

        void getBatteryLevel (const std::shared_ptr<rmw_request_id_t> request_header,
                              const std::shared_ptr<deepracer_interfaces_pkg::srv::BatteryLevelSrv::Request> req,
                              std::shared_ptr<deepracer_interfaces_pkg::srv::BatteryLevelSrv::Response> res) {
            (void)request_header;
            (void) req;

            level_ = 7;

            RCLCPP_DEBUG(this->get_logger(), "Current battery level:%d", level_);
            res->level = level_;
        }
        rclcpp::Service<deepracer_interfaces_pkg::srv::BatteryLevelSrv>::SharedPtr monitorBatteryService_;
        int level_;
    };
}



int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BoardChips::BatteryDummyNodeMgr>("battery_node"));
    rclcpp::shutdown();
    return 0;
}