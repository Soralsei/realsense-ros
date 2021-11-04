#pragma once
#include <ros_utils.h>
#include "constants.h"
#include <deque>
#include "ros_param_backend.h"

namespace realsense2_camera
{
    class Parameters
    {
        public:
            Parameters(rclcpp::Node& node);
            ~Parameters();
            rclcpp::ParameterValue setParam(std::string param_name, const rclcpp::ParameterValue& initial_value, 
                                            std::function<void(const rclcpp::Parameter&)> func = std::function<void(const rclcpp::Parameter&)>(),
                                            rcl_interfaces::msg::ParameterDescriptor descriptor=rcl_interfaces::msg::ParameterDescriptor());

            rclcpp::ParameterValue readAndDeleteParam(std::string param_name, const rclcpp::ParameterValue& initial_value);
            template <class T>
            void setParamT(std::string param_name, const rclcpp::ParameterValue& initial_value, 
                           T& param,
                           std::function<void(const rclcpp::Parameter&)> func = std::function<void(const rclcpp::Parameter&)>(),
                           rcl_interfaces::msg::ParameterDescriptor descriptor=rcl_interfaces::msg::ParameterDescriptor());
            template <class T>
            void setParamValue(T& param, const T& value); // function updates the parameter value both locally and in the parameters server
            void setRosParamValue(const std::string param_name, void const* const value); // function updates the parameters server
            void removeParam(std::string param_name);
            void pushUpdateFunctions(std::vector<std::function<void()> > funcs);

        private:
            void monitor_update_functions();

        private:
            rclcpp::Node& _node;
            rclcpp::Logger _logger;
            std::map<std::string, std::function<void(const rclcpp::Parameter&)> > _param_functions;
            std::map<void*, std::string> _param_names;
            ParametersBackend _params_backend;
            std::condition_variable _update_functions_cv;
            bool _is_running;
            std::shared_ptr<std::thread> _update_functions_t;
            std::deque<std::function<void()> > _update_functions_v;
    };
}
