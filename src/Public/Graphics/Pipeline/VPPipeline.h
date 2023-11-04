#pragma once

#include <string>
#include <vector>
#include "App/Utils/VPEngineDevice.h"
namespace VP {

    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };
    class VPPipeline {
    public:
        VPPipeline(VPEngineDevice& device, const std::string & vertFilepath, const std::string & fragFilepath, const PipelineConfigInfo & configData);
        ~VPPipeline();

        VPPipeline(const VPPipeline&) = delete;
        void operator=(const VPPipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        static PipelineConfigInfo GetDefaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> ReadFile(const std::string& filepath);
        void CreateGraphicsPipeline(const std::string & vertFilepath, const std::string & fragFilepath, const PipelineConfigInfo & configData);

        void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        VPEngineDevice& Device;
        VkPipeline GraphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
};


